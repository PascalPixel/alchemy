//! S2 source debt ratchet. Baselines and exceptions are maintained source policy;
//! only current C/header text is measured, once per owned definition or macro.
//! The initial baseline was captured from the current main source on
//! 2026-10-01, before the Wave 2 cleanup added measured documentation tags.
use crate::permute::lex::{lex, Tok};
use crate::permute::parse::{matching, scan_definitions};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;
use std::process::ExitCode;

const BASELINE: &str = "tools/alchemy/src/check/STEERING_BASELINE.tsv";
const EXCEPTIONS: &str = "tools/alchemy/src/check/STEERING_EXCEPTIONS.tsv";
const HEADER: &str = "scope\tsource\tfunction\tmetric\tcount\n";
const EXCEPTION_HEADER: &str = "id\tdate\tname\tscope\tsource\tfunction\tmetric\tallowance\treason";
const METRICS: &[&str] = &["fakematch", "register_pin", "inline_asm"];
type Key = (String, String, String, String);
type Inventory = BTreeMap<Key, usize>;

fn scope(path: &str) -> Option<&'static str> {
    if path.starts_with("games/COMMON/") {
        Some("common")
    } else if path.starts_with("games/THE BROKEN SEAL/") {
        Some("tbs")
    } else if path.starts_with("games/THE LOST AGE/") {
        Some("tla")
    } else {
        None
    }
}

/// Expose raw directive bodies to the lexer without expanding them. A macro's
/// debt belongs to its definition, regardless of how often it is expanded.
fn directive_text(source: &str) -> String {
    let mut bytes = source.as_bytes().to_vec();
    let mut start = 0;
    while start < bytes.len() {
        let end = source[start..]
            .find('\n')
            .map_or(bytes.len(), |at| start + at);
        let first = (start..end).find(|&at| !bytes[at].is_ascii_whitespace());
        if let Some(at) = first.filter(|&at| bytes[at] == b'#') {
            bytes[at] = b' ';
        }
        if end > start && bytes[end - 1] == b'\\' {
            bytes[end - 1] = b' ';
        }
        start = end + 1;
    }
    String::from_utf8(bytes).expect("masking preserves UTF-8")
}

fn macro_ranges(source: &str) -> Vec<(usize, usize, String)> {
    let mut found = Vec::new();
    let mut start = 0;
    let mut lines = source.split_inclusive('\n').peekable();
    while let Some(row) = lines.next() {
        let first = row.trim_start();
        let length = row.len();
        if let Some(definition) = first
            .strip_prefix("#define")
            .or_else(|| first.strip_prefix("# define"))
        {
            let name = definition
                .trim_start()
                .chars()
                .take_while(|c| c.is_ascii_alphanumeric() || *c == '_')
                .collect::<String>();
            let mut end = start + length;
            let mut continuation = row.trim_end_matches('\n').ends_with('\\');
            while continuation {
                let Some(next) = lines.next() else {
                    break;
                };
                end += next.len();
                continuation = next.trim_end_matches('\n').ends_with('\\');
            }
            found.push((start, end, format!("macro:{name}")));
            start = end;
        } else {
            start += length;
        }
    }
    found
}

