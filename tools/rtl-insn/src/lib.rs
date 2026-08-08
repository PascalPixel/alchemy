//! Typed extraction of RTL instructions and expressions from a gcc RTL dump,
//! built on the generic S-expression parser in the `rtl-sexpr` crate. This is
//! the layer `candidate_explain` actually reasons over: instead of grepping
//! dump text for a UID substring, it gets a real [`RtlInsn`] with a typed
//! [`RtlExpr`] tree for its SET destination/source (when the pattern is a
//! plain SET), falling back to an [`RtlExpr::Unknown`] node -- never an error
//! -- for anything this module does not model (PARALLEL bodies beyond a
//! leading SET, inline asm, unrecognized rtx codes).
//!
//! Scope: only the rtx codes that matter for a Thumb -O2 ALU/load/store/call
//! residual are modelled (reg, mem, const_int, symbol_ref, subreg, and the
//! binary arithmetic/logic/shift codes). This is intentionally not a full RTL
//! interpreter.
//!
//! Port of `tools/lib/rtl_insn.ts`.

pub mod jsnum;

use jsnum::{js_number, js_number_to_string};
use rtl_sexpr::{first_atom_deep, head, parse_all, parse_tag, render, SExpr};

/// PORT NOTE: the TS union members carry a `number` typed field. JS has a
/// single number type, so `3` and `3.0` are indistinguishable and a malformed
/// `(reg:SI 3.5 r3)` produces a genuinely fractional register number that the
/// code happily carries (only `Number.isFinite` guards it, never
/// `Number.isSafeInteger`). Storing `f64` reproduces that instead of silently
/// truncating or rejecting.
#[derive(Debug, Clone, PartialEq)]
pub enum RtlExpr {
    Reg { number: f64, name: String },
    ConstInt { value: f64 },
    SymbolRef { name: String },
    LabelRef { target: String },
    Mem { address: Box<RtlExpr> },
    Subreg { inner: Box<RtlExpr> },
    Binary { op: BinaryOp, a: Box<RtlExpr>, b: Box<RtlExpr> },
    Unary { op: String, a: Box<RtlExpr> },
    Call { target: Box<RtlExpr> },
    Unknown { raw: String },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum BinaryOp {
    Plus,
    Minus,
    Mult,
    And,
    Ior,
    Xor,
    Ashift,
    Ashiftrt,
    Lshiftrt,
}

impl BinaryOp {
    pub fn as_str(self) -> &'static str {
        match self {
            BinaryOp::Plus => "plus",
            BinaryOp::Minus => "minus",
            BinaryOp::Mult => "mult",
            BinaryOp::And => "and",
            BinaryOp::Ior => "ior",
            BinaryOp::Xor => "xor",
            BinaryOp::Ashift => "ashift",
            BinaryOp::Ashiftrt => "ashiftrt",
            BinaryOp::Lshiftrt => "lshiftrt",
        }
    }

    fn from_tag(tag: &str) -> Option<BinaryOp> {
        Some(match tag {
            "plus" => BinaryOp::Plus,
            "minus" => BinaryOp::Minus,
            "mult" => BinaryOp::Mult,
            "and" => BinaryOp::And,
            "ior" => BinaryOp::Ior,
            "xor" => BinaryOp::Xor,
            "ashift" => BinaryOp::Ashift,
            "ashiftrt" => BinaryOp::Ashiftrt,
            "lshiftrt" => BinaryOp::Lshiftrt,
            _ => return None,
        })
    }
}

/// LOG_LINKS: the insns THIS insn depends on (predecessors), each tagged with
/// a dependency kind. gcc's dump leaves a true (data) dependency untagged --
/// a bare `insn_list` -- and marks anti/output dependencies explicitly, e.g.
/// `insn_list:REG_DEP_ANTI`. This mirrors `REG_NOTE_KIND(link)==0` meaning
/// "data dependence" in haifa-sched.c's `rank_for_schedule`.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DependencyKind {
    True,
    Anti,
    Output,
}

