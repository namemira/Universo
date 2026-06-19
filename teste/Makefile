# =========================================================
# Makefile — Simulador de Universo
# Compilador: gcc (MinGW no Windows)
# =========================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99
TARGET  = simulador.exe

SRCS = main.c tabelas.c io.c pilha.c universo.c mundo.c \
       civilizacao.c tempo.c evento.c doenca.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c universo.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	del /Q *.o $(TARGET) 2>nul || true
