//! A deterministic replica of gcc-2.96's haifa-sched.c `rank_for_schedule` --
//! not an approximation, the actual tier order and comparisons, verified
//! against the real source (alchemy-gcc/gcc-2.96/gcc/haifa-sched.c:4154-4274)
//! and cross-checked against a real ready-list decision during development
//! (resource_385:0314, cycle 14: insns 43/34/21 ready, 21 chosen).
//!
//! WHY THIS FILE EXISTS: `rank_for_schedule` is a PAIRWISE comparator plugged
//! into a sort, not a single rule that explains a winner against an entire
//! ready list at once: it returns at the first tier where a pair of insns
//! differs, tier by tier, exactly like a lexicographic comparison. This module
//! mirrors that shape ([`compare_pair`]) instead of trying to find "the one
//! tier" that explains a winner beating every rival simultaneously -- a winner
//! can beat one rival on class and a different rival on original order, in the
//! same decision.
//!
//! This exists to replace "try five -fsched-* flags and see which one changes
//! the output" with "compute which tier decided each pairwise comparison,
//! using the data already in the dump." Priority and dependency counts are
//! read straight from the dump's own "Region Dependences" table; the
//! last-scheduled-insn class tier reads the LOG_LINKS dependency kind the
//! `rtl-insn` crate already parses. Two tiers this file does NOT replicate:
//! the register-pressure weight comparison (guarded by `!reload_completed`,
//! always false for the post-reload -dR/.sched2 pass this tool reads, so it
//! never fires there) and `sched_dest_order_regno` (the project's own
//! -fsched-low-dest-first/-high-dest-first Camelot-matching hook, which only
//! applies to files already routed through one of those flags -- out of scope
//! for a routing-independent diagnosis).
//!
//! The class tier's real gcc test is not just "does a LOG_LINKS edge exist"
//! -- it's `link == 0 || insn_cost(last_scheduled, link, candidate) == 1`
//! (haifa-sched.c:4200-4202): a class-1/2 edge with an effective cost of
//! exactly one cycle demotes to class 3 too, since it can't stall anything.
//! arm_adjust_cost (arm.c:2456) forces that cost to 1, UNCONDITIONALLY (not
//! behind any -f flag), in two cases this file models:
//!   - a true (REG_DEP_ANTI/OUTPUT already return 0, not 1, so they're
//!     unaffected) dependency edge into a CALL_INSN -- "call insns don't incur
//!     a stall, even if they follow a load,"
//!   - a load from the stack pointer, hard frame pointer, or a constant-pool
//!     address, whose producer is a store -- "no conflict if the load reads
//!     from a cached area." This file only models the stack-pointer case
//!     (register 13) of that rule, conservatively: a pool load's RTL address
//!     shape needs more fixture work to recognize reliably, and under-modeling
//!     only costs a `model-divergence` label on that specific pair, never a
//!     wrong classification.
//!
//! Port of `tools/lib/rtl_schedule.ts`.

use rtl_insn::jsnum::{js_number, js_number_to_string};
use rtl_insn::{InsnKind, RtlExpr, RtlInsn};

/// PORT NOTE: every numeric field here comes from `Number(text)` in the
/// TypeScript, and JS has a single number type. Storing `f64` (rather than the
/// `u32` the columns "obviously" hold) is what keeps a pathological dump --
/// a `priority` column with more digits than `u64` can hold, say -- comparing
/// and printing the way the original does instead of overflowing or refusing
/// to parse.
#[derive(Debug, Clone, PartialEq)]
pub struct DependenceRow {
    pub uid: f64,
    pub code: f64,
    /// the table's "dep" column: how many things THIS insn depends on
    pub dep_count: f64,
    pub priority: f64,
    pub cost: f64,
    /// successor UIDs -- what `rank_for_schedule`'s depend_count tier counts
    pub dependents: Vec<f64>,
}

/// PORT NOTE: a `Map<number, DependenceRow>`, not a `HashMap`. JS `Map`
/// iterates in INSERTION order and `set` on an existing key overwrites the
/// value while keeping the key's original position. The CLI serialises this
/// table, so that order is observable and load-bearing; a `HashMap` would
/// scramble it.
#[derive(Debug, Clone, Default, PartialEq)]
pub struct DependenceTable {
    rows: Vec<DependenceRow>,
}

impl DependenceTable {
    pub fn new() -> Self {
        Self::default()
    }

    /// `Map#set`. Key equality is SameValueZero, which for the non-NaN,
    /// non-negative-zero UIDs a `\d+` column can produce is plain `==`.
    pub fn set(&mut self, row: DependenceRow) {
        match self.rows.iter_mut().find(|existing| existing.uid == row.uid) {
            Some(existing) => *existing = row,
            None => self.rows.push(row),
        }
    }

    pub fn get(&self, uid: f64) -> Option<&DependenceRow> {
        self.rows.iter().find(|row| row.uid == uid)
    }

    pub fn len(&self) -> usize {
        self.rows.len()
    }

    pub fn is_empty(&self) -> bool {
        self.rows.is_empty()
    }

    pub fn iter(&self) -> std::slice::Iter<'_, DependenceRow> {
        self.rows.iter()
    }
}

