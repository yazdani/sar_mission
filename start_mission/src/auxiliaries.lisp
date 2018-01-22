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

(defvar *cached-colorlist* NIL)
(defvar *puby* NIL)

(defun initialize-colorlist ()
  (if (null *cached-colorlist*)
      (setf *cached-colorlist* (json-color-query)))
  *cached-colorlist*)

(defun json-color-query ()
  (let((sem-keys (hash-table-keys (slot-value *sem-map* 'sem-map-utils:parts)))
       (liste '())
       (query))
    (dotimes (index (length sem-keys))
      (let*((name (format NIL "http://knowrob.org/kb/knowrob.owl#~a" (nth index sem-keys))))
        (setf query (cdaar (force-ll
                            (json-prolog:prolog
                             `("owl_has" ,name
                                         "http://knowrob.org/kb/knowrob.owl#hasColor" ("literal" ("type" ?_ ?s)))))))
        (setf liste (cons (list (nth index sem-keys)  (remove #\' (symbol-name query))) liste))))
    (reverse liste)))
 
(defun get-elem-pose (name) ;;&optional (sem-map (sem-map-utils::get-semantic-map)))
  
(format t "starting method~%")
 ;; (let*((pose NIL)
 ;;       (sem-hash (slot-value sem-map 'sem-map-utils:parts))
 ;;       (new-hash (copy-hash-table sem-hash))
 ;;       (sem-keys (hash-table-keys sem-hash)))
  (setf pose NIL)
(format t "pose~%")
;;(setf sem-map (sem-map-utils::get-semantic-map))
(format t "sem-map~a~%" *sem-map*)
  (setf sem-hash (slot-value *sem-map* 'sem-map-utils:parts))
(format t "sem-hash~a~%" sem-hash)
  (setf new-hash (copy-hash-table sem-hash))
(format t "new-hash~a~%" new-hash)
  (setf sem-keys (hash-table-keys sem-hash))
  (format t "inside elem-pose~%")
       (dotimes (i (length sem-keys))
         do(if (string-equal name (nth i sem-keys))
               (setf pose (slot-value (gethash name new-hash) 'sem-map-utils:pose))
               (format t "")))
   pose)

(defun elem-in-list (name)
  (let*((sem-map *sem-map*);(sem-map-utils:get-semantic-map))
        (sem-hash (slot-value sem-map 'sem-map-utils:parts))
      ;;  (new-hash (copy-hash-table sem-hash))
        (sem-keys (hash-table-keys sem-hash))
        (ret NIL))
    (dotimes (index (length sem-keys))
      do (cond ((string-equal name (nth index sem-keys))
               (setf ret T)
               (return))
	      (t (setf ret NIL))));;(format NIL "Error: The NL-object '~a' is not inside the map!" name)))))
    ret))

(defun get-elem-type (name)
 (let*((type NIL)
       (sem-map *sem-map*);(sem-map-utils:get-semantic-map))
       (sem-hash (slot-value sem-map 'sem-map-utils:parts))
       (new-hash (copy-hash-table sem-hash))
       (sem-keys (hash-table-keys sem-hash)))
       (dotimes(i (length sem-keys))
         (cond ((string-equal name (nth i sem-keys))
                (setf type (slot-value (gethash name new-hash)
                                       'cram-semantic-map-utils::type))
                (if (or
		     (string-equal type "bigtree")
		     (string-equal type "biggesttree")
		     (string-equal type "smalltree")
		     (string-equal type "smallbigtree"))
                    (setf type "tree"))
		(if (or
		     (string-equal type "brokepylon")
		     (string-equal type "bluepylon")
		     (string-equal type "redpylon"))
		    (setf type "pylon"))
                (return))
                     (t ())))
   type))

(defun direction-symbol (sym)
  (intern (string-upcase sym) "KEYWORD"))

(defun get-human-elem-pose (object-name)
  (setf cram-tf:*fixed-frame* "human")
(cl-transforms-stamped:transform->pose (cl-tf:lookup-transform *tf* "human" (format NIL "~a_link" object-name))))

(defun get-desig-resolution (action preposition objname)
  (setf tom NIL)
(setf *obj-name* objname)
(if (string-equal preposition "null")
    (setf tom (get-elem-pose objname))
    (let*((desig (make-designator :location `((,(direction-symbol preposition) ,objname))))
          (result NIL)
          (cam (cl-transforms-stamped:make-transform-stamped "human" "camera_depth_frame" 1596.0 (cl-transforms:make-identity-vector) (cl-transforms:make-quaternion 0.0 0.0 1 -0.05)))
          ;;(cam-depth-tf-transform))
          (temp NIL)
          (tmp NIL))
      (setf result (reference desig))
      (setf cram-tf:*fixed-frame* "map")
      (if (equal *puby* NIL)
          ()
          (remove-local-tf-publisher *puby*))
      (setf temp (look-at-object-x (cl-transforms:make-pose (cl-transforms:origin (cl-transforms-stamped:pose-stamped->pose result)) (cl-transforms:orientation (cl-transforms:transform->pose cam)))  (get-human-elem-pose objname)))
      (setf tmp (cl-transforms-stamped:make-pose-stamped "human"
							 0.0 (cl-transforms:origin temp)
							 (cl-transforms:orientation temp)))
      (setf tom (cl-transforms-stamped:pose-stamped->pose (cl-tf:transform-pose *tf* :pose tmp :target-frame "map")))
      (setf *puby* (create-local-tf-publisher tom "test"))))
  tom)

(defun set-rotation-based-elem (goal)
(format t "goal ~a~%" goal)
(cond((and (not (string= "take-pictures" goal))
	   (not (null goal))
	   (not (string-equal "" goal)))
      (let*((quad-pose (cl-transforms:transform->pose (cl-tf:lookup-transform *tf* "map" "base_footprint")))
	    (elem NIL))
	(setf elem (look-at-object-x (cl-transforms:make-pose (cl-transforms:origin quad-pose)
							      (cl-transforms:orientation (cl-transforms:transform->pose 
											  (cam-depth-tf-human-transform )))) 
				     (get-elem-pose goal)))
        (roslisp:make-msg "geometry_msgs/Pose" :orientation
                          (roslisp:make-msg "geometry_msgs/Quaternion"
                                            :x (cl-transforms:x (cl-transforms:orientation elem))
                                            :y (cl-transforms:y (cl-transforms:orientation elem))
                                            :z (cl-transforms:z (cl-transforms:orientation elem))
                                            :w (cl-transforms:w (cl-transforms:orientation elem))))))
     (t (format t "not rotated~%")
	(roslisp:make-msg "geometry_msgs/Pose"))))
      
(defun cam-depth-tf-human-transform ()
  (cl-transforms-stamped:lookup-transform *tf* "human" "camera_depth_frame"))

(defun checking-object-size (name)
  (let*((type (get-elem-type name))
        (large (calculate-big-object type))
        (small (calculate-small-object type))
        (result "no-elem"))
    (dotimes(index (length large))
      (if(string-equal name (nth index large))
	 (setf result "large")))
    (dotimes(index (length small))
      (if(string-equal name (nth index small))
	 (setf result "small")))
    (if(and (equal T (elem-in-list  name))
	    (string-equal result "no-elem"))
       (setf result "middle"))
    result))
        
        
(defun get-all-objects()
  (format t "~a~%" *sem-map*)
  (let*((map *sem-map*)
        (sem-hash (slot-value map 'sem-map-utils:parts)))
   ;; (format t "~a~%" sem-hash)
    (hash-table-keys sem-hash)))

(defun get-objs-with-victim())

(defun get-objs-infrontof-human ()
(let((liste '()))
  (dotimes (index 40)
    (if (>= 10 (length liste))
        (setf liste (get-elems-infrontof-human index))
        (return)))
  (reverse liste)))

(defun get-all-elems-with-local-tf ()
  (format t "get-all-elems-with-local-tf ~%")
  (let* ((sem-map *sem-map*)
         (sem-hash (slot-value sem-map 'sem-map-utils:parts))
         (sem-keys (hash-table-keys sem-hash))
        ;; (semm-hash (copy-hash-table sem-hash))
         (new-hash (make-hash-table))(name NIL)
         (pub NIL)(pose NIL)(obj-pub NIL)(obj-pose NIL))
    (dotimes (index (length sem-keys))
      (setf name (format NIL "~a_link" (nth index sem-keys)))

      (setf obj-pose (cl-transforms-stamped:transform->pose (cl-tf:lookup-transform *tf* "human" name)))
  
      (setf (gethash (nth index sem-keys) new-hash) obj-pose))

    (copy-hash-table new-hash)))

(defun get-elem-color (name)
  (initialize-colorlist)
     (let*((color NIL))
       (dotimes (index (length *cached-colorlist*))
         (if (string-equal name (first (nth index *cached-colorlist*))) 
             (setf color (second (nth index *cached-colorlist*)))))
       color))

   ;;     (sem-hash (copy-hash-table (slot-value map 'sem-map-utils:parts)))
   ;;     (sem-keys (hash-table-keys sem-hash)))
   ;; (loop for index from 0 to (- (length sem-keys) 1)
   ;;                      do (setf liste (cons (gethash (nth index sem-keys) sem-hash) liste)))
 ;;   (objects-next-human 10 map)))
                         
                         
        ;(defun get-objects-closer-human (geom-objects param genius-pose)

;;
;; Getting the biggest object inside the map
;;
(defun calculate-big-object (nom)
  (let*((sem-map *sem-map*);;(sem-map-utils:get-semantic-map))
        (sem-hash (slot-value sem-map 'sem-map-utils:parts))
        (new-hash (copy-hash-table sem-hash))
        (sem-keys (hash-table-keys sem-hash))
        (name NIL)
        (list-keys '())
        (value 0)
        (result NIL))

 ;;   (cond ((null (elem-in-list nom)))
 ;;         (t 
             (dotimes (mass (length sem-keys))
             do(if(string-equal (get-elem-type (nth mass sem-keys)) nom)
                  (setf list-keys (cons (nth mass sem-keys) list-keys)))
)
           (dotimes (index (length list-keys))
             do(cond((< value (bbox-volume (nth index list-keys) new-hash))
                     (setf name (nth index list-keys))
                     (setf value (bbox-volume (nth index list-keys) new-hash))
                     )
                    (t())))
  (dotimes (index (length list-keys))
      do(cond((equal value (bbox-volume (nth index list-keys) new-hash))
              (setf result (cons (nth index list-keys) result)))
             (t ())))
result))


(defun bbox-volume (name sem-hash)
 (let*((dim-x (cl-transforms:x (slot-value (gethash name sem-hash) 'sem-map-utils:dimensions)))
      (dim-y (cl-transforms:y (slot-value (gethash name sem-hash) 'sem-map-utils:dimensions)))
      (dim-z (cl-transforms:z (slot-value (gethash name sem-hash) 'sem-map-utils:dimensions))))
(* dim-x dim-y dim-z)))
;;
;; Getting the smallest object inside the map
;;
(defun calculate-small-object (type)
  (let*((sem-map *sem-map*);;(sem-map-utils:get-semantic-map))
        (sem-hash (slot-value sem-map 'sem-map-utils:parts))
        (sem-keys (hash-table-keys sem-hash))
        (new-hash (copy-hash-table sem-hash))
        (name NIL)
        (result '())
        (list-keys '())
        (value 1000000))
;;    (cond ((null (elem-in-list type))
;;           (setf result (format t  "No ~a is in the map~%" type)))
;;          (t
(if (or (string-equal type "bigtree")
		       (string-equal type "smalltree")
		       (string-equal type "smallbigtree"))
                   (setf type "tree"))
(format t "sem-keys ~a~%" sem-keys)
    (dotimes (mass (length sem-keys))
      do(if(string-equal (get-elem-type (nth mass sem-keys)) type)
           (setf list-keys (cons (nth mass sem-keys) list-keys))
           (format t "")))
(format t "list-keys ~a~%" list-keys)
    (dotimes (index (length list-keys))
      do(cond((> value (bbox-volume (nth index list-keys) new-hash))
              (setf name (nth index list-keys))
              (setf value (bbox-volume (nth index list-keys) new-hash)))
             (t ()))) ;;))
   (dotimes (index (length list-keys))
      do(cond((equal value (bbox-volume (nth index list-keys) new-hash))
              (setf result (cons (nth index list-keys) result)))
             (t ())))
result))


;;
;; Calculating the sum of the bounding box
;;
(defun bbox-sum (name sem-hash)
  (let*((liste (get-bbox-as-aabb name sem-hash))
        (erst (first liste))
        (zweit (second liste))
        (sum NIL))
    (setf sum (+ (+ (cl-transforms:x erst) (cl-transforms:x zweit))
                 (+ (cl-transforms:y erst) (cl-transforms:y zweit))
                 (+ (cl-transforms:z erst) (cl-transforms:z zweit))))
    (if (minusp sum)
        (setf sum (* sum -1)))
    sum))

;;
;; Calculating all objects 
;; which are close to the human in the direction where human is looking at
;;
(defun objects-next-human (distance sem-map)
  ;(setf puber (swm->intern-tf-creater))
  (let* ((new-liste (visualize-plane distance))
         (sem-hash (slot-value sem-map 'sem-map-utils:parts))
         (smarter NIL)
         (sem-keys (hash-table-keys sem-hash))
         (elem '()))
   ; (format t "teeeest ~%")
    (setf smarter 0)
       ;;  (incrementer 1)
      ;;   (value NIL))
    ;(swm->intern-tf-remover puber)
    (dotimes (index (length new-liste))
      do; (format t "objects-next-human~%")
      (let*((new-point (nth index new-liste)))
	   ;   (smarter (+ (* 10 incrementer) index)))
	  (dotimes (jndex (length sem-keys))
		do(let* ((elem1 (first (get-bbox-as-aabb (nth jndex sem-keys) sem-hash)))
             (elem2 (second (get-bbox-as-aabb (nth jndex sem-keys) sem-hash))))
		  (setf smarter (+ smarter jndex))
		;    (setf value
	;		  (semantic-map-costmap::inside-aabb elem1 elem2 (cl-transforms:origin new-point)))
    ;    (format t "  ~a~%" (nth jndex sem-keys))
		    (cond ((equal (semantic-map-costmap::inside-aabb elem1 elem2 (cl-transforms:origin new-point)) T)
          (setf elem (append (list (nth jndex sem-keys)) elem))
              ;; (setf elem (cons (concatenate 'string (nth jndex sem-keys) " - " (write-to-string (get-distance (get-elem-pose (nth jndex sem-keys)) (cl-transforms-stamped:transform->pose (human-relative-map-pose))))) elem))
         ;;      (format t "elem ~a~%" elem)
         ;;      (format t " ~a~%"(nth jndex sem-keys))
               
		   (location-costmap::publish-point (cl-transforms:origin new-point) :id smarter)
			  ; (remove-duplicates elem)
			   )
			  (t   (location-costmap::publish-point (cl-transforms:origin new-point) :id smarter)
			   ))) )))
		;;  (setf incrementer (+ incrementer 2)))))
    (reverse (remove-duplicates elem))))

;;
;;Getting the min and max values of a bounding box
;;
(defun get-bbox-as-aabb (name sem-hash)
(let*((dim-x (cl-transforms:x (slot-value (gethash name sem-hash) 'sem-map-utils:dimensions)))
      (dim-y (cl-transforms:y (slot-value (gethash name sem-hash) 'sem-map-utils:dimensions)))
      (dim-z (cl-transforms:z (slot-value (gethash name sem-hash) 'sem-map-utils:dimensions)))
      (pose-x (cl-transforms:x (cl-transforms:origin  (slot-value (gethash name sem-hash) 'sem-map-utils:pose))))
       (pose-y (cl-transforms:y (cl-transforms:origin  (slot-value (gethash name sem-hash) 'sem-map-utils:pose))))
      (min-vec (cl-transforms:make-3d-vector (- pose-x (/ dim-x 2))
                                             (- pose-y (/ dim-y 2))
                                             0))
      (max-vec (cl-transforms:make-3d-vector (+ pose-x (/ dim-x 2))
                                             (+ pose-y (/ dim-y 2))
                                             dim-z)))
  (cram-semantic-map-costmap::get-aabb min-vec max-vec)))


;;
;; Getting all the objects around the rescuer...
;;
(defun get-objects-around-human (geom-objects param genius-pose)
  (let*((geom-list geom-objects)
        (objects '())
        (counter 0))
     (dotimes (index (length geom-list)) 
       do (setf counter (+ counter 1))
       (cond ((and T
                   (compare-distance-of-objects (slot-value (nth index geom-list) 'sem-map-utils:pose) genius-pose param))
           (publish-pose  (slot-value (nth index geom-list) 'sem-map-utils:pose) :id counter)
              ;;(format t "counter ~a~%" counter)
		       (setf objects (cons (slot-value (nth index geom-list) 'sem-map-utils:name) objects)))
		(t  (publish-pose  (slot-value (nth index geom-list) 'sem-map-utils:pose) :id counter)
       )))
    (remove-duplicates  objects)))

(defun get-geom-objects (geom-objects param object-pose)
  (let*((geom-list geom-objects)
	(objects NIL))
    (loop while (/= (length geom-list) 0) 
	  do(cond ((and T
			(compare-distance-of-objects (slot-value (car geom-list) 'sem-map-utils:pose) object-pose param))
            ;; (format t "name ~a~%"  (slot-value (car geom-list) 'sem-map-utils:name))
		   (setf objects
			 (append objects (list (first geom-list))))
		   (setf geom-list (cdr geom-list)))
		  (t (setf geom-list (cdr geom-list)))))
objects))


(defun compare-distance-of-objects (obj_position pose param)
  (let*((vector (cl-transforms:origin pose))
        (x-vec (cl-transforms:x vector))
        (y-vec (cl-transforms:y vector))
        (z-vec (cl-transforms:z vector))
        (ge-vector (cl-transforms:origin obj_position))
        (x-ge (cl-transforms:x ge-vector))
        (y-ge (cl-transforms:y ge-vector))
        (z-ge (cl-transforms:z ge-vector))
        (test NIL))
    (if (>= param (sqrt (+ (square (- x-vec x-ge))
                          (square (- y-vec y-ge))
                          (square (- z-vec z-ge)))))
     (setf test T)
     (setf test NIL))
    test))


(defun visualize-plane (num)
  (format t "visualize-plane ~%")
  (let* ((temp '()))
    (loop for jindex from 1 to num
         ; do;(loop for smart from 0 to 1 ;;5
                  do(loop for mass from 1 to 21 
                   do
                  ;;    (let*((new-point (get-gesture->relative-genius
                   ;;                       (cl-transforms:make-3d-vector
                    ;;                        jindex  (- mass 11)(- smart 1))))) ;;5
                       
                                 (setf temp (cons (get-gesture->relative-genius
                                          (cl-transforms:make-3d-vector
                                            jindex  (- mass 11) 0)) temp))))
                 (reverse temp))) 



;;################################ POINTING GESTURE CALCULATIONS########################################;;

(defun give-pointed-at-no-bbox (point sem-map)
  (let*((elem NIL)
       (num (make-list 100))
      ; (eps 0)(var 0)
       (sem-hash (slot-value sem-map 'sem-map-utils:parts))
       (sem-keys (hash-table-keys sem-hash))
       (liste-tr (list-values num point))
       (liste-up (all-ups liste-tr))
       (liste-down (all-downs liste-tr))
       (liste-right (all-rights liste-tr))
       (liste-left (all-lefts liste-tr))
       (liste-front (all-fronts liste-tr))
       (liste-back (all-backs liste-tr))
       ;(value NIL)
        )
    (dotimes (jindex (length liste-tr))
      do (dotimes(jo (length sem-keys))
          do(let* ((pose (cl-transforms:origin (slot-value (gethash (nth jo sem-keys) sem-hash) 'sem-map-utils:pose)))
                   (npoint (cl-transforms:origin (nth jindex liste-tr)))
                   (upoint (cl-transforms:origin (nth jindex liste-up)))
                   (dpoint (cl-transforms:origin (nth jindex liste-down)))
                   (rpoint (cl-transforms:origin (nth jindex liste-right)))
                   (lpoint (cl-transforms:origin (nth jindex liste-left)))
                   (fpoint (cl-transforms:origin (nth jindex liste-front)))
                   (bpoint (cl-transforms:origin (nth jindex liste-back)))
                   (value (checker-at-distance pose npoint))
                   (uvalue (checker-at-distance pose upoint))
                   (dvalue (checker-at-distance pose dpoint))
                   (rvalue (checker-at-distance pose rpoint))
                   (lvalue (checker-at-distance pose lpoint))
                   (fvalue (checker-at-distance pose fpoint))
                   (bvalue (checker-at-distance pose  bpoint)))
                  (cond ((and (or (equal value T)
                                  (equal uvalue T)
                                  (equal dvalue T)
                                  (equal rvalue T)
                                  (equal lvalue T)
                                  (equal fvalue T)
                                  (equal bvalue T))
                             (not (equal (nth jo sem-keys)
                                         (find (nth jo sem-keys)
                                           elem :test #'equal))))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-tr)) :id (+ (+ jo jindex) 1000))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-right)) :id (+ (+ jo jindex) 2000))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-left)) :id (+ (+ jo jindex) 3000))
                         (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-front)) :id (+ (+ jo jindex) 4000))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-back )) :id (+ (+ jo jindex) 5000))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-up)) :id (+ (+ jo jindex) 6000))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-down)) :id (+ (+ jo jindex) 7000))
                        (setf elem (append (list (nth jo sem-keys)) elem)))
                        
                       (t
                    ;    (format t "~a~% und ~a~%" rpoint lpoint)
                         (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-tr)) :id (+ (+ jo jindex) 11000))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-right)) :id (+ (+ jo jindex) 22000))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-left)) :id (+ (+ jo jindex) 33000))
                         (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-front)) :id (+ (+ jo jindex) 44000))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-back )) :id (+ (+ jo jindex) 55000))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-up)) :id (+ (+ jo jindex) 66000))
                        (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-down)) :id (+ (+ jo jindex) 77000))                      
                          )))))
            ;(return)))
     
  ;            (swm->intern-tf-remover *puber*)
             (reverse elem)))

(defun hash-table-keys (hash-table)
                   "Return a list of keys in HASH-TABLE."
                   (let ((keys '()))
                     (maphash (lambda (k _v) (push k keys)) hash-table)
                     keys))

(defun give-calculated-obj-pointed-at (point sem-map)
  (let*((elem NIL)
        (num (make-list 100))
       ; (eps 0)(var 0)
        (sem-hash (slot-value sem-map 'sem-map-utils:parts))
        (sem-keys (hash-table-keys sem-hash))
        (liste-tr (list-values num point))
        (liste-up (all-points-ups liste-tr))
        (liste-down (all-points-downs liste-tr))
        (liste-right (all-points-rights liste-tr))
        (liste-left (all-points-lefts liste-tr))
        (liste-front (all-points-fronts liste-tr))
        (liste-back (all-points-backs liste-tr))
        ;(value NIL)
        )
    ;;(format t "tetete~%")
    (dotimes (jindex (length liste-tr))
      do (dotimes(jo (length sem-keys))
           do(let* ((all (get-bbox-as-aabb (nth jo sem-keys) sem-hash))
                    (elem1 (first all))
                    (elem2 (second all))
                    (npoint (cl-transforms:origin (nth jindex liste-tr)))
                    (upoint (cl-transforms:origin (nth jindex liste-up)))
                    (dpoint (cl-transforms:origin (nth jindex liste-down)))
                    (rpoint (cl-transforms:origin (nth jindex liste-right)))
                    (lpoint (cl-transforms:origin (nth jindex liste-left)))
                    (fpoint (cl-transforms:origin (nth jindex liste-front)))
                    (bpoint (cl-transforms:origin (nth jindex liste-back)))
                    (value (semantic-map-costmap::inside-aabb elem1 elem2 npoint))
                    (uvalue (semantic-map-costmap::inside-aabb elem1 elem2 upoint))
                    (dvalue (semantic-map-costmap::inside-aabb elem1 elem2 dpoint))
                    (rvalue (semantic-map-costmap::inside-aabb elem1 elem2 rpoint))
                    (lvalue (semantic-map-costmap::inside-aabb elem1 elem2 lpoint))
                    (fvalue (semantic-map-costmap::inside-aabb elem1 elem2 fpoint))
                    (bvalue (semantic-map-costmap::inside-aabb elem1 elem2 bpoint)))
               (cond ((and (or (equal value T)
                               (equal uvalue T)
                               (equal dvalue T)
                               (equal rvalue T)
                               (equal lvalue T)
                               (equal fvalue T)
                               (equal bvalue T))
                           (not (equal (nth jo sem-keys)
                                       (find (nth jo sem-keys)
                                             elem :test #'equal))))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-tr)) :id (+ (+ jo jindex) 1000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-right)) :id (+ (+ jo jindex) 2000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-left)) :id (+ (+ jo jindex) 3000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-front)) :id (+ (+ jo jindex) 4000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-back )) :id (+ (+ jo jindex) 5000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-up)) :id (+ (+ jo jindex) 6000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-down)) :id (+ (+ jo jindex) 7000))
                      (setf elem (append (list (nth jo sem-keys)) elem)))
                     (t
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-tr)) :id (+ (+ jo jindex) 11000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-right)) :id (+ (+ jo jindex) 22000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-left)) :id (+ (+ jo jindex) 33000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-front)) :id (+ (+ jo jindex) 44000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-back )) :id (+ (+ jo jindex) 55000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-up)) :id (+ (+ jo jindex) 66000))
                      (location-costmap:publish-point  (cl-transforms:origin (nth jindex liste-down)) :id (+ (+ jo jindex) 77000)))))))
    ;;(return)))
    ;;(swm->intern-tf-remover *puber*)
             (reverse elem)))

(defun human-relative-map-pose ()
  (cl-transforms:transform->pose (tf:lookup-transform *tf* "map" "human")))

(defun list-values (num point)
  (let*((zet 1.0)
        (iks (cl-transforms:x point))
        (yps (cl-transforms:y point))
        (liste-tr NIL)
        (humanpose (cl-transforms:transform->pose (human-relative-map-pose)))
        (temp '()))
    (cond((and (>= iks 0) 
               (<= yps 0))
          (setf liste-tr (function-up-right point iks yps zet num liste-tr humanpose)))
         ((and (<= iks 0)
               (<= yps 0))
          (setf liste-tr (function-down-right point iks yps zet num liste-tr humanpose)))
         ((and (<= iks 0) ;<=
               (>= yps 0)) ;>=
          (setf liste-tr (function-down-left point iks yps zet num liste-tr humanpose)))
         ((and (>= iks 0)
               (>= yps 0)) ;>=
          (setf liste-tr (function-up-left point iks yps zet num liste-tr humanpose)))
         (t()))
    (dotimes (index (length liste-tr))
     (setf temp (cons (cl-transforms:make-pose
                       (cl-transforms:origin (nth index liste-tr))
                       (cl-transforms:make-identity-rotation)) temp)))
    (reverse temp)))

(defun function-up-left (point iks yps zet num liste-tr humanpose)
  (cond((and (> iks 0)
              (<= iks 0.1) 
              (> yps 0))
          (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector  (+ (cl-transforms:x point)  (* 0.1 index))  (+ (cl-transforms:y point) (* 0.5 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
        ((and (> iks 0.1)
              (<= iks 0.3) 
              (> yps 0))
          (dotimes(index (length num))
            do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector  (+ (cl-transforms:x point)  (* 0.25 index))  (+ (cl-transforms:y point) (* 0.5 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
        ((and (> iks 0.3)
              (<= iks 0.5) 
              (> yps 0))
         (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector  (+ (cl-transforms:x point)  (* 0.4 index))  (+ (cl-transforms:y point) (* 0.5 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
        ((and (> iks 0.5)
              (<= iks 0.7) 
              (> yps 0))
         (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector  (+ (cl-transforms:x point)  (* 0.6 index))  (+ (cl-transforms:y point) (* 0.4 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
        ((and (> iks 0.7)
              (<= iks 0.8) 
              (> yps 0))
         (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector  (+ (cl-transforms:x point)  (* 0.7 index))  (+ (cl-transforms:y point) (* 0.3 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
        ((and (> iks 0.8)
              (<= iks 0.9) 
              (> yps 0))
         (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector  (+ (cl-transforms:x point)  (* 0.8 index))  (+ (cl-transforms:y point) (* 0.2 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
        ((and (> iks 0.9)
              (<= iks 1) 
              (> yps 0))
         (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector  (+ (cl-transforms:x point)  (* 0.9 index))  (+ (cl-transforms:y point) (* 0.1 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
        (t()))
  liste-tr
  )

  (defun function-down-left (point iks yps zet num liste-tr humanpose)
    ;(format t "func-down-left~%")
    (cond((and (< iks 0) 
               (> yps 0)
               (<= yps 0.1))
          (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.85) index))  (+ (cl-transforms:y point)(* 0.1 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
         ((and (< iks 0) 
               (> yps 0.1)
               (<= yps 0.3))
          (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.7) index))  (+ (cl-transforms:y point)(* 0.25 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
         ((and (< iks 0) 
               (> yps 0.3)
               (<= yps 0.4))
          (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.6) index))  (+ (cl-transforms:y point)(* 0.4 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
    ((and (< iks 0) 
               (> yps 0.4)
               (<= yps 0.5))
          (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.4) index))  (+ (cl-transforms:y point)(* 0.6 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
    ((and (< iks 0) 
               (> yps 0.5)
               (<= yps 0.6))
     (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.3) index))  (+ (cl-transforms:y point)(* 0.7 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
    ((and (< iks 0) 
          (> yps 0.6)
          (<= yps 0.7))
     (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.2) index))  (+ (cl-transforms:y point)(* 0.8 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
    ((and (< iks 0) 
          (> yps 0.7)
          (<= yps 0.9))
     (dotimes(index (length num))
       do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.1) index))  (+ (cl-transforms:y point)(* 0.9 index))  (+ (cl-transforms:z point) zet)) humanpose))))))
    ((and (= iks 0) 
          (> yps 0))
     (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (cl-transforms:x point)  (+ (cl-transforms:y point)(* 0.5 index))  (+ (cl-transforms:z point) zet))humanpose))))))
             (t ()))
             liste-tr)
 
(defun function-down-right (point iks yps zet num liste-tr humanpose)
 ; (format t "func down rohjt ~%")
  (cond((and (< iks 0) 
             (= yps 0))
        (dotimes(index (length num))
        do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.5) index))  (cl-transforms:y point)  (+ (cl-transforms:z point) zet))humanpose))))))
       ((and (< iks 0) 
             (> yps -1)
             (<= yps -0.9))
        (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.1) index))  (+ (cl-transforms:y point)(* (- 0.9) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
       ((and (< iks 0) 
             (> yps -1)
             (> yps -0.9)
             (<= yps -0.8))
        (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.2) index))  (+ (cl-transforms:y point)(* (- 0.8) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
              ((and (< iks 0) 
             (> yps -1)
             (> yps -0.8)
             (<= yps -0.6))
               (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.3) index))  (+ (cl-transforms:y point)(* (- 0.7) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
              ((and (< iks 0) 
             (> yps -1)
             (> yps -0.6)
             (<= yps -0.5))
          (dotimes(index (length num))
            do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.4) index))  (+ (cl-transforms:y point)(* (- 0.6) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
               ((and (< iks 0) 
             (> yps -1)
             (> yps -0.5)
             (<= yps -0.4))
                (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.5) index))  (+ (cl-transforms:y point)(* (- 0.5) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
               ((and (< iks 0) 
                     (> yps -1)
                     (> yps -0.4)
                     (<= yps -0.3))
                (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.6) index))  (+ (cl-transforms:y point)(* (- 0.4) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
                    ((and (< iks 0) 
                     (> yps -1)
                     (> yps -0.3)
                     (<= yps -0.2))
                     (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.7) index))  (+ (cl-transforms:y point)(* (- 0.3) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
                         ((and (< iks 0) 
                     (> yps -1)
                     (> yps -0.2)
                     (<= yps -0.1))
                          (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* (- 0.85) index))  (+ (cl-transforms:y point)(* (- 0.15) index))  (+ (cl-transforms:z point) zet)) humanpose))))))                    
             (t ()))
             liste-tr)  

(defun function-up-right (point iks yps zet num liste-tr humanpose)
  (cond((and (> iks 0)
             (= yps 0))
        (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* 0.5 index))  (cl-transforms:y point)  (+ (cl-transforms:z point) zet)) humanpose))))))
         ((and (> iks 0) ;;ok
               (< yps 0)
               (>= yps (- 0.2)))
          (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* 0.8 index))  (+ (cl-transforms:y point)(* (- 0.12) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
         ((and (> iks 0) ;;ok
               (< yps 0)
               (< yps (- 0.2))
               (>= yps (- 0.4)))
          (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* 0.8 index))  (+ (cl-transforms:y point)(* (- 0.25) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
         ((and (> iks 0) ;;ok
               (< yps 0)
               (< yps (- 0.4))
               (>= yps (- 0.5)))
          (dotimes(index (length num))
            do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* 0.5 index))  (+ (cl-transforms:y point)(* (- 0.265) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
  ((and (> iks 0) ;;ok
               (< yps 0)
               (< yps (- 0.5))
               (>= yps (- 0.6)))
           (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* 0.5 index))  (+ (cl-transforms:y point)(* (- 0.35) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
  ((and (> iks 0) ;;ok
                (< yps 0)
                (< yps (- 0.6))
                (>= yps (- 0.7)))
          (dotimes(index (length num))
           do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* 0.5 index))  (+ (cl-transforms:y point)(* (- 0.55) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
          ((and (> iks 0) ;;ok
                (< yps 0)
                (< yps (- 0.7))
                (> yps (- 0.8)))
           (dotimes(index (length num))
             do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* 0.5 index))  (+ (cl-transforms:y point)(* (- 0.6) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
          ((and (> iks 0) ;;ok
                (< yps 0)
               (<= yps (- 0.8))
               (> yps (- 0.9)))
           (dotimes(index (length num))
             do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* 0.35 index))  (+ (cl-transforms:y point)(* (- 0.7) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
               ((and (> iks 0) ;;ok
                     (< yps 0)
               (<= yps (- 0.9))
               (>= yps (- 1)))
                (dotimes(index (length num))
                  do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (+ (cl-transforms:x point) (* 0.2 index))  (+ (cl-transforms:y point)(*  (- 0.99) index))  (+ (cl-transforms:z point) zet)) humanpose))))))             
             ((and (= iks 0) ;;ok
                   (< yps 0))
             (dotimes(index (length num))
                do (setf liste-tr (append liste-tr (list (get-gesture->relative-world (cl-transforms:make-3d-vector (cl-transforms:x point)  (+ (cl-transforms:y point)(* (- 0.4) index))  (+ (cl-transforms:z point) zet)) humanpose))))))
             (t ()))
             liste-tr)

(defun all-points-ups (liste)
  (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
            (cl-transforms:make-pose
             (cl-transforms:make-3d-vector (cl-transforms:x (cl-transforms:origin (nth index liste)))
                                           (cl-transforms:y (cl-transforms:origin (nth index liste)))
                                           (+ (cl-transforms:z (cl-transforms:origin (nth index liste))) 1))
             (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

(defun all-points-downs (liste)
   ; (format t "all-downs~%")
   (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
            (cl-transforms:make-pose
             (cl-transforms:make-3d-vector (cl-transforms:x (cl-transforms:origin (nth index liste)))
                                           (cl-transforms:y (cl-transforms:origin (nth index liste)))
                                           (- (cl-transforms:z (cl-transforms:origin (nth index liste))) 1))
             (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

(defun all-points-rights (liste)
  (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
                  (cl-transforms:make-pose
                   (cl-transforms:make-3d-vector (cl-transforms:x (cl-transforms:origin (nth index liste)))
                                                 (+ (cl-transforms:y (cl-transforms:origin (nth index liste))) 1)
                                                 (cl-transforms:z (cl-transforms:origin (nth index liste))))
                   (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

(defun all-points-lefts (liste)
 ; (format t "all-lefts~%")
    (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
            (cl-transforms:make-pose
             (cl-transforms:make-3d-vector (cl-transforms:x (cl-transforms:origin (nth index liste)))
                                           (- (cl-transforms:y (cl-transforms:origin (nth index liste))) 1)
                                           (cl-transforms:z (cl-transforms:origin (nth index liste))))
             (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

(defun all-points-fronts (liste)
  ;(format t "all-fronts~%")
  (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
            (cl-transforms:make-pose
             (cl-transforms:make-3d-vector (+ (cl-transforms:x (cl-transforms:origin (nth index liste))) 1)
                                           (cl-transforms:y (cl-transforms:origin (nth index liste)))
                                           (cl-transforms:z (cl-transforms:origin (nth index liste))))
            (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

(defun all-points-backs (liste)
  ;(format t "all-backs~%")
    (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
            (cl-transforms:make-pose
             (cl-transforms:make-3d-vector (- (cl-transforms:x (cl-transforms:origin (nth index liste))) 1)
                                           (cl-transforms:y (cl-transforms:origin (nth index liste)))
                                           (cl-transforms:z (cl-transforms:origin (nth index liste))))
             (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))


;;##########################CHECKING THE CALCULATED OBJECT#################################;;
(defun checking-pointed-object (elem gesture sem-map)
  ;;(format t "checker-of-pointer~%")
  (let*((sym1 NIL)
        (sym2 NIL)
        (sym NIL))
    (cond ((not (equal NIL (elem-in-list elem)))
           (setf sym (elem-in-list elem)))
          (t (setf sym1 (give-calculated-obj-pointed-at gesture sem-map))
             (if (equal sym1 NIL)
                 (setf sym2 (objects-close-to-gesture gesture sem-map))
                 (format t ""))
             (cond ((> (length sym1) 0)
                    (dotimes(index (length sym1))
                      do(cond((and (string-equal elem  (get-elem-type (nth index sym1)))
                                   (equal sym NIL))
                              (setf sym (nth index sym1)))
                             (t ()))))
                   ((and (= (length sym1) 0)(> (length sym2) 0))
                    (dotimes(index (length sym2))
                      do(cond((and (string-equal elem  (get-elem-type (nth index sym2)))
                                   (equal sym NIL))
                              (setf sym (nth index sym2)))
                             (t ()))))
                   (t ()))))
    sym))


;;############################GROUND FOR POINTING GESTURE########################################;;
;; GROUND FOR GESTURE

(defun objects-close-to-gesture (vec sem-map)
 ; (setf puber (swm->intern-tf-creater))
  (let* ((elem NIL)
         (sem-hash (slot-value sem-map 'sem-map-utils:parts))
         (sem-keys (hash-table-keys sem-hash))
         (incrementer 0)(value NIL)
         (num (make-list 300))
         (valuable (list-values num vec)))
    (let*((liste (five-down-levels valuable)))
      (dotimes (mo (length liste))
        do (let((new-point (nth mo liste)))
               ;;  (smarter (+ (* 10 incrementer) 2))
             (dotimes (jndex (length sem-keys))
               do(let* ((elem1 (first (get-bbox-as-aabb (nth jndex sem-keys) sem-hash)))
                        (elem2 (second (get-bbox-as-aabb (nth jndex sem-keys) sem-hash))))
                   ;;  (smarter (+ smarter jndex)))
                   (setf value
                         (semantic-map-costmap::inside-aabb elem1 elem2  (cl-transforms:origin new-point)))
                   (cond ((equal value T)
                          ;;  (location-costmap::publish-point (cl-transforms:origin new-point) :id smarter)
                          (setf elem (append (list (nth jndex sem-keys)) elem))
                          (remove-duplicates elem))
                         (t ;;  (location-costmap::publish-point (cl-transforms:origin new-point) :id smarter)
                          )))
               (setf incrementer (+ incrementer 2))))))
    ;;  (swm->intern-tf-remover puber)
             (reverse (remove-duplicates elem)))) 

(defun five-down-levels (liste)
   (let*((test '()))
         (dotimes (index (length liste))
           do(loop for mass from 1 to 21
                   do(loop for jindex from 1 to 11
                           do(setf test (cons 
                                         (cl-transforms:make-pose
                                          (cl-transforms:make-3d-vector (cl-transforms:x (cl-transforms:origin (nth index liste)))
                                                          (+ (cl-transforms:y (cl-transforms:origin (nth index liste))) (- 11 mass)) 
                                                          (+ (cl-transforms:z (cl-transforms:origin (nth index liste))) (- 6 jindex)))
                                            (cl-transforms:orientation (nth index liste))) test))
                (setf test (cons 
                            (cl-transforms:make-pose
                            (cl-transforms:make-3d-vector (+ (cl-transforms:x (cl-transforms:origin (nth index liste))) (- 11 mass))
                                                          (cl-transforms:y (cl-transforms:origin (nth index liste)))  
                                                          (+ (cl-transforms:z (cl-transforms:origin (nth index liste))) (- 6 jindex)))
                              (cl-transforms:orientation (nth index liste))) test)))))
                                   
    (reverse test)))

(defun get-gesture->relative-world (gesture-vec humanpose)
  (let*((ori (cl-transforms:make-3d-vector (+ (cl-transforms:x (cl-transforms:origin humanpose)) (cl-transforms:x gesture-vec))
                                          (+ (cl-transforms:y (cl-transforms:origin humanpose)) (cl-transforms:y gesture-vec))
                                          (+ (cl-transforms:z (cl-transforms:origin humanpose)) (cl-transforms:z gesture-vec))))
        (pose  (cl-transforms-stamped:make-pose ori (cl-transforms:orientation humanpose))))
    pose))

(defun checker-at-distance (ffpoint sspoint)
  (let*((spoint (cl-transforms:make-pose sspoint (cl-transforms:make-identity-rotation)))
        (fpoint (cl-transforms:make-pose ffpoint (cl-transforms:make-identity-rotation)))
        (fsec (sqrt (+ (square (- (cl-transforms:x (cl-transforms:origin spoint))
                                   (cl-transforms:x (cl-transforms:origin fpoint))))
                        (square (- (cl-transforms:y (cl-transforms:origin spoint))
                                   (cl-transforms:y (cl-transforms:origin fpoint))))
                        (square (- (cl-transforms:z (cl-transforms:origin spoint))
                                   (cl-transforms:z (cl-transforms:origin fpoint)))))))
        (forw2 (sqrt (+ (square (- (cl-transforms:x (cl-transforms:origin spoint))
                                   (+ (cl-transforms:x (cl-transforms:origin fpoint)) 1)))
                        (square (- (cl-transforms:y (cl-transforms:origin spoint))
                                   (cl-transforms:y (cl-transforms:origin fpoint))))
                        (square (- (cl-transforms:z (cl-transforms:origin spoint))
                                   (cl-transforms:z (cl-transforms:origin fpoint)))))))
        (backw2 (sqrt (+ (square (- (cl-transforms:x (cl-transforms:origin spoint))
                                   (- (cl-transforms:x (cl-transforms:origin fpoint)) 1)))
                        (square (- (cl-transforms:y (cl-transforms:origin spoint))
                                   (cl-transforms:y (cl-transforms:origin fpoint))))
                        (square (- (cl-transforms:z (cl-transforms:origin spoint))
                                   (cl-transforms:z (cl-transforms:origin fpoint)))))))
        (right2 (sqrt (+ (square (- (cl-transforms:x (cl-transforms:origin spoint))
                                    (cl-transforms:x (cl-transforms:origin fpoint))))
                         (square (- (cl-transforms:y (cl-transforms:origin spoint))
                                    (-  (cl-transforms:y (cl-transforms:origin fpoint)) 1)))
                        (square (- (cl-transforms:z (cl-transforms:origin spoint))
                                   (cl-transforms:z (cl-transforms:origin fpoint)))))))
        (left2 (sqrt (+ (square (- (cl-transforms:x (cl-transforms:origin spoint))
                                    (cl-transforms:x (cl-transforms:origin fpoint))))
                         (square (- (cl-transforms:y (cl-transforms:origin spoint))
                                    (+  (cl-transforms:y (cl-transforms:origin fpoint)) 1)))
                        (square (- (cl-transforms:z (cl-transforms:origin spoint))
                                   (cl-transforms:z (cl-transforms:origin fpoint)))))))
        (up2 (sqrt (+ (square (- (cl-transforms:x (cl-transforms:origin spoint))
                                    (cl-transforms:x (cl-transforms:origin fpoint))))
                         (square (- (cl-transforms:y (cl-transforms:origin spoint))
                                    (cl-transforms:y (cl-transforms:origin fpoint))))
                        (square (- (cl-transforms:z (cl-transforms:origin spoint))
                                   (+ (cl-transforms:z (cl-transforms:origin fpoint)) 2))))))
        (down2 (sqrt (+ (square (- (cl-transforms:x (cl-transforms:origin spoint))
                                 (cl-transforms:x (cl-transforms:origin fpoint))))
                      (square (- (cl-transforms:y (cl-transforms:origin spoint))
                                 (cl-transforms:y (cl-transforms:origin fpoint))))
                      (square (- (cl-transforms:z (cl-transforms:origin spoint))
                                 (+ (cl-transforms:z (cl-transforms:origin fpoint)) 2))))))
        (all (append (append (append (append (append (append (append '() (list fsec)) (list forw2)) (list backw2)) (list right2)) (list left2)) (list up2)) (list down2)))
        (value NIL))
    (dotimes(index (length all))
      (if (and (>= 2 (nth index all))
               (null value))
          (setf value T)))
  value))

(defun square (n)
  (* n n))

(defun all-ups (liste)
  (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
            (cl-transforms:make-pose
             (cl-transforms:make-3d-vector (cl-transforms:x (cl-transforms:origin (nth index liste)))
                                           (cl-transforms:y (cl-transforms:origin (nth index liste)))
                                           (+ (cl-transforms:z (cl-transforms:origin (nth index liste))) 1))
             (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

(defun all-downs (liste)
   ; (format t "all-downs~%")
   (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
            (cl-transforms:make-pose
             (cl-transforms:make-3d-vector (cl-transforms:x (cl-transforms:origin (nth index liste)))
                                           (cl-transforms:y (cl-transforms:origin (nth index liste)))
                                           (- (cl-transforms:z (cl-transforms:origin (nth index liste))) 1))
             (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

(defun all-rights (liste)
  (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
                  (cl-transforms:make-pose
                   (cl-transforms:make-3d-vector (cl-transforms:x (cl-transforms:origin (nth index liste)))
                                                 (+ (cl-transforms:y (cl-transforms:origin (nth index liste))) 1)
                                                 (cl-transforms:z (cl-transforms:origin (nth index liste))))
                   (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

(defun all-lefts (liste)
 ; (format t "all-lefts~%")
    (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
            (cl-transforms:make-pose
             (cl-transforms:make-3d-vector (cl-transforms:x (cl-transforms:origin (nth index liste)))
                                           (- (cl-transforms:y (cl-transforms:origin (nth index liste))) 1)
                                           (cl-transforms:z (cl-transforms:origin (nth index liste))))
             (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

(defun all-fronts (liste)
  ;(format t "all-fronts~%")
  (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
            (cl-transforms:make-pose
             (cl-transforms:make-3d-vector (+ (cl-transforms:x (cl-transforms:origin (nth index liste))) 1)
                                           (cl-transforms:y (cl-transforms:origin (nth index liste)))
                                           (cl-transforms:z (cl-transforms:origin (nth index liste))))
            (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

(defun all-backs (liste)
  ;(format t "all-backs~%")
    (let*((test '()))
    (dotimes (index (length liste))
      (setf test (cons 
            (cl-transforms:make-pose
             (cl-transforms:make-3d-vector (- (cl-transforms:x (cl-transforms:origin (nth index liste))) 1)
                                           (cl-transforms:y (cl-transforms:origin (nth index liste)))
                                           (cl-transforms:z (cl-transforms:origin (nth index liste))))
             (cl-transforms:orientation (nth index liste))) test)))
    (reverse test)))

;;####### CHECKING THE VISIBILITY OF QUADCOPTER #######;;

(defun cam-depth-tf-transform ()
  (cl-transforms-stamped:lookup-transform *tf* "human" "camera_depth_frame"))

(defun cam-rgb-tf-transform ()
  (cl-transforms-stamped:lookup-transform *tf* "map" "camera_rgb_frame"))

(defun cam-get-pose->relative-map (vec)
(let((pose-stmp (cl-transforms-stamped:make-pose-stamped "camera_depth_frame"
                                                         0.0 vec
                                                         (cl-transforms:make-identity-rotation))))
  (cl-transforms-stamped:pose-stamped->pose (cl-tf:transform-pose *tf* :pose pose-stmp :target-frame "map"))))

(defun cam-set-markers ()
  (publish-pose (cl-transforms:transform->pose (cam-depth-tf-transform)) :id 0)
  (let*((id1 0)(id2 0)
        (limiter NIL)
        (fvector NIL)
        (svec NIL))
    (loop for index from 1 to 20
          do(setf limiter (cl-transforms:make-3d-vector 0.25d0 -0.1d0 -0.1d0))
            (cond((<= index 4)                  
             (loop for jindex from (* index -1) to index
                   do (setf fvector (cam-get-pose->relative-map
                                     (cl-transforms:make-3d-vector
                                      ( * index (cl-transforms:x limiter))
                                      (* (* (cl-transforms:y limiter) (* 0.7 jindex)) index)
                                      (* (cl-transforms:z limiter) 1))))                                 (loop for mindex from -2 to 3
                             do(setf svec fvector)
                               (setf svec (cl-transforms:make-pose
                                           (cl-transforms:make-3d-vector
                                            (cl-transforms:x (cl-transforms:origin svec))
                                            (cl-transforms:y (cl-transforms:origin svec))
                                            (+ (cl-transforms:z (cl-transforms:origin svec)) (* mindex 0.2)))
                                           (cl-transforms:orientation svec)))
                               (publish-pose svec :id (+ 1000 (+ id2 1)))
                               (setf id2 (+ id2 1))) 
                    ;;  (publish-pose fvector :id (+ 100 (+ id1 (+ id1 (* index 2)))))
                      (setf id1 (+ id1 1))))
                 ((<= index 10)
                  (loop for jindex from -5 to 5
                   do            
                      (setf fvector (cam-get-pose->relative-map
                                    (cl-transforms:make-3d-vector
                                    ( * index (cl-transforms:x limiter))
                                     (* (* (cl-transforms:y limiter) (* 0.7 jindex)) 5)
                                     (* (cl-transforms:z limiter) 1))))
                                             (loop for mindex from -4 to 5
                             do(setf svec fvector)
                               (setf svec (cl-transforms:make-pose
                                           (cl-transforms:make-3d-vector
                                            (cl-transforms:x (cl-transforms:origin svec))
                                            (cl-transforms:y (cl-transforms:origin svec))
                                            (+ (cl-transforms:z (cl-transforms:origin svec)) (* mindex 0.2)))
                                           (cl-transforms:orientation svec)))
                               (publish-pose svec :id (+ 1000 (+ id2 1)))
                               (setf id2 (+ id2 1)))
                    ;; (publish-pose fvector :id (+ 100 (+ id1 (+ id1 (* index 2)))))
                     (setf id1 (+ id1 1))))
                 (t
                  (loop for jindex from -9 to 9
                   do            
                      (setf fvector (cam-get-pose->relative-map
                                    (cl-transforms:make-3d-vector
                                    ( * index (cl-transforms:x limiter))
                                     (* (* (cl-transforms:y limiter) (* 0.3 jindex)) 9)
                                     (* (cl-transforms:z limiter) 1))))
                                             (loop for mindex from -7 to 8
                             do(setf svec fvector)
                               (setf svec (cl-transforms:make-pose
                                           (cl-transforms:make-3d-vector
                                            (cl-transforms:x (cl-transforms:origin svec))
                                            (cl-transforms:y (cl-transforms:origin svec))
                                            (+ (cl-transforms:z (cl-transforms:origin svec)) (* mindex 0.2)))
                                           (cl-transforms:orientation svec)))
                                  (publish-pose svec :id (+ 3000 (+ id2 1)))
                             
                               (setf id2 (+ id2 1)))
                     ;;(publish-pose fvector :id (+ 100 (+ id1 (+ id1 (* index 2)))))
                      (setf id1 (+ id1 1))))))))


(defun set-stream (num pose)
  (publish-pose pose :id 1000)
  (let*((id1 0)
        (limiter NIL)
        (fvector NIL)
        (liste '()))
    (loop for index from 0 to (- num 1)
          do(setf limiter (cl-transforms:make-3d-vector 1.0d0 0.0d0 0.0d0))
            (setf fvector (cam-get-pose->relative-map
                                     (cl-transforms:make-3d-vector
                                      ( * index (cl-transforms:x limiter))
                                      (* (cl-transforms:y limiter) index)
                                      (* (cl-transforms:z limiter) 1))))         
            (publish-pose fvector :id (+ 5000 (+ id1 1)))
            (setf liste (append liste (list fvector)))
            (setf id1 (+ id1 1)))
    (last liste)))

(defun look-at-object-x (camera-pose object-pose)
  "this uses the optical-frame but the position of the cam ontop of the
quadrotor, so the rotation is on x-axis"
  (let* ((obj-point-in-camera (cl-transforms:v-
                               (cl-transforms:origin object-pose)
                               (cl-transforms:origin camera-pose)))
         (x-axis (cl-transforms:make-3d-vector 1 0 0))
         (angle (acos (/ (cl-transforms:dot-product
                          obj-point-in-camera x-axis)
                         (cl-transforms:v-norm obj-point-in-camera))))
         (rot-axis (cl-transforms:cross-product
                    x-axis obj-point-in-camera))
         (res-quaternion (cl-transforms:q*
                          (cl-transforms:axis-angle->quaternion rot-axis angle)
                          (cl-transforms:axis-angle->quaternion x-axis (* pi 2) ))));;(/ pi 2)))))
    ;;res-quaternion))
    (cl-transforms:make-pose (cl-transforms:origin camera-pose) res-quaternion)))


(defun look-at-object-y (camera-pose object-pose)
  "this uses the optical-frame but the position of the cam ontop of the
quadrotor, so the rotation is on y-axis"
  (let* ((obj-point-in-camera (cl-transforms:v-
                               (cl-transforms:origin object-pose)
                               (cl-transforms:origin camera-pose)))
         (y-axis (cl-transforms:make-3d-vector 1 0 0 ))
         (angle (acos (/ (cl-transforms:dot-product
                          obj-point-in-camera y-axis)
                         (cl-transforms:v-norm obj-point-in-camera))))
         (rot-axis (cl-transforms:cross-product
                    y-axis obj-point-in-camera)))
    (cl-transforms:q*
     (cl-transforms:axis-angle->quaternion rot-axis angle)
     (cl-transforms:axis-angle->quaternion y-axis (/ pi 2)))))

(defun get-distance (pose1 pose2)
(let*((vector (cl-transforms:origin pose1))
        (x-vec (cl-transforms:x vector))
        (y-vec (cl-transforms:y vector))
        (z-vec (cl-transforms:z vector))
        (ge-vector (cl-transforms:origin pose2))
        (x-ge (cl-transforms:x ge-vector))
        (y-ge (cl-transforms:y ge-vector))
        (z-ge (cl-transforms:z ge-vector)))
    (round (sqrt (+ (square (- x-vec x-ge))
             (square (- y-vec y-ge))
             (square (- z-vec z-ge)))))))
    
;; first quaternion of camera-pose aftwards location-designator resolution

(defun copy-hash-table (hash-table)
                 (let ((ht (make-hash-table
                            :test 'equal
                            :size (hash-table-size hash-table))))
                   (loop for key being each hash-key of hash-table
                         using (hash-value value)
                         do (setf (gethash key ht) value)
                            finally (return ht))))

(defun get-direction-based-on-human (vec)
  (let*((humanpose  (cl-transforms:transform->pose (human-relative-map-pose)))
        (pose (cl-transforms-stamped:make-pose-stamped "human" 0.0
                                                            vec
                                                             (cl-transforms:orientation humanpose)))
      (ret (cl-transforms-stamped:pose-stamped->pose  (cl-tf:transform-pose *tf* :pose pose :target-frame "map"))))
   ret))


(defun create-local-tf-publisher (robot-pose name)
   (let*((pub (cl-tf:make-transform-broadcaster)))
 (cl-tf:send-static-transforms pub 1.0 "quadpose" (cl-transforms-stamped:make-transform-stamped "map" name (roslisp:ros-time) (cl-transforms:origin robot-pose) (cl-transforms:orientation robot-pose)))))

(defun remove-local-tf-publisher (thread)
  (when (sb-thread:thread-alive-p thread)
    (handler-case
        (prog1 t (sb-thread:terminate-thread thread))
      (error () nil))))

;; (defun visibility-of-object (robot-pose object-pose)
 
;;   (let*((thread  (create-local-tf-publisher robot-pose))
;; 	(pose (cl-transforms:transform->pose (tf:lookup-transform *tf* "map" "new_quad_pose")))
;; 	(result (look-at-object-x pose object-pose)))
;;     (remove-local-tf-publisher thread)
;; result))

(defun get-elems-infrontof-human (num)
  (let* ((sem-map *sem-map*)
         (sem-hash (slot-value sem-map 'sem-map-utils:parts))
         (sem-keys (hash-table-keys sem-hash))
         (poses '()) (dist NIL) (liste '())(obj-pose2 NIL)
         (pub NIL)(obj-pose NIL))
    (dotimes (index (length sem-keys))
      (if (not (or (string-equal (nth index sem-keys) "human01")
                   (string-equal (nth index sem-keys)  "human02")
                   (string-equal (nth index sem-keys)  "human03")))
          (setf liste (cons (nth index sem-keys) liste))))
    (dotimes (index (length liste))
      (setf obj-pose (cl-transforms-stamped:transform->pose (cl-tf:lookup-transform *tf* "human" (format NIL "~a_link" (nth index liste)))))
         (setf obj-pose2 (cl-transforms-stamped:transform->pose (cl-tf:lookup-transform *tf* "map" (format NIL "~a_link" (nth index liste)))))
      (setf dist (get-distance (human-relative-map-pose) obj-pose2))
      (if (and (>= num dist)
               (plusp (cl-transforms:x (cl-transforms:origin obj-pose))))
                (setf poses (cons (format NIL "~a - ~a"(nth index liste) dist) poses))))
    (setf poses (sort-list poses))
       poses))


(defun checking-obj-property (name property)
  (let((result NIL))
  (cond ((or (string-equal "big" property)
             (string-equal "large" property))
         (if(string-equal name (calculate-big-object (get-elem-type name)))
            (setf result T)))
        ((string-equal "small" property)
         (if(string-equal name (calculate-small-object (get-elem-type name)))
            (setf result T)))
        ((or (string-equal "green" property)
             (string-equal "red" property)
             (string-equal "yellow" property)
             (string-equal "blue" property)
             (string-equal "red" property)
             (string-equal "white" property)
             (string-equal "darkred" property)
             (string-equal "darkgreen" property)
             (string-equal "grey" property)
             (string-equal "darkgrey" property)
             (string-equal "brown" property))
         (if(string-equal (get-elem-color name) property)
            (setf result T)))
        ((or (string-equal "victim" property)
	     (string-equal "human" property)
             (string-equal "tree" property)
	     (string-equal "pylon" property)
             (string-equal "avalanche" property)
             (string-equal "mountain" property))
         (if (string-equal property (get-elem-type name))
             (setf result T)))
        (t (setf result NIL)))
    result))
         
(defun get-property-list ()
  (list (list "behind" "relation")
        (list "left" "relation")
        (list "in-front-of" "relation")
        (list "right" "relation")
        (list "close-to" "relation")
        (list "small" "property")
        (list "large" "property")
        (list "green" "property")
        (list "darkgreen" "property")
        (list "gray" "property")
        (list "darkgray" "property")
        (list "red" "property")
        (list "darkred" "property")
        (list "yellow" "property")
        (list "white" "property")
        (list "blue" "property")
        (list "tree" "property")
        (list "victim" "property")
        (list "rock" "property")
        (list "mountain" "property")))
                    

;;;
;;;
;;;Sorting the lists by using two functions
;;;
 (defun sort-list (liste)
   (dotimes (index (length liste))
                     (setf liste (sorted-lists liste)))
                   liste)


(defun sorted-lists (liste)
  (let ((sortlist '())
        (tmp  (first liste)))
    (loop for index from 1 to (- (length liste) 1)
          do
            (let((tmpnum (read-from-string
                          (second (split-sequence:split-sequence #\- tmp))))
                 (num (read-from-string
                            (second (split-sequence:split-sequence #\- (nth index liste)))))
                 (value (nth index liste)))
             (cond ((> tmpnum  num)
                    (setf sortlist (cons value sortlist)))
                   (t
                    (setf sortlist (cons tmp sortlist))
                    (setf tmp value)
                    (setf tmpnum (read-from-string
                                  (second (split-sequence:split-sequence #\- tmp))))))))
    (setf sortlist (cons tmp sortlist))
    (reverse sortlist)))
