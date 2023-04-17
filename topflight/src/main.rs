use std::env;
use std::fmt;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

#[derive(Debug)]
struct Error {
    wrapped_error: topflight_core::Error,
    line_number: usize,
	line: String,
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Error at line {}: {}\n\t{}", self.line_number, self.wrapped_error, self.line)
    }
}

fn execute_file<P>(filename: P) -> Result<(), Error>
where
    P: AsRef<Path>,
{
    let lines = read_lines(filename).expect("Error while opening the file");
    for (i, line) in lines.enumerate() {
        let line = line.expect("Error while reading a line");
        if let Err(error) = topflight_core::handle_line(line.as_str()) {
            return Err(Error {
                wrapped_error: error,
                line_number: i + 1,
				line: line,
            });
        }
    }
    Ok(())
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() > 1 {
        if let Err(error) = execute_file(&args[1]) {
			println!("{}", error);
		};
    }
}
