//! Classify every remaining c_candidate region by what actually blocks it, so the
//! work left is a costed list rather than a count.
//!
//! Why this exists: "808 regions remain" is not a plan, and the two family sizes
//! published on 2026-07-26 were both wrong because they matched on whole-file
//! features instead of resolving operands. Every bucket here is decided from the
//! region's own decoded instruction stream, and pc-relative loads are resolved
//! against the constant pool before anything is counted.
//!
//! Rust port of `tools/decomp/remaining_survey.ts`. The regexes of the original
//! are hand-rolled here (no regex crate); each matcher carries the pattern it
//! reproduces in a comment.

use std::collections::{HashMap, HashSet};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Insn {
    pub off: usize,
    pub text: String,
}

#[derive(Debug, Default, Clone)]
pub struct Decoded {
    pub insns: Vec<Insn>,
    /// Offset -> constant-pool word.
    ///
    /// PORT NOTE: values are `u64` rather than JS `number`. `.4byte` operands are
    /// at most a 32-bit word, so this is lossless where the TS was; a literal too
    /// wide for `u64` is stored as `u64::MAX` instead of a rounded float, which
    /// cannot arise from well-formed `.4byte` input.
    pub pool: HashMap<usize, u64>,
}

/// `/^\t\.4byte\s+(0x[0-9a-f]+)/` against a line — returns the parsed word.
fn match_4byte(line: &str) -> Option<u64> {
    let after = line.strip_prefix("\t.4byte")?;
    let mut chars = after.char_indices();
    // `\s+`: at least one whitespace character.
    let (mut cut, first) = chars.next()?;
    if !first.is_whitespace() {
        return None;
    }
    cut += first.len_utf8();
    for (i, c) in chars {
        if c.is_whitespace() {
            cut = i + c.len_utf8();
        } else {
            break;
        }
    }
    let digits = after[cut..].strip_prefix("0x")?;
    // `[0-9a-f]+` — lowercase only, as in the TS.
    let end = digits
        .find(|c: char| !c.is_ascii_digit() && !('a'..='f').contains(&c))
        .unwrap_or(digits.len());
    if end == 0 {
        return None;
    }
    Some(u64::from_str_radix(&digits[..end], 16).unwrap_or(u64::MAX))
}

/// Thumb is fixed 2-byte except bl; pool words follow the code, 4-aligned.
pub fn decode(text: &str) -> Decoded {
    let mut insns: Vec<Insn> = Vec::new();
    let mut pool: HashMap<usize, u64> = HashMap::new();
    let mut off: usize = 0;
    for raw in text.split('\n') {
        // `raw.replace(/@.*$/, "")` — no /g, no /m: strip from the first `@` on.
        let stripped = match raw.find('@') {
            Some(i) => &raw[..i],
            None => raw,
        };
        let line = stripped.trim_end();
        if let Some(word) = match_4byte(line) {
            if !off.is_multiple_of(4) {
                off += 2;
            }
            pool.insert(off, word);
            off += 4;
            continue;
        }
        // `/^\t[a-z]/`
        let is_insn = line
            .strip_prefix('\t')
            .and_then(|r| r.chars().next())
            .is_some_and(|c| c.is_ascii_lowercase());
        if !is_insn {
            continue;
        }
        insns.push(Insn { off, text: line.trim().to_string() });
        off += if line.starts_with("\tbl\t") { 4 } else { 2 };
    }
    Decoded { insns, pool }
}

/// `/^ldr\t(r\d), \[pc, #(\d+)\]/` — returns (register, displacement).
/// Not anchored at the end, matching the TS.
fn match_pc_load(text: &str) -> Option<(&str, usize)> {
    let rest = text.strip_prefix("ldr\t")?;
    let reg = reg_at(rest, 0)?;
    let rest = &rest[reg.len()..];
    let rest = rest.strip_prefix(", [pc, #")?;
    let end = rest.find(|c: char| !c.is_ascii_digit()).unwrap_or(rest.len());
    if end == 0 {
        return None;
    }
    let imm: usize = rest[..end].parse().ok()?;
    rest[end..].strip_prefix(']')?;
    Some((reg, imm))
}

/// `r\d` starting at byte `at`, returned as a slice of `s`.
fn reg_at(s: &str, at: usize) -> Option<&str> {
    let bytes = s.as_bytes();
    if bytes.len() >= at + 2 && bytes[at] == b'r' && bytes[at + 1].is_ascii_digit() {
        Some(&s[at..at + 2])
    } else {
        None
    }
}

