main.run: main.o xml.o animation.o weapon.o common.o monster.o
	gcc -g -o main.run main.o xml.o animation.o weapon.o common.o monster.o -lSDL2 -lSDL2_image -lm

testxml.o: testxml.c
	gcc -g -c testxml.c

xml.o: xml.c xml.h
	gcc -g -c xml.c 

animation.o: animation.c animation.h
	gcc -g -c animation.c

main.o: main.c
	gcc -g -c main.c

weapon.o: weapon.c weapon.h
	gcc -g -c weapon.c
	
common.o: common.c common.h
	gcc -g -c common.c

monster.o: monster.c monster.h
	gcc -g -c monster.c

clean:
	rm -f *.o
