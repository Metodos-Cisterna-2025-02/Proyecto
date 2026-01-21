CC = gcc
CFLAGS = -Wall -g

# 1. Encuentra archivos con main (prueba.c, main.c, etc.)
MAINSRC := $(shell grep -l 'int main' $(shell find src/ -name '*.c'))
MAINEXE := $(MAINSRC:%.c=%)

# 2. Encuentra archivos de soporte (interfaz.c, etc.) - Los que NO tienen main
SUPPORT_SRC := $(shell grep -L 'int main' $(shell find src/ -name '*.c'))

all: $(MAINEXE)

# REGLA CORREGIDA: Ahora incluye SUPPORT_SRC
%: %.c $(SUPPORT_SRC)
	$(CC) $(CFLAGS) $^ -o "$@" -lm

clean:
	rm -f $(MAINEXE)