CC = clang
EXEC = main
SRC = main.c

all : $(EXEC)

$(EXEC) : $(SRC)
	CC $(SRC) -o $(EXEC)


clean:
	rm -f $(EXEC)
