IDIR = ./include
CFLAGS = -Wall -Werror -g -I$(IDIR)
CC = gcc

ODIR = ./src/obj

_DEPS =  roomba_control.h#.h files
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = columbus.o roomba_control.o#.o files
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TEST_OBJ = roomba_control_test.o roomba_control.o#.o files
TEST_OBJ = $(patsubst %,$(ODIR)/%,$(_TEST_OBJ))

$(ODIR)/%.o: ./src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	

all: $(OBJ)
	make columbus
	make test
	
columbus: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lm
	
test: $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lcheck `pkg-config --cflags --libs check`
	
clean: 
	$ rm -f $(ODIR)/*.o core $(INCDIR)/*~
	$ rm -f ./columbus
	$ rm -f ./test
	$ clear
	
	
.PHONY: clean
