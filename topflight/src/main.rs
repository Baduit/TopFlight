use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;
use std::env;


use topflight_core::{handle_line, Error};

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
	P: AsRef<Path>,
{
	let file = File::open(filename)?;
	Ok(io::BufReader::new(file).lines())
}

fn main() -> Result<(), Error>{
	let args: Vec<String> = env::args().collect();
	if args.len() > 1 {
		let lines = read_lines(&args[1]).expect("Error while opening the file");
		for line in lines {
			let line = line.expect("Error while reading a line");
			handle_line(line.as_str())?;
		}
	}
	Ok(())
}