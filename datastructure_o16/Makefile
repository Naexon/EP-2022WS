#Memory in KB
MEMORY = 102400
N = 10000000000000
RAMA = ramasort
CC = gcc
CFLAGS = -O3 -Wall #  -DDONT_USE_VOL

SOURCES = ramasort.c ramanujan.c Makefile

bench: $(RAMA)
	ulimit -S -v $(MEMORY); perf stat -e cycles -e instructions -e branch-misses -e LLC-load-misses -e LLC-store-misses ./$(RAMA) $(N)

clean:
	rm ramasort ramanujan

ramanujan: ramanujan.c
	$(CC) $(CFLAGS) ramanujan.c -lm -o ramanujan

ramasort: ramasort.c
	$(CC) $(CFLAGS) ramasort.c -lm -o ramasort

dist: ../ramanujan.tar.gz

../ramanujan.tar.gz: $(SOURCES)
	cd .. && tar cfz ramanujan.tar.gz ramanujan/ramasort.c ramanujan/ramanujan.c ramanujan/Makefile
