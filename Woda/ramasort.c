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

struct entry {
  int k,l;
  long value;
};

int comp_entry(const void *p1, const void *p2)
{
  const struct entry *e1 = p1;
  const struct entry *e2 = p2;
  if (e1->value < e2->value)
    return -1;
  else
    return e1->value > e2->value;
}

long cube(long n)
{
  return n*n*n;
}

size_t size_table(long n)
/* compute a table size that is large enough to keep all I^3+J^3<n */
{
  return exp(log((double)n)*(2.0/3.0))/2+100;
}

int main(int argc, char **argv)
{
  long n;
  char *endptr;
  long i,j;
  long count=0;
  struct entry *table;
  size_t table_size;
  long m=0;
  long checksum = 0;
  
  if (argc != 2)
    goto usage;
  n = strtol(argv[1],&endptr,10);
  if (*endptr != '\0')
    goto usage;
  table_size = size_table(n);
  table = calloc(table_size,sizeof(struct entry));

  for (i=0; cube(i)<=n; i++) {
    for (j=i+1; cube(i)+cube(j)<=n; j++) {
      table[m++] = (struct entry){i,j,cube(i)+cube(j)};
    }
  }
  assert(m<=table_size);
  qsort(table, m, sizeof(struct entry), comp_entry);
  for (i=1; i<m; i++) {
    if (table[i-1].value==table[i].value) {
      count++;
      checksum += table[i].value;
      while (i<m-1 && table[i].value==table[i+1].value)
        i++;
    }
  }
  printf("%ld Ramanujan numbers up to %ld, checksum=%ld\noccupation=%ld, size=%ld\n",count,n,checksum,m,table_size);
  printf("Memory usage: >=%ld\n",table_size*sizeof(struct entry));
  return 0;

 usage:
  fprintf(stderr,"usage: %s <n>\n",argv[0]);
  exit(1);
}
