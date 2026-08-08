//! The analysis core: disassembly, halfword diffing, scheduler-dump lookup,
//! decision extraction and the printed report.

use std::path::{Path, PathBuf};
use std::process::Command;

use rtl_align::{align, js_number_text};
use rtl_insn::{callee_symbol, parse_insns, RtlInsn};
use rtl_schedule::{
    diagnose, parse_dependence_table, InsnMap, PairComparison, ScheduleContext,
};
use thumb_disasm::{js_number, parse_instruction, Instruction};

use crate::jsnum::{pad_end, pad_start, parse_int_16, slice_utf16, to_string_16, trim_end_js};
use crate::matchers::{objdump_row, ready_list, scheduling_insn, scheduling_insn_present};
use crate::ordered::{OrderedMap, OrderedSet};

/// `EXPLAIN_FLAGS` -- gcc-2.96's per-pass RTL dumps.
pub const EXPLAIN_FLAGS: [&str; 3] = ["-dS", "-dR", "-fsched-verbose=9"];

/// `findDumpFile(work, routingStem, suffix)`.
///
/// PORT NOTE -- `readdirSync(...).find(...)` returns the FIRST entry in the
/// directory's own readdir order, not the lexicographically first. `read_dir`
/// preserves that same order on this platform, so the entries are consumed as
/// they arrive and are deliberately NOT sorted. Sorting would be a
/// behaviour change whenever a work directory holds both a `.sched` and a
/// stale sibling that both match.
pub fn find_dump_file(work: &Path, routing_stem: &str, suffix: &str) -> Option<PathBuf> {
    let prefix = format!("{routing_stem}.");
    let tail = format!(".{suffix}");
    let entries = std::fs::read_dir(work).ok()?;
    for entry in entries.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        if name.starts_with(&prefix) && name.ends_with(&tail) {
            return Some(work.join(name));
        }
    }
    None
}

/// `disassembleInstructions(path, base = 0)`.
pub fn disassemble_instructions(path: &Path, base: i64) -> Result<Vec<Instruction>, String> {
    let mut command = Command::new("arm-none-eabi-objdump");
    command.args([
        "-D",
        "-b",
        "binary",
        "-m",
        "arm",
        "-M",
        "force-thumb",
    ]);
    // `...(base ? [...] : [])` -- `0` is falsy, so a zero base contributes no
    // argument at all rather than `--adjust-vma=0x0`.
    if base != 0 {
        command.arg(format!("--adjust-vma=0x{}", to_string_16(base as f64)));
    }
    command.arg(path);
    let result = command
        .output()
        .map_err(|error| format!("arm-none-eabi-objdump: {error}"))?;
    if !result.status.success() {
        return Err(String::from_utf8_lossy(&result.stderr).trim().to_string());
    }
    let stdout = String::from_utf8_lossy(&result.stdout).into_owned();
    let mut rows = Vec::new();
    // `.split("\n")` -- NOT `lines()`. `lines()` also strips a trailing `\r`,
    // which would silently repair the CRLF input that JavaScript's `(.*)$`
    // rejects outright.
    for line in stdout.split('\n') {
        let Some((digits, tail)) = objdump_row(line) else {
            continue;
        };
        let offset = parse_int_16(digits) - base as f64;
        rows.push(parse_instruction(offset as i64, trim_end_js(tail)));
    }
    Ok(rows)
}

/// `differingHalfwords(actual, expected)`.
///
/// PORT NOTE -- the tail loop starts at `shared & ~1` and runs to
/// `Math.max(actual.length, expected.length)`, so every halfword past the
/// shorter buffer is reported differing even though one side has no byte
/// there. JavaScript reads `undefined` out of range and `undefined !== byte`
/// is true; Rust would panic on the same index, so the tail is written as a
/// pure range walk with no indexing at all.
pub fn differing_halfwords(actual: &[u8], expected: &[u8]) -> OrderedSet {
    let mut found = OrderedSet::new();
    let shared = actual.len().min(expected.len()) as i64;
    let mut offset = 0i64;
    while offset + 2 <= shared {
        let a = offset as usize;
        if actual[a] != expected[a] || actual[a + 1] != expected[a + 1] {
            found.add(offset);
        }
        offset += 2;
    }
    let longest = actual.len().max(expected.len()) as i64;
    let mut offset = shared & !1;
    while offset < longest {
        found.add(offset);
        offset += 2;
    }
    found
}

