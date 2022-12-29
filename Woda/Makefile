MEMORY = 102400 # Memory in KB
N = 10000000000000
RAMA = ramasort
CC = gcc
CFLAGS = -O -Wall
DFLAGS = -Og -Wall # Include debug symbols

# Switch correct flags or do not copy back this makefile !!!!!!!!!!!!!!!!

SOURCES = ramasort.c ramanujan.c Makefile

bench: $(RAMA)
	ulimit -S -v $(MEMORY); perf stat -e cycles -e instructions -e branch-misses -e LLC-load-misses -e LLC-store-misses ./$(RAMA) $(N)

benchCSV: $(RAMA)
	ulimit -S -v $(MEMORY); perf stat -x "," -e cycles -e instructions -e branch-misses -e LLC-load-misses -e LLC-store-misses ./$(RAMA) $(N)

clean:
	rm ramasort ramanujan

ramanujan: ramanujan.c
	$(CC) $(DFLAGS) ramanujan.c -lm -o ramanujan

ramasort: ramasort.c
	$(CC) $(DFLAGS) ramasort.c -lm -o ramasort

dist: ../ramanujan.tar.gz

../ramanujan.tar.gz: $(SOURCES)
	cd .. && tar cfz ramanujan.tar.gz ramanujan/ramasort.c ramanujan/ramanujan.c ramanujan/Makefile
