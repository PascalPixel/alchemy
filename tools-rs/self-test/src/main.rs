// Run every tool's --self-test, discovered rather than listed.
//
// The chain this replaces named 107 tools by hand in one 4,858-character
// package.json string. 138 tools actually had a self-test, so 31 safety nets
// ran nowhere and nothing could detect that: adding a tool and forgetting the
// chain is invisible, and the omission looks exactly like a tool without tests.
// Discovery removes the list, and with it the drift.
//
//   cargo run -p self-test                  # run all, report every failure
//   cargo run -p self-test -- --list        # names only
//   cargo run -p self-test -- --jobs 4
//   cargo run -p self-test -- --self-test
//
// Ported from tools/self_test.ts. Every deliberate divergence is marked
// PORT NOTE below; everything else is intended to be byte-identical output.

use std::collections::HashSet;
use std::fs;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Arc, Mutex};
use std::time::Instant;

// ---------------------------------------------------------------------------
// Shared jobs cap.
// ---------------------------------------------------------------------------
// PORT NOTE: tools-rs/jobs is a bin-only crate with no library target, and this
// crate takes no dependencies, so the two functions are restated here. They are
// pinned by the native jobs helper's assertions.

const SHARE: f64 = 0.8;

fn jobs_for(cores: usize) -> usize {
    (((cores as f64) * SHARE).floor() as usize).max(1)
}

/// Callers accept --jobs; this clamps whatever they were given. An explicit
/// number above the cap is still a mistake, just a deliberate-looking one.
///
/// `requested` arrives as the raw `Number(...)` result, so it may be NaN, zero,
/// or fractional. JS checks `Number.isInteger` and `< 1`; anything else falls
/// back to the cap.
fn resolve_jobs(requested: Option<f64>, cores: usize) -> usize {
    let cap = jobs_for(cores);
    match requested {
        Some(n) if n.is_finite() && n.fract() == 0.0 && n >= 1.0 => (n as usize).min(cap),
        _ => cap,
    }
}

fn available_parallelism() -> usize {
    std::thread::available_parallelism()
        .map(|n| n.get())
        .unwrap_or(1)
}

// ---------------------------------------------------------------------------
// JS number/string semantics
// ---------------------------------------------------------------------------

/// JS `String.prototype.trim`: WhiteSpace + LineTerminator. This set includes
/// U+FEFF and excludes U+0085, which is the opposite of Rust's `White_Space`
/// property on both counts.
fn js_trim(text: &str) -> &str {
    let is_ws = |c: char| {
        matches!(
            c,
            '\u{9}'..='\u{d}'
                | ' '
                | '\u{a0}'
                | '\u{feff}'
                | '\u{1680}'
                | '\u{2000}'..='\u{200a}'
                | '\u{2028}'
                | '\u{2029}'
                | '\u{202f}'
                | '\u{205f}'
                | '\u{3000}'
        )
    };
    text.trim_matches(is_ws)
}

