#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>

struct position_t {
  uint64_t x_cube;
  uint64_t y_cube;

  uint32_t x_cube_diff;
  uint32_t y_cube_diff;

  uint16_t x_element;
  uint16_t y_element;
};

uint64_t cube(uint16_t x) {
  return (uint64_t) x * (uint64_t) x * (uint64_t) x;
}

uint32_t cube_root_int(uint64_t n)
{

  // TODO there is a problem with this function

  int32_t s;
  uint32_t y;
  uint64_t b;

  y = 0;
  for (s = 63; s >= 0; s -= 3)
  {
    y += y;
    b = 3 * y * ((uint64_t)y + 1) + 1;
    if ((n >> s) >= b)
    {
      n -= b << s;
      y++;
    }
  }
  return y;
}

int main(int argc, char **argv)
{

  // defintions of problem and partition size not parsed from exec call

  uint64_t PROBLEM_SIZE = 10000000000000;
  uint8_t PARTION_SIZE = 40000000;

  // because of integer rounding in the root function, this is used to recalculate the cubes

  uint16_t rightmost_element = cube_root_int(PROBLEM_SIZE);
  uint64_t rightmost_cube = cube(rightmost_element);

  printf("rm elem %ld - rm cube %ld\n", rightmost_element, rightmost_cube);

  // we calculate another cube to the right, in order to obtain the first difference in order to calculate the cubes
  // the difference can be used to iterate over cubes without using multiplication

  struct position_t current = {
    rightmost_cube, 1,
    cube(rightmost_element + 1) - rightmost_cube, 1,
    rightmost_element, 1
  };

  printf("first diff %ld\n", current.x_cube_diff);

  // count and checksum are calculated in the result_composit
  // count in the left 32 bits (can be changed) and checksum in the right 32 bits
  // this allows us to simply add the ramanujan numbers to the composit and add a shifted 1 for the count

  uint64_t result_composite = 0;

  uint64_t count = 0;
  uint64_t count_increment = 1 << 32;

  uint64_t checksum = 0;
  uint64_t checksum_map = 4294967295;  

  // for the table we store 32 2-bit numbers in a 64 bit unsigned integer
  // each integer corresponds to a bucket and we have a map that we can shift and bitwise-AND to obtain the stored numbers
  // the numbers represent 3 states: 00 = no occurence, 01 = one occurence, 10 = two or more occurnences

  uint64_t table[PARTION_SIZE >> 5];

  uint32_t bucket_map = 63;

  // define bounds

  uint64_t upper_bound = PROBLEM_SIZE;
  uint64_t lower_bound = PROBLEM_SIZE - PARTION_SIZE;

  // iterate over all full partitions
  // if the lower_bound is bigger than the partition size, we break the loop to prevent unsigned interger underflow

  while (lower_bound > PARTION_SIZE) {

    // scan along the x axis as long as the current values are abothe the lower bound

    printf("%ld > %ld\n", current.x_cube + current.y_cube, lower_bound);

    while (current.x_cube + current.y_cube > lower_bound) {

      // define the default fallback (topmost value along the x axis)

      uint16_t x_element_fb = current.x_element - 1;
      uint32_t x_cube_diff_fb = current.x_cube_diff - x_element_fb * 6;

      struct position_t fallback = {
        current.x_cube - x_cube_diff_fb, 1,
        x_cube_diff_fb, 1,
        x_element_fb, 1
      };

      // check if the default fallback is valid

      uint8_t fb_valid = fallback.x_cube + fallback.y_cube > lower_bound;

      // scan down until you reach the upper bound

      while (current.x_cube + current.y_cube <= upper_bound) {

        // check if you hit the diagonal border

        if (current.x_element <= current.x_element) {

          // TODO: insert current

          break; 
        }

        // check if a valid fallback is present
        
        if (!fb_valid) {

          // check if a step to the left results in a valid fallback

          uint16_t x_element_fb_new = current.x_element - 1;
          uint32_t x_cube_diff_fb_new = current.x_cube_diff - x_element_fb * 6;
          uint64_t x_cube_fb_new = current.x_cube - x_cube_diff_fb_new;

          if (x_cube_diff_fb_new + current.y_cube > lower_bound) {

            // replace the fallback with a valid one

            fallback.x_cube = x_cube_fb_new;
            fallback.y_cube = current.y_cube;
            fallback.x_cube_diff = x_cube_diff_fb_new;
            fallback.y_cube_diff = current.y_cube_diff;
            fallback.x_element = x_element_fb_new;
            fallback.y_element = current.y_element;

            fb_valid = 1;
          }
        }

        // TODO: insert current

        // update the current element by going one step down on the y axis

        current.y_cube_diff = current.y_cube_diff + 6 * current.y_element;
        current.y_cube = current.y_cube + current.y_cube_diff;
        current.y_element = current.y_element + 1;

      }

      // update the current element to the fallback

      current = fallback;
    }
  
    // TODO: update the partition
    // TODO: clear the table

    break; // TODO: remove

  }



  // get the results from the result composite

  count = result_composite >> 32;
  checksum = result_composite && checksum_map;

  // print the results

  printf("%lu Ramanujan numbers up to %lu, checksum=%lu\n", count, PROBLEM_SIZE, checksum);
  return 0;
}