/// A pc-relative load names a pool slot, not an immediate; without resolving it
/// no statement about "the region uses constant X" means anything.
pub fn pool_value(decoded: &Decoded, index: usize) -> Option<u64> {
    let insn = decoded.insns.get(index)?;
    let (_, imm) = match_pc_load(&insn.text)?;
    let base = (insn.off + 4) & !3usize;
    decoded.pool.get(&(base + imm)).copied()
}

/// `/^ands\t(r\d), (r\d)$/`
fn match_ands_pair(text: &str) -> Option<(&str, &str)> {
    let rest = text.strip_prefix("ands\t")?;
    let a = reg_at(rest, 0)?;
    let rest2 = rest[a.len()..].strip_prefix(", ")?;
    let b = reg_at(rest2, 0)?;
    if rest2.len() != b.len() {
        return None;
    }
    Some((a, b))
}

/// `/^[a-z]+\t(r\d),/`
fn match_written(text: &str) -> Option<&str> {
    let mnem_end = text.find(|c: char| !c.is_ascii_lowercase()).unwrap_or(text.len());
    if mnem_end == 0 {
        return None;
    }
    let rest = text[mnem_end..].strip_prefix('\t')?;
    let reg = reg_at(rest, 0)?;
    rest[reg.len()..].strip_prefix(',')?;
    Some(reg)
}

pub fn mask_registers(decoded: &Decoded, constant: u64) -> bool {
    let mut holds: HashSet<&str> = HashSet::new();
    for index in 0..decoded.insns.len() {
        let text = decoded.insns[index].text.as_str();
        if let Some((dst, src)) = match_ands_pair(text) {
            if holds.contains(src) && dst != src {
                return true;
            }
        }
        if let Some((reg, _)) = match_pc_load(text) {
            if pool_value(decoded, index) == Some(constant) {
                holds.insert(reg);
            } else {
                holds.remove(reg);
            }
            continue;
        }
        if let Some(reg) = match_written(text) {
            holds.remove(reg);
        }
    }
    false
}

pub struct Bucket {
    pub name: &'static str,
    pub note: &'static str,
    pub stems: Vec<String>,
}

pub fn buckets() -> Vec<Bucket> {
    [
        ("not emittable: return address in ip", "neither approved compiler emits `mov ip, pc`; needs a retention class or a mode"),
        ("twelve-store record group", "blocked twice over: -mgrouped-dma-store is needed to reach the four-word transform but its three-word grouper eats the run first, and the transform wants regs_ever_live[0] clear"),
        ("DMA descriptor with completion poll", "shape solved 2026-07-26; one converted (08005a78) with -fthumb-split-group-base and -fthumb-group-control-last"),
        ("DMA descriptor, no poll", "grouped-store laws apply; the usual residual is scheduling around the descriptor"),
        ("0xffff used as an AND mask", "u32 locals reproduce it; 8 of these mask twice and need a combine our GCC performs"),
        ("bitfield insert", "an ordinary C bitfield reproduces ldrh/ands/orrs/strh; only nine converted sources use one"),
        ("plain", "no identified construct blocker; cost is drafting time and the usual allocation residuals"),
    ]
    .into_iter()
    .map(|(name, note)| Bucket { name, note, stems: Vec::new() })
    .collect()
}

/// `stmia\tr\d!, {<regs>}` anywhere in `body`.
fn has_stmia(body: &str, regs: &str) -> bool {
    let needle = "stmia\tr";
    let tail = format!("!, {{{regs}}}");
    let bytes = body.as_bytes();
    let mut from = 0;
    while let Some(rel) = body[from..].find(needle) {
        let at = from + rel + needle.len();
        if bytes.get(at).is_some_and(|c| c.is_ascii_digit()) && body[at + 1..].starts_with(&tail) {
            return true;
        }
        from = from + rel + 1;
    }
    false
}

/// `/ldr\tr\d, \[r\d, #8\]\n\tands/`
///
/// PORT NOTE: reproduced literally including the `\n\t`. Because `body` is built
/// from already-trimmed instruction texts joined with a bare `\n`, no tab can
/// follow a newline, so this never fires in practice — the TS behaves the same
/// way, and the bucket "DMA descriptor with completion poll" is consequently
/// always empty. Kept faithful rather than "fixed": changing it would change the
/// published counts.
fn has_poll(body: &str) -> bool {
    let needle = "ldr\tr";
    let bytes = body.as_bytes();
    let mut from = 0;
    while let Some(rel) = body[from..].find(needle) {
        let at = from + rel + needle.len();
        if bytes.get(at).is_some_and(|c| c.is_ascii_digit()) {
            let rest = &body[at + 1..];
            if let Some(rest) = rest.strip_prefix(", [r") {
                if rest.as_bytes().first().is_some_and(|c| c.is_ascii_digit())
                    && rest[1..].starts_with(", #8]\n\tands")
                {
                    return true;
                }
            }
        }
        from = from + rel + 1;
    }
    false
}

