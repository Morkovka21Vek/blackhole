TARGET =
TEST_TARGET = test
SRC = netstick.c
OBJ = $(SRC:.c=.o)

$(TEST_TARGET): $(OBJ)
	g++ $(OBJ) -o test

%.o:%.c
	g++ -c $< -o $@

clean:
	@rm -f test *.o
