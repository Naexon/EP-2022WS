/* Ramanujan numbers are numbers that can be formed by adding two
   cubes of integer numbers in two (or more) different ways, i.e.
   i^3+j^3 = k^3+l^3
   See <http://www.durangobill.com/Ramanujan.html>

   This program counts the Ramanujan numbers up to a given
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>

// Struct that associates a composite value with its cubed elements
struct entry
{
  int k, l;
  long value;
};

// Used as comparison function in qsort()
// Takes to entries and returnes ...
//   -1 ... if value if p1 is strictly smaller than value of p2
//   0 ... if equal (inherited from '>' operator by C11 standard)
//   1 ... otherwise (inherited from '>' operator by C11 standard)
int comp_entry(const void *p1, const void *p2)
{
  const struct entry *e1 = p1;
  const struct entry *e2 = p2;
  if (e1->value < e2->value)
    return -1;
  else
    return e1->value > e2->value;
}

// Computes the cube of n
long cube(long n)
{
  return n * n * n; // NOTE: explicit inlining
}

// Computes a table size that is large enough to keep all I^3+J^3<n
size_t size_table(long n)
{
  return exp(log((double)n) * (2.0 / 3.0)) / 2 + 100; // NOTE: What is the math behind it?
}

// main function
int main(int argc, char **argv)
{
  long n;       // n value
  char *endptr; // for argument parsing and error output

  long i, j; // loop counters

  long count = 0; // number of Ramanujan numbers

  struct entry *table; // table holding computed entries
  size_t table_size;

  long m = 0; // counter to find position of entry in table (during looping)

  long checksum = 0; // sum of all Ramanujan numbers

  // Requires 2 arguments -> filename and n
  if (argc != 2)
    goto usage;

  // n is parsed to long -> endptr holds next char after valid base 10 numerical value (not valid num value if unequal to '\0')
  n = strtol(argv[1], &endptr, 10);
  if (*endptr != '\0')
    goto usage;

  // Computes table size and allocates memory
  // calloc requests memory and sets it to zero
  table_size = size_table(n);
  table = calloc(table_size, sizeof(struct entry)); // NOTE: free? malloc better if zero not required? Error of seg fault with 'make bench'?

  // loop for all k elements in each composite number
  for (i = 0; cube(i) <= n; i++) // NOTE: k^3 <= n
  {
    // loop for all l elements in each composite number
    for (j = i + 1; cube(i) + cube(j) <= n; j++) // NOTE: k < l, k^3 + l^3 <= n
    {
      table[m++] = (struct entry){i, j, cube(i) + cube(j)};
    }
  }

  // math assertion that table_size computation was correct and we do not run into an overflow of entries
  assert(m <= table_size); // NOTE: tighter bounds?

  // C lib function
  // Sorts <n> entries of size <sizeof(struct entry)> in <table> using the comparison function <comp_entry>
  // Asc order, allows dublication (otherwise algo would not work)
  qsort(table, m, sizeof(struct entry), comp_entry);

  // for all entries in the table (that are overwritten by the loop), check if their value is equal to the previous entry
  for (i = 1; i < m; i++)
  {
    if (table[i - 1].value == table[i].value) // true if pair is found
    {
      count++;
      checksum += table[i].value;

      while (i < m - 1 && table[i].value == table[i + 1].value) // skip all entries that have equal value
        i++;
    }
  }

  // Progam output
  printf("%ld Ramanujan numbers up to %ld, checksum=%ld\noccupation=%ld, size=%ld\n", count, n, checksum, m, table_size);
  printf("Memory usage: >=%ld\n", table_size * sizeof(struct entry));
  return 0;

// Prints usage message to stderr
usage:
  fprintf(stderr, "usage: %s <n>\n", argv[0]);
  exit(1);
}
