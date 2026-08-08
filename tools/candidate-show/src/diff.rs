//! `differingOffsets` and `selfTest`.

use std::collections::BTreeSet;

/// `differingOffsets(actual, expected)`.
///
/// The set is returned ordered because the only consumers are `.size` and
/// `.has`, both order-independent; a `BTreeSet` is used rather than a `HashSet`
/// so that any future iteration is deterministic. A `HashSet` would be a
/// silent nondeterminism trap the day someone prints it.
///
/// PORT NOTE -- `readUInt16LE` THROWS a `RangeError` past the end of the
/// buffer. The `at + 2 <= shared` guard is what keeps it in range, so the loop
/// bound is copied exactly rather than rewritten as a `chunks_exact(2)`, which
/// would silently drop a trailing odd byte instead.
///
/// PORT NOTE (reproduced defect) -- when both buffers have the SAME ODD length,
/// `shared & ~1` is `shared - 1`, which is `< Math.max(...)`, so the final
/// halfword offset is reported as differing even though the buffers are
/// identical. Compiled Thumb output is always even-length, so this never fires
/// in practice. It is reproduced, not fixed. See
/// `equal_odd_length_buffers_report_a_phantom_difference`.
pub fn differing_offsets(actual: &[u8], expected: &[u8]) -> BTreeSet<usize> {
    let mut differing = BTreeSet::new();
    let shared = std::cmp::min(actual.len(), expected.len());
    let longest = std::cmp::max(actual.len(), expected.len());
    let mut at = 0usize;
    while at + 2 <= shared {
        if read_u16_le(actual, at) != read_u16_le(expected, at) {
            differing.insert(at);
        }
        at += 2;
    }
    let mut at = shared & !1usize;
    while at < longest {
        differing.insert(at);
        at += 2;
    }
    differing
}

fn read_u16_le(data: &[u8], at: usize) -> u16 {
    u16::from(data[at]) | (u16::from(data[at + 1]) << 8)
}

/// `--self-test`. Returns the line the TypeScript prints.
pub fn self_test() -> Result<String, String> {
    let same = [0x00u8, 0xb5, 0x01, 0x20];
    if !differing_offsets(&same, &same).is_empty() {
        return Err("candidate show self-test: identical buffers must not differ".to_string());
    }
    let other = [0x00u8, 0xb5, 0x02, 0x20];
    let found = differing_offsets(&same, &other);
    if found.len() != 1 || !found.contains(&2) {
        return Err("candidate show self-test: wrong differing offset".to_string());
    }
    // 長さが違うときは、短い側を越えた分をすべて差分として数える。
    if differing_offsets(&same, &same[..2]).len() != 1 {
        return Err("candidate show self-test: trailing bytes must count as differing".to_string());
    }
    Ok("candidate show self-test passed".to_string())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn self_test_matches_the_typescript_line() {
        assert_eq!(self_test().unwrap(), "candidate show self-test passed");
    }

    #[test]
    fn halfwords_are_little_endian_and_only_the_changed_one_is_marked() {
        let left = [0x00u8, 0xb5, 0x01, 0x20];
        let right = [0x00u8, 0xb5, 0x01, 0x21];
        assert_eq!(differing_offsets(&left, &right), BTreeSet::from([2]));
        let swapped = [0xb5u8, 0x00, 0x01, 0x20];
        assert_eq!(differing_offsets(&left, &swapped), BTreeSet::from([0]));
    }

    #[test]
    fn trailing_bytes_past_the_shorter_buffer_all_count() {
        let long = [0u8; 8];
        let short = [0u8; 2];
        assert_eq!(differing_offsets(&long, &short), BTreeSet::from([2, 4, 6]));
        assert_eq!(differing_offsets(&short, &long), BTreeSet::from([2, 4, 6]));
    }

    #[test]
    fn an_odd_trailing_byte_counts_once() {
        let long = [0u8; 3];
        let short = [0u8; 2];
        assert_eq!(differing_offsets(&long, &short), BTreeSet::from([2]));
    }

    #[test]
    fn equal_odd_length_buffers_report_a_phantom_difference() {
        // Reproduced TypeScript defect, not a Rust slip.
        let odd = [1u8, 2, 3];
        assert_eq!(differing_offsets(&odd, &odd), BTreeSet::from([2]));
    }

    #[test]
    fn empty_buffers_do_not_panic() {
        assert!(differing_offsets(&[], &[]).is_empty());
        assert_eq!(differing_offsets(&[], &[0u8, 1]), BTreeSet::from([0]));
    }
}
