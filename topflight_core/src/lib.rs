use thiserror::Error;

use topflight_definitions::{Instruction, ParseError, Routine};
use topflight_vm::{execute, Memory, Routines, VMError};

#[derive(Error, Debug)]
pub enum Error {
    #[error("{0}")]
    ParsingError(#[from] ParseError),
    #[error("{0}")]
    VMError(#[from] VMError),
    #[error("Invalid routine format")]
    InvalidRoutineFormat,
    #[error("It is not possible to define a routine inside a routine")]
    SubRoutineFound,
    #[error("It is not possible to define an unamed routine")]
    EmptyRoutineName,
    #[error("End of a subroutine found without start.")]
    UnexpectedEndSubroutine,
    #[error("End of a subroutine found without start.")]
    MismatchingEndSubroutine,
}

pub fn handle_line(str: &str) -> Result<(), Error> {
    let mut memory = Memory::default();
    let mut routines = Routines::new();
    let mut routine_in_construction: Option<Routine> = Option::None;

    match parse_line(str)? {
        Line::RoutineStart(routine_name) => {
            start_routine(routine_name, &mut routine_in_construction)?
        }
        Line::RoutineEnd(routine_name) => {
            end_routine(routine_name, &mut routine_in_construction, &mut routines)?
        }
        Line::Instruction(instruction) => {
            match routine_in_construction.as_mut() {
                None => execute(&mut memory, &mut routines, &instruction)?,
                Some(routine) => routine.instructions.push(instruction),
            }
        }
    };

    Ok(())
}

fn start_routine(
    routine_name: String,
    routine_in_construction: &mut Option<Routine>,
) -> Result<(), Error> {
    match routine_in_construction {
        None => routine_in_construction.replace(Routine::new(routine_name)),
        _ => return Err(Error::SubRoutineFound),
    };
    Ok(())
}
fn end_routine(
    routine_name: String,
    routine_in_construction: &mut Option<Routine>,
    routines: &mut Routines,
) -> Result<(), Error> {
    if routine_in_construction.is_none() {
        Err(Error::UnexpectedEndSubroutine)
    } else if routine_name != routine_in_construction.as_ref().unwrap().name {
        Err(Error::MismatchingEndSubroutine)
    } else {
        routines.insert(routine_name, routine_in_construction.take().unwrap());
        Ok(())
    }
}

enum Line {
    RoutineStart(String),
    RoutineEnd(String),
    Instruction(Instruction),
}

fn parse_line(str: &str) -> Result<Line, Error> {
    if str.starts_with("</") {
        if !str.ends_with(">") {
            Err(Error::InvalidRoutineFormat)
        } else if str.len() <= 2 {
            Err(Error::EmptyRoutineName)
        } else {
            Ok(Line::RoutineStart(String::from(get_routine_name_at_start(
                str,
            ))))
        }
    } else if str.starts_with("<") {
        if !str.ends_with(">") {
            Err(Error::InvalidRoutineFormat)
        } else if str.len() <= 3 {
            Err(Error::EmptyRoutineName)
        } else {
            Ok(Line::RoutineEnd(String::from(get_routine_name_at_end(str))))
        }
    } else {
        Ok(Line::Instruction(Instruction::parse(str)?))
    }
}

// <my_routine_name>
fn get_routine_name_at_start(str: &str) -> &str {
    let mut chars = str.chars();
    chars.next();
    chars.next_back();
    chars.as_str()
}

// </my_routine_name>
fn get_routine_name_at_end(str: &str) -> &str {
    let mut chars = str.chars();
    chars.next();
    chars.next();
    chars.next_back();
    chars.as_str()
}

#[cfg(test)]
mod tests {
    //use super::*;

    #[test]
    fn test_todo() {
        assert_eq!(true, true);
    }
}
