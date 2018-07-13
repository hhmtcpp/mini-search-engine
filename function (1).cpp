#include "header.h"

using namespace std;

/****************************************************************/

node::node() {
	for (int i = 0; i < 38; i++) {
		pNext[i] = NULL;
	}
}

history_node::history_node()
{
	for (int i = 0; i < 39; i++) {
		pNext[i] = NULL;
	}
}
/****************************************************************/

void OrQuery(vector<int> & res, node* word)
{
	if (word == NULL)
	{
		return;
	}
	vector<int> second, des;
	size_t sz = word->position.size();
	for (size_t i = 0; i < sz; i++) {
		second.push_back(word->position[i].first);
	}
	set_union(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
	res = des;
}

/****************************************************************/

void destructor(node *& root)
{
	if (root == NULL)
		return;
	root->title.clear();
	root->position.clear();
	for (int i = 0; i < 38; i++)
	{
		destructor(root->pNext[i]);
		root->pNext[i] = NULL;
	}
	delete root;
	return;
}

/****************************************************************/

void destructor_history_trie(history_node * root)
{
	if (root == NULL)
		return;
	for (int i = 0; i < 39; i++)
	{
		destructor_history_trie(root->pNext[i]);
		root->pNext[i] = NULL;
	}
	root->exist.clear();
	delete root;
	return;
}

/****************************************************************/

void Load_data(node* head, string article[]) {
	cout << "Loading data";
	fstream fi;
	string filename;
	string line;
	int i, j, linestart, cnt;
	int index_of_article, check, word_vector_size;
	vector<string> word_vector;
	vector<int> position_vector;
	for (i = 0; i < 14; i++) {
		cout << '.';
		if (i == 9 || i == 13)
			check = 100;
		else check = 50;
		for (j = 0; j < check; j++) {
			word_vector.clear();
			position_vector.clear();
			filename = "Group";
			filename += (i + 1) / 10 + '0';
			filename += (i + 1) % 10 + '0';
			if (i == 10)
			{
				filename += "-";
			}
			else
			{
				filename += "_";
			}
			if (j == 99)
			{
				filename += "100";
			}
			else
			{
				filename += (j + 1) / 10 + '0';
				filename += (j + 1) % 10 + '0';
			}
			filename += ".txt";
			fi.open(filename);
			if (fi.fail())
			{
				continue;
			}
			getline(fi, line);
			if (i >= 10)
			{
				article[i * 50 + 50 + j] = line;
			}
			else
			{
				article[i * 50 + j] = line;
			}
			linestart = 0;
			word_vector = ParseStream(line, position_vector, linestart);
			if (i >= 10)
			{
				index_of_article = i * 50 + j + 50;
			}
			else
			{
				index_of_article = i * 50 + j;
			}
			article[index_of_article] += '\n';
			word_vector_size = word_vector.size();
			for (cnt = 0; cnt < word_vector_size; cnt++)
			{
				insert_word(head, true, word_vector[cnt], position_vector[cnt], index_of_article);
			}
			while (!fi.eof()) {
				getline(fi, line);
				article[i * 50 + j + 800] += line + '\n';
				word_vector.clear();
				position_vector.clear();
				word_vector = ParseStream(line, position_vector, linestart);
				word_vector_size = word_vector.size();
				for (cnt = 0; cnt < word_vector_size; cnt++)
				{
					insert_word(head, false, word_vector[cnt], position_vector[cnt], index_of_article);
				}
			}
			fi.close();
		}
	}
	for (j = 801; j < 1601; j++) {
		if (j % 100 == 0)
			cout << '.';
		//get filename to open file
		i = j;
		string filename = "Data";
		if (i / 1000 != 0 || filename != "Data")
			filename += (i / 1000) + '0';
		i %= 1000;
		if (i / 100 != 0 || filename != "Data")
			filename += (i / 100) + '0';
		i %= 100;
		if (i / 10 != 0 || filename != "Data")
			filename += (i / 10) + '0';
		i %= 10;
		if (i != 0 || filename != "Data")
			filename += i + '0';
		filename += ".txt";
		fi.open(filename);
		if (fi.fail())
		{
			continue;
		}
		getline(fi, line);
		article[j] = line + '\n';
		position_vector.clear();
		word_vector.clear();
		//parse the title
		linestart = 0;
		word_vector = ParseStream(line, position_vector, linestart);
		word_vector_size = word_vector.size();
		//insert the title
		for (cnt = 0; cnt < word_vector_size; cnt++)
		{
			insert_word(head, true, word_vector[cnt], position_vector[cnt], j);
		}
		//read and insert the article from file
		while (!fi.eof()) {
			getline(fi, line);
			article[j] += line + '\n';
			word_vector = ParseStream(line, position_vector, linestart);
			word_vector_size = word_vector.size();
			for (cnt = 0; cnt < word_vector_size; cnt++)
			{
				insert_word(head, false, word_vector[cnt], position_vector[cnt], j);
			}
		}
		fi.close();
	}
}

/****************************************************************/

bool checke(char s, string tmp)
{
	char e[32] = { '+',',',';',' ','\t','\0','!','{','}','"','(',')','\f','\v','\n','\r','=','|','~','@','%', '&','<','>','/','[',']','?','`','^','_','\\' };
	for (int i = 0; i < 32; i++)
		if (s == e[i])
			return false;
	if ((s == ':') && (tmp != "intitle"))
		return false;
	if ((s == '-') && (tmp.empty()))
		return true;
	return true;
}

/****************************************************************/

vector <string> input(vector<string>&queryHistory, string &s)
{
	/*cout << "Enter search query: ";
	getline(cin, s);*/
	if (s == "EXIT")return vector<string>();
	updateHistory(queryHistory, s);
	string tmp;
	vector<string> result;
	istringstream iss(s);
	for (string s; iss >> s; )
	{
		int i, l = static_cast<int>(s.length());
		for (i = 0; i < l; i++)
		{
			if (checke(s[i], tmp))
				tmp += s[i];
			if (tmp == "intitle:")
				break;
			if (tmp == "-")
				break;
		}
		if ((tmp == "intitle:"))
		{
			result.push_back(tmp);
			tmp.clear();
			for (int i = 8; i < l; i++)
			{
				if (checke(s[i], tmp))
					tmp += s[i];
			}
		}
		if ((tmp == "-"))
		{
			result.push_back(tmp);
			tmp.clear();
			for (int i = 1; i < l; i++)
			{
				if (checke(s[i], tmp))
					tmp += s[i];
			}
		}
		/*if ((tmp == "#"))
		{
		result.push_back(tmp);
		tmp.clear();
		for (int i = 1; i < l; i++)
		{
		if (checke(s[i], tmp))
		tmp += s[i];
		}
		}*/
		if (!tmp.empty())
			result.push_back(tmp);
		tmp.clear();
	}
	return result;
}
/****************************************************************/
void CheckDup(vector<int> & res)
{
	vector<int> ans;
	int l = res.size();
	if (l == 0)return;
	sort(res.begin(), res.end());
	ans.push_back(res[0]);
	for (int i = 1; i < l; i++) {
		if (res[i] != res[i - 1])
			ans.push_back(res[i]);
	}
	res = ans;
}

/****************************************************************/

string outputFilename(int articleID)
{
	string res;
	if (articleID > 800) {
		res = "Data";
		if (articleID < 1000)
		{
			res += articleID / 100 + '0';
			articleID %= 100;
			res += articleID / 10 + '0';
			articleID %= 10;
			res += articleID + '0';
		}
		else
		{
			res += articleID / 1000 + '0';
			articleID %= 1000;
			res += articleID / 100 + '0';
			articleID %= 100;
			res += articleID / 10 + '0';
			articleID %= 10;
			res += articleID + '0';
		}
	}
	else
	{
		res = "Group";
		if (articleID < 450)
		{
			res += '0';
			res += articleID / 50 + 1 + '0';
			res += '_';
			articleID = articleID % 50 + 1;
			res += articleID / 10 + '0';
			articleID %= 10;
			res += articleID + '0';
		}
		else if (articleID < 550)
		{
			res += "10_";
			articleID = (articleID - 450) + 1;
			if (articleID == 100)
				res += "100";
			else
			{
				res += articleID / 10 + '0';
				articleID %= 10;
				res += articleID + '0';
			}
		}
		else if (articleID < 600)
		{
			res += "11-";
			articleID = articleID - 550 + 1;
			res += articleID / 10 + '0';
			articleID %= 10;
			res += articleID + '0';
		}
		else if (articleID < 650)
		{
			res += "12_";
			articleID = articleID - 600 + 1;
			res += articleID / 10 + '0';
			articleID %= 10;
			res += articleID + '0';
		}
		else if (articleID < 700)
		{
			res += "13_";
			articleID = articleID - 650 + 1;
			res += articleID / 10 + '0';
			articleID %= 10;
			res += articleID + '0';
		}
		else
		{
			res += "14_";
			articleID = (articleID - 700) + 1;
			if (articleID == 100)
				res += "100";
			else
			{
				res += articleID / 10 + '0';
				articleID %= 10;
				res += articleID + '0';
			}
		}
	}
	res += ".txt";
	return res;
}

/****************************************************************/

void output(vector<int>& res, int stime, string article[], string search_string)
{
	vector< string > page;
	string tmp;
	string ans;
	size_t sz = res.size(), cnt;
	cout << search_string << "\n\nFound " << sz << " results in " << stime << " ms\n\n\n";
	for (size_t i = 0; i < sz; i++)
	{
		ans = outputFilename(res[i]);
		//cout << ans << ":" << endl << "	";
		tmp = ans + ": ";
		cnt = 0;
		while (article[res[i]][cnt] != '\n')
		{
			if ((int)article[res[i]][cnt] <= 31 || (int)article[res[i]][cnt] >= 127)
			{
				cnt++;
				continue;
			}
			//cout << article[res[i]][cnt];
			tmp += article[res[i]][cnt];
			cnt++;
		}
		//cout << endl << endl;
		page.push_back(tmp);
	}
	int total = page.size();
	char ch = '1';
	int move_page = 0;
	ostream_iterator< string > screen(cout, "\n\n");
	if (total < 5) {
		cout << "[1] - [" << total << "]\n";
		copy(page.begin(), page.end(), screen);
		cout << "\n\n";
		cout << "Press <ENTER> to continue searching...\n";
	}
	else {
		cout << "[1] - [5]\n";
		copy(page.begin(), page.begin() + 5, screen);
		cout << "\n\n";
		cout << "Press <2> to continue next page >>\n\n";
		cout << "Press <ENTER> to continue searching...\n";
	}
	while (ch != 13) {
		while (true) {
			ch = _getch();
			if (ch == '2' && move_page + 5 < total) {
				move_page += 5;
				break;
			}
			else if (ch == '1' && move_page != 0) {
				move_page -= 5;
				break;
			}
			else if (ch == 13) {
				break;
			}
			else continue;
		}
		if (ch == 13)
			break;
		system("CLS");
		cout << search_string << "\n\nFound " << sz << " results in " << stime << " ms" << "\n\n\n";
		if (move_page + 5 >= total) {
			cout << "[" << move_page + 1 << "] - [" << total << "]\n\n";
			copy(page.begin() + move_page, page.end(), screen);
			cout << "\n<< Press <1> to continue previous page\n\n";
		}
		else if (move_page == 0) {
			cout << "[1] - [5]\n\n";
			copy(page.begin() + move_page, page.begin() + move_page + 5, screen);
			cout << "\nPress <2> to continue next page >>\n\n";
		}
		else {
			cout << "[" << move_page + 1 << "] - [" << move_page + 5 << "]\n\n";
			copy(page.begin() + move_page, page.begin() + move_page + 5, screen);
			cout << "\n\n<< Press <1> to continue previous page\t\t||\t\tPress <2> to continue next page >>\n\n";
		}
		cout << "Press <ENTER> to continue searching..." << endl;
	}
	return;
}

/****************************************************************/

void MinusQuery(vector<int>& res, node * word)
{
	if (word == NULL)
	{
		return;
	}
	vector<int>tmp;
	int ressz = res.size();
	int possz = word->position.size();
	if (possz == 0 || ressz == 0)
	{
		return;
	}
	int i = 0, j = 0;
	while (i < ressz)
	{
		if (j >= possz)
		{
			for (i = i; i < ressz; i++)
			{
				tmp.push_back(res[i]);
			}
			break;
		}
		if (res[i] == word->position[j].first)
		{
			i++;
		}
		else if (word->position[j].first < res[i])
		{
			j++;
		}
		else
		{
			tmp.push_back(res[i]);
			i++;
		}
	}
	res = tmp;
}

/****************************************************************/

void querycall(vector<string> call, node* root, string article[], string search_string)
{
	int k, len, tz;
	bool check;
	vector<int> des, second;
	vector<pair<int, int>> tmp;
	if (call.size() <= 0) {
		cout << "Result not found" << endl;
		return;
	}
	vector<int> res;
	int sz;
	res.clear();
	node *sr; // search return
	int start, stop;
	start = clock();
	int i = 1;
	/*
	if (call[0] == "intitle:")
	{
	IntitleQuery(res, search_key_word(root, call[i]));
	i++;
	}
	else
	{
	sr = search_key_word(root, call[0]);
	if (sr != NULL)
	{
	sz = static_cast<int>(sr->position.size());
	for (int j = 0; j < sz; j++)
	{
	res.push_back(sr->position[j].first);
	}
	}
	}*/
	sz = static_cast<int>(call.size());
	for (i = 0; i < sz; i++)
	{
		if (call[i] == "AND")
		{
			i++;
			if (call[i][0] == '#')
				sr = searchHashtag(root, call[i]);
			else if (call[i][0] >= '0'&&call[i][0] <= '9')
			{
				check = false;
				k = call[i].length();
				for (int j = 0; j < k; j++)
				{
					if ((call[i][j]<'0' || call[i][j]>'9') && call[i][j] != '.')
					{
						break;
					}
					if (call[i][j] == '.')
					{
						check = true;
					}
				}
				if (check)
				{
					des.clear();
					second.clear();
					second = searchNumberInRange(root, call[i]);
					set_intersection(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
					res.clear();
					res = des;
					continue;
				}
				else
					sr = search_key_word(root, call[i]);
			}
			else if (call[i][0] == '$')
			{
				check = false;
				k = call[i].length();
				for (int j = 0; j < k; j++)
				{
					if (call[i][j] == '.')
					{
						check = true;
						break;
					}
				}
				if (check)
				{
					des.clear();
					second.clear();
					second = searchMoneyInRange(root, call[i]);
					if (res.empty())
					{
						set_union(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
						res.clear();
						res = des;
						continue;
					}
					set_intersection(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
					res.clear();
					res = des;
					continue;
				}
				sr = searchMoney(root, call[i]);
			}
			else
				sr = search_key_word(root, call[i]);
			AndQuery(res, sr);
			continue;
		}
		if (call[i] == "OR")
		{
			if (call[i][0] == '#')
				sr = searchHashtag(root, call[i]);
			else if (call[i][0] >= '0'&&call[i][0] <= '9')
			{
				check = false;
				k = call[i].length();
				for (int j = 0; j < k; j++)
				{
					if ((call[i][j]<'0' || call[i][j]>'9') && call[i][j] != '.')
					{
						break;
					}
					if (call[i][j] == '.')
					{
						check = true;
					}
				}
				if (check)
				{
					des.clear();
					second.clear();
					second = searchNumberInRange(root, call[i]);
					if (res.empty())
					{
						set_union(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
						res.clear();
						res = des;
						continue;
					}
					set_intersection(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
					res.clear();
					res = des;
					continue;
				}
				else
					sr = search_key_word(root, call[i]);
			}
			else if (call[i][0] == '$')
			{
				check = false;
				k = call[i].length();
				for (int j = 0; j < k; j++)
				{
					if (call[i][j] == '.')
					{
						check = true;
						break;
					}
				}
				if (check)
				{
					des.clear();
					second.clear();
					second = searchMoneyInRange(root, call[i]);
					set_union(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
					res.clear();
					res = des;
					continue;
				}
				sr = searchMoney(root, call[i]);
			}
			else
				sr = search_key_word(root, call[i]);
			OrQuery(res, sr);
			continue;
		}
		if (call[i] == "intitle:")
		{
			IntitleQuery(res, search_key_word(root, call[++i]));
			continue;
		}
		if (call[i] == "-")
		{
			if (i == 0)
			{
				OrQuery(res, search_key_word(root, call[++i]));
				continue;
			}
			MinusQuery(res, search_key_word(root, call[++i]));
			continue;
		}
		if (call[i][0] == '#')
		{
			if (res.empty() && i == 0)
			{
				OrQuery(res, searchHashtag(root, call[i]));
				continue;
			}
			AndQuery(res, searchHashtag(root, call[i]));
			continue;
		}
		if (call[i][0] == '$')
		{
			check = false;
			k = call[i].length();
			for (int j = 0; j < k; j++)
			{
				if (call[i][j] == '.')
				{
					check = true;
					break;
				}
			}
			if (check)
			{
				des.clear();
				second.clear();
				second = searchMoneyInRange(root, call[i]);
				if (!res.empty())
				{
					set_intersection(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
					res = des;
				}
				else res = second;
				continue;
			}
			cout << "query $";
			if (res.empty() && i == 0)
			{
				OrQuery(res, searchMoney(root, call[i]));
				continue;
			}
			AndQuery(res, searchMoney(root, call[i]));
			continue;
		}

		len = call[i].length();
		if (call[i][len - 1] == '*')
		{
			tmp = searchIncompleteMatch(root, call[i].substr(0, len - 1));
			second.clear();
			des.clear();
			tz = tmp.size();
			for (int j = 0; j < tz; j++)
			{
				second.push_back(tmp[j].first);
			}
			sort(second.begin(), second.end());
			if (!res.empty())
			{
				set_intersection(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
				res = des;
			}
			else res = second;
			continue;
		}
		if (call[i][0] > '0'&&call[i][0] <= '9')
		{
			check = false;
			k = call[i].length();
			for (int j = 0; j < k; j++)
			{
				if (call[i][j] == '.')
				{
					check = true;
					break;
				}
			}
			if (!check)
			{
				OrQuery(res, search_key_word(root, call[i]));
				continue;
			}
			des.clear();
			second.clear();
			second = searchNumberInRange(root, call[i]);
			if (!res.empty())
			{
				set_intersection(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
				res = des;
			}
			else res = second;
			continue;
		}
		else
		{
			OrQuery(res, search_key_word(root, call[i]));
			continue;
		}
	}
	stop = clock();
	if (res.size() <= 0)
	{
		cout << search_string << "\n\n";
		cout << "Result not found" << endl;
		cout << endl;
		cout << "Press <ENTER> to continue..." << endl;
		char ch = 'a';
		while (ch != 13)
		{
			ch = _getch();
		}
	}
	else
	{
		CheckDup(res);
		output(res, stop - start, article, search_string);
	}
}

/****************************************************************/

vector<string> ParseStream(string & line, vector<int> & posinart, int &linestart)
{
	transform(line.begin(), line.end(), line.begin(), ::tolower); // convert string to lowercase
	size_t prev = 0, pos; // maintain position in string
	vector<string> wordVector;
	string tmp;
	size_t x;
	while ((pos = line.find_first_of(" \t\n\"`~!@%^&*()-_=+|{}[];:,'<.>/\\?", prev)) != string::npos)
	{
		if (pos > prev)
		{
			tmp = line.substr(prev, pos - prev);
			wordVector.push_back(tmp);
			posinart.push_back(prev + linestart);
		}
		prev = pos + 1;
	}
	x = line.length();
	if (prev < x)
	{
		tmp = line.substr(prev, string::npos);
		wordVector.push_back(tmp);
		posinart.push_back(prev + linestart);
	}
	linestart += (x + 1);
	return wordVector;
}

/****************************************************************/

node* search_key_word(node* root, string s) {
	vector <pair <int, int> > res;
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	node* cur = root;
	int length = s.length();
	int cnt = 0;
	if (s[cnt] == '#')
	{
		if (cur->pNext[36] == NULL)
			return NULL;
		cur = cur->pNext[36];
		cnt++;
	}
	else if (s[cnt] == '$')
	{
		if (cur->pNext[37] == NULL)
			return NULL;
		cur = cur->pNext[37];
		cnt++;
	}
	int tmp;
	while (cnt<length) {
		if ((int)(s[cnt] - '0')<10 && (int)(s[cnt] - '0') >= 0)
			tmp = (int)(s[cnt] - '0') + 26;
		else if ((int)(s[cnt] - 'a') >= 0 && (int)(s[cnt] - 'a') <= (int)('z' - 'a'))
			tmp = (int)(s[cnt] - 'a');
		else {
			cnt++;
			continue;
		}
		if (cur->pNext[tmp] == NULL)return NULL;
		cur = cur->pNext[tmp];
		cnt++;
	}
	return cur;
}

/****************************************************************/

void insert_word(node* &root, bool intitle, string s, int posinart, int article) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	if (s.length() == 0)
		return;
	int tmp, length, cnt;
	node* cur = root;
	length = s.length();
	cnt = 0;
	while (s[cnt] != '#'&&s[cnt] != '$' && (s[cnt]<'a' || s[cnt]>'z') && (s[cnt] < 'A' || s[cnt] < 'Z') && (s[cnt]<'0' || s[cnt]>'9'))
	{
		cnt++;
		if (cnt >= length)
			return;
	}
	if (s[cnt] == '#')
	{
		if (cur->pNext[36] == NULL)
			cur->pNext[36] = new node;
		cur = cur->pNext[36];
		cnt++;
	}
	else if (s[cnt] == '$')
	{
		if (cur->pNext[37] == NULL)
			cur->pNext[37] = new node;
		cur = cur->pNext[37];
		cnt++;
	}
	while (cnt<length) {
		if ((int)(s[cnt] - '0') >= 0 && (int)(s[cnt] - '0') <= 9) {
			tmp = (int)(s[cnt] - '0');
			if (cur->pNext[tmp + 26] == NULL)
				cur->pNext[tmp + 26] = new node;
			cur = cur->pNext[tmp + 26];
		}
		else if ((int)(s[cnt] - 'a') >= 0 && (int)(s[cnt] - 'a') <= (int)('z' - 'a'))
		{
			tmp = (int)(s[cnt] - 'a');
			if (cur->pNext[tmp] == NULL)
				cur->pNext[tmp] = new node;
			cur = cur->pNext[tmp];
		}
		cnt++;
	}
	if (intitle)cur->title.push_back(article);
	cur->position.push_back(make_pair(article, posinart));
	return;
}

/****************************************************************/

void AndQuery(vector<int> & res, node* word)
{
	if (word == NULL)
	{
		return;
	}
	vector<int> second, des;
	size_t sz = word->position.size();
	for (size_t i = 0; i < sz; i++) {
		second.push_back(word->position[i].first);
	}
	set_intersection(res.begin(), res.end(), second.begin(), second.end(), back_inserter(des));
	res = des;
}

/****************************************************************/

vector <int> exact_match(node* root, string s, string article[]) {
	vector <int> null;
	int length = s.length();
	int cnt = 0, tmp;
	node* cur = root;
	int z = (int)('z' - 'a');
	while (cnt<length) {
		if ((int)(s[cnt] - '0') <= 9 && (int)(s[cnt] - '0') >= 0) {
			tmp = (int)(s[cnt] - '0');
			if (cur->pNext[tmp + 26] == NULL)return null;
			cur = cur->pNext[tmp + 26];
		}
		else {
			tmp = (int)(s[cnt] - 'a');
			if (tmp<0 || tmp>(int)('z' - 'a'))break;
			if (cur->pNext[tmp] == NULL)return null;
			cur = cur->pNext[tmp];
		}
		cnt++;
	}
	vector <int> res;
	int nw = -1;
	int n = cur->position.size();
	pair <int, int> ps;
	for (int i = 0; i<n; i++) {
		ps = cur->position[i];
		if (ps.first == nw)continue;
		cnt = 0;
		bool ok = true;
		while (cnt<length) {
			if (article[ps.first][cnt] != s[cnt]) {
				ok = false;
				break;
			}
			cnt++;
		}
		if (ok) {
			res.push_back(ps.first);
			nw = ps.first;
		}
	}
	return res;
}

/****************************************************************/

void IntitleQuery(vector<int> & res, node* word)
{
	if (word == NULL)
	{
		return;
	}
	vector<int> first = res, second, des;
	second = word->title;
	if (res.size() <= 0)
	{
		res = second;
		return;
	}
	set_intersection(first.begin(), first.end(), second.begin(), second.end(), back_inserter(des));
	res = des;
}

/****************************************************************/

void updateHistory(vector<string>&queryHistory, string s)
{
	int sz = queryHistory.size();
	if (sz < 8)
	{
		queryHistory.push_back(s);
	}
	else
	{
		queryHistory.erase(queryHistory.begin());
		queryHistory.push_back(s);
	}
	return;
}

/****************************************************************/

node* searchHashtag(node * root, string s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	node* cur = root;
	int length = s.length();
	int cnt = 1, tmp;
	if (cur->pNext[36] != NULL)
		cur = cur->pNext[36];
	else
		return NULL;
	while (cnt<length) {
		if ((int)(s[cnt] - '0')<10 && (int)(s[cnt] - '0') >= 0)
			tmp = (int)(s[cnt] - '0') + 26;
		else if ((int)(s[cnt] - 'a') >= 0 && (int)(s[cnt] - 'a') <= (int)('z' - 'a'))
			tmp = (int)(s[cnt] - 'a');
		else {
			return NULL;
			continue;
		}
		if (cur->pNext[tmp] == NULL)return NULL;
		cur = cur->pNext[tmp];
		cnt++;
	}
	return cur;
}

/****************************************************************/

node * searchMoney(node * root, string s)
{
	node* cur = root;
	int length = s.length();
	int cnt = 1, tmp;
	if (cur->pNext[37] != NULL)
		cur = cur->pNext[37];
	else
		return NULL;
	while (cnt<length) {
		if ((int)(s[cnt] - '0')<10 && (int)(s[cnt] - '0') >= 0)
			tmp = (int)(s[cnt] - '0') + 26;
		else {
			return NULL;
			continue;
		}
		if (cur->pNext[tmp] == NULL)return NULL;
		cur = cur->pNext[tmp];
		cnt++;
	}
	return cur;
}

/****************************************************************/

vector<int> searchMoneyInRange(node * root, string s)
{
	//check if this is a mistake
	vector<int> res;
	node* cur = root;
	if (cur->pNext[37] == NULL)
		return res;
	cur = cur->pNext[37];
	//transform string to int
	int intst = 0;
	int l = s.length();
	if (s[1]<'0' || s[1]>'9')return res;
	int test = 2;
	while (test < l)
	{
		if ((s[test] >= '0'&&s[test] <= '9') || (s[test] == '.') || (s[test] == '$'))
			test++;
		else
			return res;
	}
	int cnt = 1;
	while (cnt < l)
	{
		intst *= 10;
		intst += (s[cnt] - '0');
		cnt++;
		if (s[cnt] == '.')
		{
			cnt++;
			break;
		}
	}
	//search from intst to inte
	if (s[cnt] != '.')
		return res;
	cnt++;
	if (s[cnt] != '$')
		return res;
	cnt++;
	int inte = 0;
	while (cnt < l)
	{
		if (s[cnt] == '.' || s[cnt] == '$')
			return res;
		inte *= 10;
		inte += (s[cnt] - '0');
		cnt++;
	}
	if (intst > inte)return res;
	int tmp = intst;
	node* nxt;
	string nxts;
	int cnt2;
	while (tmp <= inte)
	{
		nxts = "$";
		test = tmp;
		if (test == 0)cnt = 1;
		else
		{
			cnt = 0;
			while (test != 0)
			{
				cnt++;
				test /= 10;
			}
		}
		test = tmp;
		intst = 0;
		cnt2 = cnt;
		while (cnt2 != 0)
		{
			intst *= 10;
			intst += test % 10;
			test /= 10;
			cnt2--;
		}
		while (cnt != 0)
		{
			nxts += intst % 10 + '0';
			intst /= 10;
			cnt--;
		}
		nxt = searchMoney(root, nxts);
		OrQuery(res, nxt);
		tmp++;
	}
	return res;
}

/****************************************************************/

vector<int> searchNumberInRange(node * root, string s)
{
	//check if this is a mistake
	int test;
	vector<int> res;
	int l = s.length();
	int cnt = 0;
	int intst = 0, inte = 0;
	if (s[0] == '.')return res;
	while (cnt < l)
	{
		intst *= 10;
		intst += (int)(s[cnt] - '0');
		cnt++;
		if (s[cnt] == '.')
		{
			cnt++;
			break;
		}
	}
	if (s[cnt] != '.')
		return res;
	cnt++;
	if (cnt >= l)return res;
	while (cnt < l)
	{
		if (s[cnt] == '.')
			return res;
		inte *= 10;
		inte += (int)(s[cnt] - '0');
		cnt++;
	}
	if (intst > inte)return res;
	int tmp = intst;
	node* nxt;
	string nxts;
	int cnt2;
	while (tmp <= inte)
	{
		nxts.clear();
		test = tmp;
		if (test == 0)cnt = 1;
		else
		{
			cnt = 0;
			while (test != 0)
			{
				cnt++;
				test /= 10;
			}
		}
		test = tmp;
		intst = 0;
		cnt2 = cnt;
		while (cnt2 != 0)
		{
			intst *= 10;
			intst += test % 10;
			test /= 10;
			cnt2--;
		}
		while (cnt != 0)
		{
			nxts += intst % 10 + '0';
			intst /= 10;
			cnt--;
		}
		nxt = search_key_word(root, nxts);
		OrQuery(res, nxt);
		tmp++;
	}
	return res;
}

/****************************************************************/

string History_suggestion(node* history_trie) {
	HANDLE h_std_out;
	COORD coord;
	h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);

	vector< pair<int, int> > suggestion_result;
	string search_string = "";
	string suggestion_result_string;
	pair< int, int > tmp;
	char ch;
	int cnt = 0, cnt_ch = 0;

	ch = _getch();
	while (ch != 13) {
		search_string.push_back(ch);
		cout << ch;
		++cnt_ch;
		ch = _getch();
		suggestion_result = searchIncompleteMatch(history_trie, search_string);
		while (!suggestion_result.empty()) {
			tmp = suggestion_result.front();
			suggestion_result.erase(suggestion_result.begin());
			//process suggestion_result to suggestion_result_string
			++cnt;
			coord.X = 0;
			coord.Y = cnt;
			SetConsoleCursorPosition(h_std_out, coord);
			cout << tmp.first << " " << tmp.second << endl;
		}
		coord.X = cnt_ch;
		coord.Y = 0;
		SetConsoleCursorPosition(h_std_out, coord);
	}
	coord.X = 0;
	coord.Y = cnt + 1;
	SetConsoleCursorPosition(h_std_out, coord);
	//insert new string to history trie;
	return search_string;
}

/****************************************************************/

vector<pair<int, int>> searchIncompleteMatch(node * root, string s)
{
	vector< pair<int, int> > res;
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	node* cur = root;
	int length = s.length();
	if (s[0] == '#')
	{
		if (cur->pNext[36] == NULL)
			return vector<pair<int, int>>();
		cur = cur->pNext[36];
	}
	else if (s[0] == '$')
	{
		if (cur->pNext[37] == NULL)
			return vector<pair<int, int>>();
		cur = cur->pNext[37];
	}
	int tmp;
	for (int cnt = 0; cnt < length; cnt++) {
		if ((int)(s[cnt] - '0')<10 && (int)(s[cnt] - '0') >= 0)
			tmp = (int)(s[cnt] - '0') + 26;
		else if ((int)(s[cnt] - 'a') >= 0 && (int)(s[cnt] - 'a') <= (int)('z' - 'a'))
			tmp = (int)(s[cnt] - 'a');
		else continue;
		if (cur->pNext[tmp] == NULL)
			return res;
		cur = cur->pNext[tmp];
	}
	if (!(cur->position).empty())
		res = cur->position;
	queue<node*> q;
	for (int i = 0; i < 38; i++)
		if (cur->pNext[i] != NULL)
			q.push(cur->pNext[i]);
	while (!q.empty()) {
		cur = q.front();
		q.pop();
		if (!cur->position.empty())
			res.insert(res.end(), cur->position.begin(), cur->position.end());
		for (int i = 0; i < 38; i++)
			if (cur->pNext[i] != NULL)
				q.push(cur->pNext[i]);
	}
	return res;
}

/****************************************************************/

void historyInsert(history_node* history_root, string s) {
	if (s.length() == 0)
		return;
	int tmp, length, cnt;
	history_node* cur = history_root;
	length = s.length();
	cnt = 0;
	while (cnt<length) {
		if ((int)(s[cnt] - '0') >= 0 && (int)(s[cnt] - '0') <= 9) {
			tmp = (int)(s[cnt] - '0');
			if (cur->pNext[tmp + 26] == NULL)
				cur->pNext[tmp + 26] = new history_node;
			cur = cur->pNext[tmp + 26];
		}
		else if (s[cnt] >= 'A'&&s[cnt] <= 'Z')
		{
			tmp = (int)(s[cnt] - 'A');
			if (cur->pNext[tmp] == NULL)
				cur->pNext[tmp] = new history_node;
			cur = cur->pNext[tmp];
		}
		else if ((int)(s[cnt] - 'a') >= 0 && (int)(s[cnt] - 'a') <= (int)('z' - 'a'))
		{
			tmp = (int)(s[cnt] - 'a');
			if (cur->pNext[tmp] == NULL)
				cur->pNext[tmp] = new history_node;
			cur = cur->pNext[tmp];
		}
		else if (s[cnt] == '#')
		{
			if (cur->pNext[36] == NULL)
				cur->pNext[36] = new history_node;
			cur = cur->pNext[36];
		}
		else if (s[cnt] == '$')
		{
			if (cur->pNext[37] == NULL)
				cur->pNext[37] = new history_node;
			cur = cur->pNext[37];
		}
		else if (s[cnt] == ' ') {
			if (cur->pNext[38] == NULL)
				cur->pNext[38] = new history_node;
			cur = cur->pNext[38];
		}
		cnt++;
	}
	bool check = true;
	length = cur->exist.size();
	for (cnt = 0; cnt < length; cnt++)
	{
		if (cur->exist[cnt] == s)
		{
			check = false;
			break;
		}
	}
	if (check)
		cur->exist.push_back(s);
	return;
}

/****************************************************************/

vector< string > historySearch(history_node* history_root, string s) {
	vector< string > res;
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	history_node* cur = history_root;
	int length = s.length(), i = 0;
	for (; i < length; i++) {
		int tmp;
		if ((int)(s[i] - '0') < 10 && (int)(s[i] - '0') >= 0)
			tmp = (int)(s[i] - '0') + 26;
		else if ((int)(s[i] - 'a') >= 0 && (int)(s[i] - 'a') <= (int)('z' - 'a'))
			tmp = (int)(s[i] - 'a');
		else if (s[i] == ' ')
			tmp = 38;
		else if (s[i] == '$')
			tmp = 37;
		else if (s[i] == '#')
			tmp = 36;
		else continue;
		if (cur->pNext[tmp] == NULL)
			return res;
		cur = cur->pNext[tmp];
	}
	if (!(cur->exist).empty())
	{
		length = cur->exist.size();
		for (i = 0; i < length; i++)
			res.push_back(cur->exist[i]);
	}
	Queue<history_node> q;
	for (int i = 0; i < 39; i++)
		if (cur->pNext[i] != NULL)
			q.Push_back(cur->pNext[i]);
	while (!q.Empty()) {
		cur = q.Pop_front();
		if (!cur->exist.empty())
		{
			length = cur->exist.size();
			for (i = 0; i < length; i++)
				res.push_back(cur->exist[i]);
		}
		for (int i = 0; i < 39; i++)
			if (cur->pNext[i] != NULL)
				q.Push_back(cur->pNext[i]);
	}
	return res;
}

string History_suggestion(history_node* history_root) {
	HANDLE h_std_out;
	COORD coord;
	h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int x, y;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	vector< string > res;
	string search_string = "";
	string tmp;
	char ch;
	int cnt = 0, cnt_ch = 0;

	ch = _getch();
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		x = csbi.dwCursorPosition.X;
		y = csbi.dwCursorPosition.Y;
	}
	string dbg;
	while (ch != 13) {
		if (ch == '\b')
		{
			dbg.clear();
			dbg = search_string.substr(0, search_string.length() - 1);
			search_string.clear();
			search_string = dbg;
			cnt_ch -= 2;
		}
		else
			search_string.push_back(ch);
		system("CLS");
		cout << search_string;
		res.clear();
		res = historySearch(history_root, search_string);
		coord.X = 0;
		coord.Y = y + cnt + 1;
		SetConsoleCursorPosition(h_std_out, coord);
		while (!res.empty()) {
			tmp = res.back();
			res.pop_back();
			cout << tmp << endl;
		}
		++cnt_ch;
		//ch = _getch();
		coord.X = cnt_ch;
		coord.Y = y;
		SetConsoleCursorPosition(h_std_out, coord);
		ch = _getch();
	}
	coord.X = 0;
	coord.Y = cnt + 1 + y;
	SetConsoleCursorPosition(h_std_out, coord);
	historyInsert(history_root, search_string);
	return search_string;
}
