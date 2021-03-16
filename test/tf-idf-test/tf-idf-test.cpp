#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
using namespace std;

#define TEXT_FILE "rawdata/sci-ext.txt"
#define MORPHEME_FILE "utf8-sci-ext.txt"

int main()
{
	int r = system("morpheme-extraction.bat");
	if(r)
	{
		cout << "some error:" << r << endl;
		return r;
	}

	const char *datafile[] =
	{
		MORPHEME_FILE,
		nullptr,
	};
	const char** datafilepos = datafile;

	map<string, int> bag_of_words;
	map<string, bool> bag_of_words_in_sentence;
	map<string, int> bag_of_sentence_words;
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
			if(string("EOS") == word) continue;
			bag_of_words[word]++;
			bag_of_words_in_sentence[word] = true;
			if(string(".") == word)
			{
				for(auto& [word, exist] : bag_of_words_in_sentence)
				{
					bag_of_sentence_words[word]++;
				}
				bag_of_words_in_sentence.clear();
			}
		}
		fclose(fp);
	}

	int sentence_cnt = bag_of_words["."];
	int word_cnt = 0;
	for(auto& [word, cnt] : bag_of_words)
	{
		word_cnt += cnt;
	}


	struct word_info
	{
		char word[100];
		int appearance_cnt;
		int appearanced_sentence_cnt;
		double tf;
		double idf;
		double tf_idf;
	};

	vector<word_info> word_info_list;
	for(auto& [word, cnt] : bag_of_words)
	{
		word_info_list.resize(word_info_list.size()+1);
		word_info& wi = word_info_list.back();
		strcpy(wi.word, word.c_str());
		wi.appearance_cnt = cnt;
		wi.appearanced_sentence_cnt = bag_of_sentence_words[word];
		wi.tf = (double)cnt / word_cnt;
		wi.idf = log10((double)sentence_cnt / wi.appearanced_sentence_cnt);
		wi.tf_idf = wi.tf * wi.idf;
	}

	qsort(word_info_list.data(), word_info_list.size(), sizeof(word_info),
		[](const void* c1, const void* c2) -> int {
			const word_info* wi1 = (const word_info*)c1;
			const word_info* wi2 = (const word_info*)c2;

			if(wi2->appearance_cnt == wi1->appearance_cnt)
			{
				return wi2->appearanced_sentence_cnt - wi1->appearanced_sentence_cnt;
			}
			else
			{
				return wi2->appearance_cnt - wi1->appearance_cnt;
			}
		;
	});

	vector<char> sbuf(1024 * 1024);
	char* pos = sbuf.data();

	pos += sprintf(pos, "a.cnt: appearance count, a.s.cnt: appearanced sentence count\n");
	pos += sprintf(pos, "TF: Term Frequency, IDF: Inverse Document Frequency\n\n");
	pos += sprintf(pos, "no.\tmorpheme       \ta.cnt\ta.s.cnt\t  TF  \t IDF  \tTF-IDF\n");

	int num = 1;
	for(auto& wi : word_info_list)
	{
		pos += sprintf(pos, "%3d\t%-15s\t %3d \t  %3d  \t%.4lf\t%.4lf\t%.4lf\n",
			num++, wi.word, wi.appearance_cnt, wi.appearanced_sentence_cnt, wi.tf, wi.idf, wi.tf_idf);
	}
	printf("%s\n\n", sbuf.data());

	const char* fn = "utf8-tf-idf-result.txt";
	printf("The result is saved at '%s'.\n\n", fn);

	FILE* fp = fopen(fn, "w");
	fwrite(sbuf.data(), 1, pos - sbuf.data(), fp);
	fclose(fp);
}
