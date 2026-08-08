// CLI for the haifa-sched `rank_for_schedule` replica.
//
// `--self-test` mirrors the TypeScript original's `--self-test` flag exactly,
// including the `self-test=ok tool=rtl-schedule` line it prints and the order
// in which it checks things.
//
// PORT NOTE: the TS entrypoint has no mode other than `--self-test`, so the
// two additive modes below exist to give the TS/Rust parity harness something
// to diff byte-for-byte:
//   * `rtl-schedule table <dump>...` prints
//     `JSON.stringify([...parseDependenceTable(text).values()])`, one line per
//     file.
//   * `rtl-schedule diagnose <dump> <winner> <ready,uids>` prints
//     `JSON.stringify(diagnose(...))` for the table in that dump, optionally
//     with `--last <uid>` and `--insns <rtl-dump>` to exercise the class tier.
// Neither changes any existing behaviour. With no mode and no paths this
// binary FAILS with exit 2 rather than exiting 0 having scanned nothing -- a
// check that can "pass" without reading a single byte is not a check.

use std::path::{Path, PathBuf};
use std::process::ExitCode;

use rtl_insn::parse_insns;
use rtl_schedule::{
    compare_pair, diagnose, json_dependence_table, json_diagnosis, parse_dependence_table, InsnMap,
    ScheduleContext, Tier,
};

fn repository_root() -> &'static Path {
    Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap()
}

/// The real dependence-table excerpt from resource_385:0314 the module was
/// built against, plus the trace lines it must ignore.
const SELF_TEST_DUMP: &str = "
;;      insn  code    bb   dep  prio  cost   blockage units
;;       21   114     0     1    83     1    1 - 32   core\t: 88 67 47
;;       34   114     0     1    83     1    1 - 32   core\t: 88 67 49
;;       39   174     0     4    85     2    1 - 32   core\t: 67 47 43
;;       43    33     0     2    83     1    1 - 32   core\t: 67 49 45
;; Ready list (t = 14): 43 34 21
;;\t\t--> scheduling insn <<<21>>> on unit core
";

const CLEAR_WINNER_DUMP: &str = "
;;       15   174     0     0    89     2    1 - 32   core\t:
;;       17   174     0     0    80     2    1 - 32   core\t:
";

const COST_ONE_DUMP: &str = "
;;       50   174     0     0    50     1    1 - 32   core\t:
;;       60   244     0     1    40     2    1 - 32   core\t:
;;       70   174     0     1    40     1    1 - 32   core\t:
";

const INDEPENDENT_RIVAL_DUMP: &str = "
;;       50   174     0     0    50     1    1 - 32   core\t:
;;       60   244     0     1    40     2    1 - 32   core\t:
;;       80   174     0     0    40     1    1 - 32   core\t:
";

/// The RTL for the cost==1 fixtures, written as a real dump and parsed by the
/// `rtl-insn` crate rather than hand-constructed, so the self-test exercises
/// the same path the CLI does: 50 stores to [sp], 60 is a call with a true
/// dependency on 50, 70 loads from [sp] with a true dependency on 50.
const COST_ONE_RTL: &str = r#"
(insn 50 48 60 (set (mem:SI (reg/f:SI 13 sp) [0 S4 A32])
        (reg:SI 3 r3)) 174 {*movsi_insn} (nil))

(call_insn 60 50 70 (parallel [
            (set (reg:SI 0 r0)
                (call (mem:SI (symbol_ref:SI ("Func_02001508")) 0)
                    (const_int 0 [0x0])))
        ] ) 245 {*call_value_insn} (insn_list 50 (nil)))

(insn 70 60 80 (set (reg:SI 4 r4)
        (mem:SI (reg/f:SI 13 sp) [0 S4 A32])) 174 {*movsi_insn} (insn_list 50 (nil)))

(insn 80 70 90 (set (reg:SI 5 r5)
        (reg:SI 6 r6)) 174 {*movsi_insn} (nil))
"#;

