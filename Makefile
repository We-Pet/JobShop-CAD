clean:
	rm tmp/*

buildS:
	gcc -o tmp/main src/sequencial/main.c

buildP:
	gcc -o tmp/main src/parallel/main.c

run:
	./tmp/main

matrix3:
	./tmp/main input_files/ft03.jss

matrix6:
	./tmp/main input_files/ft06.jss

matrix10:
	./tmp/main input_files/ft10.jss

matrix20:
	./tmp/main input_files/ft20.jss
