# shell

## How to use

- 準備

```
$ git clone https://github.com/mamizu-git/shell.git
$ cd shell
$ make
```

- 起動

```
$ ./ish
```

- 終了

`Ctrl` + `C`

## What I implemented

- 標準入力からコマンドを受け取り実行 (絶対パスで指定)
```
ish$ /bin/ls
Makefile  ish  main.c  main.o  myexec.c  myexec.o  parser
ish$ /bin/ls -la
total 44
drwxr-xr-x  9 root root   288 Dec 14 16:44 .
drwxr-xr-x 20 root root   640 Dec 14 16:44 ..
-rw-rw-r--  1 root root   190 Jun  1  2021 Makefile
-rwxr-xr-x  1 root root 17928 Dec 14 16:42 ish
-rw-rw-r--  1 root root   422 Jun 19 06:54 main.c
-rw-r--r--  1 root root  2160 Dec 14 16:42 main.o
-rw-rw-r--  1 root root  2316 Jun 19 06:55 myexec.c
-rw-r--r--  1 root root  4208 Dec 14 16:42 myexec.o
drwxr-xr-x  7 root root   224 Dec 14 16:42 parser
```
- リダイレクション
```
ish$ /usr/bin/wc -l < /proc/meminfo > out
ish$ /bin/cat out
50
```
- 追記出力リダイレクション
```
ish$ /usr/bin/wc -l < /proc/meminfo > out
ish$ /bin/cat out
50
ish$ /usr/bin/wc -c < /proc/meminfo >> out
ish$ /bin/cat out
50
1391
```
- パイプ
```
ish$ /bin/echo Hello,world! | /usr/bin/wc -c > out
ish$ /bin/cat out
13
```
- 複数のリダイレクト・パイプの組み合わせ
```
ish$ /usr/bin/wc -c < /proc/meminfo | /bin/cat | /bin/cat | /bin/cat > out
ish$ /bin/cat out
1391
```