/// Find raw-source owners through each conditional arm. These are lexical
/// views, not evaluated editions: every device is still counted once below,
/// including devices in arms that no current edition enables. Selecting an
/// arm together with its ancestors avoids combining mutually exclusive braces
/// while preserving byte offsets back into the maintained source.
fn definition_ranges(source: &str) -> Result<Vec<(usize, usize, String)>, String> {
    let original = lex(source)?;
    let comments = original
        .iter()
        .filter(|token| matches!(token.tok, Tok::Comment(_)))
        .map(|token| token.start..token.end)
        .collect::<Vec<_>>();
    let mut lines = Vec::new();
    let mut paths = BTreeSet::from([Vec::new()]);
    let mut path = Vec::new();
    let mut group = 0;
    let mut start = 0;
    let mut continuation = false;
    for row in source.split_inclusive('\n') {
        let first = row.trim_start();
        let offset = start + row.len() - first.len();
        let directive = !continuation
            && !comments.iter().any(|range| range.contains(&offset))
            && first.starts_with('#');
        if directive {
            let word = first[1..]
                .trim_start()
                .split_whitespace()
                .next()
                .unwrap_or("");
            match word {
                "if" | "ifdef" | "ifndef" => {
                    path.push((group, 0usize));
                    group += 1;
                }
                "else" | "elif" => {
                    if let Some((_, arm)) = path.last_mut() {
                        *arm += 1;
                    }
                }
                "endif" => {
                    path.pop();
                }
                _ => {}
            }
        }
        // The lexer skips directives and complete macro continuations itself.
        // Keep those lines; only ordinary inactive source needs masking.
        lines.push((
            start,
            start + row.len(),
            path.clone(),
            directive || continuation,
        ));
        if !directive && !continuation {
            paths.insert(path.clone());
        }
        continuation = (directive || continuation) && row.trim_end_matches('\n').ends_with('\\');
        start += row.len();
    }
    let mut ranges = BTreeSet::new();
    for selected in paths {
        let mut bytes = source.as_bytes().to_vec();
        for (start, end, path, directive) in &lines {
            if !directive
                && path.iter().any(|(group, arm)| {
                    selected
                        .iter()
                        .find(|(choice, _)| choice == group)
                        .map_or(0, |(_, choice)| *choice)
                        != *arm
                })
            {
                for byte in &mut bytes[*start..*end] {
                    if *byte != b'\n' && *byte != b'\r' {
                        *byte = b' ';
                    }
                }
            }
        }
        let view = String::from_utf8(bytes).expect("masking preserves UTF-8");
        // Preserve the ordinary raw scanner's macro definition boundaries.
        // Hiding a macro here makes its leading comment part of the next
        // function's declaration range and moves existing debt to that caller.
        let tokens = lex(&directive_text(&view))?;
        for (name, boundary, _, _, open) in scan_definitions(&tokens) {
            if let Some(close) = matching(&tokens, open) {
                ranges.insert((tokens[boundary].start, tokens[close].end, name));
            }
        }
    }
    Ok(ranges.into_iter().collect())
}

fn source_inventory(path: &str, source: &str) -> Result<Inventory, String> {
    let Some(scope) = scope(path) else {
        return Ok(Inventory::new());
    };
    let expanded = directive_text(source);
    let tokens = lex(&expanded).map_err(|error| format!("{path}: {error}"))?;
    let macros = macro_ranges(source);
    let definitions = definition_ranges(source).map_err(|error| format!("{path}: {error}"))?;
    let function = |offset| {
        macros
            .iter()
            .find(|(start, end, _)| (*start..*end).contains(&offset))
            .map(|(_, _, name)| name.clone())
            .or_else(|| {
                definitions
                    .iter()
                    .find(|(start, end, _)| (*start..*end).contains(&offset))
                    .map(|(_, _, name)| name.clone())
            })
            .unwrap_or_else(|| "<file>".into())
    };
    let mut result = Inventory::new();
    for (index, token) in tokens.iter().enumerate() {
        let mut add = |metric: &str, count: usize| {
            if count > 0 {
                *result
                    .entry((
                        scope.into(),
                        path.into(),
                        function(token.start),
                        metric.into(),
                    ))
                    .or_default() += count;
            }
        };
        match &token.tok {
            Tok::Comment(comment) => add("fakematch", comment.matches("FAKEMATCH:").count()),
            Tok::Ident(word) if ["asm", "__asm", "__asm__"].contains(&word.as_str()) => {
                let boundary = tokens[..index]
                    .iter()
                    .rposition(|token| {
                        matches!(
                            token.tok,
                            Tok::Punct(";") | Tok::Punct("{") | Tok::Punct("}")
                        )
                    })
                    .map_or(0, |at| at + 1);
                let register = tokens[boundary..index]
                    .iter()
                    .any(|token| matches!(&token.tok, Tok::Ident(word) if word == "register"));
                add(
                    if register {
                        "register_pin"
                    } else {
                        "inline_asm"
                    },
                    1,
                );
            }
            _ => {}
        }
    }
    Ok(result)
}

