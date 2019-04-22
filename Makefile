GCC     = gcc -Wall -Wextra -g
SOURCES = main.c speck-prng.c
HEADERS = speck-prng.h
OBJECTS = main.o speck-prng.o
EXECBIN = speck-test

all: ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} ${OBJECTS} -o ${EXECBIN}
	chmod +x ${EXECBIN}

%.o : %.c
	${GCC} -c $<

clean:
	rm ${OBJECTS}

spotless: clean
	rm ${EXECBIN}
