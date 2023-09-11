all:line-gen lc-c

line-gen:line-gen.c
	$(CC) -O3 -o $@ $<

lc-c:lc-c.c
	$(CC) -O3 -o $@ $< -lz

clean:
	rm -fr line-gen lc-c