impl<'a> IntoIterator for &'a DependenceTable {
    type Item = &'a DependenceRow;
    type IntoIter = std::slice::Iter<'a, DependenceRow>;

    fn into_iter(self) -> Self::IntoIter {
        self.rows.iter()
    }
}

/// The `Map<number, RtlInsn>` side of a [`ScheduleContext`]. Same reasoning as
/// [`DependenceTable`]: insertion-ordered, keyed by a JS number.
#[derive(Debug, Clone, Default, PartialEq)]
pub struct InsnMap {
    entries: Vec<(f64, RtlInsn)>,
}

impl InsnMap {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn set(&mut self, uid: f64, insn: RtlInsn) {
        match self.entries.iter_mut().find(|(key, _)| *key == uid) {
            Some(slot) => slot.1 = insn,
            None => self.entries.push((uid, insn)),
        }
    }

    /// Convenience for the common `new Map(insns.map(i => [i.uid, i]))` shape.
    pub fn by_uid(insns: &[RtlInsn]) -> Self {
        let mut map = Self::new();
        for insn in insns {
            map.set(insn.uid, insn.clone());
        }
        map
    }

    pub fn get(&self, uid: f64) -> Option<&RtlInsn> {
        self.entries.iter().find(|(key, _)| *key == uid).map(|(_, insn)| insn)
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }
}

// ---------------------------------------------------------------------------
// The TABLE_ROW regex, hand-rolled.
// ---------------------------------------------------------------------------
//
// The original is:
//
//   /^;;\s+(\d+)\s+(-?\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+\d+\s*-\s*\d+\s+\S+\s*:?\s*(.*)$/
//
// The "blockage" column prints as three separate whitespace-joined tokens
// ("1 - 32" or "0 -  0", note the sometimes-doubled space), not one contiguous
// "N-M" token -- easy to under-count as a single \S+ group.
//
// PORT NOTE: no regex crate (zero third-party dependencies), so this is a
// hand-written matcher. Three JS-regex behaviours it must reproduce exactly:
//
//   * `\s` is NOT `char::is_whitespace`: JS includes U+FEFF and excludes
//     U+0085 (NEL); Rust's `White_Space` property is the reverse.
//   * `\d` is ASCII `0-9` only, never a Unicode decimal digit.
//   * `.` matches everything EXCEPT the four line terminators (U+000A,
//     U+000D, U+2028, U+2029), and `$` without the `m` flag anchors to the
//     very end of the subject. Since the subject is a line produced by
//     `split("\n")`, a CRLF dump leaves a trailing `\r` that `.*` cannot
//     cross -- so such a row matches only if some earlier `\s*` swallows the
//     `\r`, and otherwise fails to match AT ALL. That is a real behavioural
//     difference (a CRLF dump parses to an empty table), and it is
//     reproduced rather than "fixed", because fixing it here would make the
//     Rust port disagree with the tool the rest of the pipeline runs.
//
// The greedy quantifiers before the tail need no backtracking: `\s` and `\d`
// are disjoint character classes, so a greedy `\s+`/`\d+` that fails its
// successor cannot be rescued by giving a character back. Only the trailing
// `\S+ \s* :? \s* (.*) $` group can require it, and that is spelled out as an
// explicit nested search below, longest-first at every level, which is exactly
// the order a backtracking engine explores.

fn is_js_space(ch: char) -> bool {
    matches!(
        ch,
        '\t' | '\n'
            | '\u{0b}'
            | '\u{0c}'
            | '\r'
            | ' '
            | '\u{a0}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
            | '\u{feff}'
    )
}

fn is_line_terminator(ch: char) -> bool {
    matches!(ch, '\n' | '\r' | '\u{2028}' | '\u{2029}')
}

fn is_ascii_digit(ch: char) -> bool {
    ch.is_ascii_digit()
}

struct Cursor<'a> {
    chars: &'a [char],
    at: usize,
}

impl<'a> Cursor<'a> {
    fn new(chars: &'a [char]) -> Self {
        Cursor { chars, at: 0 }
    }

    fn peek(&self) -> Option<char> {
        self.chars.get(self.at).copied()
    }

    fn literal(&mut self, ch: char) -> Option<()> {
        if self.peek() == Some(ch) {
            self.at += 1;
            Some(())
        } else {
            None
        }
    }

    /// `\s*` / `\s+` (and the digit equivalents) -- greedy, and since the
    /// classes involved are disjoint from what follows them, never revisited.
    fn run(&mut self, class: fn(char) -> bool) -> usize {
        let start = self.at;
        while self.at < self.chars.len() && class(self.chars[self.at]) {
            self.at += 1;
        }
        self.at - start
    }

    fn run_at_least_one(&mut self, class: fn(char) -> bool) -> Option<()> {
        if self.run(class) == 0 {
            None
        } else {
            Some(())
        }
    }

    /// `(\d+)` -- returns the matched text.
    fn digits(&mut self) -> Option<String> {
        let start = self.at;
        self.run_at_least_one(is_ascii_digit)?;
        Some(self.chars[start..self.at].iter().collect())
    }

    /// `(-?\d+)`.
    fn signed_digits(&mut self) -> Option<String> {
        let start = self.at;
        let _ = self.literal('-');
        self.run_at_least_one(is_ascii_digit)?;
        Some(self.chars[start..self.at].iter().collect())
    }
}

