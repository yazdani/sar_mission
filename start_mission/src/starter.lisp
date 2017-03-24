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

(defvar *sem-map* NIL);;(sem-map-utils:get-semantic-map "http://knowrob.org/kb/ias_semantic_map.owl#MountainMap"))
(defvar *obj-name* NIL)

;;
;; SERVICE TO GET ALL OBJECTS
;;

(defun start_all_objects()
  (roslisp-utilities:startup-ros)
  (all-objs-call))

(defun all-objs-call ()
 (roslisp-utilities:startup-ros :name "start_all_objs"
  (roslisp:register-service "all_objs" 'cmd_mission-srv:all_objs)
  (setf *sem-map* (sem-map-utils:get-semantic-map "http://knowrob.org/kb/ias_semantic_map.owl#MountainMap"))
  (roslisp:ros-info (basics-system) "Service 'All Objects' already started.")
 (roslisp:spin-until nil 1000)))

(roslisp:def-service-callback cmd_mission-srv:all_objs (all)
  (format t "all ~a~%" all)
  (let*((liste (get-all-objects)))
  (roslisp:make-response :result_all liste)))

;;
;; SERVICE TO GET SALIENT OBJECTS INSIDE THE SEMMAP
;;
 (defun start_salient_objects ()
 ;;  (roslisp-utilities:startup-ros)
   (salient-objs-call))

(defun salient-objs-call ()
  (roslisp-utilities:startup-ros :name "start_salient_objs")
  (roslisp:register-service "salient_objs" 'cmd_mission-srv:salient_objs)
  (if (null *sem-map*)
      (setf *sem-map* (sem-map-utils:get-semantic-map "http://knowrob.org/kb/ias_semantic_map.owl#MountainMap")))
  (roslisp:ros-info (basics-system) "Service 'Salient Objects' already started.")
  (roslisp:spin-until nil 1000))

(roslisp:def-service-callback cmd_mission-srv:salient_objs (sal)
  (let*((liste (get-objs-infrontof-human)))
    (roslisp:make-response :result_salient liste)))

;;
;; SERVICE FOR CHECKING RELATION OF TWO OBJECTS IN A SPATIAL CONTEXT
;; 
(defun start_checking_relation ()
  (relation-objs-call))

(defun relation-objs-call ()
  (roslisp-utilities:startup-ros :name "start_checking_relation")
  (roslisp:register-service "check_objs_relation" 'cmd_mission-srv:check_objs_relation)
  (if (null *sem-map*)
      (setf *sem-map* (sem-map-utils:get-semantic-map "http://knowrob.org/kb/ias_semantic_map.owl#MountainMap")))
  (roslisp:ros-info (basics-system) "Service 'Checking Objects Relation' already started.")
  (roslisp:spin-until nil 1000))

(roslisp:def-service-callback cmd_mission-srv:check_objs_relation (property obj1 obj2)
(let((result NIL))
  (if (or (string-equal property "")
          (string-equal obj1 "")
          (string-equal obj2 ""))
      (setf result NIL)
      (setf result (checking-relation obj1 obj2 property)))
  (roslisp:make-response :result_check result)))

(defun start_checking_property()
  (property-checking))

(defun property-checking ()
  (roslisp-utilities:startup-ros :name "start_checking_property")
  (roslisp:register-service "check_obj_property" 'cmd_mission-srv:check_obj_property)
  (if (null *sem-map*)
  (setf *sem-map* (sem-map-utils:get-semantic-map "http://knowrob.org/kb/ias_semantic_map.owl#MountainMap")))
  (roslisp:ros-info (basics-system) "Service 'Checking Objects Property' already started.")
 (roslisp:spin-until nil 1000))

(roslisp:def-service-callback cmd_mission-srv:check_obj_property (name property)
  (let ((result NIL))
(if (or (string-equal name "")
        (string-equal property ""))
    (setf result NIL)
    (setf result (checking-obj-property name property)))
  (roslisp:make-response :result_property result)))

;;  (defun start_getting_type()
;;    (getting-object-type))

;; (defun getting-object-type ()
;;    (roslisp-utilities:startup-ros :name "start_getting_type")
;;    (roslisp:register-service "get_obj_type" 'cmd_mission-srv:get_obj_type)
;;     (if (null *sem-map*)
;;         (setf *sem-map* (sem-map-utils:get-semantic-map "http://knowrob.org/kb/ias_semantic_map.owl#MountainMap")))
;;    (roslisp:ros-info (basics-system) "Service 'Getting Objects Type' already started.")
;;    (roslisp:spin-until nil 1000))

;; (roslisp:def-service-callback cmd_mission-srv:get_obj_type (objname)
;;   (let ((result NIL))
;;     (if (string-equal objname "")
;; 	(setf result NIL)
;; 	(setf result (get-elem-type objname)))
;;   (roslisp:make-response :result_type result)))

(defun start_getting_property_list()
  (getting-property-list))

(defun getting-property-list ()
  (roslisp-utilities:startup-ros :name "start_getting_property_list")
  (roslisp:register-service "get_property_list" 'cmd_mission-srv:get_property_list)
  (if (null *sem-map*)
      (setf *sem-map* (sem-map-utils:get-semantic-map "http://knowrob.org/kb/ias_semantic_map.owl#MountainMap")))
  (roslisp:ros-info (basics-system) "Service 'Getting Property List' already started.")
  (roslisp:spin-until nil 1000))

(roslisp:def-service-callback cmd_mission-srv:get_property_list (props)
  (let ((result NIL)
        (value (get-property-list))
        (liste '()))
    (dotimes(index (length value))
      (setf liste (cons (roslisp:make-msg "cmd_mission/Subgoal"
                                          :property (first (nth index value))
                                          :kind (second (nth index value)))
                            liste)))
    (setf result (reverse liste))
    (roslisp:make-response :result_props  result)))


(defun start_getting_object_size()
 (getting-object-size))

(defun getting-object-size ()
  (roslisp-utilities:startup-ros :name "start_getting_object_size")
  (roslisp:register-service "get_obj_size" 'cmd_mission-srv:get_obj_type)
  (if (null *sem-map*)
      (setf *sem-map* (sem-map-utils:get-semantic-map "http://knowrob.org/kb/ias_semantic_map.owl#MountainMap")))
  (roslisp:ros-info (basics-system) "Service 'Getting Object Size' already started.")
  (roslisp:spin-until nil 1000))

(roslisp:def-service-callback cmd_mission-srv:get_obj_type (objname)
  (let ((result (checking-object-size objname)))
        (roslisp:make-response :result_type  result)))

(defun start_getting_reasoning_on_pose()
  (getting-reasoning-on-pose))

(defun getting-reasoning-on-pose ()
  (roslisp-utilities:startup-ros :name "start_getting_reasoning_on_pose")
  (roslisp:register-service "get_reason_pose" 'cmd_mission-srv:get_reason_pose)
  (if (null *sem-map*)
      (setf *sem-map* (sem-map-utils:get-semantic-map "http://knowrob.org/kb/ias_semantic_map.owl#MountainMap")))
  (roslisp:ros-info (basics-system) "Service 'Getting Reasoning OnProperty List' already started.")
  (roslisp:spin-until nil 1000))

(roslisp:def-service-callback cmd_mission-srv:get_reason_pose (act prep objname)
  (let*((result (get-desig-resolution act prep objname))
        (schetring NIL))
    (setf *obj-name* objname)
     (setf schetring (concatenate 'string (write-to-string (cl-transforms:x (cl-transforms:origin result))) ","
                        (write-to-string (cl-transforms:y (cl-transforms:origin result))) ","
                    (write-to-string (cl-transforms:z (cl-transforms:origin result))) ","
                        (write-to-string (cl-transforms:x (cl-transforms:orientation result))) ","
                  (write-to-string (cl-transforms:y (cl-transforms:orientation result))) ","
                      (write-to-string (cl-transforms:z (cl-transforms:orientation result))) ","
                      (write-to-string (cl-transforms:w (cl-transforms:orientation result)))))                      
    (roslisp:make-response :result_pose schetring)))

(defun start_robot_rotation()
 (rotate-robot))

(defun rotate-robot ()
  (roslisp-utilities:startup-ros :name "start_robot_rotation")
  (roslisp:register-service "get_rotation" 'cmd_mission-srv:rotate)
  (if (null *sem-map*)
      (setf *sem-map* (sem-map-utils:get-semantic-map "http://knowrob.org/kb/ias_semantic_map.owl#MountainMap")))
  (roslisp:ros-info (basics-system) "Service 'Rotation' already started.")
  (roslisp:spin-until nil 1000))

(roslisp:def-service-callback cmd_mission-srv:rotate (goal)
  (let ((result (set-rotation-based-elem goal)))
    (format t "result ~a~%" result)
    (roslisp:make-response :result  result)))
