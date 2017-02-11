main:
	clang -Werror -Wall -o evalexp evalexpmain.c evalexp.c
test:
	clang -Werror -Wall -o test mytest.c evalexp.c
clean:
	rm -f evalexp test *~
