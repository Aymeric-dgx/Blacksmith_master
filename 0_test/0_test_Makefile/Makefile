# Compilateur et options
CC = gcc
CFLAGS = -Wall -g

# Cibles
main_exe: main.o options.o
	$(CC) $(CFLAGS) main.o options.o -o main_exe
	@echo.
	.\main_exe.exe
	@echo.
	del -f *.o main_exe.exe

# Compilation des objets
main.o: main.c header.h
	$(CC) $(CFLAGS) -c main.c

options.o: options.c header.h
	$(CC) $(CFLAGS) -c options.c

# Nettoyage
clean:
	del -f *.o main_exe
