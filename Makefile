CDFLAGS=-MM -MF $@ -MP -MT

CC=gcc

SRCS=rev.c
SRC=$(addprefix src/,$(SRCS))

INCS=rev.h
INC=$(addprefix inc/,$(INCS))
DEP=$(subst .c,.d,$(SRC))
OBJ=$(subst .c,.o,$(SRC))

all: rev

rev: $(OBJ)
	$(CC) -o rev $^ -lm

-include $(DEP)

%.d: %.c
	$(CC) $(CDFLAGS) $(subst .c,.o,$<) $<

%.o: %.c $(INC)
	$(CC) $(CFLAGS) -Iinc/ -c $< -o $@

clean:
	rm -rf $(DEP) $(OBJ) rev rev.exe
