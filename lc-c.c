#include <zlib.h>
#include <stdint.h>
#include <stdio.h>
#include "kseq.h"
KSTREAM_INIT(gzFile, gzread, 0x10000)

int main(int argc, char *argv[])
{
	int64_t n_line = 0, sum = 0;
	kstring_t str = {0,0,0};
	gzFile fp;
	kstream_t *ks;
	fp = gzdopen(0, "r");
	ks = ks_init(fp);
	while (ks_getuntil(ks, KS_SEP_LINE, &str, 0) >= 0)
		++n_line, sum += str.l;
	ks_destroy(ks);
	gzclose(fp);
	printf("%ld %ld\n", (long)n_line, (long)sum);
	return 0;
}
