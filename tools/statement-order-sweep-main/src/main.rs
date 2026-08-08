// Permute independent top-level statements of one MAIN-IMAGE draft and keep the
// ordering that matches the reference best.
//
//   statement-order-sweep-main <draft.c> [--flags -fa,-fb] [--out FILE]
//                                        [--max-run N] [--jobs N]
//
// Why this exists: HANDOVER.md §4 records that store order moves register
// allocation — allocno priority is 2*refs/live_length, and a parameter's store
// position sets its live length, so permuting independent store groups is the
// general fix when two equal-priority pseudos land in the wrong registers. It
// notes 24 orderings were tried on one function and two of them gave zero where
// the obvious one gave 15. That search was done by hand. §5 adds that a
// permuter's single win in 65,543 candidates was one statement swap.
//
// Nothing automated that lever, so every "register-identity-only swap" park (§6
// lists the class as real) was triaged without it having been swept. This sweeps
// it exhaustively for small runs, which is where the entry-block parameter saves
// live.
//
// Only *independent* statements are permuted, so the transformation preserves
// semantics: a candidate ordering is generated only when no statement in the run
// reads a name another writes, and any statement touching memory or calling a
// function acts as a barrier against other such statements. Plain declarations
// are not memory accesses: the `*` in `s16 *cursor;` is a declarator. Keeping it
// behind the memory barrier hid declaration-order register-allocation wins.
//
// ---------------------------------------------------------------------------
// PORT NOTE — this is a port of tools/compiler/statement_order_sweep_main.ts.
//
// Inlined from other modules: nothing. Candidate scoring uses the native
// `candidate-show` library directly, avoiding one process launch per ordering.
//
// Deliberate behavioural differences, all of them outside the normal path:
//
//  1. Errors. The TypeScript throws at top level, so Bun prints a JavaScript
//     stack trace to stderr and exits 1. This prints `error: <same message>` to
//     stderr and exits 1. The message text and the exit code match; the stack
//     trace does not exist here.
//  2. `--jobs 0` (or any value below 1). The TypeScript loops forever: it slices
//     an empty batch and never advances `at`. This rejects the value with an
//     error instead of hanging. `--jobs NaN` matches the TypeScript exactly: the
//     batch loop runs zero times and no candidate is scored.
//  3. Non-integral `--max-run` / `--jobs` are truncated where JavaScript's array
//     `slice` would truncate them. NaN is preserved end to end, including the
//     empty run it produces.
//
// Everything on the normal path — stdout text, the order in which candidates are
// generated and scored, the files written, the scratch tree, the exit codes — is
// character-for-character identical. See src/lib.rs for the character-index and
// JS-regex-semantics notes.
// ---------------------------------------------------------------------------

use statement_order_sweep_main::{
    function_body, independent_runs, is_plain_declaration, permutations, top_level_statements,
    Piece,
};

use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicUsize, Ordering};

const USAGE: &str =
    "usage: statement-order-sweep-main <draft.c> [--flags -fa,-fb] [--out FILE] [--max-run N] [--jobs N]";

fn die(message: &str) -> ! {
    eprintln!("error: {message}");
    std::process::exit(1);
}

/// JavaScript `Number(string)` restricted to what a command line can carry:
/// leading/trailing whitespace is ignored, an empty string is zero, and
/// anything unparseable is NaN.
fn js_number(text: &str) -> f64 {
    let trimmed = statement_order_sweep_main::js_trim(text);
    if trimmed.is_empty() {
        return 0.0;
    }
    trimmed.parse::<f64>().unwrap_or(f64::NAN)
}

/// JavaScript number-to-string for the values this tool prints.
fn js_number_text(value: f64) -> String {
    if value.is_nan() {
        "NaN".to_string()
    } else if value == value.trunc() && value.abs() < 1e21 {
        format!("{}", value as i64)
    } else {
        format!("{value}")
    }
}

struct Options {
    draft: String,
    out: String,
    flags: String,
    max_run: f64,
    jobs: f64,
}

