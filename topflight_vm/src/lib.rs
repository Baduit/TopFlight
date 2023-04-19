use std::collections::HashMap;
use std::fmt::Write;
use thiserror::Error;
use topflight_definitions::*;

#[derive(Default)]
pub struct Memory {
    values: HashMap<String, Value>,
}

#[derive(Error, Debug)]
pub enum VMError {
    #[error("Variable `{0}` does not exist")]
    VariableDoesNotExist(String),
    #[error("Routine `{0}` does not exist")]
    RoutineDoesNotExist(String),
    #[error("A boolean was expected but instead got `{0}`")]
    ExpectedBoolean(Value),
    #[error("Type are mismatching, got `{0}` and `{1}`")]
    MismatchingTypes(Value, Value),
    #[error("Expected an array but got {0}")]
    ExpectedArray(Value),
    #[error("Expected an arithmetic type but bot `{0}` and `{1}`")]
    ExpectedArithmeticTypes(Value, Value),
    #[error("Index, with value `{index:?}`, is out of bound. Array size is `{index:?}`")]
    IndexOutOfBound { array_size: usize, index: usize },
    #[error("Index is negative")]
    NegativeIndex,
    #[error("Index must be an integer")]
    NonIntegerIndex,
    #[error("Error while adding something to the output buffer")]
    OutputBufferError(#[from] std::fmt::Error)
}

impl Memory {
    pub fn store(&mut self, name: &str, value: Value) {
        self.values.insert(String::from(name), value);
    }

    pub fn free(&mut self, name: &str) -> Result<(), VMError> {
        match self.values.remove(name) {
            None => Err(VMError::VariableDoesNotExist(String::from(name))),
            _ => Ok(()),
        }
    }

    pub fn load(&self, name: &str) -> Result<&Value, VMError> {
        match self.values.get(name) {
            Some(value) => Ok(value),
            None => Err(VMError::VariableDoesNotExist(String::from(name))),
        }
    }

    pub fn load_mut(&mut self, name: &str) -> Result<&mut Value, VMError> {
        match self.values.get_mut(name) {
            Some(value) => Ok(value),
            None => Err(VMError::VariableDoesNotExist(String::from(name))),
        }
    }

