//! Source selection for explicit GAS edition wrappers, shared by discovery and
//! diagnostic readers. Commands remain the authority that assemble the source.
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

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

/// Explicit edition conditionals, preserving line numbers. Unknown conditions
/// fail rather than silently discovering an inactive stream or message name.
/// Macro bodies are retained as definitions; stream includes inside macros are
/// refused by stream discovery because it cannot infer their expansions.
#[cfg(test)]
pub(crate) fn selected(text: &str, edition: &str) -> Result<String, String> {
    selected_includes(text, edition, &[])
}

/// Read active source includes for predicate definitions. Their text remains
/// an include in the result, so the source's line numbers and ownership stay
/// intact. Unknown expressions and missing includes are never guessed.
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn numeric_predicates_come_from_active_current_source_includes() {
        let work = tempfile::tempdir().unwrap();
        let root = work.path();
        let predicate = root.join("edition.inc");
        std::fs::write(&predicate, ".set International, 0\n").unwrap();
        let source = ".include \"edition.inc\"\n.ifeq International\nMsgJapanese\n.else\nMsgInternational\n.endif\n";
        let select = || selected_includes(source, "TBS_EDITION_JA", &[root]).unwrap();
        assert!(select().contains("MsgJapanese"));
        assert!(!select().contains("MsgInternational"));
        assert_eq!(select().lines().count(), source.lines().count());
        std::fs::write(&predicate, ".set International, 1\n").unwrap();
        assert!(select().contains("MsgInternational"));
        assert!(!select().contains("MsgJapanese"));
        std::fs::write(&predicate, ".set International, Unknown + 2\n").unwrap();
        assert!(selected_includes(source, "TBS_EDITION_JA", &[root]).is_err());
        std::fs::write(&predicate, ".include \"edition.inc\"\n").unwrap();
        assert!(selected_includes(source, "TBS_EDITION_JA", &[root])
            .unwrap_err()
            .contains("recursive GAS include"));
        std::fs::remove_file(&predicate).unwrap();
        assert!(selected_includes(source, "TBS_EDITION_JA", &[root]).is_err());
    }

    #[test]
    fn c_and_assembly_predicates_agree_for_every_target_and_default_and_reject_conflicts() {
        use object::{Object, ObjectSection};
        let root = crate::compiler::routing::root();
        let work = tempfile::tempdir().unwrap();
        let assembly = work.path().join("edition.s");
        let object = work.path().join("edition.o");
        let source = ".include \"games/COMMON/INCLUDE/GAME/ED_ASM.H\"\n.include \"games/COMMON/INCLUDE/GAME/ED_ASM.H\"\n.section .rodata\n.byte EDITION_INTERNATIONAL\n.ifeq EDITION_INTERNATIONAL\n.byte 0x4a\n.else\n.byte 0x49\n.endif\n";
        std::fs::write(&assembly, source).unwrap();
        let c = work.path().join("edition.c");
        std::fs::write(&c, "#include \"EDITION.H\"\nEDITION_INTERNATIONAL\n").unwrap();
        let assemble = |defines: &[&str]| {
            let mut command = crate::compiler::routing::assembly_command(
                assembly.to_str().unwrap(),
                object.to_str().unwrap(),
            );
            command[0] = crate::compiler::routing::binutils_prefix()
                .join("bin/arm-none-eabi-as")
                .to_string_lossy()
                .into_owned();
            for define in defines {
                command.extend(["--defsym".into(), format!("{define}=1")]);
            }
            psynergy::process::run(&command, root)?;
            let bytes = std::fs::read(&object).map_err(|error| error.to_string())?;
            Ok::<_, String>(
                object::File::parse(&*bytes)
                    .unwrap()
                    .section_by_name(".rodata")
                    .unwrap()
                    .data()
                    .unwrap()
                    .to_vec(),
            )
        };
        let preprocess = |defines: &[&str]| {
            let mut command = vec![
                crate::compiler::routing::bundle()
                    .join("xgcc")
                    .to_string_lossy()
                    .into_owned(),
                format!("-B{}/", crate::compiler::routing::bundle().display()),
                "-E".into(),
                "-nostdinc".into(),
                format!("-I{}", root.join("games/COMMON/INCLUDE/GAME").display()),
            ];
            command.extend(defines.iter().map(|name| format!("-D{name}=1")));
            command.push(c.to_string_lossy().into_owned());
            psynergy::process::run(&command, root)
        };
        let names =
            crate::targets::TARGET_IDS.map(|id| crate::targets::target_for(id).edition_define);
        for defines in std::iter::once(Vec::new()).chain(names.iter().map(|name| vec![*name])) {
            let international =
                u8::from(defines.first().is_some_and(|name| !name.ends_with("_JA")));
            let bytes = assemble(&defines).unwrap();
            assert_eq!(
                bytes,
                [international, if international == 0 { 0x4a } else { 0x49 }],
                "{defines:?}"
            );
            assert_eq!(
                preprocess(&defines).unwrap().lines().last().unwrap().trim(),
                international.to_string(),
                "{defines:?}"
            );
            let selected =
                selected_includes(source, defines.first().copied().unwrap_or(""), &[root]).unwrap();
            assert_eq!(selected.contains(".byte 0x49"), international != 0);
            assert_eq!(selected.contains(".byte 0x4a"), international == 0);
        }
        for (at, first) in names.iter().enumerate() {
            for second in &names[at + 1..] {
                let defines = [*first, *second];
                assert!(
                    assemble(&defines)
                        .unwrap_err()
                        .contains("multiple game editions selected"),
                    "{defines:?}"
                );
                assert!(
                    preprocess(&defines)
                        .unwrap_err()
                        .contains("multiple game editions selected"),
                    "{defines:?}"
                );
            }
        }
    }

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