fn parse_arguments(argv: &[String]) -> Options {
    let mut options = Options {
        draft: String::new(),
        out: String::new(),
        flags: String::new(),
        max_run: 6.0,
        jobs: 12.0,
    };
    let mut rest: Vec<String> = Vec::new();
    let mut index = 0;
    // `argv[++index]` on a missing value yields `undefined`; the string options
    // then become the literal "undefined" in JavaScript. Reaching that state
    // needs a trailing bare flag, which is a usage error either way, so it is
    // reported instead.
    while index < argv.len() {
        let argument = argv[index].clone();
        let value = |index: &mut usize| -> String {
            *index += 1;
            match argv.get(*index) {
                Some(value) => value.clone(),
                None => die(&format!("missing value for {argument}")),
            }
        };
        match argv[index].as_str() {
            "--flags" => options.flags = value(&mut index),
            "--out" => options.out = value(&mut index),
            "--max-run" => options.max_run = js_number(&value(&mut index)),
            "--jobs" => options.jobs = js_number(&value(&mut index)),
            "-h" | "--help" => {
                println!("{USAGE}");
                std::process::exit(0);
            }
            _ => rest.push(argv[index].clone()),
        }
        index += 1;
    }
    if rest.len() != 1 {
        die("usage: statement-order-sweep-main <draft.c>");
    }
    options.draft = rest[0].clone();
    if options.out.is_empty() {
        options.out = if options.draft.ends_with(".c") {
            format!("{}ordered.c", &options.draft[..options.draft.len() - 1])
        } else {
            options.draft.clone()
        };
    }
    options
}

/// `basename(path, ".c")` with Node's posix semantics.
fn stem_of(path: &str) -> String {
    let base = match path.rfind('/') {
        Some(at) => &path[at + 1..],
        None => path,
    };
    if base.len() > 2 && base.ends_with(".c") {
        base[..base.len() - 2].to_string()
    } else {
        base.to_string()
    }
}

fn self_test() {
    let declarations = [
        "s16 *cursor;",
        "volatile s32 *cursor;",
        "s32 values[4];",
        "struct Position *position;",
    ];
    for declaration in declarations {
        if !is_plain_declaration(declaration) {
            die(&format!(
                "statement-order self-test rejected declaration: {declaration}"
            ));
        }
    }
    for statement in ["s32 value = 0;", "s32 values[count];", "*cursor = value;"] {
        if is_plain_declaration(statement) {
            die(&format!(
                "statement-order self-test accepted effectful statement: {statement}"
            ));
        }
    }
    let pieces = top_level_statements("\n    s16 *cursor;\n    volatile s32 *other;\n");
    if pieces.len() != 3 || !pieces[0].simple || !pieces[1].simple || pieces[2].text != "\n" {
        die("statement-order self-test did not preserve trailing whitespace");
    }
    let runs = independent_runs(&pieces, 6.0);
    if runs.len() != 1 || runs[0].0 != 0 || runs[0].1 != 1.0 {
        die("statement-order self-test did not expose pointer declarations");
    }
    let nested_parameter = "void Func_08000000(s32 *value, u16 (*table)[2]) {\n    s32 index;\n}\n";
    let characters: Vec<char> = nested_parameter.chars().collect();
    let bounds = function_body(&characters);
    let ok = match bounds {
        Some((start, end)) => {
            characters[start..end].iter().collect::<String>() == "\n    s32 index;\n"
        }
        None => false,
    };
    if !ok {
        die("statement-order self-test did not parse a pointer-to-array parameter");
    }
    println!("self-test=ok");
}

#[derive(Clone, Copy, PartialEq, Eq)]
struct Score {
    diff: i64,
    size: i64,
    reference: i64,
}

