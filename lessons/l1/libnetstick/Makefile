TARGET =
TEST_TARGET = test.out
SRC = netstick.c
CFLAGS = 
OBJ = $(SRC:.c=.o)


test: $(TEST_TARGET)

$(TEST_TARGET): $(OBJ)
	gcc $^ -o $@

%.o: %.c
	gcc -c $< -o $@ $(CFLAGS)


.PHONY: clean debug

debug:
	$(info )
	$(info =========================)
	$(info TARGET = $(TARGET))
	$(info TEST_TARGET = $(TEST_TARGET))
	$(info SRC = $(SRC))
	$(info OBJ = $(OBJ))
	$(info =========================)
	$(info )

clean:
	@rm -f test *.o 2> /dev/null
