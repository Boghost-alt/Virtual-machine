CC = clang
EXEC = main
SRC = main.c

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) -o $(EXEC) $(SRC)

clean:
	rm -f $(EXEC)
