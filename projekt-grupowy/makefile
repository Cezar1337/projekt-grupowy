# change application name here (executable output name)
TARGET=switch_emulator

#compiler
CC=gcc
#debug
debug=-g
#optimisation
OPT=-O0
#warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

#linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -rdynamic

OBJS=	mainFileProject.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

mainFileProject.o: src/mainFileProject.c
	$(CC) -c $(CCFLAGS) src/mainFileProject.c $(GTKLIB) -o mainFileProject.o

clean:
	rm -F *.o $(TARGET)
