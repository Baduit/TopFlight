use std::fmt;
use std::str::{Chars, FromStr};
use thiserror::Error;

/*
** Public stuff
*/

#[derive(Debug, PartialEq, PartialOrd, Clone)]
pub enum Value {
    Integer(i64),
    Number(f64),
    Char(char),
    String(String),
    Boolean(bool),
    ArrayOfInteger(Vec<i64>),
    ArrayOfNumber(Vec<f64>),
    ArrayOfString(Vec<String>),
    ArrayOfBoolean(Vec<bool>),
}

impl fmt::Display for Value {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Value::Integer(i) => write!(f, "{}", i),
            Value::Number(n) => write!(f, "{}", n),
            Value::Char(c) => write!(f, "{}", c),
            Value::String(str) => write!(f, "{}", str),
            Value::Boolean(b) => write!(f, "{}", b),
            // I don't know if it is clean to use {:?}, but honestly I don't really care about the formatting of the array
            Value::ArrayOfInteger(v) => write!(f, "{:?}", v),
            Value::ArrayOfNumber(v) => write!(f, "{:?}", v),
            Value::ArrayOfString(v) => write!(f, "{:?}", v),
            Value::ArrayOfBoolean(v) => write!(f, "{:?}", v),
        }
    }
}

#[derive(Error, Debug, PartialEq)]
pub enum ParseError {
    #[error("Native parsing error")]
    NativeParseError,
    #[error("Format is invalid")]
    InvalidFormat,
    // todo: add a lot more of error type in the future to convey more informations
}

pub fn parse_value(input: &mut Chars) -> Result<Value, ParseError> {
    let value_typename = input.by_ref().take_while(|c| *c != '(').collect::<String>();
    match value_typename.as_str() {
        "INTEGER" => Ok(Value::Integer(parse_number::<i64>(input)?.value)),
        "NUMBER" => Ok(Value::Number(parse_number::<f64>(input)?.value)),
        "BOOLEAN" => Ok(Value::Boolean(parse_boolean(input)?.value)),
        "STRING" => Ok(Value::String(parse_string(input)?.value)),
        "CHAR" => Ok(Value::Char(parse_char(input)?.value)),
        "ARRAY_OF_INTEGER" => Ok(Value::ArrayOfInteger(
            parse_array_of_numbers::<i64>(input)?.value,
        )),
        "ARRAY_OF_NUMBER" => Ok(Value::ArrayOfNumber(
            parse_array_of_numbers::<f64>(input)?.value,
        )),
        "ARRAY_OF_BOOLEAN" => Ok(Value::ArrayOfBoolean(parse_array_of_booleans(input)?.value)),
        "ARRAY_OF_STRING" => Ok(Value::ArrayOfString(parse_array_of_strings(input)?.value)),
        _ => Err(ParseError::InvalidFormat),
    }
}

/*
** Private implementation
*/

#[derive(Debug, PartialEq)]
struct ParseSuccess<T> {
    pub value: T,
    pub is_last_value: bool,
}

type ParseResult<T> = Result<ParseSuccess<T>, ParseError>;

fn parse_number<T>(input: &mut Chars) -> ParseResult<T>
where
    T: FromStr,
{
    let mut is_last_value = true;

    let predicate = |c: &char| {
        if *c == ',' {
            is_last_value = false;
        }
        *c != ')' && *c != ','
    };

    let important_chars = input.by_ref().take_while(predicate).collect::<String>();

    let maybe_i = important_chars.parse::<T>();
    match maybe_i {
        Ok(i) => Ok(ParseSuccess {
            value: i,
            is_last_value: is_last_value,
        }),
        _ => Err(ParseError::NativeParseError),
    }
}

