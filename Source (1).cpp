/**************
GROUP 12
Members' names:
Nguyen Quoc Viet		1651069
Hoang Dinh Hieu			1651045
Huynh Ha Mai Trinh		1651034
Nguyen Vo Hong Thang	1651055
QUERY AND
QUERY OR
QUERY INTITLE
QUERY HASHTAG
QUERY PRICE
QUERY PRICE IN RANGE
QUERY MINUS
QUERY PLUS
QUERY FAMILY * (additional)
*****************/

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <fstream>
#include "header.h"

using namespace std;

int main()
{
	cout << "Welcome to the search engine" << endl;
	vector<string> queryHistory, inputvector;
	string s;
	node *root = new node;
	history_node* hiroot = new history_node;
	string article[1601];
	int start = clock();
	Load_data(root, article);
	cout << endl;
	int stop = clock();
	cout << stop - start << " ms" << endl;
	cout << "Finished Loading!" << endl;
	cout << "Here are the query types you can use:" << endl;
	cout << "> AND query search for articles containing all keywords. Ex: food AND drinks" << endl;
	cout << "> OR query search for articles which have at least 1 of the keywords. Ex: 'A OR B' ; 'A B'" << endl;
	cout << "> INTITLE query search for articles that have the keyword in the title. Ex: intitle:John " << endl;
	cout << "> '#' query search for articles which contains the hashtag. Ex: #ad " << endl;
	cout << "> '$' query search for articles that contains the price. Ex: book $100 " << endl;
	cout << "> '-' query search for articles which does not contain the specified keyword. Ex: Titanic -film" << endl;
	//cout << "> '+' query search for articles that contains the words next to it. Ex: " << endl;
	cout << "> '$x..$x' and 'x..x' query search for articles with price in range. Ex: book $50..$1160 " << endl;
	cout << "> '*' query search for articles with keywords related to it. Ex: number* " << endl;
	cout << endl << "Have you read carefully?" << endl;
	system("PAUSE");
	system("CLS");
	while (s != "EXIT")
	{
		s = History_suggestion(hiroot);
		inputvector = input(queryHistory, s);
		if (s == "EXIT")
		{
			break;
		}
		system("CLS");
		querycall(inputvector, root, article, s);
		/*int sz = queryHistory.size();
		if (sz > 0)cout << "History:" << endl;
		for (int i = 0; i < sz; i++)
		{:
		cout << queryHistory[i] << endl;
		}*/
		//system("PAUSE");
		cout << endl;
		system("CLS");
	}
	cout << "Goodbye" << endl;
	destructor(root);
	destructor_history_trie(hiroot);
	cout << endl;
	return 0;
}
