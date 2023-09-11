// adapted from https://stackabuse.com/reading-a-file-line-by-line-in-node-js/

const fs = require('fs');
const readline = require('readline');

const fileStream = fs.createReadStream('/dev/stdin');

const rl = readline.createInterface({
    input: fileStream,
    crlfDelay: Infinity
});

let n_line = 0, sum = 0;
rl.on('line', (line) => {
	++n_line, sum += line.length;
});

rl.on('close', () => {
    console.log(`${n_line} ${sum}`);
});
