n_line, s = 0, 0
with open("/dev/stdin") as file:
	for line in file:
		n_line += 1
		s += len(line)
print(n_line, s);
