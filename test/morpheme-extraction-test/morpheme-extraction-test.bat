set MECAB=..\mecab-ko\mecab.exe -r ..\mecab-ko\mecabrc -d ..\mecab-ko\mecab-ko-dic\

%MECAB% rawdata\economy.txt -o utf8-econ.txt
%MECAB% rawdata\humanities.txt -o utf8-human.txt
%MECAB% rawdata\science.txt -o utf8-sci.txt
