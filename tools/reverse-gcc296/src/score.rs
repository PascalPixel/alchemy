//! The oracle: compile a candidate and measure how far its code is from the
//! reference.
//!
//! The objective is a SIZE-INDEPENDENT structural distance, not
//! `differing_halfwords`. That distinction is load-bearing. `differing_halfwords`
//! is a positional comparison, so any size delta shifts every later halfword and
//! reshuffles which ones happen to align. It can improve by luck while the code
//! gets structurally worse: one measured pair scored 1801 against 1656, the
//! "better" candidate being 17 instructions further away. Treating the reference
//! size as a target, or ranking different-sized candidates by halfwords, is the
//! single most expensive mistake made on this codebase.

use std::path::{Path, PathBuf};
use std::process::Command;

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Score {
    /// Mismatching instructions after canonicalisation. Lower is better.
    pub distance: u32,
    /// Emitted byte size. Reported, never constrained.
    pub size: u32,
    /// Raw positional halfword mismatch straight from candidate-show. NOT an
    /// objective (it is meaningless across sizes), but zero here with a size
    /// match is the definition of done, so it is the stopping condition.
    pub halfwords: u32,
}

impl Score {
    /// The only thing that actually counts as finished.
    pub fn is_exact(&self, reference_size: u32) -> bool {
        self.halfwords == 0 && self.size == reference_size
    }
}

/// Canonicalise one disassembled instruction so that differences which are not
/// structural stop counting.
///
/// Registers MUST be blinded. Without it a run differing only in register
/// allocation has no matching text and surfaces as a large unmatched block,
/// which reads as evidence of block reordering and is not. Blinding dropped one
/// owner's mismatch count from 974 to 489.
///
/// pc-relative literal offsets must also be blinded, because they shift with any
/// size delta and would otherwise charge size-mismatched candidates a large
/// constant penalty, reintroducing the size bias through the back door.
pub fn canonicalise(instruction: &str) -> String {
    let text = match instruction.find('@') {
        Some(position) => &instruction[..position],
        None => instruction,
    };
    let mut out = String::with_capacity(text.len());
    let bytes: Vec<char> = text.chars().collect();
    let mut index = 0;
    while index < bytes.len() {
        // `[pc, #N]` -> `[pc,#N]`
        if bytes[index] == '[' && text[index..].starts_with("[pc") {
            if let Some(close) = text[index..].find(']') {
                out.push_str("[pc,#N]");
                index += close + 1;
                continue;
            }
        }
        // `0x...` -> `A`
        if bytes[index] == '0' && index + 1 < bytes.len() && bytes[index + 1] == 'x' {
            out.push('A');
            index += 2;
            while index < bytes.len() && bytes[index].is_ascii_hexdigit() {
                index += 1;
            }
            continue;
        }
        // register names -> `R`
        if is_word_start(&bytes, index) {
            let start = index;
            let mut end = index;
            while end < bytes.len() && (bytes[end].is_alphanumeric() || bytes[end] == '_') {
                end += 1;
            }
            let word: String = bytes[start..end].iter().collect();
            if is_register(&word) {
                out.push('R');
            } else {
                out.push_str(&word);
            }
            index = end;
            continue;
        }
        out.push(bytes[index]);
        index += 1;
    }
    out.split_whitespace().collect::<Vec<_>>().join(" ")
}

fn is_word_start(chars: &[char], index: usize) -> bool {
    if !(chars[index].is_alphabetic() || chars[index] == '_') {
        return false;
    }
    index == 0 || !(chars[index - 1].is_alphanumeric() || chars[index - 1] == '_')
}

fn is_register(word: &str) -> bool {
    if matches!(word, "sl" | "fp" | "ip" | "lr" | "sb" | "sp" | "pc") {
        return true;
    }
    if let Some(number) = word.strip_prefix('r') {
        return !number.is_empty()
            && number.chars().all(|c| c.is_ascii_digit())
            && number.parse::<u32>().map(|n| n <= 15).unwrap_or(false);
    }
    false
}

/// Count instructions inside non-equal regions of the best alignment between
/// the two streams. A plain longest-common-subsequence walk, which for streams
/// of a few thousand instructions is cheap enough to run per candidate.
pub fn distance(candidate: &[String], reference: &[String]) -> u32 {
    let n = candidate.len();
    let m = reference.len();
    let mut table = vec![0u32; (n + 1) * (m + 1)];
    let at = |i: usize, j: usize| i * (m + 1) + j;
    for i in (0..n).rev() {
        for j in (0..m).rev() {
            table[at(i, j)] = if candidate[i] == reference[j] {
                table[at(i + 1, j + 1)] + 1
            } else {
                table[at(i + 1, j)].max(table[at(i, j + 1)])
            };
        }
    }
    let common = table[at(0, 0)] as usize;
    // Instructions not on the common subsequence, counted the way a hunk-based
    // diff would: the larger side of each divergence.
    (n.max(m) - common) as u32
}