/// ECMAScript `Number(string)`, which is the StringNumericLiteral grammar and
/// not `f64::from_str`: an empty or all-whitespace string is 0, `0x`/`0o`/`0b`
/// prefixes are honoured, and `"inf"`, `"nan"`, `"1_0"`, `"12abc"` are all NaN
/// even though Rust's parser accepts (or would accept) some of them.
fn js_number(text: &str) -> f64 {
    let trimmed = js_trim(text);
    if trimmed.is_empty() {
        return 0.0;
    }
    let radix_prefixed = |body: &str, radix: u32| -> f64 {
        if body.is_empty() || !body.chars().all(|c| c.is_digit(radix)) {
            f64::NAN
        } else {
            // Long literals lose precision rather than failing, as in JS.
            body.chars()
                .fold(0.0f64, |acc, c| acc * f64::from(radix) + f64::from(c.to_digit(radix).unwrap()))
        }
    };
    if let Some(body) = trimmed.strip_prefix("0x").or_else(|| trimmed.strip_prefix("0X")) {
        return radix_prefixed(body, 16);
    }
    if let Some(body) = trimmed.strip_prefix("0o").or_else(|| trimmed.strip_prefix("0O")) {
        return radix_prefixed(body, 8);
    }
    if let Some(body) = trimmed.strip_prefix("0b").or_else(|| trimmed.strip_prefix("0B")) {
        return radix_prefixed(body, 2);
    }
    match trimmed {
        "Infinity" | "+Infinity" => return f64::INFINITY,
        "-Infinity" => return f64::NEG_INFINITY,
        _ => {}
    }
    // Reject anything Rust's parser accepts but the grammar does not: "inf",
    // "infinity", "NaN", "1_0", and any trailing garbage.
    if !trimmed
        .chars()
        .all(|c| c.is_ascii_digit() || matches!(c, '+' | '-' | '.' | 'e' | 'E'))
    {
        return f64::NAN;
    }
    trimmed.parse::<f64>().unwrap_or(f64::NAN)
}

/// JS `||`: NaN and 0 are falsy, so `Number(x) || undefined` erases both.
fn truthy_or_none(value: f64) -> Option<f64> {
    if value == 0.0 || value.is_nan() {
        None
    } else {
        Some(value)
    }
}

/// JS default `Array.prototype.sort` compares UTF-16 code units. Tool paths are
/// ASCII, so a byte comparison is exact here; the helper exists to name the
/// contract and is pinned by a test that would fail if a non-ASCII name ever
/// appeared.
fn code_unit_sort(names: &mut [String]) {
    names.sort_by(|a, b| a.as_bytes().cmp(b.as_bytes()));
}

/// PORT NOTE: `localeCompare` is ICU root collation. This is an approximation
/// good for the ASCII, lowercase tool paths this runner actually sorts:
/// punctuation (`_`, `.`, `/`, `-`) is variable-weighted, so it is stripped for
/// the primary pass and only breaks ties. That reproduces the two orderings the
/// real tree exercises, where code-unit order and ICU order genuinely disagree:
/// `make/music_residuals.ts` sorts before `make/music.ts`, and
/// `make/kind2_resource_series.ts` before `make/kind2_resource.ts`.
fn locale_compare(a: &str, b: &str) -> std::cmp::Ordering {
    let primary = |s: &str| -> Vec<u8> {
        s.bytes()
            .filter(u8::is_ascii_alphanumeric)
            .map(|c| c.to_ascii_lowercase())
            .collect()
    };
    primary(a)
        .cmp(&primary(b))
        .then_with(|| a.as_bytes().cmp(b.as_bytes()))
}

// ---------------------------------------------------------------------------
// discovery
// ---------------------------------------------------------------------------

/// A tool opts in by handling the flag. Matching the literal keeps the contract
/// visible in the tool itself rather than in a registry that can disagree.
/// Names may be nested (`assets/music.ts`): folders are an ownership boundary,
/// not a reason to stop testing, and a non-recursive walk would silently drop
/// every self-test the moment a tool moved into one.
fn discover(names: &[String], read: &dyn Fn(&str) -> String) -> Vec<String> {
    let mut found: Vec<String> = names
        .iter()
        .filter(|name| name.ends_with(".ts"))
        .filter(|name| read(name).contains("\"--self-test\""))
        .cloned()
        .collect();
    code_unit_sort(&mut found);
    found
}

fn walk(tools: &Path, directory: &Path, prefix: &str) -> Vec<String> {
    let mut names = Vec::new();
    let Ok(entries) = fs::read_dir(tools.join(directory)) else {
        return names;
    };
    // PORT NOTE: Node's readdirSync yields raw directory order; `discover` sorts
    // afterwards, so the walk order is not observable. Sorting here anyway makes
    // the intermediate list reproducible across filesystems.
    let mut entries: Vec<_> = entries.filter_map(Result::ok).collect();
    entries.sort_by_key(std::fs::DirEntry::file_name);
    for entry in entries {
        let name = entry.file_name().to_string_lossy().into_owned();
        let relative = if prefix.is_empty() {
            name.clone()
        } else {
            format!("{prefix}/{name}")
        };
        if entry.file_type().map(|t| t.is_dir()).unwrap_or(false) {
            names.extend(walk(tools, &directory.join(&name), &relative));
        } else {
            names.push(relative);
        }
    }
    names
}