struct RowMatch {
    uid: String,
    code: String,
    dep: String,
    priority: String,
    cost: String,
    dependents: String,
}

fn match_table_row(line: &str) -> Option<RowMatch> {
    let chars: Vec<char> = line.chars().collect();
    let mut cursor = Cursor::new(&chars);

    cursor.literal(';')?;
    cursor.literal(';')?;
    cursor.run_at_least_one(is_js_space)?;
    let uid = cursor.digits()?;
    cursor.run_at_least_one(is_js_space)?;
    let code = cursor.signed_digits()?;
    cursor.run_at_least_one(is_js_space)?;
    // the "bb" column: matched but not captured by the original
    cursor.digits()?;
    cursor.run_at_least_one(is_js_space)?;
    let dep = cursor.digits()?;
    cursor.run_at_least_one(is_js_space)?;
    let priority = cursor.digits()?;
    cursor.run_at_least_one(is_js_space)?;
    let cost = cursor.digits()?;
    cursor.run_at_least_one(is_js_space)?;
    // blockage low bound
    cursor.digits()?;
    cursor.run(is_js_space);
    cursor.literal('-')?;
    cursor.run(is_js_space);
    // blockage high bound
    cursor.digits()?;
    cursor.run_at_least_one(is_js_space)?;

    let dependents = match_row_tail(&chars, cursor.at)?;
    Some(RowMatch { uid, code, dep, priority, cost, dependents })
}

/// `\S+ \s* :? \s* (.*) $` with faithful longest-first backtracking.
fn match_row_tail(chars: &[char], from: usize) -> Option<String> {
    let run_end = |start: usize, class: fn(char) -> bool| {
        let mut end = start;
        while end < chars.len() && class(chars[end]) {
            end += 1;
        }
        end
    };

    // `\S+`, at least one character, greedy.
    let nonspace_end = run_end(from, |ch| !is_js_space(ch));
    if nonspace_end == from {
        return None;
    }
    for after_nonspace in (from + 1..=nonspace_end).rev() {
        let space_end = run_end(after_nonspace, is_js_space);
        for after_space in (after_nonspace..=space_end).rev() {
            // `:?` is greedy: try consuming the colon before skipping it.
            let colon_choices: &[usize] = if chars.get(after_space) == Some(&':') {
                &[1, 0]
            } else {
                &[0]
            };
            for consumed in colon_choices {
                let after_colon = after_space + consumed;
                let space2_end = run_end(after_colon, is_js_space);
                for tail_start in (after_colon..=space2_end).rev() {
                    // `.*$`: the remainder must contain no line terminator.
                    if chars[tail_start..].iter().copied().all(|ch| !is_line_terminator(ch)) {
                        return Some(chars[tail_start..].iter().collect());
                    }
                }
            }
        }
    }
    None
}

/// `String.prototype.trim`, which uses the same character set as JS `\s`.
fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_space)
}

