# Sudo apt-get install libboost-all-dev
CC = g++
CFLAGS = -Wall
DEST = /usr/local/bin
# LDFLAGS = 
LIBS = -lboost_program_options -lboost_filesystem
OBJS = src.o
PROGRAM = pdf2eps

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)

%.o : %.c
	gcc -o $@ -c $<

clean:; rm -f *.o *~ $(PROGRAM)

install: $(PROGRAM)
	install -s $(PROGRAM) $(DEST)