/// `interface Decision`.
#[derive(Debug, Clone, PartialEq)]
pub struct Decision {
    pub ready_uids: Vec<f64>,
    pub last_scheduled_uid: Option<f64>,
}

/// `decisionForInsn(dumpText, uid)`.
pub fn decision_for_insn(dump_text: &str, uid: f64) -> Option<Decision> {
    let lines: Vec<&str> = dump_text.split('\n').collect();
    // `line.includes("--> scheduling insn <<<${uid}>>>")` -- a plain substring
    // test built from `String(uid)`, NOT the regex. The two disagree whenever
    // `uid` is not an integral number, and reproducing the substring form is
    // what keeps a `NaN` uid finding nothing instead of matching the first
    // decision in the dump.
    let needle = format!("--> scheduling insn <<<{}>>>", js_number_text(uid));
    let own_index = lines.iter().position(|line| line.contains(&needle))?;

    let mut ready_uids: Vec<f64> = Vec::new();
    for back in (0..own_index).rev() {
        if let Some(tail) = ready_list(lines[back]) {
            // `.trim().split(/\s+/).filter(Boolean).map(Number)`
            ready_uids = split_ready_uids(tail);
            break;
        }
        if scheduling_insn_present(lines[back]) {
            break;
        }
    }

    let mut last_scheduled_uid: Option<f64> = None;
    for back in (0..own_index).rev() {
        if let Some(digits) = scheduling_insn(lines[back]) {
            last_scheduled_uid = Some(js_number(digits));
            break;
        }
    }
    Some(Decision {
        ready_uids,
        last_scheduled_uid,
    })
}

/// `text.trim().split(/\s+/).filter(Boolean).map(Number)`.
///
/// PORT NOTE -- the uid list is a `Vec`, never a set. Duplicate uids in a ready
/// list must stay duplicated: `diagnose` produces one comparison row per entry
/// and a silent deduplication would drop rows from the printed table.
fn split_ready_uids(text: &str) -> Vec<f64> {
    let trimmed = text.trim_matches(|c| {
        crate::jsnum::is_js_whitespace(c) || crate::jsnum::is_js_line_terminator(c)
    });
    trimmed
        .split(|c| crate::jsnum::is_js_whitespace(c) || crate::jsnum::is_js_line_terminator(c))
        .filter(|piece| !piece.is_empty())
        .map(js_number)
        .collect()
}

/// `describeInsn(insn)`.
pub fn describe_insn(insn: &RtlInsn) -> String {
    match callee_symbol(insn) {
        Some(callee) => format!("call {callee}"),
        None => format!("insn {}", js_number_text(insn.uid)),
    }
}

/// `formatPair(pickedUid, pair)`.
///
/// PORT NOTE -- the winner test is `pair.winner === pickedUid`, an ordinary
/// `===` on two numbers. A `NaN` on either side makes it false and routes to
/// the divergence branch. `f64 == f64` in Rust agrees; `partial_cmp` returning
/// `None` and `total_cmp` treating NaN as orderable would both diverge, so
/// neither is used.
pub fn format_pair(picked_uid: f64, pair: &PairComparison) -> String {
    if pair.winner == picked_uid {
        return format!(
            "beats {} ({}: {})",
            js_number_text(pair.loser),
            pair.tier.as_str(),
            pair.detail
        );
    }
    format!(
        "?? model expects {} to beat {} here ({}: {}) -- \
actual pick suggests an unmodeled tier (register pressure / cost==1 / dest-order hook)",
        js_number_text(pair.winner),
        js_number_text(picked_uid),
        pair.tier.as_str(),
        pair.detail
    )
}