    pub fn load_index(&self, name: &str) -> Result<usize, VMError> {
        let index = self.load(name)?;
        let index = match index {
            Value::Integer(index) => index,
            _ => return Err(VMError::NonIntegerIndex),
        };

        if *index > 0 {
            let index = *index as usize;
            Ok(index)
        } else {
            Err(VMError::NegativeIndex)
        }
    }
}

pub type Routines = HashMap<String, Routine>;

pub fn execute(
    memory: &mut Memory,
    routines: &Routines,
    instruction: &Instruction,
    output: &mut String,
) -> Result<(), VMError> {
    // Todo: refactor this shit enormous match
    match instruction {
        Instruction::Store(Store { dest, value }) => memory.store(dest, value.clone()),
        Instruction::Copy(Copy { input, dest }) => {
            let copied = memory.load(input.as_str())?.clone();
            memory.store(dest, copied);
        }
        Instruction::Free(Free { dest }) => memory.free(dest.as_str())?,
        Instruction::Print(Print { input }) => {
            let value = memory.load(input.as_str())?;
            write!(output, "{}", value)?;
        }
        Instruction::Call(Call { routine_name }) => {
            call_routine(memory, routines, routine_name.as_str(), output)?;
        }
        Instruction::CallIf(CallIf {
            routine_name,
            boolean_input_name,
        }) => {
            let value = memory.load(boolean_input_name.as_str())?;
            let call = match value {
                Value::Boolean(b) => *b,
                _ => return Err(VMError::ExpectedBoolean(value.clone())),
            };

            if call {
                call_routine(memory, routines, routine_name.as_str(), output)?;
            }
        }
        Instruction::Add(Add {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            let result = match (a, b) {
                (Value::Integer(a), Value::Integer(b)) => Value::Integer(a + b),
                (Value::Number(a), Value::Number(b)) => Value::Number(a + b),
                _ => return Err(VMError::ExpectedArithmeticTypes(a.clone(), b.clone())),
            };
            memory.store(dest.as_str(), result);
        }
        Instruction::Substract(Substract {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            let result = match (a, b) {
                (Value::Integer(a), Value::Integer(b)) => Value::Integer(a - b),
                (Value::Number(a), Value::Number(b)) => Value::Number(a - b),
                _ => return Err(VMError::ExpectedArithmeticTypes(a.clone(), b.clone())),
            };
            memory.store(dest.as_str(), result);
        }
        Instruction::Multiply(Multiply {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            let result = match (a, b) {
                (Value::Integer(a), Value::Integer(b)) => Value::Integer(a * b),
                (Value::Number(a), Value::Number(b)) => Value::Number(a * b),
                _ => return Err(VMError::ExpectedArithmeticTypes(a.clone(), b.clone())),
            };
            memory.store(dest.as_str(), result);
        }
        Instruction::Divide(Divide {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            let result = match (a, b) {
                (Value::Integer(a), Value::Integer(b)) => Value::Integer(a / b),
                (Value::Number(a), Value::Number(b)) => Value::Number(a / b),
                _ => return Err(VMError::ExpectedArithmeticTypes(a.clone(), b.clone())),
            };
            memory.store(dest.as_str(), result);
        }
        Instruction::Modulo(Modulo {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            let result = match (a, b) {
                (Value::Integer(a), Value::Integer(b)) => Value::Integer(a % b),
                _ => return Err(VMError::ExpectedArithmeticTypes(a.clone(), b.clone())),
            };
            memory.store(dest.as_str(), result);
        }
        Instruction::LogicalAnd(LogicalAnd {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            let result = match (a, b) {
                (Value::Boolean(a), Value::Boolean(b)) => Value::Boolean(*a && *b),
                _ => return Err(VMError::MismatchingTypes(a.clone(), b.clone())),
            };
            memory.store(dest.as_str(), result);
        }
        Instruction::LogicalOr(LogicalOr {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            let result = match (a, b) {
                (Value::Boolean(a), Value::Boolean(b)) => Value::Boolean(*a || *b),
                _ => return Err(VMError::MismatchingTypes(a.clone(), b.clone())),
            };
            memory.store(dest.as_str(), result);
        }
        Instruction::LogicalNot(LogicalNot { input, dest }) => {
            let a = memory.load(input.as_str())?;
            let result = match a {
                Value::Boolean(a) => Value::Boolean(!*a),
                _ => return Err(VMError::ExpectedBoolean(a.clone())),
            };
            memory.store(dest.as_str(), result);
        }
        Instruction::CompareEqual(CompareEqual {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            if std::mem::discriminant(a) != std::mem::discriminant(b) {
                return Err(VMError::MismatchingTypes(a.clone(), b.clone()));
            }
            let result = Value::Boolean(a == b);
            memory.store(dest.as_str(), result);
        }
        Instruction::CompareDifferent(CompareDifferent {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            if std::mem::discriminant(a) != std::mem::discriminant(b) {
                return Err(VMError::MismatchingTypes(a.clone(), b.clone()));
            }
            let result = Value::Boolean(a != b);
            memory.store(dest.as_str(), result);
        }
        Instruction::CompareLess(CompareLess {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            if std::mem::discriminant(a) != std::mem::discriminant(b) {
                return Err(VMError::MismatchingTypes(a.clone(), b.clone()));
            }
            let result = Value::Boolean(a < b);
            memory.store(dest.as_str(), result);
        }
        Instruction::CompareLessOrEqual(CompareLessOrEqual {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            if std::mem::discriminant(a) != std::mem::discriminant(b) {
                return Err(VMError::MismatchingTypes(a.clone(), b.clone()));
            }
            let result = Value::Boolean(a <= b);
            memory.store(dest.as_str(), result);
        }
        Instruction::CompareGreater(CompareGreater {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            if std::mem::discriminant(a) != std::mem::discriminant(b) {
                return Err(VMError::MismatchingTypes(a.clone(), b.clone()));
            }
            let result = Value::Boolean(a > b);
            memory.store(dest.as_str(), result);
        }
        Instruction::CompareGreaterOrEqual(CompareGreaterOrEqual {
            input_a,
            input_b,
            dest,
        }) => {
            let a = memory.load(input_a.as_str())?;
            let b = memory.load(input_b.as_str())?;
            if std::mem::discriminant(a) != std::mem::discriminant(b) {
                return Err(VMError::MismatchingTypes(a.clone(), b.clone()));
            }
            let result = Value::Boolean(a >= b);
            memory.store(dest.as_str(), result);
        }
        Instruction::GetAt(GetAt {
            array_input,
            index,
            output,
        }) => {
            let array_input = memory.load(array_input.as_str())?;
            let index = memory.load_index(index.as_str())?;

            let result = match array_input {
                Value::ArrayOfInteger(array_input) => {
                    Value::Integer(vector_get_copy_at(array_input, index)?)
                }
                Value::ArrayOfNumber(array_input) => {
                    Value::Number(vector_get_copy_at(array_input, index)?)
                }
                Value::ArrayOfString(array_input) => {
                    Value::String(vector_get_copy_at(array_input, index)?)
                }
                Value::ArrayOfBoolean(array_input) => {
                    Value::Boolean(vector_get_copy_at(array_input, index)?)
                }
                _ => return Err(VMError::ExpectedArray(array_input.clone())),
            };
            memory.store(output.as_str(), result);
        }
        Instruction::StoreAt(StoreAt {
            array_output,
            index,
            value,
        }) => {
            store_at(memory, array_output, index, value.clone())?;
        }
        // Todo: put the thing above in a function, then below load the input then use the function store at
        Instruction::CopyAt(CopyAt {
            array_output,
            index,
            input,
        }) => {
            let input = memory.load(input.as_str())?.clone();
            store_at(memory, array_output, index, input)?;
        }
        Instruction::Size(Size {
            array_input,
            output,
        }) => {
            let array = memory.load(array_input.as_str())?;
            let size = match array {
                Value::ArrayOfInteger(array) => array.len(),
                Value::ArrayOfNumber(array) => array.len(),
                Value::ArrayOfString(array) => array.len(),
                Value::ArrayOfBoolean(array) => array.len(),
                _ => return Err(VMError::ExpectedArray(array.clone())),
            };
            memory.store(output.as_str(), Value::Integer(size as i64));
        }
        Instruction::Resize(Resize {
            array_input,
            new_size,
        }) => {
            let size = memory.load_index(new_size.as_str())?;
            let array = memory.load_mut(array_input.as_str())?;
            match array {
                Value::ArrayOfInteger(array) => array.resize(size, 0),
                Value::ArrayOfNumber(array) => array.resize(size, 0.0),
                Value::ArrayOfString(array) => array.resize(size, String::new()),
                Value::ArrayOfBoolean(array) => array.resize(size, false),
                _ => return Err(VMError::ExpectedArray(array.clone())),
            };
        }
        Instruction::Insert(Insert {
            array_output,
            index,
            input,
        }) => {
            insert(memory, array_output, index, input)?;
        }
        Instruction::PushBack(PushBack {
            array_output,
            input,
        }) => {
            let value = memory.load(input.as_str())?.clone();
            let array_output = memory.load_mut(array_output.as_str())?;
            match (array_output, value) {
                (Value::ArrayOfInteger(array_output), Value::Integer(value)) => {
                    array_output.push(value);
                }
                (Value::ArrayOfNumber(array_output), Value::Number(value)) => {
                    array_output.push(value);
                }
                (Value::ArrayOfString(array_output), Value::String(value)) => {
                    array_output.push(value);
                }
                (Value::ArrayOfBoolean(array_output), Value::Boolean(value)) => {
                    array_output.push(value);
                }
                (a, b) => return Err(VMError::MismatchingTypes(a.clone(), b)),
            };
        }
        Instruction::Concat(Concat {
            input_a,
            input_b,
            dest,
        }) => {
            let input_a = memory.load(input_a.as_str())?;
            let input_b = memory.load(input_b.as_str())?;
            let result = match (input_a, input_b) {
                (Value::String(input_a), Value::String(input_b)) => {
                    Value::String(format!("{}{}", input_a, input_b))
                }
                // todo concat arrays
                _ => return Err(VMError::MismatchingTypes(input_a.clone(), input_b.clone())),
            };
            memory.store(dest.as_str(), result);
        }
        Instruction::Erase(Erase { array_input, index }) => {
            let index = memory.load_index(index.as_str())?;
            let array = memory.load_mut(array_input.as_str())?;
            match array {
                Value::ArrayOfInteger(array) => vector_remove(array, index)?,
                Value::ArrayOfNumber(array) => vector_remove(array, index)?,
                Value::ArrayOfString(array) => vector_remove(array, index)?,
                Value::ArrayOfBoolean(array) => vector_remove(array, index)?,
                _ => return Err(VMError::ExpectedArray(array.clone())),
            };
        }
    };
    Ok(())
}

fn call_routine(
    memory: &mut Memory,
    routines: &Routines,
    routine_name: &str,
    output: &mut String,
) -> Result<(), VMError> {
    let routine = routines.get(routine_name);
    let routine = match routine {
        None => return Err(VMError::RoutineDoesNotExist(String::from(routine_name))),
        Some(routine) => routine,
    };

    for i in routine.instructions.iter() {
        execute(memory, routines, i, output)?;
    }
    Ok(())
}

fn vector_get_copy_at<T>(vec: &Vec<T>, index: usize) -> Result<T, VMError>
where
    T: Clone,
{
    match vec.get(index) {
        Some(value) => Ok(value.clone()),
        None => Err(VMError::IndexOutOfBound {
            array_size: vec.len(),
            index,
        }),
    }
}

fn vector_set_at<T>(vec: &mut Vec<T>, new_value: T, index: usize) -> Result<(), VMError> {
    match vec.get_mut(index) {
        Some(old_value) => {
            *old_value = new_value;
            Ok(())
        }
        None => Err(VMError::IndexOutOfBound {
            array_size: vec.len(),
            index,
        }),
    }
}

fn vector_insert<T>(vec: &mut Vec<T>, new_value: T, index: usize) -> Result<(), VMError> {
    if vec.len() > index {
        vec.insert(index, new_value);
        Ok(())
    } else {
        Err(VMError::IndexOutOfBound {
            array_size: vec.len(),
            index,
        })
    }
}

fn vector_remove<T>(vec: &mut Vec<T>, index: usize) -> Result<(), VMError> {
    if vec.len() > index {
        vec.remove(index);
        Ok(())
    } else {
        Err(VMError::IndexOutOfBound {
            array_size: vec.len(),
            index,
        })
    }
}

fn store_at(
    memory: &mut Memory,
    array_output: &str,
    index: &str,
    value: Value,
) -> Result<(), VMError> {
    let index = memory.load_index(index)?;
    let array_output = memory.load_mut(array_output)?;
    match (array_output, value) {
        (Value::ArrayOfInteger(array_output), Value::Integer(value)) => {
            vector_set_at(array_output, value, index)?;
        }
        (Value::ArrayOfNumber(array_output), Value::Number(value)) => {
            vector_set_at(array_output, value, index)?;
        }
        (Value::ArrayOfString(array_output), Value::String(value)) => {
            vector_set_at(array_output, value, index)?;
        }
        (Value::ArrayOfBoolean(array_output), Value::Boolean(value)) => {
            vector_set_at(array_output, value, index)?;
        }
        _ => return Err(VMError::MismatchingTypes(Value::Boolean(true), Value::Boolean(true))),
    };
    Ok(())
}

fn insert(
    memory: &mut Memory,
    array_output: &str,
    index: &str,
    value: &str,
) -> Result<(), VMError> {
    let value = memory.load(value)?.clone();
    let index = memory.load_index(index)?;
    let array_output = memory.load_mut(array_output)?;
    match (array_output, value) {
        (Value::ArrayOfInteger(array_output), Value::Integer(value)) => {
            vector_insert(array_output, value, index)?;
        }
        (Value::ArrayOfNumber(array_output), Value::Number(value)) => {
            vector_insert(array_output, value, index)?;
        }
        (Value::ArrayOfString(array_output), Value::String(value)) => {
            vector_insert(array_output, value, index)?;
        }
        (Value::ArrayOfBoolean(array_output), Value::Boolean(value)) => {
            vector_insert(array_output, value, index)?;
        }
        (a, b) => return Err(VMError::MismatchingTypes(a.clone(), b)),
    };
    Ok(())
}