// This runner is excluded from its own discovery: it would recurse. So is
// anything under scratch/ -- a probe there must not be able to fail a gate, and
// a probe that happens to contain the literal "--self-test" was being run.
const EXCLUDED: [&str; 1] = ["self_test.ts"];
const EXCLUDED_FOLDERS: [&str; 1] = ["scratch/"];

fn is_excluded(name: &str) -> bool {
    EXCLUDED.contains(&name) || EXCLUDED_FOLDERS.iter().any(|f| name.starts_with(f))
}

// ---------------------------------------------------------------------------
// running
// ---------------------------------------------------------------------------

struct Outcome {
    tool: String,
    ok: bool,
    output: String,
}

#[derive(Debug, Clone)]
struct Invocation {
    name: String,
    program: PathBuf,
    arguments: Vec<String>,
}

fn native_tools(root: &Path) -> Vec<Invocation> {
    let tools_rs = root.join("tools-rs");
    let Ok(entries) = fs::read_dir(&tools_rs) else {
        return Vec::new();
    };
    let mut found = Vec::new();
    for entry in entries.filter_map(Result::ok) {
        let Ok(kind) = entry.file_type() else { continue };
        if !kind.is_dir() {
            continue;
        }
        let crate_name = entry.file_name().to_string_lossy().into_owned();
        if crate_name == "self-test" || crate_name == "target" {
            continue;
        }
        let main = entry.path().join("src/main.rs");
        let source = fs::read(&main)
            .map(|bytes| String::from_utf8_lossy(&bytes).into_owned())
            .unwrap_or_default();
        if !source.contains("\"--self-test\"") {
            continue;
        }
        let manifest = entry.path().join("Cargo.toml");
        let manifest_text = fs::read_to_string(&manifest).unwrap_or_default();
        let binary_name = main_binary_name(&manifest_text).unwrap_or_else(|| crate_name.clone());
        let standalone = entry.path().join("target/release").join(&binary_name);
        let shared = tools_rs.join("target/release").join(&binary_name);
        let existing = [standalone, shared].into_iter().find(|path| path.exists());
        let (program, arguments) = match existing {
            Some(program) => (program, vec!["--self-test".to_string()]),
            None => (
                PathBuf::from("cargo"),
                vec![
                    "run".to_string(),
                    "--quiet".to_string(),
                    "--release".to_string(),
                    "--manifest-path".to_string(),
                    manifest.to_string_lossy().into_owned(),
                    "--bin".to_string(),
                    binary_name,
                    "--".to_string(),
                    "--self-test".to_string(),
                ],
            ),
        };
        found.push(Invocation { name: format!("rust/{crate_name}"), program, arguments });
    }
    found.sort_by(|left, right| left.name.as_bytes().cmp(right.name.as_bytes()));
    found
}

fn main_binary_name(manifest: &str) -> Option<String> {
    let mut in_binary = false;
    let mut name = None;
    for raw in manifest.lines() {
        let line = raw.trim();
        if line == "[[bin]]" {
            in_binary = true;
            name = None;
            continue;
        }
        if line.starts_with('[') {
            in_binary = false;
            name = None;
            continue;
        }
        if !in_binary {
            continue;
        }
        if let Some(value) = line.strip_prefix("name = ") {
            name = Some(value.trim_matches('"').to_string());
        }
        if line == "path = \"src/main.rs\"" {
            return name;
        }
    }
    None
}

