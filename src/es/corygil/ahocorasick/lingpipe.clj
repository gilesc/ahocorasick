(ns es.corygil.ahocorasick.lingpipe
  (:import
    (com.aliasi.dict ExactDictionaryChunker MapDictionary DictionaryEntry)
    (com.aliasi.tokenizer IndoEuropeanTokenizerFactory)
    (com.aliasi.chunk ChunkingImpl)))

(defn- clojurify-chunking [chunking]
  (map #(hash-map :id (try (Integer/parseInt (.type %))
                           (catch Exception _ (.type %)))
                  :start (.start %)
                  :end (.end %))
       chunking))

(defn build [term-ids &
             {:keys [case-sensitive? allow-overlaps? tokenizer-factory]
              :or {case-sensitive? false allow-overlaps? false
                   tokenizer-factory (IndoEuropeanTokenizerFactory/INSTANCE)}}]
  (let [md (MapDictionary.)
        tf (IndoEuropeanTokenizerFactory/INSTANCE)]
    (doseq [[term id] term-ids]
      (.addEntry md
                 (DictionaryEntry. term (str id))))
    (let [chunker (ExactDictionaryChunker. md tokenizer-factory
                                           allow-overlaps? case-sensitive?)]
      (with-meta
        (fn [text]
          (clojurify-chunking
           (.chunk chunker text)))
        {:chunkers [chunker]}))))

(defn combine [& chunkers]
  (let [chunker-objs (apply concat (map #(:chunkers (meta %)) chunkers))]
    (with-meta
      (fn [text]
        (clojurify-chunking 
         (reduce (fn [chunking chunker]
                   (ChunkingImpl/merge chunking
                                       (.chunk chunker text)))
                 (.chunk (first chunker-objs) text)
                 (rest chunker-objs))))
      {:chunkers chunker-objs})))
