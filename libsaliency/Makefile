# Makefile for saliency_lib

.IGNORE:

.KEEP_STATE:

.SUFFIXES: .o .c

.c.o: 
	$(CC) $(FFLAGS) $(CCFLAGS) -c $< -o $@
.s.o: 
	$(AS) $< -o $@

CROSS = 

CC = $(CROSS)gcc
AS = $(CROSS)as
AR = $(CROSS)ar
RANLIB = $(CROSS)ranlib

FFLAGS += 
CCFLAGS += -g -Wall

UPDATE_OFFSETS = x

LIB = libsaliency.a

SRCS = $(wildcard *.c) 
ASM_SRCS = $(wildcard *.s)
OBJS = $(SRCS:.c=.o) $(ASM_SRCS:.s=.o)

all: $(LIB)


$(LIB):	$(OBJS)
	$(AR) rcu ../$@ $(OBJS)
	$(RANLIB) ../$@

clean:
	rm -rf $(OBJS)