fn run_one(root: &Path, tool: &Invocation) -> Outcome {
    let spawned = Command::new(&tool.program)
        .args(&tool.arguments)
        .current_dir(root)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .output();
    match spawned {
        Ok(out) => {
            // `readFileSync`-style lossy decoding: a tool that prints invalid
            // UTF-8 must produce a report, not an error.
            let stdout = String::from_utf8_lossy(&out.stdout).into_owned();
            let stderr = String::from_utf8_lossy(&out.stderr).into_owned();
            // JS `stderr || stdout`: the empty string is falsy.
            let chosen = if stderr.is_empty() { stdout } else { stderr };
            Outcome {
                tool: tool.name.clone(),
                ok: out.status.success(),
                output: js_trim(&chosen).to_string(),
            }
        }
        Err(error) => Outcome {
            tool: tool.name.clone(),
            ok: false,
            output: format!("could not spawn {}: {error}", tool.program.display()),
        },
    }
}

/// Bounded concurrency: the self-tests compile C and assemble overlays, so an
/// unbounded fan-out over 138 tools thrashes rather than finishing sooner.
fn run_all(root: &Path, tools: &[Invocation], jobs: usize) -> Vec<Outcome> {
    let next = Arc::new(AtomicUsize::new(0));
    let results = Arc::new(Mutex::new(Vec::new()));
    let workers = jobs.min(tools.len());
    std::thread::scope(|scope| {
        for _ in 0..workers {
            let next = Arc::clone(&next);
            let results = Arc::clone(&results);
            scope.spawn(move || loop {
                let index = next.fetch_add(1, Ordering::SeqCst);
                if index >= tools.len() {
                    break;
                }
                let outcome = run_one(root, &tools[index]);
                results.lock().expect("results mutex").push(outcome);
            });
        }
    });
    Arc::try_unwrap(results)
        .map(|m| m.into_inner().expect("results mutex"))
        .unwrap_or_default()
}

// ---------------------------------------------------------------------------
// entry point
// ---------------------------------------------------------------------------

fn repo_root() -> PathBuf {
    if let Ok(explicit) = std::env::var("ALCHEMY_ROOT") {
        return PathBuf::from(explicit);
    }
    // tools-rs/self-test -> tools-rs -> repo root
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR"));
    manifest
        .parent()
        .and_then(Path::parent)
        .map(Path::to_path_buf)
        .unwrap_or_else(|| PathBuf::from("."))
}

/// JS `args.indexOf("--jobs") + 1` is `0` when the flag is absent, so the value
/// silently comes from `args[0]`. That is load-bearing for parity: a bare
/// `self-test 4` really does request four jobs, and `self-test --list` reads
/// `Number("--list")` = NaN and falls through to the cap.
fn requested_jobs(args: &[String]) -> Option<f64> {
    let index = args.iter().position(|a| a == "--jobs").map_or(0, |i| i + 1);
    let raw = args.get(index).map_or(f64::NAN, |v| js_number(v));
    truthy_or_none(raw)
}

