MEMORY = 10002400 # Memory in KB, original: 102400
N = 10000000000000 # original: 10000000000000
RAMA = ramasort
CC = gcc
CFLAGS = -O -Wall
DFLAGS = -Og -Wall # Include debug symbols

# Switch correct flags or do not copy back this makefile !!!!!!!!!!!!!!!!

SOURCES = ramasort.c ramanujan.c Makefile

bench: $(RAMA)
	ulimit -S -v $(MEMORY); perf stat -e cycles -e instructions -e branch-misses -e LLC-load-misses -e LLC-store-misses ./$(RAMA) $(N)

benchCSV: $(RAMA)
	ulimit -S -v $(MEMORY); perf stat -x , -e cycles -e instructions -e branch-misses -e LLC-load-misses -e LLC-store-misses -e duration_time ./$(RAMA) $(N)

clean:
	rm ramasort ramanujan

ramanujan: ramanujan.c
	$(CC) $(DFLAGS) ramanujan.c -lm -o ramanujan

ramasort: ramasort.c
	$(CC) $(DFLAGS) ramasort.c -lm -o ramasort

test: test.c
	$(CC) -O3 -Wall test.c -lm -o test

dist: ../ramanujan.tar.gz

../ramanujan.tar.gz: $(SOURCES)
	cd .. && tar cfz ramanujan.tar.gz ramanujan/ramasort.c ramanujan/ramanujan.c ramanujan/Makefile
