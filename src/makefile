EXEC = navigation
OBJS = main.o graph.o gml_scanner.o gml_parser.o
SRCS = .
INCS = ./inc

CC = gcc
CFLAGS = -Wall -O3 -c
LFLAGS = -Wall -O3 -o $(EXEC)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS)

main.o: $(SRCS)/main.c $(INCS)/graph.h $(INCS)/gml_parser.h
	$(CC) $(CFLAGS) $(SRCS)/main.c

graph.o: $(SRCS)/graph.c $(INCS)/graph.h
	$(CC) $(CFLAGS) $(SRCS)/graph.c

gml_scanner.o: $(SRCS)/gml_scanner.c $(INCS)/gml_scanner.h
	$(CC) $(CFLAGS) $(SRCS)/gml_scanner.c

gml_parser.o: $(SRCS)/gml_parser.c $(INCS)/gml_parser.h
	$(CC) $(CFLAGS) $(SRCS)/gml_parser.c

clean:
	@rm *.o