/// `/candidate=(\d+) reference=(\d+) differing_halfwords=(\d+)/` — leftmost match.
fn parse_score(text: &str) -> Option<Score> {
    let characters: Vec<char> = text.chars().collect();
    let digits = |at: usize| -> Option<(i64, usize)> {
        let mut cursor = at;
        while cursor < characters.len() && characters[cursor].is_ascii_digit() {
            cursor += 1;
        }
        if cursor == at {
            return None;
        }
        let value: String = characters[at..cursor].iter().collect();
        // `Number("999...")` on an over-long run would lose precision; these are
        // byte counts, so an i64 is always wide enough in practice.
        Some((value.parse::<i64>().ok()?, cursor))
    };
    let literal = |at: usize, word: &str| -> Option<usize> {
        let mut cursor = at;
        for wanted in word.chars() {
            if cursor >= characters.len() || characters[cursor] != wanted {
                return None;
            }
            cursor += 1;
        }
        Some(cursor)
    };
    for start in 0..characters.len() {
        let Some(at) = literal(start, "candidate=") else { continue };
        let Some((size, at)) = digits(at) else { continue };
        let Some(at) = literal(at, " reference=") else { continue };
        let Some((reference, at)) = digits(at) else { continue };
        let Some(at) = literal(at, " differing_halfwords=") else { continue };
        let Some((diff, _)) = digits(at) else { continue };
        return Some(Score { diff, size, reference });
    }
    None
}

struct Sweeper {
    root: PathBuf,
    scratch: PathBuf,
    stem: String,
    flags: String,
    probes: AtomicUsize,
}

impl Sweeper {
    // 採点は candidate_show の一行目に任せる。経路名は必ず <stem>.c にする。
    // 別名にすると §7 の経路トラップでフラグが素の設定に落ちる。
    fn score(&self, source: &str, slot: usize) -> Option<Score> {
        let directory = self.scratch.join(format!("s{slot}"));
        if let Err(error) = std::fs::create_dir_all(&directory) {
            die(&format!("{}: {error}", directory.display()));
        }
        let path = directory.join(format!("{}.c", self.stem));
        if let Err(error) = std::fs::write(&path, source) {
            die(&format!("{}: {error}", path.display()));
        }
        let mut arguments = vec![
            path.to_string_lossy().into_owned(),
            "--work".to_string(),
            directory.join("w").to_string_lossy().into_owned(),
        ];
        if !self.flags.is_empty() {
            arguments.push("--flags".to_string());
            arguments.push(self.flags.clone());
        }
        let options = match candidate_show::options_of(&self.root, &arguments) {
            Ok(candidate_show::ParseOutcome::Options(options)) => options,
            Ok(candidate_show::ParseOutcome::Help) => return None,
            Err(_) => return None,
        };
        let rendered = candidate_show::render(&self.root, &options).ok();
        self.probes.fetch_add(1, Ordering::SeqCst);
        rendered.map(|output| Score {
            diff: output.differing_halfwords as i64,
            size: output.candidate_length as i64,
            reference: output.reference_length as i64,
        })
    }
}

fn render(source: &[char], bounds: (usize, usize), current: &[Piece]) -> String {
    let mut out: String = source[..bounds.0].iter().collect();
    for piece in current {
        out.push_str(&piece.text);
    }
    out.extend(source[bounds.1..].iter());
    out
}

/// `Array.prototype.slice`'s end index for the run bound `to + 1`.
fn slice_end(to: f64, length: usize) -> usize {
    if to.is_nan() {
        return 0;
    }
    let end = (to + 1.0).trunc();
    if end <= 0.0 {
        0
    } else if end >= length as f64 {
        length
    } else {
        end as usize
    }
}