impl DependencyKind {
    pub fn as_str(self) -> &'static str {
        match self {
            DependencyKind::True => "true",
            DependencyKind::Anti => "anti",
            DependencyKind::Output => "output",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Dependency {
    pub uid: f64,
    pub kind: DependencyKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum InsnKind {
    Insn,
    CallInsn,
    JumpInsn,
}

impl InsnKind {
    pub fn as_str(self) -> &'static str {
        match self {
            InsnKind::Insn => "insn",
            InsnKind::CallInsn => "call_insn",
            InsnKind::JumpInsn => "jump_insn",
        }
    }

    fn from_head(raw_head: &str) -> Option<InsnKind> {
        Some(match raw_head {
            "insn" => InsnKind::Insn,
            "call_insn" => InsnKind::CallInsn,
            "jump_insn" => InsnKind::JumpInsn,
            _ => return None,
        })
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct Set {
    pub dest: RtlExpr,
    pub src: RtlExpr,
}

#[derive(Debug, Clone, PartialEq)]
pub struct RtlInsn {
    pub uid: f64,
    pub kind: InsnKind,
    /// The pattern's own head: `"set"`, `"parallel"`, `"use"`,
    /// `"unspec_volatile"`, ... `None` when the pattern slot is absent or is
    /// not a list with an atom head.
    pub code: Option<String>,
    pub set: Option<Set>,
    pub call_target: Option<RtlExpr>,
    pub dependencies: Vec<Dependency>,
    pub raw: String,
}

fn atom_value(expr: Option<&SExpr>) -> Option<&str> {
    match expr {
        Some(SExpr::Atom(value)) => Some(value.as_str()),
        _ => None,
    }
}

/// PORT NOTE: the TS calls `parseRtlExpr(expr.items[1])` unguarded in the
/// `mem`, `subreg`, `unary` and `call` branches. When that slot is missing the
/// argument is `undefined` and the very first line (`expr.kind === "atom"`)
/// throws a TypeError. A panic here reproduces that: an ill-formed
/// `(mem)` is a hard failure in both implementations, not a silent
/// `unknown` node.
fn required_item<'a>(items: &'a [SExpr], index: usize, tag: &str) -> &'a SExpr {
    items.get(index).unwrap_or_else(|| {
        panic!("rtl-insn: `({tag} ...)` has no operand at index {index}; the TypeScript original throws a TypeError here")
    })
}

/// Strips one leading and one trailing `"`, exactly as the TS
/// `.replace(/^"|"$/g, "")` does.
fn strip_surrounding_quotes(text: &str) -> String {
    let without_leading = text.strip_prefix('"').unwrap_or(text);
    without_leading
        .strip_suffix('"')
        .unwrap_or(without_leading)
        .to_string()
}

pub fn parse_rtl_expr(expr: &SExpr) -> RtlExpr {
    let items: &[SExpr] = match expr {
        SExpr::Atom(value) => {
            let as_number = js_number(value);
            // PORT NOTE: the digit regex is `/^-?\d+$/` -- anchored on both
            // ends, ASCII digits only, and it rejects a leading `+`, a
            // fraction, and the `0x` form that `Number` would otherwise
            // accept. Both guards must hold, so reproduce both.
            let looks_integral = {
                let body = value.strip_prefix('-').unwrap_or(value);
                !body.is_empty() && body.bytes().all(|byte| byte.is_ascii_digit())
            };
            if as_number.is_finite() && looks_integral {
                return RtlExpr::ConstInt { value: as_number };
            }
            return RtlExpr::Unknown { raw: value.clone() };
        }
        SExpr::List { items, .. } => items,
    };

    let Some(raw_head) = head(expr) else {
        return RtlExpr::Unknown { raw: render(expr) };
    };
    let tag = parse_tag(raw_head).tag;

    if tag == "reg" {
        // A missing/non-atom slot becomes `Number(NaN)` in the TS, which fails
        // the isFinite check and falls through to the generic `unknown`.
        let number = atom_value(items.get(1)).map_or(f64::NAN, js_number);
        let name = atom_value(items.get(2))
            .map(str::to_string)
            .unwrap_or_else(|| format!("r{}", js_number_to_string(number)));
        if number.is_finite() {
            return RtlExpr::Reg { number, name };
        }
    }
    if tag == "const_int" {
        let value = atom_value(items.get(1)).map_or(f64::NAN, js_number);
        if value.is_finite() {
            return RtlExpr::ConstInt { value };
        }
    }
    if tag == "symbol_ref" {
        // PORT NOTE: `expr.items[1] ?? expr.items[0]` -- when there is no
        // operand the *head atom itself* is used, so `(symbol_ref:SI)` yields
        // the name "symbol_ref:SI". Preserved rather than "fixed".
        let source = items.get(1).or_else(|| items.first());
        let name = source.and_then(first_atom_deep).unwrap_or("");
        return RtlExpr::SymbolRef { name: strip_surrounding_quotes(name) };
    }
    if tag == "label_ref" {
        let target = match atom_value(items.get(1)) {
            Some(value) => value.to_string(),
            None => first_atom_deep(expr).unwrap_or("").to_string(),
        };
        return RtlExpr::LabelRef { target };
    }
    if tag == "mem" {
        return RtlExpr::Mem { address: Box::new(parse_rtl_expr(required_item(items, 1, "mem"))) };
    }
    if tag == "subreg" {
        return RtlExpr::Subreg { inner: Box::new(parse_rtl_expr(required_item(items, 1, "subreg"))) };
    }
    if let Some(op) = BinaryOp::from_tag(&tag) {
        if items.len() >= 3 {
            return RtlExpr::Binary {
                op,
                a: Box::new(parse_rtl_expr(&items[1])),
                b: Box::new(parse_rtl_expr(&items[2])),
            };
        }
    }
    if matches!(tag.as_str(), "not" | "neg" | "sign_extend" | "zero_extend") {
        return RtlExpr::Unary {
            op: tag.clone(),
            a: Box::new(parse_rtl_expr(required_item(items, 1, &tag))),
        };
    }
    if tag == "call" {
        return RtlExpr::Call { target: Box::new(parse_rtl_expr(required_item(items, 1, "call"))) };
    }
    RtlExpr::Unknown { raw: render(expr) }
}

/// A SET's destination is often a REG, but can be a MEM (a store:
/// `(set (mem ...) (reg ...))`) or a SUBREG. Anything qualifies as long as it
/// is not itself the src side -- extraction here never needs to reject a
/// shape.
///
/// PORT NOTE: the head comparison is against the *raw* head, so a
/// mode-suffixed `set:SI` would not match. That is the TS behaviour and gcc
/// never prints a mode on SET.
fn extract_set(pattern: &SExpr) -> Option<Set> {
    if head(pattern) != Some("set") {
        return None;
    }
    let items = pattern.items();
    if items.len() < 3 {
        return None;
    }
    Some(Set { dest: parse_rtl_expr(&items[1]), src: parse_rtl_expr(&items[2]) })
}

struct Extracted {
    code: Option<String>,
    set: Option<Set>,
    call_target: Option<RtlExpr>,
}

/// A PARALLEL's first vector member is almost always the operative SET (or a
/// call-value SET) for the call/return patterns this tool cares about; the
/// remaining members are USE/CLOBBER bookkeeping. Only the first is surfaced.
fn extract_from_pattern(pattern: &SExpr) -> Extracted {
    let code = head(pattern).map(str::to_string);
    let call_target_of = |set: &Option<Set>| -> Option<RtlExpr> {
        match set {
            Some(Set { src: RtlExpr::Call { target }, .. }) => Some((**target).clone()),
            _ => None,
        }
    };

    match code.as_deref() {
        Some("set") => {
            let set = extract_set(pattern);
            let call_target = call_target_of(&set);
            Extracted { code, set, call_target }
        }
        Some("parallel") => {
            let body = pattern.items().get(1);
            let first = match body {
                Some(node @ SExpr::List { .. }) => node.items().first(),
                _ => None,
            };
            if let Some(first) = first {
                if head(first) == Some("set") {
                    let set = extract_set(first);
                    let call_target = call_target_of(&set);
                    return Extracted { code, set, call_target };
                }
            }
            Extracted { code, set: None, call_target: None }
        }
        Some("call") => {
            // PORT NOTE: the TS throws here if the pattern is an atom. That
            // branch is unreachable in both implementations -- `head()`
            // returns null for an atom, so `code === "call"` already implies a
            // list -- but it is mirrored rather than dropped.
            assert!(pattern.is_list(), "call pattern is an atom");
            let call_target = Some(parse_rtl_expr(required_item(pattern.items(), 1, "call")));
            Extracted { code, set: None, call_target }
        }
        _ => Extracted { code, set: None, call_target: None },
    }
}

/// LOG_LINKS is a hand-rolled linked list, not a normal S-expression list:
/// `(insn_list[:KIND] UID (insn_list[:KIND] UID (nil)))`, printed as gcc's
/// generic INSN_LIST/EXPR_LIST chain format. Walks it iteratively; stops at
/// `(nil)` or anything that does not keep the chain shape, rather than
/// failing -- a REG_NOTES chain (`expr_list:REG_DEAD ...`) can appear in the
/// same position on some insns and must not be misread as dependencies.
fn parse_log_links(chain: Option<&SExpr>) -> Vec<Dependency> {
    let mut dependencies: Vec<Dependency> = Vec::new();
    let mut node = chain;
    while let Some(current @ SExpr::List { .. }) = node {
        let Some(raw_head) = head(current) else { break };
        // The dependency kind rides in the MODE slot, not a `/flag`:
        // `insn_list:REG_DEP_ANTI` -> tag "insn_list", mode "REG_DEP_ANTI".
        let parsed = parse_tag(raw_head);
        if parsed.tag != "insn_list" {
            break;
        }
        let items = current.items();
        let uid = atom_value(items.get(1)).map_or(f64::NAN, js_number);
        if !uid.is_finite() {
            break;
        }
        let kind = match parsed.mode.as_deref() {
            Some("REG_DEP_ANTI") => DependencyKind::Anti,
            Some("REG_DEP_OUTPUT") => DependencyKind::Output,
            _ => DependencyKind::True,
        };
        dependencies.push(Dependency { uid, kind });
        node = items.get(2);
    }
    dependencies
}

/// Parses every top-level `(insn|call_insn|jump_insn UID PREV NEXT PATTERN
/// ...)` statement in a dump, in the order they appear in the file. For a
/// post-scheduling dump (the `-dR` / `.sched2` file this tool reads) that
/// order IS the final emitted instruction order: confirmed by inspection, not
/// documented anywhere, since these forms only occur once in that file, all
/// after the human-readable scheduling trace text (which `parse_all` already
/// strips as `;;` comments).
pub fn parse_insns(dump_text: &str) -> Vec<RtlInsn> {
    let mut insns: Vec<RtlInsn> = Vec::new();
    for form in parse_all(dump_text) {
        let items = match &form {
            SExpr::List { items, .. } if items.len() >= 4 => items,
            _ => continue,
        };
        let Some(raw_head) = head(&form) else { continue };
        let Some(kind) = InsnKind::from_head(raw_head) else { continue };
        let uid = atom_value(items.get(1)).map_or(f64::NAN, js_number);
        if !uid.is_finite() {
            continue;
        }
        // Layout is (KIND UID PREV NEXT PATTERN ...): the pattern is the fifth
        // list member, index 4, not 3 -- easy to mis-count since UID/PREV/NEXT
        // read like a compact triple but the head atom occupies index 0 too.
        let extracted = match items.get(4) {
            Some(pattern) => extract_from_pattern(pattern),
            None => Extracted { code: None, set: None, call_target: None },
        };
        // Layout continues (KIND UID PREV NEXT PATTERN CODE {NAME} LOG_LINKS
        // NOTES...): LOG_LINKS is index 7. It is defensively re-found by shape
        // (head "insn_list", or the literal "nil") if index 7 is not it, since
        // a pattern with no matched template name can shift the field over by
        // one.
        let expected_links = items.get(7);
        let is_links = |node: &SExpr| {
            head(node) == Some("insn_list") || matches!(node, SExpr::Atom(value) if value == "nil")
        };
        let log_links = match expected_links {
            Some(node) if is_links(node) => Some(node),
            // PORT NOTE: JS `Array.prototype.slice(5)` CLAMPS -- on a
            // four-item form (`(insn 17 16 18)`, which passes the `length >= 4`
            // gate) it returns `[]`. Rust's `items[5..]` panics on the same
            // input, so the clamping `get(5..)` is required, not stylistic.
            _ => items.get(5..).unwrap_or(&[]).iter().find(|node| head(node) == Some("insn_list")),
        };
        insns.push(RtlInsn {
            uid,
            kind,
            code: extracted.code,
            set: extracted.set,
            call_target: extracted.call_target,
            dependencies: parse_log_links(log_links),
            raw: render(&form),
        });
    }
    insns
}

/// A CALL's operand is the memory location holding the callee
/// (`(call (mem (symbol_ref "NAME")) args)`), so the readable name sits one
/// MEM layer under `call_target`. Returns `None` for indirect calls (register
/// target) rather than guessing.
pub fn callee_symbol(insn: &RtlInsn) -> Option<&str> {
    let target = insn.call_target.as_ref()?;
    let callee = match target {
        RtlExpr::Mem { address } => address.as_ref(),
        other => other,
    };
    match callee {
        RtlExpr::SymbolRef { name } => Some(name.as_str()),
        _ => None,
    }
}

/// The destination register an insn writes, if any -- the single strongest,
/// cheapest correlation signal against a disassembled instruction's own
/// destination operand.
pub fn dest_register(insn: &RtlInsn) -> Option<f64> {
    match &insn.set.as_ref()?.dest {
        RtlExpr::Reg { number, .. } => Some(*number),
        RtlExpr::Subreg { inner } => match inner.as_ref() {
            RtlExpr::Reg { number, .. } => Some(*number),
            _ => None,
        },
        _ => None,
    }
}

// --- JSON rendering -------------------------------------------------------
//
// WHY: the TS module has no output of its own beyond a self-test, so the only
// way to measure parity over real dumps is to serialize the parsed structures
// identically on both sides. These functions reproduce `JSON.stringify` for
// the exact object literals `rtl_insn.ts` builds, INCLUDING key order, which
// in JS is insertion order and therefore load-bearing for a byte-for-byte
// diff. (This is also why no serde: a derived Serialize would need a
// hand-written impl to keep the tag from being emitted first.)

pub fn json_escape(text: &str, out: &mut String) {
    out.push('"');
    for ch in text.chars() {
        match ch {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\u{08}' => out.push_str("\\b"),
            '\u{0c}' => out.push_str("\\f"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            ch if (ch as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", ch as u32)),
            ch => out.push(ch),
        }
    }
    out.push('"');
}

fn json_expr(expr: &RtlExpr, out: &mut String) {
    match expr {
        RtlExpr::Reg { number, name } => {
            out.push_str("{\"kind\":\"reg\",\"number\":");
            out.push_str(&js_number_to_string(*number));
            out.push_str(",\"name\":");
            json_escape(name, out);
            out.push('}');
        }
        RtlExpr::ConstInt { value } => {
            out.push_str("{\"kind\":\"const_int\",\"value\":");
            out.push_str(&js_number_to_string(*value));
            out.push('}');
        }
        RtlExpr::SymbolRef { name } => {
            out.push_str("{\"kind\":\"symbol_ref\",\"name\":");
            json_escape(name, out);
            out.push('}');
        }
        RtlExpr::LabelRef { target } => {
            out.push_str("{\"kind\":\"label_ref\",\"target\":");
            json_escape(target, out);
            out.push('}');
        }
        RtlExpr::Mem { address } => {
            out.push_str("{\"kind\":\"mem\",\"address\":");
            json_expr(address, out);
            out.push('}');
        }
        RtlExpr::Subreg { inner } => {
            out.push_str("{\"kind\":\"subreg\",\"inner\":");
            json_expr(inner, out);
            out.push('}');
        }
        RtlExpr::Binary { op, a, b } => {
            out.push_str("{\"kind\":\"binary\",\"op\":\"");
            out.push_str(op.as_str());
            out.push_str("\",\"a\":");
            json_expr(a, out);
            out.push_str(",\"b\":");
            json_expr(b, out);
            out.push('}');
        }
        RtlExpr::Unary { op, a } => {
            out.push_str("{\"kind\":\"unary\",\"op\":");
            json_escape(op, out);
            out.push_str(",\"a\":");
            json_expr(a, out);
            out.push('}');
        }
        RtlExpr::Call { target } => {
            out.push_str("{\"kind\":\"call\",\"target\":");
            json_expr(target, out);
            out.push('}');
        }
        RtlExpr::Unknown { raw } => {
            out.push_str("{\"kind\":\"unknown\",\"raw\":");
            json_escape(raw, out);
            out.push('}');
        }
    }
}

fn json_insn(insn: &RtlInsn, out: &mut String) {
    out.push_str("{\"uid\":");
    out.push_str(&js_number_to_string(insn.uid));
    out.push_str(",\"kind\":\"");
    out.push_str(insn.kind.as_str());
    out.push_str("\",\"code\":");
    match &insn.code {
        Some(code) => json_escape(code, out),
        None => out.push_str("null"),
    }
    out.push_str(",\"set\":");
    match &insn.set {
        Some(set) => {
            out.push_str("{\"dest\":");
            json_expr(&set.dest, out);
            out.push_str(",\"src\":");
            json_expr(&set.src, out);
            out.push('}');
        }
        None => out.push_str("null"),
    }
    out.push_str(",\"callTarget\":");
    match &insn.call_target {
        Some(target) => json_expr(target, out),
        None => out.push_str("null"),
    }
    out.push_str(",\"dependencies\":[");
    for (position, dependency) in insn.dependencies.iter().enumerate() {
        if position > 0 {
            out.push(',');
        }
        out.push_str("{\"uid\":");
        out.push_str(&js_number_to_string(dependency.uid));
        out.push_str(",\"kind\":\"");
        out.push_str(dependency.kind.as_str());
        out.push_str("\"}");
    }
    out.push_str("],\"raw\":");
    json_escape(&insn.raw, out);
    out.push('}');
}

/// `JSON.stringify(parseInsns(text))`, byte for byte.
pub fn json_insns(insns: &[RtlInsn]) -> String {
    let mut out = String::from("[");
    for (position, insn) in insns.iter().enumerate() {
        if position > 0 {
            out.push(',');
        }
        json_insn(insn, &mut out);
    }
    out.push(']');
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    /// A trimmed but real excerpt: a plain ALU insn, a store, and a call insn,
    /// exactly as gcc-2.96 prints them in a -dR/.sched2 dump.
    const REAL_EXCERPT: &str = r#"
;; Ready list (t = 4): 19 28
;;		--> scheduling insn <<<28>>> on unit core

(insn 28 15 17 (set (reg:SI 3 r3)
        (plus:SI (reg/v:SI 3 r3)
            (reg/v:SI 2 r2))) 5 {*thumb_addsi3} (nil))

(insn 30 28 32 (set (mem:QI (plus:SI (reg/f:SI 0 r0)
                (const_int 9 [0x9])) [0 S1 A8])
        (reg:SI 3 r3)) 174 {*movqi_insn} (insn_list 28 (insn_list:REG_DEP_ANTI 17 (nil)))
    (expr_list:REG_DEAD (reg:SI 3 r3)
        (nil)))

(call_insn 67 58 71 (parallel [
            (set (reg:SI 0 r0)
                (call (mem:SI (symbol_ref:SI ("Func_02001508")) 0)
                    (const_int 0 [0x0])))
            (use (const_int 0 [0x0]))
            (clobber (reg:SI 14 lr))
        ] ) 245 {*call_value_insn} (nil))
"#;

    // Port of the TS `selfTest()`.
    #[test]
    fn self_test_excerpt() {
        let insns = parse_insns(REAL_EXCERPT);
        assert_eq!(insns.len(), 3, "expected 3 insns");
        let (add, store, call) = (&insns[0], &insns[1], &insns[2]);

        assert_eq!(add.uid, 28.0);
        assert_eq!(add.kind, InsnKind::Insn);
        assert_eq!(dest_register(add), Some(3.0));
        assert!(matches!(
            add.set.as_ref().map(|set| &set.src),
            Some(RtlExpr::Binary { op: BinaryOp::Plus, .. })
        ));

        assert_eq!(store.uid, 30.0);
        assert_eq!(dest_register(store), None, "a store's dest is a MEM");
        assert!(matches!(store.set.as_ref().map(|set| &set.dest), Some(RtlExpr::Mem { .. })));
        assert_eq!(store.dependencies.len(), 2);
        assert_eq!(store.dependencies[0], Dependency { uid: 28.0, kind: DependencyKind::True });
        assert_eq!(store.dependencies[1], Dependency { uid: 17.0, kind: DependencyKind::Anti });
        assert!(add.dependencies.is_empty(), "(nil) LOG_LINKS means no dependencies");

        assert_eq!(call.uid, 67.0);
        assert_eq!(call.kind, InsnKind::CallInsn);
        assert_eq!(callee_symbol(call), Some("Func_02001508"));
    }

    #[test]
    fn store_address_is_a_typed_tree_not_a_string() {
        let insns = parse_insns(REAL_EXCERPT);
        let RtlExpr::Mem { address } = &insns[1].set.as_ref().unwrap().dest else {
            panic!("expected a MEM destination");
        };
        let RtlExpr::Binary { op, a, b } = address.as_ref() else {
            panic!("expected a (plus base offset) address");
        };
        assert_eq!(*op, BinaryOp::Plus);
        assert_eq!(**a, RtlExpr::Reg { number: 0.0, name: "r0".to_string() });
        assert_eq!(**b, RtlExpr::ConstInt { value: 9.0 });
    }

    #[test]
    fn parallel_surfaces_only_the_leading_set() {
        let insns = parse_insns(REAL_EXCERPT);
        let call = &insns[2];
        assert_eq!(call.code.as_deref(), Some("parallel"));
        // The USE and CLOBBER members are bookkeeping and must not leak in.
        assert_eq!(dest_register(call), Some(0.0));
    }

    #[test]
    fn parallel_without_a_leading_set_yields_no_set() {
        let insns = parse_insns("(insn 1 0 2 (parallel [(clobber (reg:SI 0 r0))]) 3 {x} (nil))");
        assert_eq!(insns.len(), 1);
        assert_eq!(insns[0].code.as_deref(), Some("parallel"));
        assert!(insns[0].set.is_none());
        assert!(insns[0].call_target.is_none());
    }

    #[test]
    fn a_bare_call_pattern_has_a_target_but_no_set() {
        let insns =
            parse_insns("(call_insn 9 8 10 (call (mem:SI (symbol_ref:SI (\"memcpy\")) 0) (const_int 0 [0x0])) 1 {*call_insn} (nil))");
        assert_eq!(insns.len(), 1);
        assert_eq!(insns[0].code.as_deref(), Some("call"));
        assert!(insns[0].set.is_none());
        assert_eq!(callee_symbol(&insns[0]), Some("memcpy"));
    }

    #[test]
    fn an_indirect_call_has_no_callee_symbol() {
        let insns = parse_insns("(call_insn 9 8 10 (call (mem:SI (reg:SI 3 r3) 0) (const_int 0 [0x0])) 1 {x} (nil))");
        assert_eq!(callee_symbol(&insns[0]), None);
        assert!(insns[0].call_target.is_some());
    }

    #[test]
    fn subreg_destination_reports_the_inner_register() {
        let insns = parse_insns("(insn 1 0 2 (set (subreg:QI (reg:SI 5 r5) 0) (const_int 1 [0x1])) 3 {x} (nil))");
        assert_eq!(dest_register(&insns[0]), Some(5.0));
    }

    #[test]
    fn unmodelled_rtx_codes_become_unknown_not_an_error() {
        let insns = parse_insns("(insn 1 0 2 (set (reg:SI 0 r0) (unspec:SI [(reg:SI 1 r1)] 3)) 4 {x} (nil))");
        let src = &insns[0].set.as_ref().unwrap().src;
        let RtlExpr::Unknown { raw } = src else { panic!("expected unknown, got {src:?}") };
        assert_eq!(raw, "(unspec:SI [(reg:SI 1 r1)] 3)");
    }

    #[test]
    fn log_links_stops_at_a_reg_notes_chain() {
        // `expr_list:REG_DEAD` sitting where LOG_LINKS would be must yield no
        // dependencies rather than being misread as one.
        let insns = parse_insns(
            "(insn 1 0 2 (set (reg:SI 0 r0) (const_int 1 [0x1])) 3 {x} (expr_list:REG_DEAD (reg:SI 0 r0) (nil)))",
        );
        assert!(insns[0].dependencies.is_empty());
    }

    #[test]
    fn output_dependencies_are_tagged() {
        let insns = parse_insns(
            "(insn 1 0 2 (set (reg:SI 0 r0) (const_int 1 [0x1])) 3 {x} (insn_list 4 (insn_list:REG_DEP_OUTPUT 7 (nil))))",
        );
        assert_eq!(
            insns[0].dependencies,
            vec![
                Dependency { uid: 4.0, kind: DependencyKind::True },
                Dependency { uid: 7.0, kind: DependencyKind::Output },
            ]
        );
    }

    #[test]
    fn a_log_links_chain_whose_first_link_is_tagged_is_dropped_entirely() {
        // PORT NOTE: a real quirk of the original, preserved. The index-7
        // recognizer and the shape-based re-find both compare the RAW head
        // against the literal "insn_list", so `insn_list:REG_DEP_OUTPUT` in
        // the leading position matches neither and the whole chain is
        // discarded -- even though `parseLogLinks` itself would have parsed
        // it (it splits the tag before comparing). Only a chain that STARTS
        // with an untagged (true) link is ever seen.
        let insns = parse_insns(
            "(insn 1 0 2 (set (reg:SI 0 r0) (const_int 1 [0x1])) 3 {x} (insn_list:REG_DEP_OUTPUT 7 (nil)))",
        );
        assert!(insns[0].dependencies.is_empty());
    }

    #[test]
    fn log_links_are_refound_when_the_template_name_is_missing() {
        // No `{name}` field: LOG_LINKS lands at index 6, not 7, and the
        // shape-based re-find has to recover it.
        let shifted = "(insn 1 0 2 (set (reg:SI 0 r0) (const_int 1 [0x1])) 3 (insn_list 99 (nil)))";
        let insns = parse_insns(shifted);
        assert_eq!(insns[0].dependencies, vec![Dependency { uid: 99.0, kind: DependencyKind::True }]);
    }

    #[test]
    fn non_insn_forms_and_short_forms_are_skipped() {
        let insns = parse_insns("(note 1 0 2) (barrier 3 2 4 (x)) (code_label 5 4 6 (x)) (insn 7 6 8 (use (reg:SI 0 r0)) -1 (nil))");
        assert_eq!(insns.len(), 1);
        assert_eq!(insns[0].uid, 7.0);
        assert_eq!(insns[0].code.as_deref(), Some("use"));
        assert!(insns[0].set.is_none());
    }

    #[test]
    fn a_four_item_insn_form_does_not_overrun_the_log_links_search() {
        // Regression: `(insn 17 16 18)` clears the `length >= 4` gate but has
        // no index 5, and the shape-based LOG_LINKS re-find slices from 5.
        let insns = parse_insns("(insn 17 16 18)");
        assert_eq!(insns.len(), 1);
        assert_eq!(insns[0].uid, 17.0);
        assert_eq!(insns[0].code, None);
        assert!(insns[0].set.is_none());
        assert!(insns[0].dependencies.is_empty());
    }

    #[test]
    fn a_flagged_insn_head_is_not_treated_as_an_insn() {
        // PORT NOTE: INSN_KINDS is checked against the RAW head, so gcc's
        // `insn/f` (frame-related) form is skipped by both implementations.
        assert!(parse_insns("(insn/f 1 0 2 (set (reg:SI 0 r0) (const_int 1 [0x1])) 3 {x} (nil))").is_empty());
    }

    #[test]
    fn symbol_ref_without_the_extra_paren_also_works() {
        let insns = parse_insns("(call_insn 9 8 10 (call (mem:SI (symbol_ref:SI \"plain\") 0) (const_int 0 [0x0])) 1 {x} (nil))");
        assert_eq!(callee_symbol(&insns[0]), Some("plain"));
    }

    #[test]
    fn label_ref_target_and_unary_codes() {
        let insns = parse_insns("(jump_insn 1 0 2 (set (pc) (label_ref 42)) 3 {x} (nil))");
        assert_eq!(insns[0].kind, InsnKind::JumpInsn);
        let src = &insns[0].set.as_ref().unwrap().src;
        assert_eq!(*src, RtlExpr::LabelRef { target: "42".to_string() });

        let extended = parse_insns("(insn 1 0 2 (set (reg:SI 0 r0) (zero_extend:SI (reg:QI 1 r1))) 3 {x} (nil))");
        let RtlExpr::Unary { op, a } = &extended[0].set.as_ref().unwrap().src else {
            panic!("expected a unary node");
        };
        assert_eq!(op, "zero_extend");
        assert_eq!(**a, RtlExpr::Reg { number: 1.0, name: "r1".to_string() });
    }

    #[test]
    fn a_negative_const_int_atom_is_a_const_int_but_a_hex_atom_is_unknown() {
        // The atom branch is gated on `/^-?\d+$/`, so `0x9` standing alone is
        // NOT a const_int even though `Number("0x9")` is 9.
        assert_eq!(parse_rtl_expr(&rtl_sexpr::atom("-7")), RtlExpr::ConstInt { value: -7.0 });
        assert_eq!(parse_rtl_expr(&rtl_sexpr::atom("0x9")), RtlExpr::Unknown { raw: "0x9".to_string() });
        assert_eq!(parse_rtl_expr(&rtl_sexpr::atom("+7")), RtlExpr::Unknown { raw: "+7".to_string() });
        // ...but inside `(const_int ...)` there is no regex, so hex parses.
        let hex = parse_rtl_expr(&rtl_sexpr::parse_all("(const_int 0x10)")[0]);
        assert_eq!(hex, RtlExpr::ConstInt { value: 16.0 });
    }

    #[test]
    fn a_reg_without_a_name_slot_synthesises_one() {
        let expr = parse_rtl_expr(&rtl_sexpr::parse_all("(reg:SI 12)")[0]);
        assert_eq!(expr, RtlExpr::Reg { number: 12.0, name: "r12".to_string() });
    }

    #[test]
    fn a_reg_with_an_unparseable_number_falls_through_to_unknown() {
        let expr = parse_rtl_expr(&rtl_sexpr::parse_all("(reg:SI rx r3)")[0]);
        assert_eq!(expr, RtlExpr::Unknown { raw: "(reg:SI rx r3)".to_string() });
    }

    #[test]
    fn json_matches_javascript_key_order_and_number_formatting() {
        let insns = parse_insns("(insn 1 0 2 (set (reg:SI 0 r0) (const_int -3 [0xfffffffd])) 4 {x} (insn_list 9 (nil)))");
        let json = json_insns(&insns);
        assert_eq!(
            json,
            concat!(
                r#"[{"uid":1,"kind":"insn","code":"set","set":{"dest":{"kind":"reg","number":0,"name":"r0"},"#,
                r#""src":{"kind":"const_int","value":-3}},"callTarget":null,"#,
                r#""dependencies":[{"uid":9,"kind":"true"}],"#,
                r#""raw":"(insn 1 0 2 (set (reg:SI 0 r0) (const_int -3 [0xfffffffd])) 4 {x} (insn_list 9 (nil)))"}]"#,
            )
        );
    }

    #[test]
    fn json_escapes_control_characters_and_quotes() {
        let mut out = String::new();
        json_escape("a\"b\\c\nd\u{1}e", &mut out);
        assert_eq!(out, "\"a\\\"b\\\\c\\nd\\u0001e\"");
    }

    #[test]
    fn empty_input_yields_no_insns() {
        assert!(parse_insns("").is_empty());
        assert!(parse_insns(";; only a scheduling trace\n;; more trace\n").is_empty());
        assert_eq!(json_insns(&[]), "[]");
    }

    #[test]
    fn real_dump_files_parse_into_insns() {
        let root = std::path::Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap();
        let dump = root.join("work/rtl/08006088.c.18.greg");
        let Ok(text) = std::fs::read_to_string(&dump) else {
            // A work artifact; skip rather than fail if absent.
            return;
        };
        let insns = parse_insns(&text);
        assert!(insns.len() > 10, "expected many insns, got {}", insns.len());
        assert!(
            insns.iter().any(|insn| insn.set.is_some()),
            "expected at least one modelled SET"
        );
    }
}
