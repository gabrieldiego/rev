CDFLAGS=-E -MM

CC=gcc

SRCS=rev.c config.c
SRC=$(addprefix src/,$(SRCS))

INCS=rev.h config.h
INC=$(addprefix inc/,$(INCS))
INCLUDES=-Iinc/
DEP=$(subst .c,.d,$(SRC))
OBJ=$(subst .c,.o,$(SRC))

all: rev

rev: $(OBJ)
	$(CC) -o rev $^ -lm

-include $(DEP)

%.d: %.c
	$(CC) $(INCLUDES) $(CDFLAGS) $(CFLAGS) $< >> $@

%.o: %.c $(INC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(DEP) $(OBJ) rev rev.exe
