# Makefile for Saliency using libsaliency.a

.IGNORE:

.KEEP_STATE:

.SUFFIXES: .o .c

.c.o: 
	$(CC) $(FFLAGS) $(CCFLAGS) -c $< -o $@

CC = gcc

FFLAGS +=
CCFLAGS += -g -Wall 

LIB_SAL = libsaliency.a
BIN = saliency

SRCS = $(wildcard *.c) 
OBJS = $(SRCS:.c=.o)

SUBDIR_SAL = "libsaliency"

all: $(LIB_SAL) $(BIN)

$(BIN): $(OBJS) $(LIB_SAL)
	$(CC) $(FFLAGS) $(CCFLAGS) -o $(BIN) $(OBJS) -I . -L . $(LIB_SAL)

$(LIB_SAL): 
	@cd $(SUBDIR_SAL) ; make
	
clean:
	rm -rf $(OBJS) $(BIN) $(LIB_SAL) $(BIN).gdb
	cd $(SUBDIR_SAL); make clean