/// `/str[hb]?\t/`
fn has_store(body: &str) -> bool {
    let mut from = 0;
    while let Some(rel) = body[from..].find("str") {
        let at = from + rel + 3;
        let rest = &body[at..];
        if rest.starts_with('\t') || rest.starts_with("h\t") || rest.starts_with("b\t") {
            return true;
        }
        from = from + rel + 1;
    }
    false
}

/// Bucket index for one region's assembly source. Mirrors the TS if/else chain.
pub fn classify(text: &str) -> usize {
    let decoded = decode(text);
    let body = decoded.insns.iter().map(|i| i.text.as_str()).collect::<Vec<_>>().join("\n");
    // `/^mov\tip, pc$/m` over a body whose lines are exactly the insn texts.
    if decoded.insns.iter().any(|i| i.text == "mov\tip, pc") {
        0
    } else if has_stmia(&body, "r1, r2, r3, r4") {
        1
    } else if has_stmia(&body, "r0, r1, r2") {
        if has_poll(&body) {
            2
        } else {
            3
        }
    } else if mask_registers(&decoded, 0xffff) {
        4
    } else if body.contains("\torrs\t") && body.contains("\tands\t") && has_store(&body) {
        5
    } else {
        6
    }
}

/// The exact stdout of the tool for a set of filled buckets.
pub fn render(buckets: &[Bucket]) -> String {
    let total: usize = buckets.iter().map(|b| b.stems.len()).sum();
    let mut out = format!("{total} c_candidate regions remain\n\n");
    for bucket in buckets {
        if bucket.stems.is_empty() {
            continue;
        }
        out.push_str(&format!("{:>4}  {}\n", bucket.stems.len(), bucket.name));
        out.push_str(&format!("      {}\n", bucket.note));
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn decode_skips_comments_and_labels() {
        let d = decode("_start:\n\tmovs\tr0, #1 @ comment\n\tbl\tfoo\n\tmovs\tr1, #2\n");
        assert_eq!(d.insns.len(), 3);
        assert_eq!(d.insns[0], Insn { off: 0, text: "movs\tr0, #1".into() });
        // bl is four bytes, so the following insn sits at 6, not 4.
        assert_eq!(d.insns[1], Insn { off: 2, text: "bl\tfoo".into() });
        assert_eq!(d.insns[2], Insn { off: 6, text: "movs\tr1, #2".into() });
        assert!(d.pool.is_empty());
    }

    #[test]
    fn pool_words_align_to_four() {
        // One 2-byte insn leaves off == 2, so the word aligns up to 4.
        let d = decode("\tmovs\tr0, #1\n\t.4byte 0x0000ffff\n\t.4byte 0xdeadbeef\n");
        assert_eq!(d.pool.get(&4), Some(&0xffff));
        assert_eq!(d.pool.get(&8), Some(&0xdead_beef));
        assert_eq!(d.pool.len(), 2);
    }

    #[test]
    fn four_byte_needs_lowercase_hex_and_whitespace() {
        assert_eq!(match_4byte("\t.4byte 0x1a"), Some(0x1a));
        assert_eq!(match_4byte("\t.4byte\t0x1a"), Some(0x1a));
        // No separating whitespace: the TS `\s+` fails too, and the line then
        // falls through to the `^\t[a-z]` insn test, which `.` also fails.
        assert_eq!(match_4byte("\t.4byte0x1a"), None);
        // Uppercase hex is outside `[0-9a-f]`, so only the `1` is taken.
        assert_eq!(match_4byte("\t.4byte 0x1A"), Some(0x1));
        assert_eq!(match_4byte("\t.4byte 0xzz"), None);
        assert_eq!(match_4byte("  .4byte 0x1a"), None);
    }

    #[test]
    fn pool_value_resolves_pc_relative_load() {
        // ldr at off 0: base = (0 + 4) & ~3 = 4, so #4 names offset 8.
        let d = decode("\tldr\tr0, [pc, #4]\n\tmovs\tr1, #0\n\t.4byte 0x00000001\n\t.4byte 0x0000ffff\n");
        assert_eq!(pool_value(&d, 0), Some(0xffff));
        assert_eq!(pool_value(&d, 1), None);
        assert_eq!(pool_value(&d, 9), None);
    }

    #[test]
    fn pool_value_misses_return_none() {
        let d = decode("\tldr\tr0, [pc, #64]\n");
        assert_eq!(pool_value(&d, 0), None);
    }

    #[test]
    fn mask_registers_detects_and_against_loaded_constant() {
        let d = decode("\tldr\tr1, [pc, #4]\n\tmovs\tr2, #3\n\tands\tr0, r1\n\t.4byte 0x0000ffff\n");
        assert!(mask_registers(&d, 0xffff));
        assert!(!mask_registers(&d, 0x00ff));
    }

    #[test]
    fn mask_registers_rejects_self_and_and_clobbered_holder() {
        let same = decode("\tldr\tr1, [pc, #4]\n\tnop\n\tands\tr1, r1\n\t.4byte 0x0000ffff\n");
        assert!(!mask_registers(&same, 0xffff));
        let clobbered =
            decode("\tldr\tr1, [pc, #8]\n\tmovs\tr1, #0\n\tnop\n\tands\tr0, r1\n\t.4byte 0x0000ffff\n");
        assert!(!mask_registers(&clobbered, 0xffff));
        // A second pc-load of a different word drops the register from `holds`.
        let replaced =
            decode("\tldr\tr1, [pc, #12]\n\tldr\tr1, [pc, #8]\n\tnop\n\tands\tr0, r1\n\t.4byte 0x0000ffff\n\t.4byte 0x00000001\n");
        assert!(!mask_registers(&replaced, 0xffff));
    }

    #[test]
    fn ands_pair_is_anchored_at_both_ends() {
        assert_eq!(match_ands_pair("ands\tr0, r1"), Some(("r0", "r1")));
        assert_eq!(match_ands_pair("ands\tr0, r1 "), None);
        assert_eq!(match_ands_pair("ands\tr0, #1"), None);
        assert_eq!(match_written("ands\tr0, r1"), Some("r0"));
        assert_eq!(match_written("bl\tfoo"), None);
        assert_eq!(match_written("stmia\tr0!, {r1}"), None);
    }

    #[test]
    fn classify_ip_move_wins() {
        assert_eq!(classify("\tmov\tip, pc\n\tstmia\tr0!, {r1, r2, r3, r4}\n"), 0);
    }

    #[test]
    fn classify_store_groups() {
        assert_eq!(classify("\tstmia\tr0!, {r1, r2, r3, r4}\n"), 1);
        assert_eq!(classify("\tstmia\tr3!, {r0, r1, r2}\n"), 3);
        // The poll pattern demands a tab after a newline, which a trimmed body
        // can never contain, so this stays in bucket 3 exactly as the TS leaves it.
        assert_eq!(classify("\tstmia\tr3!, {r0, r1, r2}\n\tldr\tr0, [r1, #8]\n\tands\tr0, r2\n"), 3);
    }

    #[test]
    fn classify_mask_and_plain() {
        assert_eq!(classify("\tldr\tr1, [pc, #4]\n\tnop\n\tands\tr0, r1\n\t.4byte 0x0000ffff\n"), 4);
        assert_eq!(classify("\tmovs\tr0, #0\n\tbx\tlr\n"), 6);
        assert_eq!(classify(""), 6);
        // Bucket 5 requires a tab before `orrs`/`ands`, which trimmed lines never
        // provide; ordinary bitfield code therefore lands in `plain`.
        assert_eq!(classify("\tldrh\tr0, [r1]\n\tands\tr0, r2\n\torrs\tr0, r3\n\tstrh\tr0, [r1]\n"), 6);
    }

    #[test]
    fn has_store_matches_only_str_variants() {
        assert!(has_store("strh\tr0, [r1]"));
        assert!(has_store("str\tr0, [r1]"));
        assert!(has_store("strb\tr0, [r1]"));
        assert!(!has_store("strd r0"));
        assert!(!has_store("ldr\tr0, [r1]"));
    }

    #[test]
    fn render_matches_the_reference_layout() {
        let mut bs = buckets();
        bs[3].stems.push("08005a78".into());
        bs[6].stems.extend(["a".to_string(), "b".to_string()]);
        assert_eq!(
            render(&bs),
            format!(
                "3 c_candidate regions remain\n\n   1  {}\n      {}\n   2  {}\n      {}\n",
                bs[3].name, bs[3].note, bs[6].name, bs[6].note
            )
        );
    }

    #[test]
    fn render_with_no_regions() {
        assert_eq!(render(&buckets()), "0 c_candidate regions remain\n\n");
    }
}
