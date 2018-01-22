;;; Copyright (c) 2016, Fereshta Yazdani <yazdani@cs.uni-bremen.de>
;;; All rights reserved.
;; 
;;; Redistribution and use in source and binary forms, with or without
;;; modification, are permitted provided that the following conditions are met:
;;; 
;;;     * Redistributions of source code must retain the above copyright
;;;       notice, this list of conditions and the following disclaimer.
;;;     * Redistributions in binary form must reproduce the above copyright
;;;       notice, this list of conditions and the following disclaimer in the
;;;       documentation and/or other materials provided with the distribution.
;;;     * Neither the name of the Institute for Artificial Intelligence/
;;;       Universitaet Bremen nor the names of its contributors may be used to 
;;;       endorse or promote products derived from this software without 
;;;       specific prior written permission.
;;; 
;;; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;;; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;;; ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
;;; LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;;; CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;;; SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;;; INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;;; CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;;; ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;;; POSSIBILITY OF SUCH DAMAGE.

(in-package :start-mission)

(defvar *tf* NIL)
;(defvar *pub* NIL)

(defun init-tf ()
  (setf *tf* (make-instance 'cl-tf:transform-listener)))
 ;; (setf *pub* (cl-tf:make-transform-broadcaster)))

(roslisp-utilities:register-ros-init-function init-tf)

(defun make-reasoning-cost-function (location axis pred threshold)
  (roslisp:ros-info (sherpa-spatial-relations) "calculate the costmap")
  (let* ((new-loc (cl-transforms:make-pose
                   (cl-transforms:origin location)
                   (cl-transforms:make-identity-rotation)))
         (transformation (cl-transforms:pose->transform new-loc)) ;;location
         (world->location-transformation (cl-transforms:transform-inv transformation)))
    (lambda (x y)
      (let* ((point (cl-transforms:transform-point world->location-transformation
                                                   (cl-transforms:make-3d-vector x y 0)))
             (coord (ecase axis
                      (:x (cl-transforms:x point))
                      (:y (cl-transforms:y point))))
             (mode (sqrt (+   (* (cl-transforms:x point) (cl-transforms:x point))
                             (* (cl-transforms:y point) (cl-transforms:y point))))))
       ;; (format t "pred: ~a~% coord: ~a~%" pred coord)
        (if (funcall pred coord 0.0d0)
            (if (> (abs (/ coord mode)) threshold)
                (abs (/ coord mode))
                0.0d0)
            0.0d0)))))

(defun make-bb-height-function (obj-name height)
  (sem-map-utils::get-semantic-map)
  (let* ((sem-hash (slot-value semantic-map-utils::*cached-semantic-map* 'sem-map-utils:parts))
         (dim  (slot-value (gethash obj-name sem-hash) 'sem-map-utils:dimensions)))
    (setf height (/ (cl-transforms:z dim) 2))
    (if (< height 1)
	(setf height 1.5)
	(setf height height))
    (lambda (x y)
      (declare (ignore x y))
      (list height))))

;; (defun swm->make-height-human (pose height)
;;    (setf height (+ 1 4))
;;   (format t "inside human height~%")
;;   (lambda (x y)
;;     (declare (ignore x y))
;;     (list height)))

(defun swm->make-constant-height-function (obj-name height &optional (sem-map (sem-map-utils::get-semantic-map)))
  (let* ((sem-hash (slot-value sem-map 'sem-map-utils:parts))
         (dim  (slot-value (gethash obj-name sem-hash) 'sem-map-utils:dimensions)))
    (setf height  (+ (cl-transforms:z dim) 10));;0.5))
    (lambda (x y)
      (declare (ignore x y))
      (list height))))

(defun make-constant-height-function (obj-name height)
  (sem-map-utils::get-semantic-map)
  (let* ((sem-hash (slot-value semantic-map-utils::*cached-semantic-map* 'sem-map-utils:parts))
         (dim  (slot-value (gethash obj-name sem-hash) 'sem-map-utils:dimensions)))
    (setf height  (+ (cl-transforms:z dim) 1))
    (lambda (x y)
      (declare (ignore x y))
      (list height))))

(defun make-costmap-bbox-gen (objs &key invert padding)
(force-ll objs)
 ;;(format t "objs ~a jdjddsj~%" objs)
  (when objs
    (let ((aabbs (loop for obj in (cut:force-ll objs)
                       collecting (btr:aabb obj))))
      (lambda (x y)
        (block nil
          (dolist (bounding-box aabbs (if invert 1.0d0 0.0d0))
            (let* ((bb-center (cl-bullet:bounding-box-center bounding-box))
                   (dimensions-x/2
                     (+ (/ (cl-transforms:x (cl-bullet:bounding-box-dimensions bounding-box)) 2)
                        padding))
                   (dimensions-y/2
                     (+ (/ (cl-transforms:y (cl-bullet:bounding-box-dimensions bounding-box)) 2)
                        padding)))
              (when (and
                     (< x (+ (cl-transforms:x bb-center) dimensions-x/2))
                     (> x (- (cl-transforms:x bb-center) dimensions-x/2))
                     (< y (+ (cl-transforms:y bb-center) dimensions-y/2))
                     (> y (- (cl-transforms:y bb-center) dimensions-y/2)))
                (return (if invert 0.0d0 1.0d0)))))))))) 

(defun make-costmap-bbox-gen-obj (obj-name &key invert padding)
  (let*((sem-hash (slot-value semantic-map-utils::*cached-semantic-map* 'sem-map-utils:parts))
        (sem-obj (gethash obj-name sem-hash))
        (pose (slot-value sem-obj 'cram-semantic-map-utils:pose))
        (dim (slot-value sem-obj 'cram-semantic-map-utils:dimensions))
        (z/2-dim (/ (cl-transforms:z (slot-value sem-obj 'cram-semantic-map-utils:dimensions)) 2)))
    (lambda (a b)
      (block nil
        (let*((bb-center (cl-transforms:make-3d-vector 
                           (cl-transforms:x (cl-transforms:origin pose)) (cl-transforms:y (cl-transforms:origin pose)) (+ (cl-transforms:z (cl-transforms:origin pose)) z/2-dim)))
              (dim-x/2 (+ (/ (cl-transforms:x dim) 2) padding))
              (dim-y/2 (+ (/ (cl-transforms:y dim) 2) padding)))
          (when (and
                 (< a (+ (cl-transforms:x bb-center) dim-x/2))
                 (> a (- (cl-transforms:x bb-center) dim-x/2))
                 (< b (+ (cl-transforms:y bb-center) dim-y/2))
                 (> b (- (cl-transforms:y bb-center) dim-y/2)))
            (return (if invert 0.0d0 1.0d0))))))))

(defun make-location-cost-function (loc std-dev)
  (let ((loc (cl-transforms:origin loc)))
    (make-gauss-cost-function loc `((,(float (* std-dev std-dev) 0.0d0) 0.0d0)
                                    (0.0d0 ,(float (* std-dev std-dev)))))))

(defun get-sem-object-pose->map (object &optional (semantic-map (sem-map-utils::get-semantic-map)));(sem-map-utils::get-semantic-map)))
  (let*((obj (sem-map-utils::semantic-map-part semantic-map object))
       (obj-pose (slot-value obj 'sem-map-utils:pose))
       (obj-pstamped (cl-transforms-stamped:ensure-pose-stamped
                      obj-pose "/map" 0.0)))
       (get-sem-object-transform->relative-map obj-pstamped)))

(defun get-sem-object-pose->genius (object &optional (semantic-map (sem-map-utils::get-semantic-map)))
  (let*((obj (sem-map-utils::semantic-map-part semantic-map object))
       (obj-pose (slot-value obj 'sem-map-utils:pose))
       (obj-pstamped (cl-transforms-stamped:ensure-pose-stamped
                      obj-pose "/map" 0.0)))
       (get-model-pose->relative-genius obj-pstamped)))

(defun get-genius-pose->map-frame (frame-id)
  (roslisp:ros-info (START-MISSION::cost-functions) "Get the position of the genius a link in relation with the world_frame")
 ;; (roslisp-utilities:startup-ros)
  (let((*tf* (make-instance 'cl-tf:transform-listener)))
    (cl-transforms:transform->pose (cl-tf:lookup-transform *tf* "map" frame-id :timeout 2.0))))
  ;;  (cl-transforms-stamped:transform->pose transform)))

(defun get-sem-object-transform->relative-map (obj-stamped)
  (roslisp:ros-info (START-MISSION::cost-functions) "Get the position of the object in relation with the map_frame")
 ;; (roslisp-utilities:startup-ros)
    (cl-transforms-stamped:pose-stamped->pose (cl-tf:transform-pose  *tf* :pose obj-stamped :target-frame "map")))
                                                          
(defun get-model-pose->relative-genius (obj-pst)
  (roslisp:ros-info (START-MISSION::cost-functions) "Get the position of a link in relation with the world_frame")
 ;; (roslisp-utilities:startup-ros)
     (cl-transforms-stamped:pose-stamped->pose (cl-tf:transform-pose  *tf* :pose obj-pst :target-frame "genius_link")))

(defun semantic-map->geom-object (geom-objects object-name)
(let*((geom-list geom-objects)
      (object NIL))
  (loop while (and (/= (length geom-list) 0) (equal object NIL))
	do(cond ((string-equal (slot-value (car geom-list) 'sem-map-utils:name) object-name)
		 (setf object (car geom-list)))
		(t (setf geom-list (cdr geom-list)))))
(list object)))


(defun get-gesture->relative-genius (gesture-vec)
;  (format t "get-gesture->relative-genius~%")
  (let*((pose (cl-tf:make-pose-stamped  "human" 0.0
                                                        gesture-vec
                                                        (cl-transforms:make-identity-rotation))))
 ;   (format t "pose ~a~%" pose)
                                                        ;(cl-transforms:make-quaternion 0 0 -1 1))))
    (cl-transforms-stamped:pose-stamped->pose  (cl-tf:transform-pose *tf* :pose pose :target-frame "map"))))

(defun compare-distance-of-objects (obj-pose genius-pose param)
  (let*((vector (cl-transforms:origin genius-pose))
        (x-vec (cl-transforms:x vector))
        (y-vec (cl-transforms:y vector))
        (z-vec (cl-transforms:z vector))
        (ge-vector (cl-transforms:origin obj-pose))
        (x-ge (cl-transforms:x ge-vector))
        (y-ge (cl-transforms:y ge-vector))
        (z-ge (cl-transforms:z ge-vector))
        (test NIL))
    (if (> param (sqrt (+ (square (- x-vec x-ge))
                          (square (- y-vec y-ge))
                          (square (- z-vec z-ge)))))
     (setf test T)
     (setf test NIL))
    test))

(defun determine-relations (key humanpose)
  (let* ((axis NIL)
         (pred NIL)
         (npose NIL)
         (cvector NIL))
    (cond((equal key :right)
          (setf npose (get-direction-based-on-human (cl-transforms:make-3d-vector 0 -1 0)))
          (setf cvector (cl-transforms:make-3d-vector (- (cl-transforms:x (cl-transforms:origin npose)) (cl-transforms:x (cl-transforms:origin humanpose)))
                (- (cl-transforms:y (cl-transforms:origin npose)) (cl-transforms:y (cl-transforms:origin humanpose)))
                (- (cl-transforms:z (cl-transforms:origin npose)) (cl-transforms:z (cl-transforms:origin humanpose)))))
          (cond ((and (plusp (cl-transforms:x cvector))
                      (> (cl-transforms:x cvector) 0.7)
                      (< (cl-transforms:y cvector) 0.6))
                 (setf axis :X)
                 (setf pred '>))
                ((and (minusp (cl-transforms:x cvector))
                      (< (cl-transforms:x cvector) -0.7)
                      (< (cl-transforms:y cvector) 0.6))
                 (setf axis :X)
                 (setf pred '<))
                ((and (plusp (cl-transforms:y cvector))
                      (> (cl-transforms:y cvector) 0.7)
                      (< (cl-transforms:x cvector) 0.6))
                 (setf axis :Y)
                 (setf pred '>))
                ((and (minusp (cl-transforms:y cvector))
                      (< (cl-transforms:y cvector) -0.7)
                      (< (cl-transforms:x cvector) 0.6))
                 (setf axis :Y)
                 (setf pred '<))
                (t ())))                                 
         ((equal key :left)
          (setf npose (get-direction-based-on-human (cl-transforms:make-3d-vector 0 1 0)))
          (setf cvector (cl-transforms:make-3d-vector (- (cl-transforms:x (cl-transforms:origin npose)) (cl-transforms:x (cl-transforms:origin humanpose)))
                (- (cl-transforms:y (cl-transforms:origin npose)) (cl-transforms:y (cl-transforms:origin humanpose)))
                (- (cl-transforms:z (cl-transforms:origin npose)) (cl-transforms:z (cl-transforms:origin humanpose)))))
          (cond ((and (plusp (cl-transforms:x cvector))
                      (> (cl-transforms:x cvector) 0.7)
                       (< (cl-transforms:y cvector) 0.6))
                 (setf axis :X)
                 (setf pred '>))
                ((and (minusp (cl-transforms:x cvector))
                      (< (cl-transforms:x cvector) -0.7)
                      (< (cl-transforms:y cvector) 0.6))
                 (setf axis :X)
                 (setf pred '<))
                ((and (plusp (cl-transforms:y cvector))
                      (> (cl-transforms:y cvector) 0.7)
                      (< (cl-transforms:x cvector) 0.6))
                 (setf axis :Y)
                 (setf pred '>))
                ((and (minusp (cl-transforms:y cvector))
                      (< (cl-transforms:y cvector) -0.7)
                      (< (cl-transforms:x cvector) 0.6))
                 (setf axis :Y)
                 (setf pred '<))
                (t ())))
        ((equal key :front)
          (setf npose (get-direction-based-on-human (cl-transforms:make-3d-vector 1 0 0)))
          (setf cvector (cl-transforms:make-3d-vector (- (cl-transforms:x (cl-transforms:origin npose)) (cl-transforms:x (cl-transforms:origin humanpose)))
                (- (cl-transforms:y (cl-transforms:origin npose)) (cl-transforms:y (cl-transforms:origin humanpose)))
                (- (cl-transforms:z (cl-transforms:origin npose)) (cl-transforms:z (cl-transforms:origin humanpose)))))
          (cond ((and (plusp (cl-transforms:x cvector))
                      (> (cl-transforms:x cvector) 0.7)
                      (< (cl-transforms:y cvector) 0.6))
                 (setf axis :X)
                 (setf pred '>))
                ((and (minusp (cl-transforms:x cvector))
                      (< (cl-transforms:x cvector) -0.7)
                      (< (cl-transforms:y cvector) 0.6))
                 (setf axis :X)
                 (setf pred '<))
                ((and (plusp (cl-transforms:y cvector))
                      (> (cl-transforms:y cvector) 0.7)
                      (< (cl-transforms:x cvector) 0.6))
                 (setf axis :Y)
                 (setf pred '>))
                ((and (minusp (cl-transforms:y cvector))
                      (< (cl-transforms:y cvector) -0.7)
                      (< (cl-transforms:x cvector) 0.6))
                 (setf axis :Y)
                 (setf pred '<))
                (t ())))      
         ((equal key :behind)
          (setf npose (get-direction-based-on-human (cl-transforms:make-3d-vector -1 0 0)))
          (setf cvector (cl-transforms:make-3d-vector (- (cl-transforms:x (cl-transforms:origin npose)) (cl-transforms:x (cl-transforms:origin humanpose)))
                (- (cl-transforms:y (cl-transforms:origin npose)) (cl-transforms:y (cl-transforms:origin humanpose)))
                (- (cl-transforms:z (cl-transforms:origin npose)) (cl-transforms:z (cl-transforms:origin humanpose)))))
          (cond ((and (plusp (cl-transforms:x cvector))
                      (> (cl-transforms:x cvector) 0.7)
                      (< (cl-transforms:y cvector) 0.6))
                 (setf axis :X)
                 (setf pred '>))
                ((and (minusp (cl-transforms:x cvector))
                      (< (cl-transforms:x cvector) -0.7)
                      (< (cl-transforms:y cvector) 0.6))
                 (setf axis :X)
                 (setf pred '<))
                ((and (plusp (cl-transforms:y cvector))
                      (> (cl-transforms:y cvector) 0.7)
                      (< (cl-transforms:x cvector) 0.6))
                 (setf axis :Y)
                 (setf pred '>))
                ((and (minusp (cl-transforms:y cvector))
                      (< (cl-transforms:y cvector) -0.7)
                      (< (cl-transforms:x cvector) 0.6))
                 (setf axis :Y)
                 (setf pred '<))
                (t ())))
         (t (format t "func(): determine-relations~%")))
    (list axis pred)))

(defun make-consistent-height-function (obj-name height);; &optional (sem-map (sem-map-utils::get-semantic-map)))
  (format t "sem-map ~a~%"*sem-map*)
  (setf sem-hash (slot-value *sem-map* 'sem-map-utils:parts))
  (format t "sem-hash ~a~%" sem-hash)
  (setf sem-hash (copy-hash-table sem-hash)) 
  (setf dim  (slot-value (gethash obj-name sem-hash) 'sem-map-utils:dimensions))
  (format t "dim ~a~%"dim)
    (setf height  (+ (cl-transforms:z dim) 1))
    (lambda (x y)
      (declare (ignore x y))
      (list height)))

(defun make-location-function (loc std-dev)
  (let ((loc (cl-transforms:origin loc)))
    (make-gauss-cost-function loc `((,(float (* std-dev std-dev) 0.0d0) 0.0d0)
                                    (0.0d0 ,(float (* std-dev std-dev)))))))

(defun checking-relation (obj1 obj2 property)
  (let*((sem-hash (get-all-elems-with-local-tf))
        (obj1-pose (gethash obj1 sem-hash))
        (obj2-pose (gethash obj2 sem-hash))
        (tmp NIL))

    (cond ((string-equal property "behind")
         (setf tmp (and (> (cl-transforms:x (cl-transforms:origin obj1-pose))
                       (cl-transforms:x (cl-transforms:origin obj2-pose)))
                        (plusp (cl-transforms:x (cl-transforms:origin obj1-pose))))))
          ((string-equal property "in-front-of")
         (setf tmp (and (< (cl-transforms:x (cl-transforms:origin obj1-pose))
                           (cl-transforms:x (cl-transforms:origin obj2-pose)))
                        (plusp (cl-transforms:x (cl-transforms:origin obj2-pose))))))
        ((string-equal property "right")
         (setf tmp (< (cl-transforms:y (cl-transforms:origin obj1-pose))
                      (cl-transforms:y (cl-transforms:origin obj2-pose)))))
        ((string-equal property "left")
         (setf tmp (> (cl-transforms:y (cl-transforms:origin obj1-pose))
                      (cl-transforms:y (cl-transforms:origin obj2-pose)))))
        ((string-equal property "close-to")
         (if (>= 4 (get-distance obj1-pose obj2-pose))
             (setf tmp T)
             (setf tmp NIL))))
    tmp))
         
(defun make-semantic-map-costmap-human (objects &key invert (padding 0.0))
  "Generates a semantic-map costmap for all `objects'. `objects' is a
list of SEM-MAP-UTILS:SEMANTIC-MAP-GEOMs"
  (let ((costmap-generators (mapcar (lambda (object)
                                      (make-semantic-map-object-costmap-human-generator
                                       object :padding padding))
                                    (cut:force-ll objects))))
    (flet ((invert-matrix (matrix)
             (declare (type cma:double-matrix matrix))
             (dotimes (row (cma:height matrix) matrix)
               (dotimes (column (cma:width matrix))
                 (if (eql (aref matrix row column) 0.0d0)
                     (setf (aref matrix row column) 1.0d0)
                     (setf (aref matrix row column) 0.0d0)))))
           (generator (costmap-metadata matrix)
             (declare (type cma:double-matrix matrix))
             (dolist (generator costmap-generators matrix)
               (setf matrix (funcall generator costmap-metadata matrix)))))
      (make-instance 'map-costmap-generator
        :generator-function (if invert
                                (alexandria:compose #'invert-matrix #'generator)
                                #'generator)))))

(defun make-semantic-map-object-costmap-human-generator (object &key (padding 0.0))
  (declare (type sem-map-utils:semantic-map-geom object))
 ;; (format t "object is ~a~%"  (cl-transforms:pose->transform  (cl-transforms:make-pose (cl-transforms:origin (get-human-elem-pose (sem-map-utils:name object))) (cl-transforms:make-identity-rotation))))
  (let* ((transform (cl-transforms:pose->transform  (cl-transforms:make-pose (cl-transforms:origin (get-human-elem-pose (sem-map-utils:name object))) (cl-transforms:make-identity-rotation))))
         (dimensions (cl-transforms:v+
                      (sem-map-utils:dimensions object)
                      (cl-transforms:make-3d-vector padding padding padding)))
         (pt->obj-transform (cl-transforms:transform-inv transform))
         ;; Since our map is 2d we need to select a z value for our
         ;; point. We just use the pose's z value since it should be
         ;; inside the object.
         (z-value (cl-transforms:z (cl-transforms:translation transform))))
    (destructuring-bind ((obj-min obj-max)
                         (local-min local-max))
        (list (semantic-map-costmap::2d-object-bb dimensions transform)
              (semantic-map-costmap::2d-object-bb dimensions))
      (flet ((generator-function (semantic-map-costmap::costmap-metadata result)
               (with-slots (origin-x origin-y resolution) costmap-metadata
                 ;; For performance reasons, we first check if the point is
                 ;; inside the object's bounding box in map and then check if it
                 ;; really is inside the object.
                 (let ((min-index-x (map-coordinate->array-index
                                     (cl-transforms:x obj-min)
                                     resolution origin-x))
                       (max-index-x (map-coordinate->array-index
                                     (cl-transforms:x obj-max)
                                     resolution origin-x))
                       (min-index-y (map-coordinate->array-index
                                     (cl-transforms:y obj-min)
                                     resolution origin-y))
                       (max-index-y (map-coordinate->array-index
                                     (cl-transforms:y obj-max)
                                     resolution origin-y)))
                   (loop for y-index from min-index-y to max-index-y
                         for y from (- (cl-transforms:y obj-min) resolution)
                           by resolution do
                             (loop for x-index from min-index-x to max-index-x
                                   for x from (- (cl-transforms:x obj-min) resolution)
                                     by resolution do
                                       (when (semantic-map-costmap::inside-aabb
                                              local-min local-max
                                              (cl-transforms:transform-point
                                               pt->obj-transform
                                               (cl-transforms:make-3d-vector
                                                x y z-value)))
                                         (setf (aref result y-index x-index) 1.0d0))))))
               result))
        #'generator-function))))