/// Parses the "Region Dependences" table gcc prints once per basic block at
/// the head of a -dR/.sched2 dump, before any scheduling trace text:
/// ```text
/// ;;      insn  code    bb   dep  prio  cost   blockage units
/// ;;       15   174     0     0    89     2    1 - 32   core : 67 47 39 28 19
/// ```
pub fn parse_dependence_table(dump_text: &str) -> DependenceTable {
    let mut table = DependenceTable::new();
    for line in dump_text.split('\n') {
        let Some(matched) = match_table_row(line) else { continue };
        let trimmed = js_trim(&matched.dependents);
        let dependents: Vec<f64> = if trimmed.is_empty() {
            Vec::new()
        } else {
            // PORT NOTE: `split(/\s+/)` on an already-trimmed, non-empty
            // string never yields an empty field, so no filter is needed
            // before `Number`; `Number.isFinite` then drops NaN/Infinity.
            trimmed
                .split(is_js_space)
                .filter(|part| !part.is_empty())
                .map(js_number)
                .filter(|value| value.is_finite())
                .collect()
        };
        table.set(DependenceRow {
            uid: js_number(&matched.uid),
            code: js_number(&matched.code),
            dep_count: js_number(&matched.dep),
            priority: js_number(&matched.priority),
            cost: js_number(&matched.cost),
            dependents,
        });
    }
    table
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Tier {
    Priority,
    Class,
    DependCount,
    OriginalOrder,
    /// PORT NOTE: present in the TypeScript `Tier` union but never produced by
    /// `comparePair` (the original-order tier is total, so no comparison can
    /// fall through). Kept so the two type surfaces match.
    Indeterminate,
}

impl Tier {
    pub fn as_str(self) -> &'static str {
        match self {
            Tier::Priority => "priority",
            Tier::Class => "class",
            Tier::DependCount => "depend-count",
            Tier::OriginalOrder => "original-order",
            Tier::Indeterminate => "indeterminate",
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct PairComparison {
    pub winner: f64,
    pub loser: f64,
    pub tier: Tier,
    pub detail: String,
}

const STACK_POINTER_REGNUM: f64 = 13.0;

fn is_mem_from(expr: &RtlExpr, regnum: f64) -> bool {
    let RtlExpr::Mem { address } = expr else { return false };
    match address.as_ref() {
        RtlExpr::Reg { number, .. } => *number == regnum,
        // The TS re-wraps `address.a` in a synthetic `mem` node and recurses,
        // i.e. it walks the LEFT operand chain of the address expression only.
        RtlExpr::Binary { a, .. } => is_mem_from(&RtlExpr::Mem { address: a.clone() }, regnum),
        _ => false,
    }
}

/// arm_adjust_cost's "no conflict if the load reads from a cached area" rule:
/// the dependent insn is a plain register load from the stack pointer, and the
/// producer (the last-scheduled insn) is a plain store.
fn is_cached_stack_load_after_store(candidate: &RtlInsn, producer: &RtlInsn) -> bool {
    let (Some(candidate_set), Some(producer_set)) = (&candidate.set, &producer.set) else {
        return false;
    };
    if !matches!(candidate_set.dest, RtlExpr::Reg { .. })
        || !is_mem_from(&candidate_set.src, STACK_POINTER_REGNUM)
    {
        return false;
    }
    matches!(producer_set.dest, RtlExpr::Mem { .. })
}

fn class_of(insn: &RtlInsn, last_scheduled_uid: Option<f64>, insns_by_uid: &InsnMap) -> u8 {
    let Some(last_scheduled_uid) = last_scheduled_uid else { return 3 };
    let Some(link) = insn.dependencies.iter().find(|dependency| dependency.uid == last_scheduled_uid)
    else {
        return 3;
    };
    if link.kind != rtl_insn::DependencyKind::True {
        return 2;
    }
    if insn.kind == InsnKind::CallInsn {
        return 3;
    }
    match insns_by_uid.get(last_scheduled_uid) {
        Some(producer) if is_cached_stack_load_after_store(insn, producer) => 3,
        _ => 1,
    }
}

pub struct ScheduleContext<'a> {
    pub table: &'a DependenceTable,
    pub last_scheduled_uid: Option<f64>,
    pub insns_by_uid: &'a InsnMap,
}

/// PORT NOTE: `Math.max`/`Math.min` propagate NaN, which `f64::max`/`f64::min`
/// do NOT (they return the non-NaN operand). Reachable only via a pathological
/// dump, but the whole point of these helpers is that the parity harness never
/// has to argue about which inputs are reachable.
fn js_max(a: f64, b: f64) -> f64 {
    if a.is_nan() || b.is_nan() {
        f64::NAN
    } else if a >= b {
        a
    } else {
        b
    }
}

fn js_min(a: f64, b: f64) -> f64 {
    if a.is_nan() || b.is_nan() {
        f64::NAN
    } else if a <= b {
        a
    } else {
        b
    }
}

fn optional_number_text(value: Option<f64>) -> String {
    match value {
        // JS template literals stringify `null` as "null".
        None => "null".to_string(),
        Some(number) => js_number_to_string(number),
    }
}

/// Compares two ready insns exactly the way `rank_for_schedule` does: tier by
/// tier, in order, returning at the first tier that differs. `a` and `b` are
/// symmetric inputs; the return always names the actual winner/loser by UID
/// rather than leaving the caller to work it out from a signed number, since
/// this function exists to be READ, not just sorted by.
pub fn compare_pair(a_uid: f64, b_uid: f64, context: &ScheduleContext) -> PairComparison {
    let a_row = context.table.get(a_uid);
    let b_row = context.table.get(b_uid);
    if let (Some(a_row), Some(b_row)) = (a_row, b_row) {
        if a_row.priority != b_row.priority {
            let (winner, loser) = if a_row.priority > b_row.priority {
                (a_uid, b_uid)
            } else {
                (b_uid, a_uid)
            };
            return PairComparison {
                winner,
                loser,
                tier: Tier::Priority,
                detail: format!(
                    "priority {} beats {}",
                    js_number_to_string(js_max(a_row.priority, b_row.priority)),
                    js_number_to_string(js_min(a_row.priority, b_row.priority)),
                ),
            };
        }
    }

    let a_insn = context.insns_by_uid.get(a_uid);
    let b_insn = context.insns_by_uid.get(b_uid);
    if let (Some(a_insn), Some(b_insn)) = (a_insn, b_insn) {
        let a_class = class_of(a_insn, context.last_scheduled_uid, context.insns_by_uid);
        let b_class = class_of(b_insn, context.last_scheduled_uid, context.insns_by_uid);
        if a_class != b_class {
            let (winner, loser, winner_class, loser_class) = if a_class > b_class {
                (a_uid, b_uid, a_class, b_class)
            } else {
                (b_uid, a_uid, b_class, a_class)
            };
            return PairComparison {
                winner,
                loser,
                tier: Tier::Class,
                detail: format!(
                    "class {winner_class} (relative to last-scheduled insn {}) beats class {loser_class}",
                    optional_number_text(context.last_scheduled_uid),
                ),
            };
        }
    }

    if let (Some(a_row), Some(b_row)) = (a_row, b_row) {
        if a_row.dependents.len() != b_row.dependents.len() {
            let (winner, loser) = if a_row.dependents.len() > b_row.dependents.len() {
                (a_uid, b_uid)
            } else {
                (b_uid, a_uid)
            };
            return PairComparison {
                winner,
                loser,
                tier: Tier::DependCount,
                detail: format!(
                    "{} dependents beats {}",
                    a_row.dependents.len().max(b_row.dependents.len()),
                    a_row.dependents.len().min(b_row.dependents.len()),
                ),
            };
        }
    }

    // Approximates INSN_LUID (original creation order) with UID, since both
    // increase monotonically with insn creation in the common case; flagged as
    // an approximation in the detail text rather than asserted as exact.
    let (winner, loser) = if a_uid < b_uid { (a_uid, b_uid) } else { (b_uid, a_uid) };
    PairComparison {
        winner,
        loser,
        tier: Tier::OriginalOrder,
        detail: "lower UID (approximates INSN_LUID, gcc's true tie-break key)".to_string(),
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct Diagnosis {
    pub winner_uid: f64,
    pub per_rival: Vec<PairComparison>,
}

/// Applies [`compare_pair`] between the actual winner and every other ready
/// insn, mirroring what the sort that produced this ready-list ordering
/// actually did pairwise. A winner can beat different rivals at different
/// tiers in the same decision (e.g. beat one rival on class, another on
/// original order) -- reporting each pair separately is what makes this a
/// genuine explanation rather than a single tier label pretending to cover
/// every comparison at once.
pub fn diagnose(ready_uids: &[f64], winner_uid: f64, context: &ScheduleContext) -> Diagnosis {
    let per_rival = ready_uids
        .iter()
        .copied()
        .filter(|uid| *uid != winner_uid)
        .map(|rival_uid| compare_pair(winner_uid, rival_uid, context))
        .collect();
    Diagnosis { winner_uid, per_rival }
}

// ---------------------------------------------------------------------------
// JSON rendering for the CLI's parity surface.
// ---------------------------------------------------------------------------

fn push_number_array(values: &[f64], out: &mut String) {
    out.push('[');
    for (index, value) in values.iter().enumerate() {
        if index > 0 {
            out.push(',');
        }
        out.push_str(&js_number_to_string(*value));
    }
    out.push(']');
}

/// `JSON.stringify([...table.values()])`, field order matching the TypeScript
/// object literal (`uid, code, depCount, priority, cost, dependents`).
pub fn json_dependence_table(table: &DependenceTable) -> String {
    let mut out = String::from("[");
    for (index, row) in table.iter().enumerate() {
        if index > 0 {
            out.push(',');
        }
        out.push_str(&format!(
            r#"{{"uid":{},"code":{},"depCount":{},"priority":{},"cost":{},"dependents":"#,
            js_number_to_string(row.uid),
            js_number_to_string(row.code),
            js_number_to_string(row.dep_count),
            js_number_to_string(row.priority),
            js_number_to_string(row.cost),
        ));
        push_number_array(&row.dependents, &mut out);
        out.push('}');
    }
    out.push(']');
    out
}

/// `JSON.stringify(diagnose(...))`.
pub fn json_diagnosis(diagnosis: &Diagnosis) -> String {
    let mut out = format!(
        r#"{{"winnerUid":{},"perRival":["#,
        js_number_to_string(diagnosis.winner_uid)
    );
    for (index, pair) in diagnosis.per_rival.iter().enumerate() {
        if index > 0 {
            out.push(',');
        }
        out.push_str(&format!(
            r#"{{"winner":{},"loser":{},"tier":"#,
            js_number_to_string(pair.winner),
            js_number_to_string(pair.loser),
        ));
        let mut escaped = String::new();
        rtl_insn::json_escape(pair.tier.as_str(), &mut escaped);
        out.push_str(&escaped);
        out.push_str(",\"detail\":");
        let mut escaped_detail = String::new();
        rtl_insn::json_escape(&pair.detail, &mut escaped_detail);
        out.push_str(&escaped_detail);
        out.push('}');
    }
    out.push_str("]}");
    out
}

#[cfg(test)]
mod tests {
    use super::*;
    use rtl_insn::{parse_insns, Dependency, DependencyKind, Set};

    fn reg(number: f64) -> RtlExpr {
        RtlExpr::Reg { number, name: format!("r{}", js_number_to_string(number)) }
    }

    fn insn(uid: f64, dependencies: Vec<Dependency>) -> RtlInsn {
        RtlInsn {
            uid,
            kind: InsnKind::Insn,
            code: Some("set".to_string()),
            set: None,
            call_target: None,
            dependencies,
            raw: String::new(),
        }
    }

    /// The real dependence-table excerpt this module was built against
    /// (resource_385:0314, verified 2026-08-04).
    const REAL_EXCERPT: &str = "
;;      insn  code    bb   dep  prio  cost   blockage units
;;       21   114     0     1    83     1    1 - 32   core\t: 88 67 47
;;       34   114     0     1    83     1    1 - 32   core\t: 88 67 49
;;       39   174     0     4    85     2    1 - 32   core\t: 67 47 43
;;       43    33     0     2    83     1    1 - 32   core\t: 67 49 45
";

    #[test]
    fn parses_the_real_dependence_table() {
        let table = parse_dependence_table(REAL_EXCERPT);
        assert_eq!(table.len(), 4);
        let row = table.get(21.0).expect("row 21");
        assert_eq!(row.priority, 83.0);
        assert_eq!(row.dependents, vec![88.0, 67.0, 47.0]);
        assert_eq!(row.code, 114.0);
        assert_eq!(row.dep_count, 1.0);
        assert_eq!(row.cost, 1.0);
        // Insertion order, not sorted order.
        let uids: Vec<f64> = table.iter().map(|row| row.uid).collect();
        assert_eq!(uids, vec![21.0, 34.0, 39.0, 43.0]);
    }

    #[test]
    fn parses_the_none_unit_zero_blockage_row() {
        // Regression guard: the "0 -  0   none" blockage form (doubled space, a
        // "none" unit instead of "core") is what a zero-latency/no-unit insn
        // prints as; it must parse the same as the ordinary "1 - 32   core".
        let table = parse_dependence_table(";;       71    -1     0     1    33     1    0 -  0   none\t: 88 80");
        let row = table.get(71.0).expect("row 71");
        assert_eq!(row.dependents, vec![88.0, 80.0]);
        assert_eq!(row.code, -1.0);
    }

    #[test]
    fn a_row_with_no_dependents_yields_an_empty_list() {
        let table = parse_dependence_table(";;       15   174     0     0    89     2    1 - 32   core\t:");
        assert_eq!(table.get(15.0).expect("row 15").dependents, Vec::<f64>::new());
    }

    #[test]
    fn non_table_lines_are_ignored() {
        let table = parse_dependence_table(
            ";; Ready list (t = 4): 19 28\n;;\t\t--> scheduling insn <<<28>>> on unit core\n\n(insn 28 15 17 (nil))",
        );
        assert!(table.is_empty());
    }

    #[test]
    fn a_repeated_uid_overwrites_in_place() {
        // Map#set keeps the original key position while replacing the value.
        let table = parse_dependence_table(
            ";;       21   114     0     1    83     1    1 - 32   core\t: 1\n\
             ;;       34   114     0     1    80     1    1 - 32   core\t: 2\n\
             ;;       21   114     0     1    99     1    1 - 32   core\t: 3 4\n",
        );
        assert_eq!(table.iter().map(|row| row.uid).collect::<Vec<_>>(), vec![21.0, 34.0]);
        assert_eq!(table.get(21.0).expect("row 21").priority, 99.0);
        assert_eq!(table.get(21.0).expect("row 21").dependents, vec![3.0, 4.0]);
    }

    #[test]
    fn a_carriage_return_after_content_defeats_the_dollar_anchor() {
        // JS `.` cannot match `\r` and `$` (no `m` flag) anchors to end of
        // input, so a CRLF line with trailing content simply does not match.
        let crlf = ";;       21   114     0     1    83     1    1 - 32   core\t: 88 67 47\r";
        assert!(parse_dependence_table(crlf).is_empty());
        // ...but a `\r` that a trailing `\s*` can swallow still matches, with
        // an empty dependents list.
        let swallowed = ";;       21   114     0     1    83     1    1 - 32   core\t:\r";
        assert_eq!(
            parse_dependence_table(swallowed).get(21.0).expect("row 21").dependents,
            Vec::<f64>::new()
        );
    }

    #[test]
    fn greedy_nonspace_takes_the_colon_when_no_space_precedes_it() {
        // `\S+` is greedy, so with "core:88 67" it swallows "core:88" and the
        // dependents column is only "67". Reproducing the greed matters: a
        // naive "split on colon" port would report [88, 67].
        let table = parse_dependence_table(";;       21   114     0     1    83     1    1 - 32   core:88 67");
        assert_eq!(table.get(21.0).expect("row 21").dependents, vec![67.0]);
    }

    #[test]
    fn js_space_class_differs_from_rust_whitespace() {
        // U+FEFF is JS `\s` but not Rust `White_Space`; U+0085 is the reverse.
        let feff = ";;\u{feff}21   114     0     1    83     1    1 - 32   core\t: 88";
        assert_eq!(parse_dependence_table(feff).len(), 1);
        let nel = ";;\u{85}21   114     0     1    83     1    1 - 32   core\t: 88";
        assert!(parse_dependence_table(nel).is_empty());
    }

    #[test]
    fn hex_and_garbage_dependents_follow_number_semantics() {
        // The dependents column goes through `Number` + `Number.isFinite`, so
        // "0x10" is 16 and "12abc" is dropped.
        let table = parse_dependence_table(";;       21   114     0     1    83     1    1 - 32   core\t: 0x10 12abc 7");
        assert_eq!(table.get(21.0).expect("row 21").dependents, vec![16.0, 7.0]);
    }

    #[test]
    fn priority_tier_needs_no_insn_data() {
        let table = parse_dependence_table(
            ";;       15   174     0     0    89     2    1 - 32   core\t:\n\
             ;;       17   174     0     0    80     2    1 - 32   core\t:\n",
        );
        let empty = InsnMap::new();
        let context = ScheduleContext { table: &table, last_scheduled_uid: None, insns_by_uid: &empty };
        let pick = compare_pair(15.0, 17.0, &context);
        assert_eq!(pick.winner, 15.0);
        assert_eq!(pick.tier, Tier::Priority);
        assert_eq!(pick.detail, "priority 89 beats 80");
    }

    #[test]
    fn the_real_cycle_14_decision_reproduces() {
        let table = parse_dependence_table(REAL_EXCERPT);
        let mut insns_by_uid = InsnMap::new();
        insns_by_uid.set(21.0, insn(21.0, vec![]));
        insns_by_uid.set(34.0, insn(34.0, vec![]));
        insns_by_uid.set(
            43.0,
            insn(43.0, vec![Dependency { uid: 39.0, kind: DependencyKind::True }]),
        );
        let context =
            ScheduleContext { table: &table, last_scheduled_uid: Some(39.0), insns_by_uid: &insns_by_uid };

        let versus_43 = compare_pair(21.0, 43.0, &context);
        assert_eq!((versus_43.winner, versus_43.tier), (21.0, Tier::Class));
        assert_eq!(
            versus_43.detail,
            "class 3 (relative to last-scheduled insn 39) beats class 1"
        );

        let versus_34 = compare_pair(21.0, 34.0, &context);
        assert_eq!((versus_34.winner, versus_34.tier), (21.0, Tier::OriginalOrder));

        let diagnosis = diagnose(&[21.0, 34.0, 43.0], 21.0, &context);
        assert_eq!(diagnosis.per_rival.len(), 2);
        let mut tiers: Vec<&str> = diagnosis.per_rival.iter().map(|pair| pair.tier.as_str()).collect();
        tiers.sort_unstable();
        assert_eq!(tiers, vec!["class", "original-order"]);
    }

    fn cost_one_fixture() -> (DependenceTable, InsnMap) {
        let table = parse_dependence_table(
            ";;       50   174     0     0    50     1    1 - 32   core\t:\n\
             ;;       60   244     0     1    40     2    1 - 32   core\t:\n\
             ;;       70   174     0     1    40     1    1 - 32   core\t:\n",
        );
        let mut insns = InsnMap::new();
        // 50 is the producer (last-scheduled): a plain store to memory.
        insns.set(
            50.0,
            RtlInsn {
                uid: 50.0,
                kind: InsnKind::Insn,
                code: Some("set".to_string()),
                set: Some(Set { dest: RtlExpr::Mem { address: Box::new(reg(13.0)) }, src: reg(3.0) }),
                call_target: None,
                dependencies: vec![],
                raw: String::new(),
            },
        );
        // 60 is a call_insn with a TRUE dependency on 50 -- class 1 under the
        // naive rule, but arm_adjust_cost forces cost 1, so class 3.
        insns.set(
            60.0,
            RtlInsn {
                uid: 60.0,
                kind: InsnKind::CallInsn,
                code: Some("call".to_string()),
                set: None,
                call_target: Some(reg(0.0)),
                dependencies: vec![Dependency { uid: 50.0, kind: DependencyKind::True }],
                raw: String::new(),
            },
        );
        // 70 loads from the stack pointer, TRUE dependent on 50's store --
        // also forced to cost 1 ("no conflict, cached area").
        insns.set(
            70.0,
            RtlInsn {
                uid: 70.0,
                kind: InsnKind::Insn,
                code: Some("set".to_string()),
                set: Some(Set { dest: reg(4.0), src: RtlExpr::Mem { address: Box::new(reg(13.0)) } }),
                call_target: None,
                dependencies: vec![Dependency { uid: 50.0, kind: DependencyKind::True }],
                raw: String::new(),
            },
        );
        (table, insns)
    }

    #[test]
    fn cost_one_shortcuts_demote_both_sides_to_class_three() {
        let (table, insns) = cost_one_fixture();
        let context =
            ScheduleContext { table: &table, last_scheduled_uid: Some(50.0), insns_by_uid: &insns };
        let shortcut = compare_pair(60.0, 70.0, &context);
        assert_ne!(shortcut.tier, Tier::Class);
        assert_eq!((shortcut.winner, shortcut.tier), (60.0, Tier::OriginalOrder));
    }

    #[test]
    fn a_call_insns_true_dependency_does_not_win_on_class() {
        let table = parse_dependence_table(
            ";;       50   174     0     0    50     1    1 - 32   core\t:\n\
             ;;       60   244     0     1    40     2    1 - 32   core\t:\n\
             ;;       80   174     0     0    40     1    1 - 32   core\t:\n",
        );
        let (_, cost_one_insns) = cost_one_fixture();
        let mut insns = InsnMap::new();
        insns.set(50.0, cost_one_insns.get(50.0).expect("50").clone());
        insns.set(60.0, cost_one_insns.get(60.0).expect("60").clone());
        insns.set(
            80.0,
            RtlInsn {
                uid: 80.0,
                kind: InsnKind::Insn,
                code: Some("set".to_string()),
                set: Some(Set { dest: reg(5.0), src: reg(6.0) }),
                call_target: None,
                dependencies: vec![],
                raw: String::new(),
            },
        );
        let context =
            ScheduleContext { table: &table, last_scheduled_uid: Some(50.0), insns_by_uid: &insns };
        assert_ne!(compare_pair(60.0, 80.0, &context).tier, Tier::Class);
    }

    #[test]
    fn an_anti_dependency_is_class_two() {
        let table = parse_dependence_table(
            ";;       60   244     0     1    40     2    1 - 32   core\t:\n\
             ;;       80   174     0     0    40     1    1 - 32   core\t:\n",
        );
        let mut insns = InsnMap::new();
        insns.set(50.0, insn(50.0, vec![]));
        insns.set(60.0, insn(60.0, vec![Dependency { uid: 50.0, kind: DependencyKind::Anti }]));
        insns.set(80.0, insn(80.0, vec![]));
        let context =
            ScheduleContext { table: &table, last_scheduled_uid: Some(50.0), insns_by_uid: &insns };
        let pair = compare_pair(60.0, 80.0, &context);
        assert_eq!((pair.winner, pair.tier), (80.0, Tier::Class));
        assert_eq!(pair.detail, "class 3 (relative to last-scheduled insn 50) beats class 2");
    }

    #[test]
    fn depend_count_breaks_a_class_tie() {
        let table = parse_dependence_table(
            ";;       21   114     0     1    83     1    1 - 32   core\t: 1 2 3\n\
             ;;       34   114     0     1    83     1    1 - 32   core\t: 4\n",
        );
        let empty = InsnMap::new();
        let context = ScheduleContext { table: &table, last_scheduled_uid: None, insns_by_uid: &empty };
        let pair = compare_pair(34.0, 21.0, &context);
        assert_eq!((pair.winner, pair.tier), (21.0, Tier::DependCount));
        assert_eq!(pair.detail, "3 dependents beats 1");
    }

    #[test]
    fn a_null_last_scheduled_uid_prints_as_null() {
        // Unreachable through class_of (a None last-scheduled makes every insn
        // class 3), so this pins the formatter directly.
        assert_eq!(optional_number_text(None), "null");
        assert_eq!(optional_number_text(Some(39.0)), "39");
    }

    #[test]
    fn an_unknown_uid_falls_through_to_original_order() {
        let table = DependenceTable::new();
        let empty = InsnMap::new();
        let context = ScheduleContext { table: &table, last_scheduled_uid: None, insns_by_uid: &empty };
        let pair = compare_pair(90.0, 12.0, &context);
        assert_eq!((pair.winner, pair.loser, pair.tier), (12.0, 90.0, Tier::OriginalOrder));
    }

    #[test]
    fn diagnose_skips_the_winner_and_keeps_ready_list_order() {
        let table = parse_dependence_table(REAL_EXCERPT);
        let empty = InsnMap::new();
        let context = ScheduleContext { table: &table, last_scheduled_uid: None, insns_by_uid: &empty };
        let diagnosis = diagnose(&[43.0, 21.0, 34.0, 21.0], 21.0, &context);
        assert_eq!(diagnosis.winner_uid, 21.0);
        assert_eq!(
            diagnosis.per_rival.iter().map(|pair| pair.loser).collect::<Vec<_>>(),
            vec![43.0, 34.0]
        );
    }

    #[test]
    fn is_mem_from_walks_the_left_operand_chain_only() {
        use rtl_insn::BinaryOp;
        let left = RtlExpr::Mem {
            address: Box::new(RtlExpr::Binary {
                op: BinaryOp::Plus,
                a: Box::new(reg(13.0)),
                b: Box::new(RtlExpr::ConstInt { value: 4.0 }),
            }),
        };
        assert!(is_mem_from(&left, 13.0));
        let right = RtlExpr::Mem {
            address: Box::new(RtlExpr::Binary {
                op: BinaryOp::Plus,
                a: Box::new(RtlExpr::ConstInt { value: 4.0 }),
                b: Box::new(reg(13.0)),
            }),
        };
        assert!(!is_mem_from(&right, 13.0));
        assert!(!is_mem_from(&reg(13.0), 13.0));
    }

    #[test]
    fn insn_map_by_uid_round_trips_a_real_dump() {
        let insns = parse_insns(
            "(insn 15 10 17 (set (reg:SI 3 r3)\n\
             \x20       (plus:SI (reg/v:SI 3 r3)\n\
             \x20           (reg/v:SI 2 r2))) 5 {*thumb_addsi3} (nil))\n",
        );
        let map = InsnMap::by_uid(&insns);
        assert_eq!(map.len(), 1);
        assert_eq!(map.get(15.0).expect("insn 15").uid, 15.0);
        assert!(map.get(16.0).is_none());
    }

    #[test]
    fn json_rendering_matches_the_typescript_shapes() {
        let table = parse_dependence_table(";;       21   114     0     1    83     1    1 - 32   core\t: 88 67");
        assert_eq!(
            json_dependence_table(&table),
            r#"[{"uid":21,"code":114,"depCount":1,"priority":83,"cost":1,"dependents":[88,67]}]"#
        );
        let empty = InsnMap::new();
        let context = ScheduleContext { table: &table, last_scheduled_uid: None, insns_by_uid: &empty };
        let diagnosis = diagnose(&[21.0, 99.0], 21.0, &context);
        assert_eq!(
            json_diagnosis(&diagnosis),
            r#"{"winnerUid":21,"perRival":[{"winner":21,"loser":99,"tier":"original-order","detail":"lower UID (approximates INSN_LUID, gcc's true tie-break key)"}]}"#
        );
    }
}