/// `traceForInsn(dumpText, insn, context)`.
pub fn trace_for_insn(dump_text: &str, insn: &RtlInsn, context: &ScheduleContext) -> String {
    let description = describe_insn(insn);
    let Some(decision) = decision_for_insn(dump_text, insn.uid) else {
        return description;
    };
    let context = ScheduleContext {
        table: context.table,
        last_scheduled_uid: decision.last_scheduled_uid,
        insns_by_uid: context.insns_by_uid,
    };
    let diagnosis = diagnose(&decision.ready_uids, insn.uid, &context);
    if diagnosis.per_rival.is_empty() {
        return description;
    }
    let rivals: Vec<String> = diagnosis
        .per_rival
        .iter()
        .map(|pair| format_pair(insn.uid, pair))
        .collect();
    format!("{description} -- {}", rivals.join("; "))
}

/// `report(actual, expected, work, actualPath, routingStem)`, returning the
/// text the TypeScript sends to `console.log` (one trailing newline per call).
pub fn report(
    actual: &[u8],
    expected: &[u8],
    work: &Path,
    actual_path: &Path,
    routing_stem: &str,
) -> Result<String, String> {
    let instructions = disassemble_instructions(actual_path, 0)?;
    let differing = differing_halfwords(actual, expected);

    let dump_path = find_dump_file(work, routing_stem, "sched2")
        .or_else(|| find_dump_file(work, routing_stem, "sched"));
    // `readFileSync(path, "utf8")` is lossy on invalid UTF-8, matching
    // `String::from_utf8_lossy` rather than `read_to_string`, which errors.
    let dump_text = match &dump_path {
        None => String::new(),
        Some(path) => String::from_utf8_lossy(
            &std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?,
        )
        .into_owned(),
    };
    let insns: Vec<RtlInsn> = match &dump_path {
        None => Vec::new(),
        Some(_) => parse_insns(&dump_text),
    };
    let alignment = align(&insns, &instructions);
    let mut by_offset: OrderedMap<usize> = OrderedMap::new();
    for (insn_index, instruction_index) in alignment.pairs.iter().copied() {
        by_offset.set(instructions[instruction_index].offset, insn_index);
    }
    let table = match &dump_path {
        None => rtl_schedule::DependenceTable::new(),
        Some(_) => parse_dependence_table(&dump_text),
    };
    let insns_by_uid = InsnMap::by_uid(&insns);

    let mut out = String::new();
    out.push_str(&format!(
        "candidate={} reference={} differing_halfwords={}\n",
        actual.len(),
        expected.len(),
        differing.size()
    ));
    if dump_path.is_none() {
        out.push_str(
            "(no scheduler dump found -- family/route may not support -dR, or nothing was scheduled)\n",
        );
    } else {
        out.push_str(&format!(
            "(scheduler trace: {} RTL insns, {} real instructions, \
{} aligned by class+destination-register; \
{} real instruction(s) had no RTL match -- \
typically prologue/epilogue multi-insn expansion, shown without a trace column below)\n",
            insns.len(),
            instructions.len(),
            alignment.pairs.len(),
            alignment.unmatched_instructions.len(),
        ));
    }

    out.push_str("      offset  candidate                      insn  scheduler trace\n");
    for instruction in &instructions {
        let offset = instruction.offset;
        let mark = if differing.has(offset) { "!" } else { " " };
        // `.replace(/\t/g, " ")` then `padEnd(30)` then `slice(0, 30)`: the pad
        // runs first, so a row longer than 30 units is truncated and a shorter
        // one is padded, in that order.
        let candidate_column = slice_utf16(&pad_end(&instruction.raw.replace('\t', " "), 30), 30);
        let insn_index = by_offset.get(offset).copied();
        let uid_label = match insn_index {
            None => String::new(),
            Some(index) => pad_start(&js_number_text(insns[index].uid), 4, ' '),
        };
        let trace = match (insn_index, &dump_path) {
            (Some(index), Some(_)) => {
                let context = ScheduleContext {
                    table: &table,
                    last_scheduled_uid: None,
                    insns_by_uid: &insns_by_uid,
                };
                trace_for_insn(&dump_text, &insns[index], &context)
            }
            _ => String::new(),
        };
        out.push_str(&format!(
            "  {mark} {}  {candidate_column} {uid_label}  {trace}\n",
            pad_start(&to_string_16(offset as f64), 4, '0'),
        ));
    }
    if let Some(path) = &dump_path {
        out.push_str(&format!("\nfull dump: {}\n", path.display()));
    }
    Ok(out)
}
