#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
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
	vector<Node*> children;
	Node* parent;
	Node* fail;
	Node* find(char c);
	Node* find_or_fail(char c);
};

int Node::depth() {
	if (parent == NULL)
		return 0;
	return parent->depth() + 1;
}

Node* Node::find(char c) {
	for (int i=0; i<children.size(); i++) {
		if (children[i]->content == c)
			return children[i];
	}
	return NULL;
};

Node* Node::find_or_fail(char c) {
	Node* result = find(c);
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
	Node* root;
	void add(string s) {add(-1,s);};
	void add(int id, string s);
	void build();
	vector<Match> search(string s);
private:
	void add_fail_transitions(Node* n);
};

Trie::Trie() {
	root = new Node();
}

void Trie::add(int id, string s) {
	Node* current = root;
	for (int i=0; i<s.length(); i++) {
		Node* child = current->find(s[i]);
		if (child == NULL) {
			child = new Node(s[i]);
			current->children.push_back(child);
			child->parent = current;
		}
		current = child;
	}
	current->terminal = true;
	current->id = id;
}

void Trie::add_fail_transitions(Node* node) {
	vector<Node*> children = node->children;
	Node* child;
	Node* parent_tsn;
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
	Node* current = root;	
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
int main2(int argc, char** argv) {
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
