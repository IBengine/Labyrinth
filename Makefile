COMPILATEUR=gcc
CFLAGS=-W -Wall -ansi -pedantic
EXEC=labypath0
MAIN=main_original2
# mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
# current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

all: $(EXEC) clean exe

		
$(EXEC): $(MAIN).o
		@echo ""
		@echo ""
		@echo "#	-	Editeur de lien       -        #"
		@echo ""
		@echo ""
		$(COMPILATEUR) -o $(EXEC) $(MAIN).o

$(MAIN).o: $(MAIN).c
		@echo ""
		@echo ""
		@echo "#	-	Compilation program C       -        #"
		@echo ""
		@echo ""
		$(COMPILATEUR) -o $(MAIN).o -c $(MAIN).c $(CFLAGS)

clean:
		cls
		@echo ""
		@echo ""
		@echo "#	-	Clean       -        #"
		@echo ""
		@echo ""
		del *.o
		
exe:
		@echo ""
		@echo ""
		@echo "#	-	Execution       -        #"
		@echo ""
		@echo ""
		./$(EXEC)
		@echo ""
		@echo ""
		@echo "#	-	Fin program C       -        #"
		@echo ""
		@echo ""