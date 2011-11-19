# ahocorasick

Fast string searching in native Clojure using the Aho-Corasick algorithm (same as used by fgrep).


## Usage

Provide search patterns as a list or map of term-id pairs, build the automaton, and use it to search:

   (require '[es.corygil.ahocorasick :as ahocorasick])
   (use 'clojure.pprint)

   (def patterns
     (zipmap ["he" "she" "his" "hers"]
               (range 4)))

   (def automaton
     (ahocorasick/build patterns))

   (pprint (ahocorasick/search automaton "she likes his face"))

   => [{:id 1, :start 0, :end 3}
       {:id 0, :start 1, :end 3}
       {:id 2, :start 9, :end 13}]

## Tokenization

By default, each character is a token.  If you pre-tokenize your patterns, however, you can speed up the automaton construction and searching.  As a caveat, however, you have to use the same tokenizer on your query that you used to construct the automaton.  (It's on my to-do list to integrate this feature directly into the module). For example:

   (defn tokenize [s] (.split s " "))

   (def patterns
        (zipmap (map tokenize ["i like ham" "i like spam"]) (range 2)))


   (def automaton (ahocorasick/build patterns))

   (ahocorasick/search automaton 
      (tokenize "when i go to the store i like ham , but when camping i like spam"))

   => [{:id 0, :start 6, :end 9} {:id 1, :start 13, :end 16}]

Note that if you do this, the start and end indices correspond to tokens, not characters.

## License

Copyright (C) 2011 Cory Giles

Distributed under the Eclipse Public License, the same as Clojure.