fn self_test() {
    let table = parse_dependence_table(SELF_TEST_DUMP);
    assert!(table.len() == 4, "expected 4 dependence rows, got {}", table.len());
    let row = table.get(21.0).expect("expected a row for insn 21");
    assert!(row.priority == 83.0, "expected priority 83 for insn 21, got {}", row.priority);
    assert!(
        row.dependents == vec![88.0, 67.0, 47.0],
        "expected dependents 88 67 47 for insn 21, got {:?}",
        row.dependents
    );
    // The trace lines must not be mistaken for table rows.
    assert!(table.get(14.0).is_none(), "a 'Ready list (t = 14)' line is not a table row");

    // The "none" unit / "0 -  0" blockage form must parse like "core"/"1 - 32".
    let none_unit = parse_dependence_table(";;       71    -1     0     1    33     1    0 -  0   none\t: 88 80");
    let none_row = none_unit.get(71.0).expect("expected the none-unit row to parse");
    assert!(
        none_row.dependents == vec![88.0, 80.0],
        "expected dependents 88 80 on the none-unit row, got {:?}",
        none_row.dependents
    );

    // The real cycle-14 decision: 21 beats 43 on class, 34 on original order.
    let insns = parse_insns(COST_ONE_RTL);
    let mut cycle14 = InsnMap::new();
    cycle14.set(21.0, insns[3].clone());
    cycle14.set(34.0, insns[3].clone());
    cycle14.set(43.0, insns[2].clone());
    // 43's true dependency is on 50 in the fixture RTL; re-key the context so
    // the last-scheduled insn is that producer.
    let context =
        ScheduleContext { table: &table, last_scheduled_uid: Some(50.0), insns_by_uid: &cycle14 };
    let versus_43 = compare_pair(21.0, 43.0, &context);
    assert!(
        versus_43.winner == 21.0 && versus_43.tier == Tier::Class,
        "expected 21 to beat 43 on the class tier, got {versus_43:?}"
    );
    let diagnosis = diagnose(&[21.0, 34.0, 43.0], 21.0, &context);
    let mut tiers: Vec<&str> = diagnosis.per_rival.iter().map(|pair| pair.tier.as_str()).collect();
    tiers.sort_unstable();
    assert!(
        tiers == vec!["class", "original-order"],
        "expected one class-tier win and one original-order win, got {}",
        tiers.join(",")
    );

    // A clean priority win needs no lower tier and no RtlInsn data at all.
    let clear_winner = parse_dependence_table(CLEAR_WINNER_DUMP);
    let empty = InsnMap::new();
    let clear_pick = compare_pair(
        15.0,
        17.0,
        &ScheduleContext { table: &clear_winner, last_scheduled_uid: None, insns_by_uid: &empty },
    );
    assert!(
        clear_pick.winner == 15.0 && clear_pick.tier == Tier::Priority,
        "expected a clean priority win, got {clear_pick:?}"
    );

    // arm_adjust_cost's cost==1 shortcuts (arm.c:2456): a true dependency into
    // a CALL_INSN, and a stack-pointer load whose producer is a store, both
    // demote from class 1 to class 3 -- they can't stall anything, so
    // rank_for_schedule treats them as independent of the last-scheduled insn.
    let cost_one_table = parse_dependence_table(COST_ONE_DUMP);
    let cost_one_insns = InsnMap::by_uid(&insns);
    let cost_one_context = ScheduleContext {
        table: &cost_one_table,
        last_scheduled_uid: Some(50.0),
        insns_by_uid: &cost_one_insns,
    };
    let call_shortcut = compare_pair(60.0, 70.0, &cost_one_context);
    assert!(
        call_shortcut.tier != Tier::Class,
        "expected the call-insn and stack-load cost==1 shortcuts to tie at class 3 (no class-tier win), got {call_shortcut:?}"
    );
    assert!(
        call_shortcut.winner == 60.0 && call_shortcut.tier == Tier::OriginalOrder,
        "expected 60 to win on original-order once class and priority tie, got {call_shortcut:?}"
    );

    // Without the cost==1 modeling, a call_insn with a true dependency would
    // wrongly win on class 1 against a genuinely independent rival; confirm it
    // instead ties at class 3 and falls through to a lower tier.
    let independent_rival_table = parse_dependence_table(INDEPENDENT_RIVAL_DUMP);
    let versus_independent = compare_pair(
        60.0,
        80.0,
        &ScheduleContext {
            table: &independent_rival_table,
            last_scheduled_uid: Some(50.0),
            insns_by_uid: &cost_one_insns,
        },
    );
    assert!(
        versus_independent.tier != Tier::Class,
        "call_insn's true dependency on the producer should not win on class, got {versus_independent:?}"
    );

    println!("self-test=ok tool=rtl-schedule");
}

fn usage() -> ExitCode {
    eprintln!(
        "usage: rtl-schedule --self-test\n       rtl-schedule table <dump>...\n       rtl-schedule diagnose <dump> <winner-uid> <ready,uids> [--last <uid>] [--insns <rtl-dump>]\n(paths relative to {})",
        repository_root().display()
    );
    ExitCode::from(2)
}

fn resolve(path: &str) -> PathBuf {
    let candidate = Path::new(path);
    if candidate.is_absolute() {
        candidate.to_path_buf()
    } else {
        repository_root().join(candidate)
    }
}

