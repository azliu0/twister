# compile flags
CC=gcc
CFLAGS=-fno-stack-protector
LDFLAGS=-L. -lmt19937
LIB_NAME=libmt19937.so

# object files for shared library
LIB_SRC=mt19937.c
LIB_OBJ=$(LIB_SRC:.c=.o)

# program source
PROG_SRC=twister.c
PROG_TEST_SRC=t_mt19937.cpp

# main challenge binary
PROG_TARGET=binaries/twister
PROG_TEST_TARGET=t_mt19937

# binary with mt19937 dynamically linked
# not used in challenge but this hides the mt19937
# implementations in the compiled executable
PROG_TARGET_LINKED=binaries/twister_linked

.PHONY: all clean test

all: $(LIB_NAME) $(PROG_TARGET) $(PROG_TARGET_LINKED)
test:
	g++ -std=c++17 $(PROG_TEST_SRC) mt19937.c -lgtest -o $(PROG_TEST_TARGET)
	./$(PROG_TEST_TARGET)

# compile twister object file
%.o: %.c
	$(CC) -fPIC -c $< -o $@ $(CFLAGS)

# create shared library from object file
# sudo cp $(LIB_NAME) /usr/local/lib
$(LIB_NAME): $(LIB_OBJ)
	$(CC) -shared -o $@ $^

# compile main challenge binary
$(PROG_TARGET): $(PROG_SRC) $(LIB_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# compile binary with mt19937 dynamically linked
$(PROG_TARGET_LINKED): $(PROG_SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(LIB_OBJ) $(LIB_NAME) $(PROG_TARGET) $(PROG_TARGET_LINKED) $(PROG_TEST_TARGET)
