#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint64_t splitmix64(uint64_t *x)
{
	uint64_t z = (*x += 0x9e3779b97f4a7c15ULL);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
	z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
	return z ^ (z >> 31);
}

int main(int argc, char *argv[])
{
	uint64_t x = 11;
	uint8_t *buf;
	int32_t max_len = 1000, n_line = 10000000, i, j;
	buf = (uint8_t*)malloc(max_len);
	for (i = 0; i < n_line; ++i) {
		uint64_t z;
		int32_t len, c;
		z = splitmix64(&x);
		len = z % max_len;
		c = z % 94 + 33;
		memset(buf, c, len);
		fwrite(buf, 1, len, stdout);
		fputc('\n', stdout);
	}
	free(buf);
	return 0;
}
