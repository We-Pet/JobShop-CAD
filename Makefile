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

validateS-%:
	gcc -o tmp/output_validator src/output_validator/main.c && ./tmp/output_validator output_files/sequencial/$*.jss

buildValidateP:
	gcc -o tmp/output_validator src/output_validator/main.c
validateP-%:
	make buildValidateP && ./tmp/output_validator input_files/ft0$*.jss output_files/parallel/ft_$*.jss $* 