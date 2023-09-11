my ($n_line, $sum) = (0, 0);
while (<>) {
	++$n_line;
	$sum += length($_) - 1;
}
print "$n_line $sum\n";
