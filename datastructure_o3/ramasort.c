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
#include <limits.h>

struct entry
{
  int k, l;
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
  return n * n * n;
}

size_t size_heap(long n)
/* compute a table size that is large enough to keep all I^3+J^3<n */
{
  return cbrt((double)n);
}

// HEAP    ///////////////////////////////////////////////////////
int maxNumbers;
int heapSize;
struct entry *heap;
/*Initialize Heap*/
void Init()
{
  heapSize = 0;
  heap[0] = (struct entry){0, 0, -INT_MAX};
}

/*Insert an element into the heap */
int Insert(struct entry element)
{
  heapSize++;
  heap[heapSize] = element; /*Insert in the last place*/
  /*Adjust its position*/
  int now = heapSize;
  int now_half = now >> 1;
  while (heap[now_half].value > element.value)
  {
    heap[now] = heap[now_half];
    now = now_half;
    now_half = now_half >> 1;
  }
  heap[now] = element;
  return now;
}

int CheckForOneSameSum(int i)
{
  int c = 0;

  long val = heap[1].value;

  if (2 < heapSize)
  {
    if (heap[2].value == val)
      c++;
  }
  if (4 < heapSize)
  {
    if (heap[4].value == val)
      if (c == 1)
        return 0;
      else
        c++;
  }
  if (5 < heapSize)
  {
    if (heap[5].value == val)
      if (c == 1)
        return 0;
      else
        c++;
  }
  if (3 < heapSize)
  {
    if (heap[3].value == val)
      if (c == 1)
        return 0;
      else
        c++;
  }
  if (6 < heapSize)
  {
    if (heap[6].value == val)
      if (c == 1)
        return 0;
      else
        c++;
  }
  if (7 < heapSize)
  {
    if (heap[7].value == val)
      if (c == 1)
        return 0;
      else
        c++;
  }

  if (c == 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

struct entry DeleteMin()
{
  /* heap[1] is the minimum element. So we remove heap[1]. Size of the heap is decreased.
   Now heap[1] has to be filled. We put the last element in its place and see if it fits.
   If it does not fit, take minimum element among both its children and replaces parent with it.
   Again See if the last element fits in that place.*/
  struct entry minElement, lastElement;
  int now, child;
  minElement = heap[1];
  lastElement = heap[heapSize--];
  /* now refers to the index at which we are now */
  for (now = 1; now * 2 <= heapSize; now = child)
  {
    /* child is the index of the element which is minimum among both the children */
    /* Indexes of children are i*2 and i*2 + 1*/
    child = now * 2;
    /*child!=heapSize beacuse heap[heapSize+1] does not exist, which means it has only one
     child */
    if (child != heapSize && heap[child + 1].value < heap[child].value)
    {
      child++;
    }
    /* To check if the last element fits ot not it suffices to check if the last element
     is less than the minimum element among both the children*/
    if (lastElement.value > heap[child].value)
    {
      heap[now] = heap[child];
    }
    else /* It fits there */
    {
      break;
    }
  }
  heap[now] = lastElement;
  return minElement;
}

void printHeap()
{

  for (int i = 1; i <= heapSize; i++)
  {
    printf("k %ld, l %ld, value %ld\n", heap[i].k, heap[i].l, heap[i].value);
  }

  printf("\n");
}
// HEAP    ///////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  long n;
  char *endptr;
  long count = 0;
  long m = 0;
  long checksum = 0;

  if (argc != 2)
    goto usage;
  n = strtol(argv[1], &endptr, 10);
  if (*endptr != '\0')
    goto usage;
  // table_size = size_table(n);
  // table = calloc(table_size,sizeof(struct entry));
  maxNumbers = size_heap(n);
  heap = malloc((maxNumbers + 10) * sizeof(struct entry));

  Init();

  for (int j = 1; j <= maxNumbers; j++)
  {
    Insert((struct entry){0, j, cube(j)});
  }
  printf("hSize %ld\n", maxNumbers);

  int cc = 0;
  while (heapSize > 0)
  {
    cc++;
    // printf("size %ld\n", heapSize);
    // printHeap();
    if (CheckForOneSameSum(1))
    {
      count++;
      // printf("found:       k %ld, l %ld, value %ld, size %ld\n", heap[1].k, heap[1].l, heap[1].value, heapSize);
      checksum += heap[1].value;
    }

    struct entry min = DeleteMin();
    // printf("k%ld, l%ld, value %ld\n",min.k, min.l, min.value);

    // Insert new node if i < j
    min.k++;
    if (min.k < min.l && cube(min.k) + cube(min.l) <= n)
    {
      min.value = cube(min.k) + cube(min.l);
      Insert(min);
    }
  }

  /*assert(m<=table_size);
  qsort(table, m, sizeof(struct entry), comp_entry);
  for (i=1; i<m; i++) {
    if (table[i-1].value==table[i].value) {
      count++;
      checksum += table[i].value;
      while (i<m-1 && table[i].value==table[i+1].value)
        i++;
    }
  } */

  printf("%ld Ramanujan numbers up to %ld, checksum=%ld\noccupation=%ld, size=%ld\n", count, n, checksum, m, 0);
  printf("Memory usage: >=%ld\n", maxNumbers * sizeof(struct entry));
  printf("cc %ld\n", cc);

  return 0;

usage:
  fprintf(stderr, "usage: %s <n>\n", argv[0]);
  exit(1);
}
