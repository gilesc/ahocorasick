#include <string>
#include <vector>
using namespace std;

namespace ahocorasick {

struct Match {
	int id;
	int start;
	int end;
};

class Trie {
public:
	Trie();
	vector<Match> search(string s);
};

}
