use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

#[derive(Clone, Copy)]
struct Branch {
    parent: bool,
    taken: bool,
    active: bool,
    else_seen: bool,
}

pub(crate) fn without_comments(text: &str) -> String {
    let mut bytes = text.as_bytes().to_vec();
    let (mut at, mut quote, mut block) = (0, false, false);
    while at < bytes.len() {
        if block {
            if bytes[at..].starts_with(b"*/") {
                bytes[at] = b' ';
                bytes[at + 1] = b' ';
                at += 2;
                block = false;
            } else {
                if bytes[at] != b'\n' {
                    bytes[at] = b' ';
                }
                at += 1;
            }
            continue;
        }
        if quote {
            if bytes[at] == b'\\' {
                at += 2;
                continue;
            }
            if bytes[at] == b'"' {
                quote = false;
            }
            at += 1;
            continue;
        }
        if bytes[at] == b'"' {
            quote = true;
            at += 1;
        } else if bytes[at..].starts_with(b"/*") {
            bytes[at] = b' ';
            bytes[at + 1] = b' ';
            at += 2;
            block = true;
        } else if bytes[at] == b'@' {
            while at < bytes.len() && bytes[at] != b'\n' {
                bytes[at] = b' ';
                at += 1;
            }
        } else {
            at += 1;
        }
    }
    String::from_utf8(bytes).expect("masking preserves UTF-8")
}

fn value(text: &str, symbols: &BTreeMap<String, String>) -> Result<i64, String> {
    let mut visited = BTreeSet::new();
    let mut text = text.trim();
    while let Some(expression) = symbols.get(text) {
        if !visited.insert(text) {
            return Err(format!("recursive GAS selection value {text:?}"));
        }
        text = expression.trim();
    }
    let text = text.trim();
    let (negative, digits) = text
        .strip_prefix('-')
        .map_or((false, text), |digits| (true, digits));
    let parsed = if let Some(digits) = digits.strip_prefix("0x") {
        i64::from_str_radix(digits, 16)
    } else {
        digits.parse::<i64>()
    };
    parsed.map(|number| if negative { -number } else { number }).map_err(|_| format!("unsupported GAS selection expression {text:?}; use a source-owned numeric predicate or explicit edition .ifdef wrapper"))
}

fn condition(
    directive: &str,
    argument: &str,
    symbols: &BTreeMap<String, String>,
) -> Result<bool, String> {
    match directive {
        ".ifdef" => Ok(symbols.contains_key(argument.trim())),
        ".ifndef" | ".ifnotdef" => Ok(!symbols.contains_key(argument.trim())),
        ".if" | ".ifne" => Ok(value(argument, symbols)? != 0),
        ".ifeq" => Ok(value(argument, symbols)? == 0),
        ".ifge" => Ok(value(argument, symbols)? >= 0),
        ".ifgt" => Ok(value(argument, symbols)? > 0),
        ".ifle" => Ok(value(argument, symbols)? <= 0),
        ".iflt" => Ok(value(argument, symbols)? < 0),
        _ => Err(format!("unsupported GAS selection directive {directive}")),
    }
}

pub(crate) fn selected_includes(
    text: &str,
    edition: &str,
    directories: &[&Path],
) -> Result<String, String> {
    let mut symbols = BTreeMap::new();
    if !edition.is_empty() {
        symbols.insert(edition.to_string(), "1".into());
    }
    select(text, &mut symbols, directories, &mut BTreeSet::new())
}

fn select(
    text: &str,
    symbols: &mut BTreeMap<String, String>,
    directories: &[&Path],
    includes: &mut BTreeSet<PathBuf>,
) -> Result<String, String> {
    let mut stack = Vec::<Branch>::new();
    let mut output = String::new();
    let mut macro_depth = 0usize;
    let visible = without_comments(text);
    for (number, (row, syntax)) in text.lines().zip(visible.lines()).enumerate() {
        let syntax = syntax.trim();
        let (directive, argument) = syntax
            .split_once(char::is_whitespace)
            .unwrap_or((syntax, ""));
        let active = stack.last().is_none_or(|branch| branch.active);
        let error = |error| format!("line {}: {error}", number + 1);
        if macro_depth > 0 {
            if directive == ".macro" {
                macro_depth += 1;
            }
            if directive == ".endm" {
                macro_depth -= 1;
            }
            if active {
                output.push_str(row);
            }
            output.push('\n');
            continue;
        }
        if directive == ".macro" {
            macro_depth = 1;
            if active {
                output.push_str(row);
            }
            output.push('\n');
            continue;
        }
        if directive.starts_with(".if") {
            let selected = if active {
                condition(directive, argument, symbols).map_err(error)?
            } else {
                false
            };
            stack.push(Branch {
                parent: active,
                taken: selected,
                active: active && selected,
                else_seen: false,
            });
        } else if directive == ".else" {
            let branch = stack
                .last_mut()
                .ok_or_else(|| error(".else without .if".into()))?;
            if branch.else_seen {
                return Err(error("duplicate .else".into()));
            }
            branch.active = branch.parent && !branch.taken;
            branch.taken = true;
            branch.else_seen = true;
        } else if directive == ".elseif" {
            let branch = stack
                .last_mut()
                .ok_or_else(|| error(".elseif without .if".into()))?;
            if branch.else_seen {
                return Err(error(".elseif after .else".into()));
            }
            let selected = if branch.parent && !branch.taken {
                condition(".if", argument, symbols).map_err(error)?
            } else {
                false
            };
            branch.active = branch.parent && !branch.taken && selected;
            branch.taken |= selected;
        } else if directive == ".endif" {
            stack
                .pop()
                .ok_or_else(|| error(".endif without .if".into()))?;
        } else if active {
            if directive == ".error" {
                return Err(error(format!("active GAS .error {argument}")));
            }
            if directive == ".include" && !directories.is_empty() {
                let name = argument.trim().trim_matches('"');
                let path = directories
                    .iter()
                    .map(|directory| directory.join(name))
                    .find(|path| path.is_file())
                    .ok_or_else(|| error(format!("missing GAS include {name:?}")))?;
                let path =
                    std::fs::canonicalize(path).map_err(|failure| error(failure.to_string()))?;
                if !includes.insert(path.clone()) {
                    return Err(error(format!("recursive GAS include {}", path.display())));
                }
                let source =
                    std::fs::read_to_string(&path).map_err(|failure| error(failure.to_string()))?;
                select(&source, symbols, directories, includes)
                    .map_err(|failure| error(format!("{}: {failure}", path.display())))?;
                includes.remove(&path);
            }
            output.push_str(row);
            if let Some(name) = syntax.strip_suffix(':') {
                symbols.insert(name.trim().into(), "<label>".into());
            }
            if matches!(directive, ".set" | ".equ" | ".equiv") {
                if let Some((name, expression)) = argument.split_once(',') {
                    symbols.insert(name.trim().into(), expression.trim().into());
                }
            }
        }
        output.push('\n');
    }
    if !stack.is_empty() {
        return Err("unterminated GAS edition conditional".into());
    }
    if macro_depth > 0 {
        return Err("unterminated GAS macro".into());
    }
    Ok(output)
}
