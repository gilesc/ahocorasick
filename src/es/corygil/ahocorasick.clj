(ns es.corygil.ahocorasick)

"TODO: the atoms seem to be pointing to newly created nodes instead of the already existing ones"
"TODO: provide tokenizer baked into trie building for faster creation/search"
"TODO: why is building so slow"
"TODO: provide searcher as closure instead of object"
"TODO: provide remove-overlaps and case sensitivity as an option"

(deftype Node [id depth children fail])

(defrecord Match [id start end])

(defmulti terminal? type :default :default)

(defmethod terminal? Node [n]
  (empty? (.children n)))

(defmethod terminal? :default [patterns]
  (and (= (count patterns) 1)
       (not (seq (ffirst patterns)))))

(defn- transition [node chr]  
  ((or (.children node) {})
   chr @(.fail node)))

(defn- fail-transition [_ child parent chr]
  (let [node (transition @(.fail parent) chr)]
    (if (= node child)
      @(.fail parent)
      node)))

(defn- add-fail-transitions! [root]
  "Adds fail transitions breadth-first"
  (swap! (.fail root) (fn [_] root))
  (loop [queue (conj (clojure.lang.PersistentQueue/EMPTY) root)]
    (if-let [node (peek queue)]
      (do
        (doseq [[chr child] (.children node)]
          (swap! (.fail child) fail-transition child node chr)) 
        (recur
         (reduce conj (pop queue) (vals (.children node))))))))

(defn- build* [id depth patterns]
  (Node. id depth
         (if-not (terminal? patterns)
           (if-let [children
              (seq (for [[startc patterns] (group-by ffirst patterns)
                     :let [child-patterns (zipmap (map rest (keys patterns))
                                            (vals patterns))
                           id (-> (filter (comp not seq first) child-patterns)
                                  first second)]]
                 [startc
                  (build* id (inc depth) (filter #(seq (first %)) child-patterns))]))]
             (into {} children)))
         (atom nil)))

(defn build [patterns]
  "Builds an Aho-Corasick trie. patterns is a map of strings to ids."
  (let [root (build* nil 0 patterns)]
    (add-fail-transitions! root)
    root))

;;TODO: this (concat query " ") is not the right way to solve the problem
(defn search [trie query]
  (loop [idx 0 node trie q (concat query " ") results []]
    (let [[nidx nnode nq] (if (terminal? node)
                            [idx @(.fail node) q]
                            [(inc idx) (transition node (first q)) (rest q)])]
      (if-not (or (seq nq) (terminal? nnode))
        results
        (recur nidx nnode nq
               (if (.id nnode)
                    (conj results (Match.
                                   (.id nnode) (max 0 (- nidx (.depth nnode)))
                                   nidx))
                    results))))))
