This repo evaluates the line-reading performance. The test was run on a MacBook
Pro with an Apple M1 CPU. The following command lines were used for timing:
```sh
make   # compile line-gen and lc-c
./line-gen | \time node lc-node.js
```

| Language | Runtime | Version | User time (s) | Sys time (s) | Code |
|:---------|:--------|:--------|-----:|-----:|:-----------|
|JavaScript| node    | 20.6.1  | 3.87 | 0.76 | [lc-node.js](lc-node.js) |
|          | bun     | 1.0.0   | 4.66 | 1.00 | [lc-node.js](lc-node.js) |
|          | k8      | 1.0     | 1.15 | 0.27 | [lc-k8.js](lc-k8.js) |
|C         | clang   | 14.0.0  | 0.40 | 0.31 | [lc-c.c](lc-c.c) |
|python    | python  | 3.10.10 | 3.04 | 0.28 | [lc-python.py](lc-python.py) |
|perl      | perl    | 5.34.1  | 1.23 | 0.32 | [lc-perl.pl](lc-perl.pl) |
|awk       | mawk    | 1.3.4   | 1.35 | 0.56 | [lc-awk.awk](lc-awk.awk) |
|          | apple awk | ?     |90.84 | 0.70 | [lc-awk.awk](lc-awk.awk) |