fn parse_boolean(input: &mut Chars) -> ParseResult<bool> {
    let mut is_last_value = true;

    let predicate = |c: &char| {
        if *c == ',' {
            is_last_value = false;
        }
        *c != ')' && *c != ','
    };

    let important_chars = input.by_ref().take_while(predicate).collect::<String>();

    match important_chars.as_str() {
        "true" => Ok(ParseSuccess {
            value: true,
            is_last_value: is_last_value,
        }),
        "false" => Ok(ParseSuccess {
            value: false,
            is_last_value: is_last_value,
        }),
        _ => Err(ParseError::InvalidFormat),
    }
}

// There is no 'ARRAY_OF_CHAR' so no "strategy" arg
fn parse_char(input: &mut Chars) -> ParseResult<char> {
    if input.next() != Some('\'') {
        return Err(ParseError::InvalidFormat);
    }

    let c = input.next();
    let parsed_char = match c {
        None => return Err(ParseError::InvalidFormat),
        Some('\\') => parse_backslash_char(input.next(), '\'')?,
        Some(normal_char) => normal_char,
    };

    if input.next() != Some('\'') && input.next() != Some(')') {
        Err(ParseError::InvalidFormat)
    } else {
        Ok(ParseSuccess {
            value: parsed_char,
            is_last_value: true,
        })
    }
}

fn parse_string(input: &mut Chars) -> ParseResult<String> {
    if input.next() != Some('"') {
        return Err(ParseError::InvalidFormat);
    }

    let mut result = String::new();
    loop {
        let c = match input.next() {
            None => return Err(ParseError::InvalidFormat),
            Some('"') => break,
            Some('\\') => parse_backslash_char(input.next(), '"')?,
            Some(normal_char) => normal_char,
        };
        result.push(c);
    }

    match input.next() {
        None => Err(ParseError::InvalidFormat),
        Some(')') => Ok(ParseSuccess {
            value: result,
            is_last_value: true,
        }),
        Some(',') => Ok(ParseSuccess {
            value: result,
            is_last_value: false,
        }),
        _ => Err(ParseError::InvalidFormat),
    }
}

// Todo: remove duplicated code for the arrays, the only difference is the parse_* function
// Probably use something like a function pointer or a callable. Need to see the idiomatic rust way
fn parse_array_of_numbers<T>(input: &mut Chars) -> ParseResult<Vec<T>>
where
    T: FromStr,
{
    let mut result: Vec<T> = Vec::new();
    loop {
        let parsed_result = parse_number::<T>(input)?;
        result.push(parsed_result.value);
        if parsed_result.is_last_value {
            break;
        }
    }
    Ok(ParseSuccess {
        value: result,
        is_last_value: true,
    })
}

fn parse_array_of_booleans(input: &mut Chars) -> ParseResult<Vec<bool>> {
    let mut result: Vec<bool> = Vec::new();
    loop {
        let parsed_result = parse_boolean(input)?;
        result.push(parsed_result.value);
        if parsed_result.is_last_value {
            break;
        }
    }
    Ok(ParseSuccess {
        value: result,
        is_last_value: true,
    })
}

fn parse_array_of_strings(input: &mut Chars) -> ParseResult<Vec<String>> {
    let mut result: Vec<String> = Vec::new();
    loop {
        let parsed_result = parse_string(input)?;
        result.push(parsed_result.value);
        if parsed_result.is_last_value {
            break;
        }
    }
    Ok(ParseSuccess {
        value: result,
        is_last_value: true,
    })
}

fn parse_backslash_char(c: Option<char>, quote: char) -> Result<char, ParseError> {
    match c {
        None => Err(ParseError::InvalidFormat),
        Some('\\') => Ok('\\'),
        Some('0') => Ok('\0'),
        Some('n') => Ok('\n'),
        Some('r') => Ok('\r'),
        Some('t') => Ok('\t'),
        _ if c.unwrap() == quote => Ok(quote),
        _ => Err(ParseError::InvalidFormat),
    }
}

