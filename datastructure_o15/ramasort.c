/* Ramanujan numbers are numbers that can be formed by adding two
   cubes of integer numbers in two (or more) different ways, i.e.
   i^3+j^3 = k^3+l^3
   See <http://www.durangobill.com/Ramanujan.html>

   This program counts the Ramanujan numbers up to a given
*/

// based on o14 -> make everything global -> very bad

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

// size_heap
int s;
unsigned int y;
unsigned long b;

// HEAP    ///////////////////////////////////////////////////////

// Init
int heapSize;
struct entry *heap;

// Insert
int now;
int now_half;

// CheckForOneSameSum
int c = 0;
long val;

// DeleteMin
struct entry minElement_d, lastElement_d;
int now_d, child_d, now2_d;

// ReplaceMin
struct entry minElement, lastElement;
int now, child, now2;

// main
int maxNumbers;

long n;
char *endptr;
long count = 0;
long m = 0;
long checksum = 0;
int cc = 0;
int j;

long inline cube(long n)
{
  return n * n * n;
}

size_t size_heap(long x)
/* compute a table size that is large enough to keep all I^3+J^3<n */
{
  y = 0;
  for (s = 63; s >= 0; s -= 3)
  {
    y += y;
    b = 3 * y * ((unsigned long)y + 1) + 1;
    if ((x >> s) >= b)
    {
      x -= b << s;
      y++;
    }
  }
  return y;
}

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
  now_d = heapSize;
  now_half = now_d >> 1;

  while (heap[now_half].value > element.value)
  {
    heap[now_d] = heap[now_half];
    now_d = now_half;
    now_half = now_half >> 1;
  }

  heap[now_d] = element;
  return now_d;
}

int inline CheckForOneSameSum(int i)
{
  val = heap[1].value;

  if (2 < heapSize)
  {
    if (heap[2].value == val)
      c++;
  }

  if (4 < heapSize)
  {
    if (heap[4].value == val)
      c++;
  }

  if (5 < heapSize)
  {
    if (heap[5].value == val)
      c++;
  }

  if (3 < heapSize)
  {
    if (heap[3].value == val)
      c++;
  }

  if (6 < heapSize)
  {
    if (heap[6].value == val)
      c++;
  }

  if (7 < heapSize)
  {
    if (heap[7].value == val)
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

  minElement_d = heap[1];
  lastElement_d = heap[heapSize--];

  /* now refers to the index at which we are now */
  for (now_d = 1, now2_d = 2; now2_d <= heapSize; now_d = child_d, now2_d = now_d << 1)
  {
    /* child_d is the index of the element which is minimum among both the children */
    /* Indexes of children are i*2 and i*2 + 1*/
    child_d = now2_d;

    /*child_d!=heapSize beacuse heap[heapSize+1] does not exist, which means it has only one
     child_d */
    if (child_d != heapSize && heap[child_d + 1].value < heap[child_d].value)
    {
      child_d++;
    }

    /* To check if the last element fits ot not it suffices to check if the last element
     is less than the minimum element among both the children*/
    if (lastElement_d.value > heap[child_d].value)
    {
      heap[now_d] = heap[child_d];
    }
    else /* It fits there */
    {
      break;
    }
  }

  heap[now_d] = lastElement_d;

  return minElement_d;
}

struct entry ReplaceMin(struct entry newEntry)
{
  /* heap[1] is the minimum element. So we remove heap[1]. Size of the heap is decreased.
   Now heap[1] has to be filled. We put the last element in its place and see if it fits.
   If it does not fit, take minimum element among both its children and replaces parent with it.
   Again See if the last element fits in that place.*/

  minElement = heap[1];
  lastElement = newEntry;

  /* now refers to the index at which we are now */
  for (now = 1, now2 = 2; now2 <= heapSize; now = child, now2 = now << 1)
  {
    /* child is the index of the element which is minimum among both the children */
    /* Indexes of children are i*2 and i*2 + 1*/
    child = now2;

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

// HEAP    ///////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  if (argc != 2)
    goto usage;

  n = strtol(argv[1], &endptr, 10);
  if (*endptr != '\0')
    goto usage;

  maxNumbers = size_heap(n);
  heap = malloc((maxNumbers + 10) * sizeof(struct entry));

  Init();

  for (j = 1; j <= maxNumbers; j++)
  {
    Insert((struct entry){0, j, cube(j)});
  }

  printf("hSize %ld\n", maxNumbers);

  while (heapSize > 0)
  {
    struct entry min = heap[1];
    cc++;

    if (CheckForOneSameSum(1))
    {
      count++;
      checksum += min.value;
    }

    // Insert new node if i < j
    min.k++;
    if (min.k < min.l && cube(min.k) + cube(min.l) <= n)
    {
      min.value = cube(min.k) + cube(min.l);
      ReplaceMin(min);
    }
    else
    {
      DeleteMin();
    }
  }

  printf("%ld Ramanujan numbers up to %ld, checksum=%ld\noccupation=%ld, size=%ld\n", count, n, checksum, m, 0);
  printf("Memory usage: >=%ld\n", maxNumbers * sizeof(struct entry));
  printf("cc %ld\n", cc);

  return 0;

usage:
  fprintf(stderr, "usage: %s <n>\n", argv[0]);
  exit(1);
}
