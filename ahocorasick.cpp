#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
using namespace std;

namespace ahocorasick {

class Node {
public:
	Node() {};
	Node(char c) {content = c; terminal = false;}
	int id;
	int depth();
	char content;		
	bool terminal;
	vector<shared_ptr<Node> > children;
	shared_ptr<Node> parent;
	shared_ptr<Node> fail;
	shared_ptr<Node> find(char c);
	shared_ptr<Node> find_or_fail(char c);
};

int Node::depth() {
	if (parent == NULL)
		return 0;
	return parent->depth() + 1;
}

shared_ptr<Node>
Node::find(char c) {
	for (int i=0; i<children.size(); i++) {
		if (children[i]->content == c)
			return children[i];
	}
	return NULL;
};

shared_ptr<Node>
Node::find_or_fail(char c) {
	shared_ptr<Node> result = find(c);
	if (result == NULL) {
		return fail;
	} else {
		return result;
	}
}

struct Match {
	int id;
	int start;
	int end;
};

class Trie {
public:
	Trie();
	Trie(bool case_sensitive);
	shared_ptr<Node> root;
	void add(string s) {add(-1,s);};
	void add(int id, string s);
	void build();
	vector<Match> search(string s);
private:
	void add_fail_transitions(shared_ptr<Node> n);
	bool case_sensitive;
};

Trie::Trie() {
	root = shared_ptr<Node>(new Node());
}

Trie::Trie(bool is_case_sensitive) {
	case_sensitive = is_case_sensitive;
	root = shared_ptr<Node>(new Node());
}

void Trie::add(int id, string s) {
	shared_ptr<Node> current = root;
	if (!case_sensitive)
		transform(s.begin(), s.end(), s.begin(), ::tolower);
	for (int i=0; i<s.length(); i++) {
		shared_ptr<Node> child = current->find(s[i]);
		if (child == NULL) {
			shared_ptr<Node> c(new Node(s[i]));
			child = c;
			current->children.push_back(child);
			child->parent = current;
		}
		current = child;
	}
	current->terminal = true;
	current->id = id;
}

void Trie::add_fail_transitions(shared_ptr<Node> node) {
	vector<shared_ptr<Node> > children = node->children;
	shared_ptr<Node> child;
	shared_ptr<Node> parent_tsn;
	for (int i=0; i<children.size(); i++) {
		child = children[i];
		if (node == root) {
			child->fail = root;
		} else {
			parent_tsn = node->fail->find(child->content);
			if (parent_tsn == NULL)
				child->fail = root;
			else
				child->fail = parent_tsn;
		}
		add_fail_transitions(child);
	}
}

void Trie::build() {
	add_fail_transitions(root);
	root->fail = root;
}

vector<Match>
Trie::search(string s) {
	if (!case_sensitive)
		transform(s.begin(), s.end(), s.begin(), ::tolower);
	shared_ptr<Node> current = root;	
	vector<Match> matches;
	for (int i=0; i<s.length(); i++) {
		current = current->find_or_fail(s[i]);
		if (current->terminal) {
			Match match;
			match.id = current->id;
			match.start = i + 1 - current->depth();
			match.end = i;
			matches.push_back(match);
		}
	}
	return matches;
}

}
/*
int main(int argc, char** argv) {
	ahocorasick::Trie t;

	ifstream file(argv[1]);
	string line;
	while (getline(file, line)) {
		stringstream linestrm(line);
		string field;
		getline(linestrm, field, '\t');
		int id;
		istringstream(field) >> id;
		getline(linestrm, field, '\t');
		cerr << id << "\t" << field << endl;
		t.add(id, field);
	}	
	t.build();
	string s = "he likes his caffeine she hers";
	vector<ahocorasick::Match> result = t.search(s);
	for (int i=0; i<result.size(); i++) {
		cout << result[i].id << "\t" << result[i].start << "\t" << result[i].end << endl;
	}
	return 0;
}
*/
