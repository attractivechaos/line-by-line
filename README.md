This repo evaluates the line-reading performance. The test was run on a MacBook
Pro with an Apple M1 CPU. The following command lines were used for timing:
```sh
make   # compile line-gen and lc-c
./line-gen > /tmp/test.txt
hyperfine './lc-c < /tmp/test.txt'
```

| Language | Runtime | Version | Elapsed time (s) | User time (s) | Sys time (s) | Code |
|:---------|:--------|:--------|-----:|-----:|-----:|:-----------|
|JavaScript| node    | 21.5.0  | 6.30 | 5.33 | 0.90 | [lc-node.js](lc-node.js) |
|          | node    | 21.5.0  | 22.34| 20.41| 2.24 | [lc-n-readlines.js](lc-n-readlines.js) |
|          | bun     | 1.0.20  | 4.91 | 5.30 | 1.47 | [lc-node.js](lc-node.js) |
|          | bun     | 1.0.20  | 21.16| 19.22| 3.37 | [lc-n-readlines.js](lc-n-readlines.js) |
|          | k8      | 1.0     | 1.49 | 1.06 | 0.37 | [lc-k8.js](lc-k8.js) |
|C         | clang   | 15.0.0  | 0.71 | 0.35 | 0.35 | [lc-c.c](lc-c.c) |
|python    | python  | 3.11.17 | 3.48 | 2.85 | 0.62 | [lc-python.py](lc-python.py) |
|perl      | perl    | 5.34.3  | 1.70 | 1.13 | 0.57 | [lc-perl.pl](lc-perl.pl) |
|awk       | mawk    | 1.3.4   | 2.08 | 1.27 | 0.80 | [lc-awk.awk](lc-awk.awk) |
|          | apple awk | ?     |90.06 | 87.90| 1.12 | [lc-awk.awk](lc-awk.awk) |