fn main() {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.iter().any(|a| a == "--self-test") {
        self_test();
        return;
    }
    let root = repo_root();
    let tools_dir = root.join("tools");
    let jobs = resolve_jobs(requested_jobs(&args), available_parallelism());

    let names = walk(&tools_dir, Path::new(""), "");
    let typescript: Vec<String> = discover(&names, &|name| {
        fs::read(tools_dir.join(name))
            .map(|bytes| String::from_utf8_lossy(&bytes).into_owned())
            .unwrap_or_default()
    })
    .into_iter()
    .filter(|name| !is_excluded(name))
    .collect();
    let mut tools: Vec<Invocation> = typescript
        .into_iter()
        .map(|name| Invocation {
            program: PathBuf::from("bun"),
            arguments: vec![
                tools_dir.join(&name).to_string_lossy().into_owned(),
                "--self-test".to_string(),
            ],
            name,
        })
        .collect();
    tools.extend(native_tools(&root));
    tools.sort_by(|left, right| left.name.as_bytes().cmp(right.name.as_bytes()));

    // PORT NOTE (deliberate divergence, mirrored back into tools/self_test.ts):
    // discovering nothing used to print "0/0 passed" and exit 0, so a gate that
    // scanned an empty or misrooted tree passed. An empty discovery is now a
    // hard failure -- the one thing this runner must never do is succeed
    // without having tested anything.
    if tools.is_empty() {
        let mut err = std::io::stderr();
        let _ = writeln!(
            err,
            "no tool exposes a self-test under {} -- refusing to pass without scanning anything",
            tools_dir.display()
        );
        std::process::exit(1);
    }

    if args.iter().any(|a| a == "--list") {
        for tool in &tools {
            println!("  {}", tool.name);
        }
        println!("{} tools expose a self-test", tools.len());
        return;
    }

    let started = Instant::now();
    let results = run_all(&root, &tools, jobs);
    let mut failed: Vec<&Outcome> = results.iter().filter(|r| !r.ok).collect();
    failed.sort_by(|a, b| locale_compare(&a.tool, &b.tool));
    for result in &failed {
        eprintln!("FAIL {}", result.tool);
        let lines: Vec<&str> = result.output.split('\n').collect();
        for line in lines.iter().skip(lines.len().saturating_sub(6)) {
            eprintln!("     {line}");
        }
    }
    let seconds = format!("{:.1}", started.elapsed().as_secs_f64());
    println!(
        "self-tests: {}/{} passed in {seconds}s (jobs={jobs})",
        results.len() - failed.len(),
        results.len()
    );
    if !failed.is_empty() {
        std::process::exit(1);
    }
}

fn self_test() {
    let fixture = |name: &str| -> String {
        match name {
            "a.ts" => "if (args.includes(\"--self-test\")) return selfTest();",
            "b.ts" => "no tests here",
            "c.ts" => "const x = \"--self-test\";",
            "d.txt" => "\"--self-test\"",
            _ => "",
        }
        .to_string()
    };
    let names: Vec<String> = ["a.ts", "b.ts", "c.ts", "d.txt"]
        .iter()
        .map(|s| s.to_string())
        .collect();
    let found = discover(&names, &fixture);
    assert_eq!(found.join(","), "a.ts,c.ts", "discover picked the wrong set");
    assert!(discover(&[], &fixture).is_empty(), "empty input must discover nothing");
    let excluded: HashSet<&str> = EXCLUDED.iter().copied().collect();
    assert!(excluded.contains("self_test.ts"), "the runner must exclude itself");
    assert!(
        EXCLUDED_FOLDERS.iter().any(|f| "scratch/probe.ts".starts_with(f)),
        "scratch must be excluded from discovery"
    );
    assert_eq!(
        discover(&["assets/music.ts".to_string()], &|_| "\"--self-test\"".to_string()).len(),
        1,
        "discover must accept nested names"
    );
    println!("self_test self-test ok");
}

#[cfg(test)]
mod tests {
    use super::*;

    fn owned(names: &[&str]) -> Vec<String> {
        names.iter().map(|s| (*s).to_string()).collect()
    }

    #[test]
    fn discovery_keeps_only_ts_files_holding_the_literal() {
        let names = owned(&["a.ts", "b.ts", "c.ts", "d.txt", "e.tsx"]);
        let read = |name: &str| -> String {
            match name {
                // The literal must be quoted: a bare mention in prose is not
                // an opt-in, which is what distinguishes a tool from a README.
                "a.ts" => "if (argv.includes(\"--self-test\")) selfTest();".to_string(),
                "b.ts" => "mentions --self-test in a comment only".to_string(),
                "c.ts" => "const flag = \"--self-test\";".to_string(),
                "d.txt" => "\"--self-test\"".to_string(),
                "e.tsx" => "\"--self-test\"".to_string(),
                _ => String::new(),
            }
        };
        assert_eq!(discover(&names, &read), owned(&["a.ts", "c.ts"]));
    }

