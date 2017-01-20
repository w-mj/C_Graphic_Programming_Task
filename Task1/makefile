main:main.o
	gcc -o main.out main.o -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm -g
main.o:
	gcc -c main.c -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
clean:
	rm main.o
