#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0

uint8_t clz_iteration(uint32_t x)
{
  	uint8_t  n;
  	if (x == 0) return 32;
	for (n = 0; ((x & 0x80000000) == 0); n++, x <<= 1);
	return n;
}

uint8_t clz_recursive(uint32_t x,int count)
{
	count = count / 2;
	if (count < 1)
		return 0;
	
	uint16_t upper = (x >> count);
	uint16_t lower = (x & 0xFFFF);

	return upper ? clz_recursive(upper,count) : count + clz_recursive(lower,count);
}

uint8_t clz_Harley(uint32_t x)
{
    static uint8_t const Table[] = {
      0xFF, 0, 0xFF, 15, 0xFF, 1, 28, 0xFF,
      16, 0xFF, 0xFF, 0xFF, 2, 21, 29, 0xFF,
      0xFF, 0xFF, 19, 17, 10, 0xFF, 12, 0xFF,
      0xFF, 3, 0xFF, 6, 0xFF, 22, 30, 0xFF,
      14, 0xFF, 27, 0xFF, 0xFF, 0xFF, 20, 0xFF,
      18, 9, 11, 0xFF, 5, 0xFF, 0xFF, 13,
      26, 0xFF, 0xFF, 8, 0xFF, 4, 0xFF, 25,
      0xFF, 7, 24, 0xFF, 23, 0xFF, 31, 0xFF,
    };

    /* Propagate leftmost 1-bit to the right */
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
 
    /* x = x * 0x6EB14F9 */
    x = (x << 3) - x;   /* Multiply by 7. */
    x = (x << 8) - x;   /* Multiply by 255. */
    x = (x << 8) - x;   /* Again. */
    x = (x << 8) - x;   /* Again. */

    return Table[x >> 26];
}

uint8_t clz_binary_search(uint32_t x)
{
	if (x==0) return 32;
	int n = 0;
	if (x <= 0x0000FFFF) { n += 16; x <<= 16;}
	if (x <= 0x00FFFFFF) { n += 8; x <<= 8;}
	if (x <= 0x0FFFFFFF) { n += 4; x <<= 4;}
	if (x <= 0x3FFFFFFF) { n += 2; x <<= 2;}
	if (x <= 0x7FFFFFFF) { n += 1; x <<= 1;}
	
	return n;
}

uint8_t clz_byte_shift(uint32_t x)
{
	if (x==0) return 32;
	int n = 1;
	if ((x >> 16) == 0) { n += 16; x <<= 16;}
	if ((x >> 24) == 0) { n += 8; x <<= 8;}
	if ((x >> 28) == 0) { n += 4; x <<= 4;}
	if ((x >> 30) == 0) { n += 2; x <<= 2;}
	n = n - (x >> 31);	

	return n;
}

int main(){
	struct timespec start = {0, 0};
    struct timespec end = {0, 0};
	
	uint32_t number = 300000;
	int loop = 10000;	
	

	clock_gettime(CLOCK_ID, &start);
	//printf("value = %u , ", clz_iteration(number));
	for (int i=0;i<loop;i++){clz_iteration(number);}
    clock_gettime(CLOCK_ID, &end);
    printf("%lf ", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);

	clock_gettime(CLOCK_ID, &start);
	//printf("value = %u , ", clz_recursive(number,32));
	for (int i=0;i<loop;i++){clz_iteration(number);}
    clock_gettime(CLOCK_ID, &end);
    printf("%lf", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);

	clock_gettime(CLOCK_ID, &start);
	//printf("value = %u , ", 31 - clz_Harley(number));
	for (int i=0;i<loop;i++){clz_iteration(number);}
    clock_gettime(CLOCK_ID, &end);
    printf("%lf ", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);

	clock_gettime(CLOCK_ID, &start);
	//printf("value = %u , ", clz_binary_search(number));
	for (int i=0;i<loop;i++){clz_iteration(number);}
	clock_gettime(CLOCK_ID, &end);
    printf("%lf ", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);

	clock_gettime(CLOCK_ID, &start);
	//printf("value = %u , ", clz_byte_shift(number));
	for (int i=0;i<loop;i++){clz_iteration(number);}
    clock_gettime(CLOCK_ID, &end);
    printf("%lf", (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);
	
	return 0;
}
