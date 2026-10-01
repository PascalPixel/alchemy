//! Source selection for explicit GAS edition wrappers, shared by discovery and
//! diagnostic readers. Commands remain the authority that assemble the source.
use std::collections::BTreeSet;

#[derive(Clone, Copy)]
struct Branch {
    parent: bool,
    taken: bool,
    active: bool,
    else_seen: bool,
}

/// GAS @ comments and C block comments, with strings and line numbers intact.
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

fn value(text: &str, edition: &str) -> Result<i64, String> {
    let text = text.trim();
    if text == edition {
        return Ok(1);
    }
    let (negative, digits) = text
        .strip_prefix('-')
        .map_or((false, text), |digits| (true, digits));
    let parsed = if let Some(digits) = digits.strip_prefix("0x") {
        i64::from_str_radix(digits, 16)
    } else {
        digits.parse::<i64>()
    };
    parsed.map(|number| if negative { -number } else { number }).map_err(|_| format!("unsupported GAS selection expression {text:?}; use an explicit edition .ifdef wrapper"))
}

fn condition(
    directive: &str,
    argument: &str,
    edition: &str,
    defined: &BTreeSet<String>,
) -> Result<bool, String> {
    match directive {
        ".ifdef" => Ok(defined.contains(argument.trim())),
        ".ifndef" | ".ifnotdef" => Ok(!defined.contains(argument.trim())),
        ".if" | ".ifne" => Ok(value(argument, edition)? != 0),
        ".ifeq" => Ok(value(argument, edition)? == 0),
        ".ifge" => Ok(value(argument, edition)? >= 0),
        ".ifgt" => Ok(value(argument, edition)? > 0),
        ".ifle" => Ok(value(argument, edition)? <= 0),
        ".iflt" => Ok(value(argument, edition)? < 0),
        _ => Err(format!("unsupported GAS selection directive {directive}")),
    }
}

/// Explicit edition conditionals, preserving line numbers. Unknown conditions
/// fail rather than silently discovering an inactive stream or message name.
/// Macro bodies are retained as definitions; stream includes inside macros are
/// refused by stream discovery because it cannot infer their expansions.
pub(crate) fn selected(text: &str, edition: &str) -> Result<String, String> {
    let mut stack = Vec::<Branch>::new();
    let mut defined = [edition.to_string()].into_iter().collect::<BTreeSet<_>>();
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
                condition(directive, argument, edition, &defined).map_err(error)?
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
                condition(".if", argument, edition, &defined).map_err(error)?
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
            output.push_str(row);
            if let Some(name) = syntax.strip_suffix(':') {
                defined.insert(name.trim().into());
            }
            if matches!(directive, ".set" | ".equ" | ".equiv") {
                if let Some((name, _)) = argument.split_once(',') {
                    defined.insert(name.trim().into());
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

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn selects_nested_edition_wrappers_without_reading_inactive_unknown_conditions() {
        let text = ".ifdef TBS_EDITION_JA\nJA\n.if 0\nDEAD\n.else\nLIVE\n.endif\n.else\n.ifdef TBS_EDITION_EN\nEN\n.else\nEU\n.endif\n.endif\n";
        let ja = selected(text, "TBS_EDITION_JA").unwrap();
        assert!(ja.contains("JA\n") && ja.contains("LIVE\n"));
        assert!(!ja.contains("DEAD") && !ja.contains("EN\n"));
        assert_eq!(ja.lines().count(), text.lines().count());
        assert!(selected(text, "TBS_EDITION_EN").unwrap().contains("EN\n"));
        assert!(selected(
            ".if 0\n.if SYMBOL + 2\nignored\n.endif\n.endif\n",
            "TBS_EDITION_JA"
        )
        .is_ok());
        assert!(selected(".if SYMBOL + 2\nunknown\n.endif\n", "TBS_EDITION_JA").is_err());
        assert!(selected(".else\n", "TBS_EDITION_JA").is_err());
        assert!(selected(".ifdef TBS_EDITION_JA\n", "TBS_EDITION_JA").is_err());
        assert!(selected(
            ".if 0\n.macro skipped\n.if SYMBOL + 2\n.endif\n.endm\n.endif\n",
            "TBS_EDITION_JA"
        )
        .is_ok());
    }
}
