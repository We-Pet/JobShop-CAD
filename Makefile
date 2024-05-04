ifeq ($(OS),Windows_NT)

clean:
	del /Q tmp\*

remove:
	del /Q output_files\*\*.jss

buildS:
	gcc -o tmp\sequencial src\sequencial\main.c

buildP:
	gcc -o tmp\parallel src\parallel\main.c

runS-%:
	make buildS
	tmp\sequencial.exe input_files\$*.jss output_files\sequencial\$*.jss

runP-%:
	make buildP
	tmp\parallel.exe input_files\$*.jss output_files\parallel\$*.jss $(threads)

buildValidate:
	gcc -o tmp\output_validatorS src\output_validator\main.c

validateS-%:
	make buildValidate
	tmp\output_validator.exe input_files\ft$*.jss output_files\sequencial\ft$*.jss $*

validateP-%:
	make buildValidate
	tmp\output_validator.exe input_files\ft$*.jss output_files\parallel\ft$*.jss $*

else

clean:
	rm -f tmp/*

remove:
	rm -f output_files/*/*.jss

buildS:
	gcc -o tmp/sequencial src/sequencial/main.c

buildP:
	gcc -lpthread -o tmp/parallel src/parallel/main.c

runS-%:
	make buildS && ./tmp/sequencial input_files/$*.jss output_files/sequencial/$*.jss

runP-%:
	make buildP && ./tmp/parallel input_files/$*.jss output_files/parallel/$*.jss $(threads)

buildValidate:
	gcc -o tmp/output_validatorS src/output_validator/main.c

validateS-%:
	make buildValidate && ./tmp/output_validator input_files/ft$*.jss output_files/sequencial/ft$*.jss $*

validateP-%:
	make buildValidate && ./tmp/output_validator input_files/ft$*.jss output_files/parallel/ft$*.jss $*

endif