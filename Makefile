clean:
	rm -f tmp/*

remove:
	rm -f output_files/*/*.jss

buildS:
	gcc -o tmp/sequencial src/sequencial/main.c

buildP:
	gcc -lpthread -o tmp/parallel src/parallel/main.c

runS-%:
	make buildS && ./tmp/sequencial input_files/$*.jss

runP-%:
	make buildP && ./tmp/parallel input_files/$*.jss $(threads)
