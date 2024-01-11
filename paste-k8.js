function main(args) {
	if (args.length == 0) {
		print("Usage: k8 paste-k8.js <in1.txt> [in2.txt [...]]");
		return;
	}
	let fn = [], buf = new Bytes();
	for (let i = 0; i < args.length; ++i)
		fn[i] = new File(args[i]);
	let a = [];
	for (;;) {
		let last = -1;
		for (let i = 0; i < fn.length; ++i) {
			if (fn[i].readline(buf) >= 0)
				last = i, a[i] = buf.toString();
			else a[i] = "";
		}
		if (last < 0) break;
		print(a.slice(0, last + 1).join("\t"));
	}
	// cleanup
	for (let i = 0; i < fn.length; ++i)
		fn[i].close();
	buf.destroy();
}

main(arguments);