fn inventory(root: &Path) -> Result<Inventory, String> {
    let mut result = Inventory::new();
    for path in crate::compiler::no_asm::source_files(&root.join("games"))
        .map_err(|error| error.to_string())?
    {
        let canonical = path.canonicalize().map_err(|error| error.to_string())?;
        if !canonical.starts_with(root.canonicalize().map_err(|error| error.to_string())?) {
            return Err(format!("{}: source escapes root", path.display()));
        }
        let source = std::fs::read_to_string(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        let name = path.strip_prefix(root).unwrap_or(&path).to_string_lossy();
        for (key, count) in source_inventory(&name, &source)? {
            *result.entry(key).or_default() += count;
        }
    }
    Ok(result)
}

fn parse_inventory(text: &str) -> Result<Inventory, String> {
    let mut rows = text.lines();
    if rows.next() != Some(HEADER.trim_end()) {
        return Err("invalid steering baseline header".into());
    }
    let mut result = Inventory::new();
    for (index, row) in rows.enumerate() {
        if row.is_empty() {
            continue;
        }
        let fields = row.split('\t').collect::<Vec<_>>();
        if fields.len() != 5 {
            return Err(format!("baseline row {}: expected five fields", index + 2));
        }
        let count = fields[4]
            .parse::<usize>()
            .map_err(|_| format!("baseline row {}: invalid count", index + 2))?;
        let key = (
            fields[0].into(),
            fields[1].into(),
            fields[2].into(),
            fields[3].into(),
        );
        validate_key(&key)?;
        if count == 0 || result.insert(key, count).is_some() {
            return Err(format!(
                "baseline row {}: zero or duplicate site",
                index + 2
            ));
        }
    }
    Ok(result)
}
fn validate_key(key: &Key) -> Result<(), String> {
    let (group, source, function, metric) = key;
    if scope(source) != Some(group.as_str())
        || source.contains("..")
        || source.contains('\\')
        || function.is_empty()
        || function.contains('*')
        || !METRICS.contains(&metric.as_str())
    {
        return Err(format!(
            "invalid steering site {source}:{function} {metric}"
        ));
    }
    Ok(())
}
fn write_inventory(counts: &Inventory) -> String {
    let mut text = HEADER.to_string();
    for ((scope, source, function, metric), count) in counts {
        text.push_str(&format!(
            "{scope}\t{source}\t{function}\t{metric}\t{count}\n"
        ));
    }
    text
}
fn totals(counts: &Inventory) -> BTreeMap<(String, String), usize> {
    let mut result = BTreeMap::new();
    for ((scope, _, _, metric), count) in counts {
        *result.entry((scope.clone(), metric.clone())).or_default() += count;
    }
    result
}
fn date_valid(date: &str) -> bool {
    let fields = date.split('-').collect::<Vec<_>>();
    if fields.len() != 3 || fields[0].len() != 4 || fields[1].len() != 2 || fields[2].len() != 2 {
        return false;
    }
    let (Ok(year), Ok(month), Ok(day)) = (
        fields[0].parse::<usize>(),
        fields[1].parse::<usize>(),
        fields[2].parse::<usize>(),
    ) else {
        return false;
    };
    let days = match month {
        1 | 3 | 5 | 7 | 8 | 10 | 12 => 31,
        4 | 6 | 9 | 11 => 30,
        2 if year % 400 == 0 || year % 4 == 0 && year % 100 != 0 => 29,
        2 => 28,
        _ => return false,
    };
    year > 0 && day > 0 && day <= days
}

fn allowances(text: &str, baseline: &Inventory, current: &Inventory) -> Result<Inventory, String> {
    let mut rows = text.lines();
    if rows.next() != Some(EXCEPTION_HEADER) {
        return Err("invalid steering exception header".into());
    }
    let mut ids = BTreeSet::new();
    let mut result = Inventory::new();
    for (index, row) in rows.enumerate() {
        if row.is_empty() {
            continue;
        }
        let fields = row.split('\t').collect::<Vec<_>>();
        if fields.len() != 9 || fields.iter().any(|field| field.trim().is_empty()) {
            return Err(format!(
                "exception row {}: missing named, dated exact-site fields",
                index + 2
            ));
        }
        if !date_valid(fields[1]) || !ids.insert(fields[0]) {
            return Err(format!(
                "exception row {}: invalid date or duplicate id",
                index + 2
            ));
        }
        let key = (
            fields[3].into(),
            fields[4].into(),
            fields[5].into(),
            fields[6].into(),
        );
        validate_key(&key)?;
        let count = fields[7]
            .parse::<usize>()
            .map_err(|_| format!("exception row {}: invalid allowance", index + 2))?;
        if count == 0 || result.contains_key(&key) {
            return Err(format!(
                "exception row {}: zero or duplicate site",
                index + 2
            ));
        }
        let growth = current
            .get(&key)
            .copied()
            .unwrap_or_default()
            .saturating_sub(baseline.get(&key).copied().unwrap_or_default());
        if count > growth {
            return Err(format!(
                "exception {}: allowance {count} has no matching growth at {}:{} {}",
                fields[0], key.1, key.2, key.3
            ));
        }
        result.insert(key, count);
    }
    Ok(result)
}

fn rises(baseline: &Inventory, current: &Inventory, allowed: &Inventory) -> Vec<String> {
    let before = totals(baseline);
    let exceptions = totals(allowed);
    totals(current)
        .into_iter()
        .filter_map(|(key, count)| {
            let limit = before.get(&key).copied().unwrap_or_default();
            let allowance = exceptions.get(&key).copied().unwrap_or_default();
            (count > limit + allowance).then(|| {
                format!(
                    "{} {} rose {limit} -> {count}; exact-site exceptions allow {allowance}",
                    key.0, key.1
                )
            })
        })
        .collect()
}

pub fn entry(arguments: &[String]) -> ExitCode {
    let root = crate::compiler::routing::root();
    let result = (|| {
        if let [flag, directory] = arguments {
            if flag == "--inventory" {
                print!("{}", write_inventory(&inventory(Path::new(directory))?));
                return Ok(());
            }
        }
        if arguments == ["--help"] || arguments == ["-h"] {
            println!("usage: alchemy check steering-debt [--strict|--tighten|--inventory ROOT]\nS2: count current source sites; main refuses rises without named, dated exact-site exceptions.\n--strict enforces the main ceiling on any branch; --tighten only lowers existing site ceilings after a passing strict check.");
            return Ok(());
        }
        if !arguments.is_empty() && arguments != ["--strict"] && arguments != ["--tighten"] {
            return Err(
                "usage: alchemy check steering-debt [--strict|--tighten|--inventory ROOT]".into(),
            );
        }
        let baseline = parse_inventory(
            &std::fs::read_to_string(root.join(BASELINE)).map_err(|error| error.to_string())?,
        )?;
        let current = inventory(root)?;
        let allowed = allowances(
            &std::fs::read_to_string(root.join(EXCEPTIONS)).map_err(|error| error.to_string())?,
            &baseline,
            &current,
        )?;
        let failures = rises(&baseline, &current, &allowed);
        let current_totals = totals(&current);
        for group in ["common", "tbs", "tla"] {
            for metric in METRICS {
                let count = current_totals
                    .get(&(group.into(), (*metric).into()))
                    .copied()
                    .unwrap_or_default();
                println!("S2 {group} {metric}={count}");
            }
        }
        let strict = !arguments.is_empty() || crate::verify::is_main(root)?;
        if !failures.is_empty() {
            let message = failures.join("\n");
            if strict {
                return Err(message);
            }
            eprintln!("branch debt report:\n{message}");
        }
        if arguments == ["--tighten"] {
            let tightened = baseline
                .into_iter()
                .filter_map(|(key, count)| {
                    let next = count.min(current.get(&key).copied().unwrap_or_default());
                    (next > 0).then_some((key, next))
                })
                .collect();
            let failures = rises(&tightened, &current, &allowed);
            if !failures.is_empty() {
                return Err(format!(
                    "downward ratchet needs complete exact-site exceptions:\n{}",
                    failures.join("\n")
                ));
            }
            std::fs::write(root.join(BASELINE), write_inventory(&tightened))
                .map_err(|error| error.to_string())?;
        }
        Ok(())
    })();
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    const SOURCE: &str = "games/THE BROKEN SEAL/SRC/A.C";
    fn counts(text: &str) -> Inventory {
        source_inventory(SOURCE, text).unwrap()
    }
    #[test]
    fn owned_macro_and_header_sites_are_counted_once_and_literals_do_not_count() {
        let source = "#define PIN(v) do { /* FAKEMATCH: measured. */ register int r __asm__(\"r0\"); __asm__(\"\"); } while(0)\nvoid F(void) { const char *s = \"FAKEMATCH: asm()\"; PIN(1); PIN(2); }";
        let inventory = counts(source);
        assert_eq!(inventory.values().sum::<usize>(), 3);
        assert!(inventory.keys().all(|key| key.2 == "macro:PIN"));
        assert_eq!(
            totals(&inventory).get(&("tbs".into(), "register_pin".into())),
            Some(&1)
        );
    }
    #[test]
    fn conditional_braces_keep_all_raw_devices_in_their_real_function() {
        let source = r#"void F(void) {
#if EUROPE
    if (ready) { /* FAKEMATCH: first arm. */ }
    else {
#else
    if (!ready) {
        /* FAKEMATCH: inactive arm also counts. */
#endif
#if EXTRA
        /* FAKEMATCH: nested arm. */
        register int priority __asm__("r1");
#else
        register int actor __asm__("r0");
#endif
    }
}
void G(void) { /* FAKEMATCH: separate owner. */ }
"#;
        let inventory = counts(source);
        let get = |function: &str, metric: &str| {
            inventory.get(&("tbs".into(), SOURCE.into(), function.into(), metric.into()))
        };
        assert_eq!(get("F", "fakematch"), Some(&3));
        assert_eq!(get("F", "register_pin"), Some(&2));
        assert_eq!(get("G", "fakematch"), Some(&1));
        assert!(inventory.keys().all(|key| key.2 != "<file>"));
        let before = counts("void F(void) { /* FAKEMATCH: existing. */ }");
        let growth = rises(&before, &inventory, &Inventory::new());
        assert_eq!(growth.len(), 2);
        assert!(growth.iter().any(|row| row.contains("fakematch")));
        assert!(growth.iter().any(|row| row.contains("register_pin")));
    }
    #[test]
    fn inactive_function_definitions_and_macro_devices_keep_their_owners() {
        let source = r#"#if FIRST
void F(void) { /* FAKEMATCH: first definition. */ }
#elif SECOND
void G(void) { /* FAKEMATCH: second definition. */ }
#else
#if NESTED
void H(void) { /* FAKEMATCH: nested inactive definition. */ }
#endif
#endif
#define DEVICE(v) do { /* FAKEMATCH: macro. */ asm(""); } while (0)
"#;
        let inventory = counts(source);
        assert_eq!(inventory.values().sum::<usize>(), 5);
        for function in ["F", "G", "H", "macro:DEVICE"] {
            assert_eq!(
                inventory.get(&(
                    "tbs".into(),
                    SOURCE.into(),
                    function.into(),
                    "fakematch".into()
                )),
                Some(&1)
            );
        }
    }
    #[test]
    fn conditional_views_preserve_preamble_owners_before_macro_bodies() {
        let source = r#"/* FAKEMATCH: macro template preamble. */
#define DEFINE_WRITE(name) \
    void name(void) { /* FAKEMATCH: macro body. */ }
DEFINE_WRITE(Write)
void Next(void) {}
"#;
        let inventory = counts(source);
        assert_eq!(
            inventory.get(&(
                "tbs".into(),
                SOURCE.into(),
                "name".into(),
                "fakematch".into()
            )),
            Some(&1)
        );
        assert_eq!(
            inventory.get(&(
                "tbs".into(),
                SOURCE.into(),
                "macro:DEFINE_WRITE".into(),
                "fakematch".into()
            )),
            Some(&1)
        );
        assert!(inventory.keys().all(|key| key.2 != "Next"));
    }
    #[test]
    fn each_metric_requires_its_own_live_exact_site_allowance() {
        let before = counts("void F(void) {}\n");
        let after = counts("void F(void) { /* FAKEMATCH: measured. */ asm(\"\"); }\n");
        let exception = format!("{EXCEPTION_HEADER}\nx\t2026-10-01\tPascal\ttbs\t{SOURCE}\tF\tfakematch\t1\tExisting measured workaround is now tagged.\n");
        let allowed = allowances(&exception, &before, &after).unwrap();
        assert_eq!(rises(&before, &after, &allowed).len(), 1);
        assert!(rises(&before, &after, &allowed)[0].contains("inline_asm"));
        assert!(allowances(&exception.replace("\tF\t", "\tOther\t"), &before, &after).is_err());
        assert!(allowances(&exception, &after, &after).is_err());
    }
    #[test]
    fn malformed_dates_duplicate_records_and_wildcards_cannot_grant_growth() {
        assert!(!date_valid("2026-02-29"));
        assert!(date_valid("2028-02-29"));
        let after = counts("void F(void) { /* FAKEMATCH: measured. */ }\n");
        let row = format!("x\t2026-10-01\tPascal\ttbs\t{SOURCE}\tF\tfakematch\t1\tMeasured.\n");
        for text in [
            format!("{EXCEPTION_HEADER}\n{row}{row}"),
            format!(
                "{EXCEPTION_HEADER}\n{}",
                row.replace("2026-10-01", "2026-02-30")
            ),
            format!("{EXCEPTION_HEADER}\n{}", row.replace("\tF\t", "\t*\t")),
        ] {
            assert!(allowances(&text, &Inventory::new(), &after).is_err());
        }
    }
    #[test]
    fn baseline_round_trip_preserves_source_ownership() {
        let inventory = counts("/* FAKEMATCH: global. */\nregister int r asm(\"r0\");\nvoid F(void) { /* FAKEMATCH: body. */ asm(\"\"); }");
        assert_eq!(
            parse_inventory(&write_inventory(&inventory)).unwrap(),
            inventory
        );
        assert!(parse_inventory(&write_inventory(&inventory).replace("tbs\t", "other\t")).is_err());
    }
}