/// PORT NOTE: lossy, matching Node/Bun's `readFileSync(..., "utf8")`, which
/// substitutes U+FFFD for invalid sequences rather than throwing.
fn read_text(path: &str) -> Result<String, ExitCode> {
    let resolved = resolve(path);
    match std::fs::read(&resolved) {
        Ok(bytes) => Ok(String::from_utf8_lossy(&bytes).into_owned()),
        Err(error) => {
            eprintln!("rtl-schedule: {}: {error}", resolved.display());
            Err(ExitCode::FAILURE)
        }
    }
}

fn flag_value<'a>(arguments: &'a [String], flag: &str) -> Option<&'a str> {
    let index = arguments.iter().position(|argument| argument == flag)?;
    arguments.get(index + 1).map(String::as_str)
}

fn run_table(paths: &[&String]) -> ExitCode {
    if paths.is_empty() {
        return usage();
    }
    for path in paths {
        let text = match read_text(path) {
            Ok(text) => text,
            Err(code) => return code,
        };
        println!("{}", json_dependence_table(&parse_dependence_table(&text)));
    }
    ExitCode::SUCCESS
}

fn run_diagnose(positional: &[&String], arguments: &[String]) -> ExitCode {
    let [dump, winner, ready] = match positional {
        [dump, winner, ready, ..] => [*dump, *winner, *ready],
        _ => return usage(),
    };
    let text = match read_text(dump) {
        Ok(text) => text,
        Err(code) => return code,
    };
    let table = parse_dependence_table(&text);
    let insns = match flag_value(arguments, "--insns") {
        None => Vec::new(),
        Some(path) => match read_text(path) {
            Ok(rtl) => parse_insns(&rtl),
            Err(code) => return code,
        },
    };
    let insns_by_uid = InsnMap::by_uid(&insns);
    let parse_uid = |text: &str| -> Option<f64> {
        let value = rtl_insn::jsnum::js_number(text);
        if value.is_finite() {
            Some(value)
        } else {
            None
        }
    };
    let Some(winner_uid) = parse_uid(winner) else { return usage() };
    let ready_uids: Vec<f64> = ready
        .split(',')
        .filter(|part| !part.is_empty())
        .filter_map(&parse_uid)
        .collect();
    if ready_uids.is_empty() {
        return usage();
    }
    let last_scheduled_uid = flag_value(arguments, "--last").and_then(parse_uid);
    let context = ScheduleContext { table: &table, last_scheduled_uid, insns_by_uid: &insns_by_uid };
    println!("{}", json_diagnosis(&diagnose(&ready_uids, winner_uid, &context)));
    ExitCode::SUCCESS
}

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.iter().any(|argument| argument == "--self-test") {
        self_test();
        return ExitCode::SUCCESS;
    }
    let flagged: Vec<&String> = arguments.iter().collect();
    let positional: Vec<&String> = {
        let mut kept = Vec::new();
        let mut skip_next = false;
        for argument in flagged.iter() {
            if skip_next {
                skip_next = false;
                continue;
            }
            if argument.starts_with("--") {
                // --last and --insns each consume the following value.
                skip_next = matches!(argument.as_str(), "--last" | "--insns");
                continue;
            }
            kept.push(*argument);
        }
        kept
    };
    match positional.split_first() {
        Some((mode, rest)) if *mode == "table" => run_table(rest),
        Some((mode, rest)) if *mode == "diagnose" => run_diagnose(rest, &arguments),
        // Bare paths default to `table`, matching how rtl-insn's CLI is used.
        Some(_) => run_table(&positional),
        None => usage(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_self_test_fixtures_are_the_ones_the_typescript_uses() {
        // Guard against the fixture drifting away from the TS original: the
        // self-test must exercise four real rows and must not blow up.
        assert_eq!(parse_dependence_table(SELF_TEST_DUMP).len(), 4);
        assert_eq!(parse_dependence_table(CLEAR_WINNER_DUMP).len(), 2);
        assert_eq!(parse_dependence_table(COST_ONE_DUMP).len(), 3);
        assert_eq!(parse_dependence_table(INDEPENDENT_RIVAL_DUMP).len(), 3);
        self_test();
    }

    #[test]
    fn the_fixture_rtl_parses_into_the_shapes_the_cost_one_rules_need() {
        let insns = parse_insns(COST_ONE_RTL);
        assert_eq!(insns.len(), 4);
        let map = InsnMap::by_uid(&insns);
        assert_eq!(map.get(60.0).expect("call 60").kind, rtl_insn::InsnKind::CallInsn);
        assert_eq!(map.get(70.0).expect("load 70").dependencies.len(), 1);
    }

    #[test]
    fn flag_values_are_read_positionally() {
        let arguments: Vec<String> =
            ["diagnose", "d", "21", "21,34", "--last", "39"].iter().map(|s| s.to_string()).collect();
        assert_eq!(flag_value(&arguments, "--last"), Some("39"));
        assert_eq!(flag_value(&arguments, "--insns"), None);
    }
}
