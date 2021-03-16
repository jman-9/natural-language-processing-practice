#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

int main()
{
    map<string, int> bag_of_words;

	char buf[10240];
	char word[1024];
	int cnt;

	FILE* fp = fopen("data/utf8-bag-of-words.txt", "r");
	while(!feof(fp))
	{
		buf[0] = '\0';
		fgets(buf, sizeof(buf), fp);
		if(buf[0] == '\0') continue;

		stringstream ss(buf);
		ss >> word >> cnt;
		if(string("EOS") == word) continue;
		bag_of_words[word] = cnt;
	}
	fclose(fp);

	fp = fopen("data/origin_stopwords.txt", "r");
	while(!feof(fp))
	{
		buf[0] = '\0';
		if(fscanf(fp, "%s", buf) < 1) continue;
		if(buf[0] == '\0') continue;

		bag_of_words.erase(buf);
	}
	fclose(fp);

	fp = fopen("utf8-bag-of-words-removed-stop-words.txt", "w");
	for(auto& [word, cnt] : bag_of_words)
	{
		fprintf(fp, "%s %d\n", word.c_str(), cnt);
	}
	fclose(fp);
}