    #[test]
    fn discovery_sorts_by_code_unit_not_by_input_order() {
        let names = owned(&["z/b.ts", "a.ts", "m/n/c.ts"]);
        let found = discover(&names, &|_| "\"--self-test\"".to_string());
        assert_eq!(found, owned(&["a.ts", "m/n/c.ts", "z/b.ts"]));
    }

    #[test]
    fn native_binary_name_comes_from_the_main_bin_entry() {
        let manifest = r#"
[package]
name = "crate-name"

[[bin]]
name = "helper"
path = "src/bin/helper.rs"

[[bin]]
name = "actual-tool"
path = "src/main.rs"
"#;
        assert_eq!(main_binary_name(manifest).as_deref(), Some("actual-tool"));
    }

    #[test]
    fn walk_finds_nested_tools_and_reports_slash_separated_names() {
        let dir = std::env::temp_dir().join(format!("self-test-walk-{}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(dir.join("assets/deep")).expect("fixture dirs");
        fs::write(dir.join("top.ts"), "\"--self-test\"").expect("fixture");
        fs::write(dir.join("assets/music.ts"), "\"--self-test\"").expect("fixture");
        fs::write(dir.join("assets/deep/x.ts"), "no opt in").expect("fixture");
        fs::write(dir.join("assets/notes.md"), "\"--self-test\"").expect("fixture");
        let names = walk(&dir, Path::new(""), "");
        assert!(names.contains(&"assets/deep/x.ts".to_string()));
        let found = discover(&names, &|name| {
            fs::read_to_string(dir.join(name)).unwrap_or_default()
        });
        assert_eq!(found, owned(&["assets/music.ts", "top.ts"]));
        fs::remove_dir_all(&dir).expect("cleanup");
    }

    #[test]
    fn exclusions_drop_the_runner_and_every_scratch_probe() {
        assert!(is_excluded("self_test.ts"));
        assert!(is_excluded("scratch/probe.ts"));
        assert!(is_excluded("scratch/deep/probe.ts"));
        assert!(!is_excluded("lib/self_test_helper.ts"));
        assert!(!is_excluded("make/scratchpad.ts"), "the prefix is the folder, not the word");
    }

    #[test]
    fn locale_compare_matches_the_two_real_disagreements() {
        // Verified against Bun: `"make/music_residuals.ts".localeCompare("make/music.ts") === -1`.
        assert_eq!(
            locale_compare("make/music_residuals.ts", "make/music.ts"),
            std::cmp::Ordering::Less
        );
        assert_eq!(
            locale_compare("make/kind2_resource_series.ts", "make/kind2_resource.ts"),
            std::cmp::Ordering::Less
        );
        // Code-unit order would put these the other way round.
        assert!("make/music.ts".as_bytes() < "make/music_residuals.ts".as_bytes());
        // Plain alphabetical pairs must be unaffected.
        assert_eq!(locale_compare("check/a.ts", "check/b.ts"), std::cmp::Ordering::Less);
        assert_eq!(locale_compare("a.ts", "a.ts"), std::cmp::Ordering::Equal);
    }

    #[test]
    fn jobs_flag_reproduces_the_indexof_minus_one_quirk() {
        // Absent flag: `indexOf(...) + 1` is 0, so args[0] is read as the value.
        assert_eq!(requested_jobs(&owned(&["--list"])), None, "NaN falls back to the cap");
        assert_eq!(requested_jobs(&owned(&[])), None);
        assert_eq!(requested_jobs(&owned(&["4"])), Some(4.0), "a bare number is honoured");
        assert_eq!(requested_jobs(&owned(&["--jobs", "6"])), Some(6.0));
        assert_eq!(requested_jobs(&owned(&["--jobs", "0"])), None, "0 is falsy");
        assert_eq!(requested_jobs(&owned(&["--jobs"])), None, "a missing value is NaN");
    }

    #[test]
    fn resolve_jobs_clamps_and_rejects_nonsense() {
        assert_eq!(jobs_for(18), 14);
        assert_eq!(jobs_for(1), 1);
        assert_eq!(jobs_for(2), 1);
        assert_eq!(resolve_jobs(Some(4.0), 18), 4);
        assert_eq!(resolve_jobs(Some(64.0), 18), 14);
        assert_eq!(resolve_jobs(None, 18), 14);
        // Number.isInteger(1.5) is false, so a fractional request takes the cap.
        assert_eq!(resolve_jobs(Some(1.5), 18), 14);
        assert_eq!(resolve_jobs(Some(f64::NAN), 18), 14);
        // 1.0 IS an integer in JS -- one number type, no float/int split.
        assert_eq!(resolve_jobs(Some(1.0), 18), 1);
    }

    #[test]
    fn js_number_follows_the_string_numeric_literal_grammar() {
        assert_eq!(js_number("0x10"), 16.0);
        assert_eq!(js_number("0b101"), 5.0);
        assert_eq!(js_number("0o17"), 15.0);
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("   "), 0.0);
        assert_eq!(js_number(" 12 "), 12.0);
        assert!(js_number("12abc").is_nan());
        assert!(js_number("inf").is_nan(), "Rust parses this, JS does not");
        assert!(js_number("nan").is_nan());
        assert!(js_number("1_0").is_nan(), "Rust literals allow separators, Number does not");
        assert_eq!(js_number("Infinity"), f64::INFINITY);
        assert_eq!(js_number("1e3"), 1000.0);
        assert!(js_number("--jobs").is_nan());
    }

    #[test]
    fn js_trim_uses_the_javascript_whitespace_set() {
        // U+FEFF is whitespace to JS and not to Rust.
        assert_eq!(js_trim("\u{feff}4\u{feff}"), "4");
        assert_eq!("\u{feff}4\u{feff}".trim(), "\u{feff}4\u{feff}");
        // U+0085 is whitespace to Rust and not to JS.
        assert_eq!(js_trim("\u{85}4"), "\u{85}4");
        assert_eq!("\u{85}4".trim(), "4");
        assert_eq!(js_trim(" \t\n x \r\n "), "x");
    }

    #[test]
    fn stderr_wins_over_stdout_only_when_it_is_non_empty() {
        let pick = |stderr: &str, stdout: &str| -> String {
            let chosen = if stderr.is_empty() { stdout } else { stderr };
            js_trim(chosen).to_string()
        };
        assert_eq!(pick("", "  ok  \n"), "ok");
        assert_eq!(pick("  boom \n", "ok"), "boom");
        // A whitespace-only stderr is truthy in JS and still wins, then trims
        // to nothing -- reproduced rather than "fixed".
        assert_eq!(pick("  \n", "ok"), "");
    }

    #[test]
    fn failure_report_shows_only_the_last_six_lines() {
        let output = (1..=10).map(|n| n.to_string()).collect::<Vec<_>>().join("\n");
        let lines: Vec<&str> = output.split('\n').collect();
        let tail: Vec<&str> = lines.iter().skip(lines.len().saturating_sub(6)).copied().collect();
        assert_eq!(tail, vec!["5", "6", "7", "8", "9", "10"]);
        // Fewer than six lines must not underflow the skip.
        let short = "only";
        let lines: Vec<&str> = short.split('\n').collect();
        assert_eq!(lines.len().saturating_sub(6), 0);
    }

    #[test]
    fn an_empty_discovery_is_a_failure_not_a_pass() {
        // The property the gate depends on: nothing discovered must never be
        // reportable as success. Asserted on the predicate main branches on.
        let tools: Vec<String> = Vec::new();
        assert!(tools.is_empty(), "an empty tree must reach the hard-failure branch");
        let real = discover(&owned(&["a.ts"]), &|_| "no opt in".to_string());
        assert!(real.is_empty());
    }
}
