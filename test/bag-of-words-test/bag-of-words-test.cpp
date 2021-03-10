#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

int main()
{
	const char *datafile[] = 
	{ 
		"data/utf8-econ.txt",
		"data/utf8-human.txt",
		"data/utf8-sci.txt",
		nullptr,
	};
	const char** datafilepos = datafile;

	map<string, int> bag_of_words;	
	char word[1024];
	char buf[10240];

	datafilepos--;
	while(*(++datafilepos))
	{
		FILE* fp = fopen(*datafilepos, "r");

		while(!feof(fp))
		{
			buf[0] = '\0';
			fgets(buf, sizeof(buf), fp);
			if(buf[0] == '\0') continue;

			stringstream ss(buf);
			ss >> word;
			bag_of_words[word]++;        
		}
		fclose(fp);
	}

	FILE* fp = fopen("utf8-bag-of-words.txt", "w");
	for(auto& [word, cnt] : bag_of_words)
	{
		fprintf(fp, "%s %d\n", word.c_str(), cnt);
	}
	fclose(fp);
}
