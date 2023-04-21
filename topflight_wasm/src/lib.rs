use wasm_bindgen::prelude::*;
use std::fmt;

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

fn execute_code_impl(code: String) -> Result<String, Error> {
    let mut output = String::new();
    let mut memory = topflight_core::Memory::default();
    let mut routines = topflight_core::Routines::new();
    let mut routine_in_construction: Option<topflight_core::Routine> = Option::None;
    let lines = code.split('\n');
    for (i, line) in lines.enumerate() {
        if let Err(error) = topflight_core::handle_line(
            line,
            &mut memory,
            &mut routines,
            &mut routine_in_construction,
            &mut output,
        ) {
            return Err(Error {
                wrapped_error: error,
                line_number: i + 1,
                line: line.to_string(),
            });
        }
    }
    Ok(output)
}

#[wasm_bindgen]
pub fn execute_code(code: String) -> String {
    match execute_code_impl(code) {
        Ok(str) => str,
        Err(error) => format!("{}", error),
    }
}
