main: xml.o testxml.o
	gcc -o main.run testxml.o xml.o -g

testxml.o: testxml.c
	gcc -c testxml.c

xml.o: xml.c xml.h
	gcc -c xml.c
