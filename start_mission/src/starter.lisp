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


(defun start-scenario ()
  (roslisp-utilities:startup-ros))

;;
;; SERVICE IN ORDER TO ASK FOR ALL OBJECTS INSIDE THE SEMMAP
;;
 (defun start_all_objects ()
   (service-call-one))

(defun service-call-one ()
 ; (roslisp-utilities:startup-ros :name "start-all-objects");; :master-uri (roslisp:make-uri "localhost" 11311)  :name "service_node")
(roslisp:with-ros-node ("start_all_node" :spin t)
  (roslisp:register-service "all_objs" 'cmd_mission-srv:all_objs)
  (roslisp:ros-info (basics-system) "start all service for the msg.")))

 ; (roslisp:spin-until nil 1000))
(roslisp:def-service-callback cmd_mission-srv:all_objs (all)
  (format t "robot ~a~%" all)
  (let*((vec '())
	(liste (get-obj-list)))
    (dotimes(index (length liste))
      do(setf vec (cons (roslisp:make-msg "cmd_mission/Subgoal" :name (nth index liste)) vec)))
  (roslisp:make-response :result_all (reverse vec))))

;;
;; SERVICE TO ASK FOR SALIENT OBJECTS INSIDE THE SEMMAP
;;
 (defun start_salient_objects ()
   (service-call-two))

(defun service-call-two ()
  (roslisp:with-ros-node ("start_salient_objs_node" :spin t)
  ;(roslisp-utilities:startup-ros :name "start_salient_objects");; :master-uri (roslisp:make-uri "localhost" 11311)  :name "service_node")
;;  (roslisp:with-ros-node ("getting service node" :spin t)
  (roslisp:register-service "salient_objs" 'cmd_mission-srv:salient_objs)
  (roslisp:ros-info (basics-system) "start salient service for the msg.")))
 ; (roslisp:spin-until nil 1000))

(roslisp:def-service-callback cmd_mission-srv:salient_objs (sal)
  (format t "robot ~a~%" sal)
(let*((vec '())
      (liste (get-objs-infrontof-human)))
  (dotimes(index (length liste))
    do(setf vec (cons (roslisp:make-msg "cmd_mission/Subgoal" :name (nth index liste)) vec)))
  (roslisp:make-response :result_salient (reverse vec))))

;;
;; SERVICE FOR CHECKING RELATION OF TWO OBJECTS IN A SPATIAL CONTEXT
;; 

 (defun start_checking_spatial_objects ()
   (service-call-three))

(defun service-call-three ()
  (roslisp:with-ros-node ("start_checking_objs_node" :spin t)
  (roslisp-utilities:startup-ros :name "start-checking-objects");; :master-uri (roslisp:make-uri "localhost" 11311)  :name "service_node")
;;  (roslisp:with-ros-node ("getting service node" :spin t)
  (roslisp:register-service "check_msg" 'cmd_mission-srv:check_msg)
  (roslisp:ros-info (basics-system) "start check service for the msg.")))
 ; (roslisp:spin-until nil 1000))

(roslisp:def-service-callback cmd_mission-srv:check_msg (property obj1 obj2)
  (roslisp:make-response :result_check (checking-property obj1 obj2 property)))
