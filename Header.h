#pragma once
#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <fstream>
#include <ctime>
#include <queue>
using namespace std;


struct node
{
	vector <int> title;
	vector < pair<int, int> > position; // article ; position in article
	node* pNext[38];//a->z, 0->9, #, $
	node();
};

struct history_node
{
	vector <string> exist;
	history_node* pNext[39];//a-> z, 0->9, #, $, space
	history_node();
};

void destructor(node* &root);
void destructor_history_trie(history_node* root);
void Load_data(node* head, string article[]);
void OrQuery(vector<int> & res, node* word);
node* search_key_word(node* root, string s);
void insert_word(node* &root, bool intitle, string s, int posinart, int article);
vector<string> ParseStream(string & line, vector<int> & posinart, int &linestart);
bool checke(char s, string tmp);
vector <string> input(vector<string>&queryHistory, string &s);
void querycall(vector<string> call, node* root, string article[], string);
vector <int> exact_match(node* root, string s, string article[]);
void AndQuery(vector<int> & res, node* word);
void IntitleQuery(vector<int> & res, node* word);
void CheckDup(vector<int> & res);
string outputFilename(int articleID);
void output(vector<int>&res, int stime, string article[], string);
void MinusQuery(vector<int> &res, node* word);
void updateHistory(vector<string> &query, string s);
node* searchHashtag(node* root, string s);
node* searchMoney(node* root, string s);
vector<int> searchMoneyInRange(node* root, string s);
vector<int> searchNumberInRange(node* root, string s);
string History_suggestion(node* history_trie);
vector < pair<int, int> > searchIncompleteMatch(node* history_trie, string search_string);
void historyInsert(history_node* history_root, string s);
vector< string > historySearch(history_node* history_root, string s);
string History_suggestion(history_node* history_root);


template<class QT>
struct QNode
{
	QT* node;
	QNode* next;
	QNode* prev;
};

template<class QT>
class Queue
{
	QNode<QT> *first;
	QNode<QT> *last;
public:
	Queue() {
		first = last = nullptr;
	}
	bool Empty() {
		if (first == last && last == nullptr)
			return true;
		return false;
	}

	void Push_back(QT* x) {
		if (first == nullptr) {
			first = last = new QNode<QT>;
			first->node = x;
			first->next = first->prev = nullptr;
		}
		else {
			last->next = new QNode<QT>;
			last->next->prev = last;
			last->next->next = nullptr;
			last = last->next;
			last->node = x;
		}
	}

	QT* Pop_front() {
		if (first == last && last == nullptr)
			return nullptr;
		else if (first == last) {
			QT* res = first->node;
			delete first;
			first = last = nullptr;
			return res;
		}
		else {
			QT* res = last->node;
			QNode<QT>* tmp = last;
			last = last->prev;
			delete tmp;
			last->next = nullptr;
			return res;
		}
	}
};

#endif _FUNCTION_H
#pragma once
