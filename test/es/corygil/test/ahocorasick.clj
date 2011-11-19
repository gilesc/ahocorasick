(ns es.corygil.test.ahocorasick
  (:use [es.corygil.ahocorasick])
  (:use [clojure.test]))

(def patterns
  (zipmap ["he" "she" "his" "hers"]
          (range 4)))

(def automaton
  (build patterns))

(deftest simple
  (let [result (search automaton "he")]
    (is (count result) 1)
    (is (:start (first result)) 0)
    (is (:end (first result)) 2)))
