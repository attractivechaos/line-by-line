import { open as fsOpenAsync } from 'node:fs/promises'
import { createWriteStream } from 'node:fs'

// Implemented by user Dimava in https://stackoverflow.com/a/77799701/172818

/**
 * Read multiple files line by line and write lines concatenated by `\t`
 */
async function paste(fn) {
	const files = await Promise.all(fn.map(fn => fsOpenAsync(fn)));
	const zip = zipAsyncs(files.map(f => f.readLines()[Symbol.asyncIterator]()));
	for await (const lines of zip)
		console.log(`${lines.map(e => e ?? '').join('\t')}`);
	await Promise.all(files.map(f => f.close()));
}

/**
 * Zip multiple async iterables, returning `undefined` for missing values
 * @template {T}
 * @param {AsyncIterator<T>[]} its
 * @returns {AsyncGenerator<IteratorResult<T | undefined, any>[]>}
 */
async function* zipAsyncs(its) {
	while (true) {
		const results = await Promise.all(its.map(e => e.next()));
		yield results.map(r => r.value);
		if (results.every(r => r.done))
			return;
	}
}

async function main(args) {
	if (args.length == 0) {
		console.log("Usage: node paste-Dimava.mjs <in1.txt> [in2.txt [...]]");
		return;
	}
	await paste(args);
}

await main(process.argv.splice(2));
