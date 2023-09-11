function* k8_readline(fn) {
	let buf = new Bytes();
	let file = new File(fn);
	while (file.readline(buf) >= 0) {
		yield buf.toString();
	}
	file.close();
	buf.destroy();
}

let n_line = 0, sum = 0;
for (const line of k8_readline("-"))
	++n_line, sum += line.length;
print(`${n_line} ${sum}`);
