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
        write!(
            f,
            "Error at line {}: {}\n\t{}",
            self.line_number, self.wrapped_error, self.line
        )
    }
}

fn execute_file<P>(filename: P, args: Vec<String>) -> Result<(), Error>
where
    P: AsRef<Path>,
{
    let mut memory = topflight_core::Memory::default();
    if !args.is_empty() {
        memory.store("args", topflight_core::Value::ArrayOfString(args));
    }

    let mut routines = topflight_core::Routines::new();
    let mut routine_in_construction: Option<topflight_core::Routine> = Option::None;
    let lines = read_lines(filename).expect("Error while opening the file");
    for (i, line) in lines.enumerate() {
        let mut output = String::new();
        let line = line.expect("Error while reading a line");
        if let Err(error) = topflight_core::handle_line(
            line.as_str(),
            &mut memory,
            &mut routines,
            &mut routine_in_construction,
            &mut output,
        ) {
            return Err(Error {
                wrapped_error: error,
                line_number: i + 1,
                line
            });
        } else if !output.is_empty() {
            print!("{}", output);
        }
    }
    Ok(())
}

fn main() {
    let mut args: Vec<String> = env::args().skip(1).collect();
    if !args.is_empty() {
        let filename = args.remove(0);
        println!("{}", filename);
        if let Err(error) = execute_file(filename, args) {
            println!("{}", error);
        };
    }
}
