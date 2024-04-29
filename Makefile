# Clean all temporary files
clean:
	del /Q tmp\*

# Remove all .jss files from output_files directory and its subdirectories
remove:
	del /Q output_files\*.jss

# Build sequential executable
buildS:
	gcc -o tmp\sequencial src\sequencial\main.c

# Build parallel executable
buildP:
	gcc -o tmp\parallel src\parallel\main.c -pthread

# Run sequential with input file $*
runS-%:
	make buildS
	tmp\sequencial input_files\$*.jss

# Run parallel with input file $* and $(threads) threads
runP-%:
	make buildP
	tmp\parallel input_files\$*.jss $(threads)
