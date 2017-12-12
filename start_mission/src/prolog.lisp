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

(defmethod costmap-generator-name->score ((name (eql 'collisions))) 10)

(defclass reasoning-generator () ())
(defmethod costmap-generator-name->score ((name reasoning-generator)) 7)

(defclass gaussian-generator () ())     
(defmethod costmap-generator-name->score ((name gaussian-generator)) 6)
(defclass range-generator () ())
(defmethod costmap-generator-name->score ((name range-generator)) 2)
(defmethod costmap-generator-name->score ((name (eql 'semantic-map-free-space))) 11)


(def-fact-group cognitive-reasoning-costmap (desig-costmap)
  (<- (desig-costmap ?desig ?costmap)
    (costmap ?costmap)
    (prepositions ?desig ?pose ?costmap))

  (<- (prepositions ?desig ?pose ?costmap)
    (or (desig-prop ?desig (:right ?object-name))
        (desig-prop ?desig (:right-of ?object-name)))
    (lisp-fun get-human-elem-pose ?object-name ?object-pose)
    (instance-of reasoning-generator ?reasoning-generator-id)
    (costmap-add-function
     ?reasoning-generator-id
     (make-reasoning-cost-function ?object-pose :Y < 0.3)
     ?costmap)
    (adjust-map ?costmap ?object-pose ?object-name))


  (<- (prepositions ?desig ?pose ?costmap)
    (or (desig-prop ?desig (:left ?object-name))
        (desig-prop ?desig (:left-of ?object-name)))
    (lisp-fun get-human-elem-pose ?object-name ?object-pose)
    (instance-of reasoning-generator ?reasoning-generator-id)
    (costmap-add-function
     ?reasoning-generator-id
     (make-reasoning-cost-function ?object-pose :Y > 0.3)
     ?costmap)
    (adjust-map ?costmap ?object-pose ?object-name))

  (<- (prepositions ?desig ?pose ?costmap)
    (or (desig-prop ?desig (:front-of ?object-name))
        (desig-prop ?desig (:in-front-of ?object-name)))
    (lisp-fun get-human-elem-pose ?object-name ?object-pose)
    (instance-of reasoning-generator ?reasoning-generator-id)
    (costmap-add-function
     ?reasoning-generator-id
     (make-reasoning-cost-function ?object-pose :X > 0.1)
     ?costmap)
    (adjust-map ?costmap ?object-pose ?object-name))

    (<- (prepositions ?desig ?pose ?costmap)
    (or (desig-prop ?desig (:behind ?object-name))
        (desig-prop ?desig (:behind-of ?object-name)))
    (lisp-fun get-human-elem-pose ?object-name ?object-pose)
    (instance-of reasoning-generator ?reasoning-generator-id)
    (costmap-add-function
     ?reasoning-generator-id
     (make-reasoning-cost-function ?object-pose :X < 0.1)
     ?costmap)
    (adjust-map ?costmap ?object-pose ?object-name))

  (<- (prepositions ?desig ?pose ?costmap)
      (desig-prop ?desig (:to ?object-name))
      (lisp-fun get-human-elem-pose ?object-name ?object-pose)
      (adjust-map ?costmap ?object-pose ?object-name))

(<- (adjust-map ?costmap ?object-pose ?object-name)
  ;;(semantic-map-costmap::semantic-map-objects ?all-objects)
  (format "get-elem-pose~%")
  (lisp-fun get-elem-pose ?object-name ?pose)
  ;;(lisp-fun get-geom-objects ?all-objects 10 ?pose ?objects)
  (costmap-padding ?padding)
  ;;(costmap-add-function semantic-map-free-space
  ;;                      (make-semantic-map-costmap-human
  ;;                       ?objects :invert t :padding ?padding)
  ;;                      ?costmap)
  (costmap ?costmap)
  (instance-of gaussian-generator ?gaussian-generator-id)
  (costmap-add-function ?gaussian-generator-id
                        (make-location-cost-function ?object-pose  3.0)
                        ?costmap)
  (costmap ?costmap)
 (costmap-add-height-generator
   (make-consistent-height-function ?object-name ?resulting-z)
   ?costmap)
))



;; (def-fact-group reasoning-costmap (desig-costmap)
;;   (<- (desig-costmap ?desig ?costmap)
;;       (costmap ?costmap)
;;       ;;(lisp-fun get-genius-pose->map-frame "genius_link" ?pose)
;;       (preps ?desig ?pose ?costmap))
  
;;   ;;############### RIGHT OF ####################;;
;;   (<- (preps ?desig ?pose ?costmap)
;;       (or (desig-prop ?desig (:right-of ?object1-name))
;; 	  (desig-prop ?desig (:right ?object1-name)))
;;       (lisp-fun determine-relations :right ?pose ?list-values)
;;       (general ?list-values ?object1-name ?costmap))
  
;;   ;;############### LEFT OF ####################;;
;;   (<- (preps ?desig ?pose ?costmap)
;;       (or (desig-prop ?desig (:left-of ?object1-name))
;; 	  (desig-prop ?desig (:left ?object1-name)))
;;       (lisp-fun determine-relations :left ?pose ?list-values)
;;       (general ?list-values ?object1-name ?costmap))
  
;;   ;;############### BEHIND OF ####################;;
;;   (<- (preps ?desig ?pose ?costmap)
;;       (or (desig-prop ?desig (:behind-of ?object1-name))
;; 	  (desig-prop ?desig (:across ?object1-name))
;; 	  (desig-prop ?desig (:over ?object1-name))
;; 	  (desig-prop ?desig (:behind ?object1-name)))
;;       (lisp-fun determine-relations :behind ?pose ?list-values)
;;       (general ?list-values ?object1-name ?costmap))

;;   ;;############### IN FRONT OF ####################;;
;;   (<- (preps ?desig ?pose ?costmap)
;;       (or (desig-prop ?desig (:in-front-of ?object1-name))
;; 	  (desig-prop ?desig (:front ?object1-name))
;; 	  (desig-prop ?desig (:straight ?object1-name))
;; 	  (desig-prop ?desig (:to ?object1-name))
;; 	  (desig-prop ?desig (:ahead ?object1-name))
;; 	  (desig-prop ?desig (:close ?object1-name))
;; 	  (desig-prop ?desig (:next ?object1-name)))
;;       (lisp-fun determine-relations :front ?pose ?list-values)
;;       (general ?list-values ?object1-name ?costmap))
  
;;   ;;############### ABOVE ####################;;
;;   (<- (prepositions ?desig ?pose ?costmap)
;;       (desig-prop ?desig (:above ?object1-name))
;;       ;;(lisp-fun get-sem-object-pose->map ?object1-name ?object1-pose)
;;       (lisp-fun get-elem-pose ?object1-name ?object1-pose)
;;       (instance-of gaussian-generator ?gaussian-generator-id)
;;       (costmap-add-function ?gaussian-generator-id
;; 			    (make-location-function ?object1-pose 5.0)
;; 			    ?costmap)
;;       (costmap-add-height-generator
;;        (make-consistent-height-function ?object1-name ?resulting-z)
;;        ?costmap))
  
;;   ;;############### AROUND ####################;;
;;   (<- (prepositions ?desig ?pose ?costmap)
;;       (desig-prop ?desig (:around ?object1-name)) 
;;       ;; (lisp-fun get-sem-object-pose->map ?object1-name ?object1-pose)
;;       (lisp-fun get-elem-pose ?object1-name ?object1-pose)
;;       (semantic-map-costmap::semantic-map-objects ?all-objects)
;;       (lisp-fun get-objects-closer-agent ?all-objects 10 ?object1-pose ?objects)
;;       (costmap-padding ?padding)
;;       (costmap-add-function semantic-map-free-space
;; 			    (semantic-map-costmap::make-semantic-map-costmap
;; 			     ?objects :invert t :padding ?padding)
;; 			    ?costmap)
;;       (costmap ?costmap)
;;       (instance-of gaussian-generator ?gaussian-generator-id)
;;       (costmap-add-function ?gaussian-generator-id
;; 			    (make-location-function ?object1-pose  6.0)
;; 			    ?costmap)
;;       (costmap-add-height-generator
;;        (make-consistent-height-function ?object1-name ?resulting-z)
;;        ?costmap))

;;   ;;#######################GENERAL############################;;    
;;   (<- (general ?list-values ?obj-name ?costmap)
;;       (prolog::equal (?fvalue ?svalue) ?list-values)
;;       (lisp-fun get-elem-pose ?obj-name ?obj-pose)
;;       (costmap ?costmap)
;;       (semantic-map-costmap::semantic-map-objects ?all-objects)
;;       (lisp-fun get-objects-closer-agent ?all-objects 10 ?obj-pose ?objects)
;;       (costmap-padding ?padding)
;;       (costmap-add-function semantic-map-free-space
;; 			    (semantic-map-costmap::make-semantic-map-costmap
;; 			     ?objects :invert t :padding ?padding)
;; 			    ?costmap)
;;       (instance-of reasoning-generator ?reasoning-generator-id)
;;       (costmap-add-function
;;        ?reasoning-generator-id
;;        (make-reasoning-cost-function ?object1-pose ?fvalue ?svalue 0.5)
;;        ?costmap)
;;       (costmap ?costmap)
;;       (instance-of gaussian-generator ?gaussian-generator-id)
;;       (costmap-add-function ?gaussian-generator-id
;; 			    (make-location-function ?obj-pose 8.0)
;; 			    ?costmap)
;;       (costmap-add-height-generator
;;        (make-consistent-height-function ?object1-name ?resulting-z)
;;        ?costmap)))
