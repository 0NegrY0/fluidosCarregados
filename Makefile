#Makefile padrão simplificado (sem warnings/com math.h) 1/2/1 (main/data/header)
#Escrito por Carlos Negri

$(CC) = gcc

final: main.o data.o movimento.o
	$(CC) main.o data.o movimento.o -o final -w -lm

main.o: main.c data.h
	$(CC) -c main.c

data.o: data.c data.h
	$(CC) -c data.c

movimento.o: movimento.c data.h
	$(CC) -c movimento.c

clean:
	rm *.o final

bin:
	rm *.bin

txt:
	rm *.txt

clean_all:
	rm *.o *.bin *.txt final