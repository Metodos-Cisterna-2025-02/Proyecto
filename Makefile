CC = gcc
CFLAGS = -Wall -g

# Find all .c files containing main()
MAINSRC := $(shell grep -l 'int main' $(shell find src/ -name '*.c'))
MAINEXE := $(MAINSRC:%.c=%)

all: $(MAINEXE)

%: %.c
	$(CC) $(CFLAGS) "$<" -o "$@" -lm

clean:
	rm -f $(MAINEXE)