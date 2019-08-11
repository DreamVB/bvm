all: 
	gcc bvm.c -o bvm
	gcc assembler.c -o assembler

clean: 
	rm bvm
	rm assembler
