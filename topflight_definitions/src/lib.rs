use thiserror::Error;

mod value;
pub use value::Value;

use topflight_macro_utils::GenericInstruction;

pub struct Routine {
    pub name: String,
    pub instructions: Vec<Instruction>,
}

impl Routine {
    pub fn new(name: String) -> Routine {
        Routine { name: name, instructions: Vec::new() }
    }
}

pub enum Instruction {
    Store(Store),
    Copy(Copy),
    Free(Free),
    Print(Print),
    Call(Call),
    CallIf(CallIf),
    Add(Add),
    Substract(Substract),
    Multiply(Multiply),
    Divide(Divide),
    Modulo(Modulo),
    LogicalAnd(LogicalAnd),
    LogicalOr(LogicalOr),
    LogicalNot(LogicalNot),
    CompareEqual(CompareEqual),
    CompareDifferent(CompareDifferent),
    CompareLess(CompareLess),
    CompareLessOrEqual(CompareLessOrEqual),
    CompareGreater(CompareGreater),
    CompareGreaterOrEqual(CompareGreaterOrEqual),
    GetAt(GetAt),
    StoreAt(StoreAt),
    CopyAt(CopyAt),
    Size(Size),
    Resize(Resize),
    Insert(Insert),
    PushBack(PushBack),
    Concat(Concat),
    Erase(Erase),
}

impl Instruction {
    pub fn parse(str: &str) -> Result<Instruction, ParseError> {
        let mut chars = str.chars();
        let name = chars.by_ref().take_while(|c| *c != ' ').collect::<String>();
        match name.as_str() {
            Store::NAME => Ok(Store::parse(chars.as_str())?),
            Copy::NAME => Ok(Copy::parse(chars.as_str())?),
            Free::NAME => Ok(Free::parse(chars.as_str())?),
            Print::NAME => Ok(Print::parse(chars.as_str())?),
            Call::NAME => Ok(Call::parse(chars.as_str())?),
            CallIf::NAME => Ok(CallIf::parse(chars.as_str())?),
            Add::NAME => Ok(Add::parse(chars.as_str())?),
            Substract::NAME => Ok(Substract::parse(chars.as_str())?),
            Multiply::NAME => Ok(Multiply::parse(chars.as_str())?),
            Divide::NAME => Ok(Divide::parse(chars.as_str())?),
            Modulo::NAME => Ok(Modulo::parse(chars.as_str())?),
            LogicalAnd::NAME => Ok(LogicalAnd::parse(chars.as_str())?),
            LogicalOr::NAME => Ok(LogicalOr::parse(chars.as_str())?),
            LogicalNot::NAME => Ok(LogicalNot::parse(chars.as_str())?),
            CompareEqual::NAME => Ok(CompareEqual::parse(chars.as_str())?),
            CompareDifferent::NAME => Ok(CompareDifferent::parse(chars.as_str())?),
            CompareLess::NAME => Ok(CompareLess::parse(chars.as_str())?),
            CompareLessOrEqual::NAME => Ok(CompareLessOrEqual::parse(chars.as_str())?),
            CompareGreater::NAME => Ok(CompareGreater::parse(chars.as_str())?),
            CompareGreaterOrEqual::NAME => Ok(CompareGreaterOrEqual::parse(chars.as_str())?),
            GetAt::NAME => Ok(GetAt::parse(chars.as_str())?),
            StoreAt::NAME => Ok(StoreAt::parse(chars.as_str())?),
            CopyAt::NAME => Ok(CopyAt::parse(chars.as_str())?),
            Size::NAME => Ok(Size::parse(chars.as_str())?),
            Resize::NAME => Ok(Resize::parse(chars.as_str())?),
            Insert::NAME => Ok(Insert::parse(chars.as_str())?),
            PushBack::NAME => Ok(PushBack::parse(chars.as_str())?),
            Concat::NAME => Ok(Concat::parse(chars.as_str())?),
            Erase::NAME => Ok(Erase::parse(chars.as_str())?),
            _ => Err(ParseError::InstructionDoesNotExist(name)),
        }
    }
}

#[derive(Error, Debug)]
pub enum ParseError {
    #[error("Instruction `{0}` does not exist")]
    InstructionDoesNotExist(String),
    #[error("{0}")]
    ValueParsingError(#[from] value::ParseError),
}

pub trait GenericInstruction {
    // Instruction name is already parsed, it means that it is not in the str passed to this function
    fn parse(str: &str) -> Result<Instruction, ParseError>;
}

/*
** Misc
*/
#[derive(GenericInstruction)]
pub struct Store {
    pub dest: String,
    pub value: Value,
}

#[derive(GenericInstruction)]
pub struct Copy {
    pub input: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct Free {
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct Print {
    pub input: String,
}

#[derive(GenericInstruction)]
pub struct Call {
    pub routine_name: String,
}

#[derive(GenericInstruction)]
pub struct CallIf {
    pub routine_name: String,
    pub boolean_input_name: String,
}

/*
** Arithmetic
*/
#[derive(GenericInstruction)]
pub struct Add {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct Substract {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct Multiply {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct Divide {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct Modulo {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

/*
** Logical
*/
#[derive(GenericInstruction)]
pub struct LogicalAnd {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct LogicalOr {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct LogicalNot {
    pub input: String,
    pub dest: String,
}

/*
** Comparaison
*/
#[derive(GenericInstruction)]
pub struct CompareEqual {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct CompareDifferent {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct CompareLess {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct CompareLessOrEqual {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct CompareGreater {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct CompareGreaterOrEqual {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}
/*
** Array operations
** Note: "index" are String because it will load a variable at this location
*/
#[derive(GenericInstruction)]
pub struct GetAt {
    pub array_input: String,
    pub index: String,
    pub output: String,
}

#[derive(GenericInstruction)]
pub struct StoreAt {
    pub array_output: String,
    pub index: String,
    pub value: Value,
}

#[derive(GenericInstruction)]
pub struct CopyAt {
    pub array_output: String,
    pub index: String,
    pub input: String,
}

#[derive(GenericInstruction)]
pub struct Size {
    pub array_input: String,
    pub output: String,
}

#[derive(GenericInstruction)]
pub struct Resize {
    pub array_input: String,
    pub new_size: String,
}

#[derive(GenericInstruction)]
pub struct Insert {
    pub array_output: String,
    pub index: String,
    pub input: String,
}

#[derive(GenericInstruction)]
pub struct PushBack {
    pub array_output: String,
    pub input: String,
}

#[derive(GenericInstruction)]
pub struct Concat {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction)]
pub struct Erase {
    pub array_input: String,
    pub index: String,
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_get_name() {
        assert_eq!("STORE", Store::NAME);
    }

    #[test]
    fn test_store_parse() {
        let expected = Store {
            dest: String::from("lol"),
            value: value::Value::Integer(1),
        };
        let input = "lol INTEGER(1)";
        let received = Store::parse(input).expect("Well, parsing failed :'(");
        if let Instruction::Store(store) = received {
            assert_eq!(expected.dest, store.dest);
        } else {
            panic!("How the fuck did we have something else than a store when calling Store::parse ?!?");
        }
    }

    #[test]
    fn test_instruction_store_parse() {
        let expected = Store {
            dest: String::from("lol"),
            value: value::Value::Integer(1),
        };
        let input = "STORE lol INTEGER(1)";
        let received = Instruction::parse(input).expect("Well, parsing failed :'(");
        if let Instruction::Store(store) = received {
            assert_eq!(expected.dest, store.dest);
        } else {
            panic!("How the fuck did we have something else than a store when calling Store::parse ?!?");
        }
    }
}