fn main() {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    if argv.len() == 1 && argv[0] == "--self-test" {
        self_test();
        std::process::exit(0);
    }

    let options = parse_arguments(&argv);
    let root = Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf();

    let stem = stem_of(&options.draft);
    let bytes = match std::fs::read(&options.draft) {
        Ok(bytes) => bytes,
        Err(error) => die(&format!("{}: {error}", options.draft)),
    };
    // `readFileSync(path, "utf8")` replaces invalid bytes with U+FFFD.
    let original: Vec<char> = String::from_utf8_lossy(&bytes).chars().collect();
    let Some(bounds) = function_body(&original) else {
        die(&format!("no Func_ body found in {}", options.draft));
    };
    let body: String = original[bounds.0..bounds.1].iter().collect();
    let pieces = top_level_statements(&body);

    let scratch = root.join("out/statement-order").join(&stem);
    let _ = std::fs::remove_dir_all(&scratch);
    if let Err(error) = std::fs::create_dir_all(&scratch) {
        die(&format!("{}: {error}", scratch.display()));
    }

    let sweeper = Sweeper {
        root: root.clone(),
        scratch: scratch.clone(),
        stem: stem.clone(),
        flags: options.flags.clone(),
        probes: AtomicUsize::new(0),
    };

    let baseline = match sweeper.score(&original.iter().collect::<String>(), 0) {
        Some(score) => score,
        None => die(&format!("draft does not compile: {}", options.draft)),
    };
    println!(
        "start differing_halfwords={} size={}/{}",
        baseline.diff, baseline.size, baseline.reference
    );

    let jobs = options.jobs;
    if !jobs.is_nan() && jobs < 1.0 {
        die("--jobs must be at least 1");
    }
    let batch = if jobs.is_nan() { 0 } else { jobs.trunc() as usize };

    let mut best_pieces = pieces.clone();
    let mut best = baseline;
    let mut changed = false;
    let mut improved = true;
    while improved {
        improved = false;
        for (from, to) in independent_runs(&best_pieces, options.max_run) {
            let end = slice_end(to, best_pieces.len()).max(from);
            let run: Vec<Piece> = best_pieces[from..end].to_vec();
            let identity: Vec<usize> = (0..run.len()).collect();
            let mut candidates: Vec<Vec<Piece>> = Vec::new();
            for ordering in permutations(&identity) {
                if ordering == identity {
                    continue;
                }
                let mut candidate: Vec<Piece> = best_pieces[..from].to_vec();
                candidate.extend(ordering.iter().map(|at| run[*at].clone()));
                candidate.extend_from_slice(&best_pieces[end..]);
                candidates.push(candidate);
            }
            // 並べ替えは互いに独立なので、まとめて走らせて最良だけ採る。
            let mut at = 0usize;
            while batch > 0 && at < candidates.len() {
                let slice = &candidates[at..(at + batch).min(candidates.len())];
                let scored: Vec<Option<Score>> = std::thread::scope(|scope| {
                    let handles: Vec<_> = slice
                        .iter()
                        .enumerate()
                        .map(|(slot, candidate)| {
                            let sweeper = &sweeper;
                            let original = &original;
                            scope.spawn(move || {
                                sweeper.score(&render(original, bounds, candidate), slot + 1)
                            })
                        })
                        .collect();
                    handles.into_iter().map(|handle| handle.join().unwrap()).collect()
                });
                for which in 0..slice.len() {
                    let Some(result) = scored[which] else { continue };
                    let better = result.diff < best.diff
                        || (result.diff == best.diff
                            && result.size == result.reference
                            && best.size != best.reference);
                    if better {
                        best = result;
                        best_pieces = slice[which].clone();
                        changed = true;
                        improved = true;
                        println!(
                            "  reorder [{}..{}]   differing_halfwords={} size={}/{}",
                            from,
                            js_number_text(to),
                            best.diff,
                            best.size,
                            best.reference
                        );
                    }
                }
                if best.diff == 0 && best.size == best.reference {
                    break;
                }
                at += batch;
            }
            if best.diff == 0 && best.size == best.reference {
                break;
            }
        }
    }

    println!(
        "final differing_halfwords={} size={}/{} probes={}",
        best.diff,
        best.size,
        best.reference,
        sweeper.probes.load(Ordering::SeqCst)
    );
    if changed {
        let rendered = render(&original, bounds, &best_pieces);
        if let Err(error) = std::fs::write(&options.out, rendered) {
            die(&format!("{}: {error}", options.out));
        }
        println!("wrote reordered draft to {}", options.out);
    }
    let _ = std::fs::remove_dir_all(&scratch);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn score_line_is_parsed_leftmost() {
        let score = parse_score("candidate=40 reference=40 differing_halfwords=0\nrest\n").unwrap();
        assert_eq!((score.size, score.reference, score.diff), (40, 40, 0));
        // Embedded in a longer line, exactly as candidate_show prints it.
        let score =
            parse_score("08004760 candidate=52 reference=48 differing_halfwords=7 flags=-O2")
                .unwrap();
        assert_eq!((score.size, score.reference, score.diff), (52, 48, 7));
        assert!(parse_score("candidate=x reference=1 differing_halfwords=1").is_none());
        assert!(parse_score("candidate=1  reference=1 differing_halfwords=1").is_none());
        assert!(parse_score("").is_none());
    }

    #[test]
    fn stem_strips_only_a_real_c_suffix() {
        assert_eq!(stem_of("exact/08002ee4.c"), "08002ee4");
        assert_eq!(stem_of("08002ee4.c"), "08002ee4");
        assert_eq!(stem_of("draft.cpp"), "draft.cpp");
        assert_eq!(stem_of(".c"), ".c");
    }

    #[test]
    fn default_out_path_replaces_a_trailing_dot_c() {
        let options = parse_arguments(&["exact/08002ee4.c".to_string()]);
        assert_eq!(options.out, "exact/08002ee4.ordered.c");
        assert_eq!(options.max_run, 6.0);
        assert_eq!(options.jobs, 12.0);
        assert_eq!(options.flags, "");
    }

    #[test]
    fn flags_are_parsed_and_out_is_respected() {
        let options = parse_arguments(&[
            "--flags".to_string(),
            "-fa,-fb".to_string(),
            "d.c".to_string(),
            "--out".to_string(),
            "o.c".to_string(),
            "--max-run".to_string(),
            "4".to_string(),
            "--jobs".to_string(),
            "3".to_string(),
        ]);
        assert_eq!(options.flags, "-fa,-fb");
        assert_eq!(options.draft, "d.c");
        assert_eq!(options.out, "o.c");
        assert_eq!(options.max_run, 4.0);
        assert_eq!(options.jobs, 3.0);
    }

    #[test]
    fn js_number_matches_the_cases_a_command_line_can_reach() {
        assert_eq!(js_number("6"), 6.0);
        assert_eq!(js_number(" 6 "), 6.0);
        assert_eq!(js_number(""), 0.0);
        assert!(js_number("six").is_nan());
        assert_eq!(js_number("2.5"), 2.5);
    }

    #[test]
    fn js_number_text_drops_the_decimal_for_integral_values() {
        assert_eq!(js_number_text(3.0), "3");
        assert_eq!(js_number_text(2.5), "2.5");
        assert_eq!(js_number_text(f64::NAN), "NaN");
    }

    #[test]
    fn slice_end_truncates_like_javascript() {
        assert_eq!(slice_end(3.0, 10), 4);
        assert_eq!(slice_end(f64::NAN, 10), 0);
        assert_eq!(slice_end(-1.0, 10), 0);
        assert_eq!(slice_end(2.5, 10), 3);
        assert_eq!(slice_end(99.0, 10), 10);
    }

    #[test]
    fn render_rebuilds_the_source_around_the_body() {
        let source = "// 関数本体\nvoid Func_08000000(void) {a;b;}\n";
        let characters: Vec<char> = source.chars().collect();
        let bounds = function_body(&characters).unwrap();
        let body: String = characters[bounds.0..bounds.1].iter().collect();
        let pieces = top_level_statements(&body);
        assert_eq!(render(&characters, bounds, &pieces), source);
        let swapped = vec![pieces[1].clone(), pieces[0].clone()];
        assert_eq!(
            render(&characters, bounds, &swapped),
            "// 関数本体\nvoid Func_08000000(void) {b;a;}\n"
        );
    }
}
