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
        Routine {
            name,
            instructions: Vec::new(),
        }
    }
}

#[derive(Debug, Clone)]
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
#[derive(GenericInstruction, Debug, Clone)]
pub struct Store {
    pub dest: String,
    pub value: Value,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Copy {
    pub input: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Free {
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Print {
    pub input: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Call {
    pub routine_name: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct CallIf {
    pub routine_name: String,
    pub boolean_input_name: String,
}

/*
** Arithmetic
*/
#[derive(GenericInstruction, Debug, Clone)]
pub struct Add {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Substract {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Multiply {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Divide {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Modulo {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

/*
** Logical
*/
#[derive(GenericInstruction, Debug, Clone)]
pub struct LogicalAnd {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct LogicalOr {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct LogicalNot {
    pub input: String,
    pub dest: String,
}

/*
** Comparaison
*/
#[derive(GenericInstruction, Debug, Clone)]
pub struct CompareEqual {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct CompareDifferent {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct CompareLess {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct CompareLessOrEqual {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct CompareGreater {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct CompareGreaterOrEqual {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}
/*
** Array operations
** Note: "index" are String because it will load a variable at this location
*/
#[derive(GenericInstruction, Debug, Clone)]
pub struct GetAt {
    pub array_input: String,
    pub index: String,
    pub output: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct StoreAt {
    pub array_output: String,
    pub index: String,
    pub value: Value,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct CopyAt {
    pub array_output: String,
    pub index: String,
    pub input: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Size {
    pub array_input: String,
    pub output: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Resize {
    pub array_input: String,
    pub new_size: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Insert {
    pub array_output: String,
    pub index: String,
    pub input: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct PushBack {
    pub array_output: String,
    pub input: String,
}

#[derive(GenericInstruction, Debug, Clone)]
pub struct Concat {
    pub input_a: String,
    pub input_b: String,
    pub dest: String,
}

#[derive(GenericInstruction, Debug, Clone)]
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
        assert_eq!("COPY", Copy::NAME);
        assert_eq!("FREE", Free::NAME);
        assert_eq!("PRINT", Print::NAME);
        assert_eq!("CALL", Call::NAME);
        assert_eq!("CALL_IF", CallIf::NAME);
        assert_eq!("ADD", Add::NAME);
        assert_eq!("SUBSTRACT", Substract::NAME);
        assert_eq!("MULTIPLY", Multiply::NAME);
        assert_eq!("DIVIDE", Divide::NAME);
        assert_eq!("MODULO", Modulo::NAME);
        assert_eq!("LOGICAL_AND", LogicalAnd::NAME);
        assert_eq!("LOGICAL_OR", LogicalOr::NAME);
        assert_eq!("LOGICAL_NOT", LogicalNot::NAME);
        assert_eq!("COMPARE_EQUAL", CompareEqual::NAME);
        assert_eq!("COMPARE_DIFFERENT", CompareDifferent::NAME);
        assert_eq!("COMPARE_LESS", CompareLess::NAME);
        assert_eq!("COMPARE_LESS_OR_EQUAL", CompareLessOrEqual::NAME);
        assert_eq!("COMPARE_GREATER", CompareGreater::NAME);
        assert_eq!("COMPARE_GREATER_OR_EQUAL", CompareGreaterOrEqual::NAME);
        assert_eq!("GET_AT", GetAt::NAME);
        assert_eq!("STORE_AT", StoreAt::NAME);
        assert_eq!("COPY_AT", CopyAt::NAME);
        assert_eq!("SIZE", Size::NAME);
        assert_eq!("RESIZE", Resize::NAME);
        assert_eq!("INSERT", Insert::NAME);
        assert_eq!("PUSH_BACK", PushBack::NAME);
        assert_eq!("CONCAT", Concat::NAME);
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
