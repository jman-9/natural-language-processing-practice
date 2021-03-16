set MECAB=..\mecab-ko\mecab.exe -r ..\mecab-ko\mecabrc -d ..\mecab-ko\mecab-ko-dic\

call %MECAB% rawdata\sci-ext.txt -o utf8-sci-ext.txt