/*
** Unit tests
*/

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_integer() {
        {
            let input = "5";
            let mut input_it = input.chars();
            assert_eq!(
                parse_number(&mut input_it),
                Ok(ParseSuccess {
                    value: 5,
                    is_last_value: true
                })
            );
        }
        {
            let input = "5)";
            let mut input_it = input.chars();
            assert_eq!(
                parse_number(&mut input_it),
                Ok(ParseSuccess {
                    value: 5,
                    is_last_value: true
                })
            );
        }
        {
            let input = "5,-5)";
            let mut input_it = input.chars();
            assert_eq!(
                parse_number(&mut input_it),
                Ok(ParseSuccess {
                    value: 5,
                    is_last_value: false
                })
            );
            assert_eq!(
                parse_number(&mut input_it),
                Ok(ParseSuccess {
                    value: -5,
                    is_last_value: true
                })
            );
        }
        {
            let input = "-5,toto";
            let mut input_it = input.chars();
            assert_eq!(
                parse_number(&mut input_it),
                Ok(ParseSuccess {
                    value: -5,
                    is_last_value: false
                })
            );
        }
        {
            let input = "5 5";
            let mut input_it = input.chars();
            assert_eq!(
                parse_number::<i64>(&mut input_it),
                Err(ParseError::NativeParseError)
            );
        }
        {
            let input = "lol";
            let mut input_it = input.chars();
            assert_eq!(
                parse_number::<i64>(&mut input_it),
                Err(ParseError::NativeParseError)
            );
        }
    }

    #[test]
    fn test_parse_bool() {
        {
            let input = "true";
            let mut input_it = input.chars();
            assert_eq!(
                parse_boolean(&mut input_it),
                Ok(ParseSuccess {
                    value: true,
                    is_last_value: true
                })
            );
        }
        {
            let input = "true)";
            let mut input_it = input.chars();
            assert_eq!(
                parse_boolean(&mut input_it),
                Ok(ParseSuccess {
                    value: true,
                    is_last_value: true
                })
            );
        }
        {
            let input = "false";
            let mut input_it = input.chars();
            assert_eq!(
                parse_boolean(&mut input_it),
                Ok(ParseSuccess {
                    value: false,
                    is_last_value: true
                })
            );
        }
        {
            let input = "false,true)";
            let mut input_it = input.chars();
            assert_eq!(
                parse_boolean(&mut input_it),
                Ok(ParseSuccess {
                    value: false,
                    is_last_value: false
                })
            );
            assert_eq!(
                parse_boolean(&mut input_it),
                Ok(ParseSuccess {
                    value: true,
                    is_last_value: true
                })
            );
        }
        {
            let input = "lol";
            let mut input_it = input.chars();
            assert_eq!(parse_boolean(&mut input_it), Err(ParseError::InvalidFormat));
        }
        {
            let input = "truel)";
            let mut input_it = input.chars();
            assert_eq!(parse_boolean(&mut input_it), Err(ParseError::InvalidFormat));
        }
        {
            let input = "true+)";
            let mut input_it = input.chars();
            assert_eq!(parse_boolean(&mut input_it), Err(ParseError::InvalidFormat));
        }
    }

    #[test]
    fn test_parse_char() {
        {
            let input = "'a')";
            let mut input_it = input.chars();
            assert_eq!(
                parse_char(&mut input_it),
                Ok(ParseSuccess {
                    value: 'a',
                    is_last_value: true,
                })
            );
        }
        {
            let input = "'9')";
            let mut input_it = input.chars();
            assert_eq!(
                parse_char(&mut input_it),
                Ok(ParseSuccess {
                    value: '9',
                    is_last_value: true,
                })
            );
        }
        {
            let input = "'\\t')";
            let mut input_it = input.chars();
            assert_eq!(
                parse_char(&mut input_it),
                Ok(ParseSuccess {
                    value: '\t',
                    is_last_value: true,
                })
            );
        }
        {
            let input = "'\\'')";
            let mut input_it = input.chars();
            assert_eq!(
                parse_char(&mut input_it),
                Ok(ParseSuccess {
                    value: '\'',
                    is_last_value: true,
                })
            );
        }
        {
            let input = "'\\o')";
            let mut input_it = input.chars();
            assert_eq!(parse_char(&mut input_it), Err(ParseError::InvalidFormat));
        }
        {
            let input = "'\\')";
            let mut input_it = input.chars();
            assert_eq!(parse_char(&mut input_it), Err(ParseError::InvalidFormat));
        }
        {
            let input = "'a)";
            let mut input_it = input.chars();
            assert_eq!(parse_char(&mut input_it), Err(ParseError::InvalidFormat));
        }
        {
            let input = "z')";
            let mut input_it = input.chars();
            assert_eq!(parse_char(&mut input_it), Err(ParseError::InvalidFormat));
        }
        {
            let input = "'')";
            let mut input_it = input.chars();
            assert_eq!(parse_char(&mut input_it), Err(ParseError::InvalidFormat));
        }
        {
            let input = "";
            let mut input_it = input.chars();
            assert_eq!(parse_char(&mut input_it), Err(ParseError::InvalidFormat));
        }
        {
            let input = "'";
            let mut input_it = input.chars();
            assert_eq!(parse_char(&mut input_it), Err(ParseError::InvalidFormat));
        }
        {
            let input = "%";
            let mut input_it = input.chars();
            assert_eq!(parse_char(&mut input_it), Err(ParseError::InvalidFormat));
        }
    }

    #[test]
    fn test_parse_string() {
        {
            let input = "\"lol\")";
            let mut input_it = input.chars();
            assert_eq!(
                parse_string(&mut input_it),
                Ok(ParseSuccess {
                    value: String::from("lol"),
                    is_last_value: true,
                })
            );
        }
        {
            let input = "\"lola\",\"lol\")";
            let mut input_it = input.chars();
            assert_eq!(
                parse_string(&mut input_it),
                Ok(ParseSuccess {
                    value: String::from("lola"),
                    is_last_value: false,
                })
            );
            assert_eq!(
                parse_string(&mut input_it),
                Ok(ParseSuccess {
                    value: String::from("lol"),
                    is_last_value: true,
                })
            );
        }
    }

    #[test]
    fn test_parse_array_of_integers() {
        let input = "1,2,3)";
        let mut input_it = input.chars();
        assert_eq!(
            parse_array_of_numbers(&mut input_it),
            Ok(ParseSuccess {
                value: vec![1, 2, 3],
                is_last_value: true,
            })
        );
    }

    #[test]
    fn test_parse_array_of_numbers() {
        let input = "1.1,2.2,3.3)";
        let mut input_it = input.chars();
        assert_eq!(
            parse_array_of_numbers(&mut input_it),
            Ok(ParseSuccess {
                value: vec![1.1, 2.2, 3.3],
                is_last_value: true,
            })
        );
    }

    #[test]
    fn test_parse_array_of_booleans() {
        let input = "true,true,false)";
        let mut input_it = input.chars();
        assert_eq!(
            parse_array_of_booleans(&mut input_it),
            Ok(ParseSuccess {
                value: vec![true, true, false],
                is_last_value: true,
            })
        );
    }

    #[test]
    fn test_parse_array_of_strings() {
        let input = "\"true\",\"true\",\"false\")";
        let mut input_it = input.chars();
        assert_eq!(
            parse_array_of_strings(&mut input_it),
            Ok(ParseSuccess {
                value: vec![
                    String::from("true"),
                    String::from("true"),
                    String::from("false")
                ],
                is_last_value: true,
            })
        );
    }

    #[test]
    fn test_parse_integer_value() {
        let input = "INTEGER(5)";
        let mut input_it = input.chars();
        assert_eq!(parse_value(&mut input_it), Ok(Value::Integer(5)));
    }
}