/// Disassemble a flat binary into canonicalised instruction text.
pub fn disassemble(objdump: &Path, binary: &Path) -> Result<Vec<String>, String> {
    let output = Command::new(objdump)
        .args(["-D", "-b", "binary", "-m", "armv4t", "-M", "force-thumb"])
        .arg(binary)
        .output()
        .map_err(|error| format!("{}: {error}", objdump.display()))?;
    if !output.status.success() {
        return Err(format!("{} failed on {}", objdump.display(), binary.display()));
    }
    let text = String::from_utf8_lossy(&output.stdout);
    let mut instructions = Vec::new();
    for line in text.lines() {
        // `   1a2:\t<bytes>\t<instruction>`
        let mut parts = line.splitn(3, '\t');
        let head = parts.next().unwrap_or("");
        if !head.trim_end().ends_with(':') {
            continue;
        }
        let _bytes = match parts.next() {
            Some(value) => value,
            None => continue,
        };
        if let Some(instruction) = parts.next() {
            instructions.push(canonicalise(instruction));
        }
    }
    Ok(instructions)
}

/// Compile one candidate and score it. Returns `None` when it does not compile,
/// which the driver treats as "not a neighbour" rather than as an error.
pub fn score(
    root: &Path,
    candidate_show: &Path,
    objdump: &Path,
    source: &str,
    stem: &str,
    work: &Path,
) -> Option<Score> {
    std::fs::create_dir_all(work).ok()?;
    // Routing resolves the reference by file stem, so the scratch copy must
    // carry the owner's address as its name.
    let file = work.join(format!("{stem}.c"));
    std::fs::write(&file, source).ok()?;
    let build = work.join("w");

    let output = Command::new(candidate_show)
        .arg(&file)
        .arg("--work")
        .arg(&build)
        .current_dir(root)
        .output()
        .ok()?;
    let text = String::from_utf8_lossy(&output.stdout);
    let first = text.lines().next().unwrap_or("");
    if !first.contains("differing_halfwords") {
        return None;
    }
    let size = field(first, "candidate=")?;
    let halfwords = field(first, "differing_halfwords=")?;

    let candidate = disassemble(objdump, &build.join("candidate.bin")).ok()?;
    let reference = disassemble(objdump, &build.join("reference.bin")).ok()?;
    Some(Score { distance: distance(&candidate, &reference), size, halfwords })
}

fn field(line: &str, key: &str) -> Option<u32> {
    let start = line.find(key)? + key.len();
    let rest = &line[start..];
    let end = rest.find(|c: char| !c.is_ascii_digit()).unwrap_or(rest.len());
    rest[..end].parse().ok()
}

/// Locate the ARM objdump the repository already depends on.
pub fn find_objdump() -> Option<PathBuf> {
    for candidate in [
        "/opt/homebrew/bin/arm-none-eabi-objdump",
        "/usr/local/bin/arm-none-eabi-objdump",
        "/usr/bin/arm-none-eabi-objdump",
    ] {
        let path = PathBuf::from(candidate);
        if path.exists() {
            return Some(path);
        }
    }
    None
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn registers_are_blinded() {
        assert_eq!(canonicalise("movs\tr1, #156"), canonicalise("movs\tr3, #156"));
    }

    #[test]
    fn pc_relative_offsets_are_blinded() {
        assert_eq!(
            canonicalise("ldr\tr1, [pc, #472]"),
            canonicalise("ldr\tr3, [pc, #480]"),
            "otherwise every size delta is charged as structural difference"
        );
    }

    #[test]
    fn comments_are_stripped() {
        assert_eq!(canonicalise("movs\tr3, #156\t@ 0x9c"), "movs R, #156");
    }

    #[test]
    fn distinct_opcodes_still_differ() {
        assert_ne!(canonicalise("adds\tr1, r2, r3"), canonicalise("subs\tr1, r2, r3"));
    }

    #[test]
    fn non_register_words_survive() {
        let text = canonicalise("bl\t0xffffffb0");
        assert!(text.starts_with("bl"), "opcode must not be blinded: {text}");
    }

    #[test]
    fn identical_streams_have_zero_distance() {
        let a: Vec<String> = ["a", "b", "c"].iter().map(|s| s.to_string()).collect();
        assert_eq!(distance(&a, &a), 0);
    }

    #[test]
    fn one_substitution_costs_one() {
        let a: Vec<String> = ["a", "b", "c"].iter().map(|s| s.to_string()).collect();
        let b: Vec<String> = ["a", "x", "c"].iter().map(|s| s.to_string()).collect();
        assert_eq!(distance(&a, &b), 1);
    }

    #[test]
    fn insertion_costs_one() {
        let a: Vec<String> = ["a", "b"].iter().map(|s| s.to_string()).collect();
        let b: Vec<String> = ["a", "x", "b"].iter().map(|s| s.to_string()).collect();
        assert_eq!(distance(&a, &b), 1);
    }

    #[test]
    fn reordering_is_not_free() {
        let a: Vec<String> = ["a", "b", "c", "d"].iter().map(|s| s.to_string()).collect();
        let b: Vec<String> = ["d", "c", "b", "a"].iter().map(|s| s.to_string()).collect();
        assert!(distance(&a, &b) >= 3);
    }
}
