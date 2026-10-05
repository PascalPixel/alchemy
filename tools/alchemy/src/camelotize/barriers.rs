//! The `barriers` rewrite: empty asm barriers, `asm("");` and its
//! `__asm__`/volatile spellings, each alone on its line, together with the
//! FAKEMATCH comment directly above. Run when the compiler's schedule may no
//! longer need them; `alchemy keep` puts back every file whose bytes move.
use regex::Regex;
use std::sync::OnceLock;

fn barrier() -> &'static Regex {
    static PATTERN: OnceLock<Regex> = OnceLock::new();
    PATTERN.get_or_init(|| {
        Regex::new(r#"^\s*(?:__asm__|asm)(?:\s+volatile|\s+__volatile__)?\s*\(\s*""\s*\);\s*$"#)
            .expect("barrier pattern")
    })
}

/// The text without its tagged empty barriers, and how many went.
pub(crate) fn rewrite(text: &str) -> (String, usize) {
    let mut out: Vec<&str> = Vec::new();
    let mut removed = 0;
    for line in text.split_inclusive('\n') {
        if !barrier().is_match(line.trim_end_matches(['\n', '\r'])) {
            out.push(line);
            continue;
        }
        // Only a barrier its own FAKEMATCH comment explains goes.
        let Some(end) = out.len().checked_sub(1).filter(|&k| out[k].trim_end().ends_with("*/")) else {
            out.push(line);
            continue;
        };
        let Some(start) = (0..=end).rev().find(|&k| out[k].contains("/*")) else {
            out.push(line);
            continue;
        };
        if !out[start..=end].concat().contains("FAKEMATCH") {
            out.push(line);
            continue;
        }
        out.truncate(start);
        removed += 1;
    }
    (out.concat(), removed)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_tagged_barrier_goes_with_its_comment() {
        let text = "    x = 1;\n    /* FAKEMATCH: keep the store first,\n       as the native does. */\n    __asm__ volatile(\"\");\n    y = 2;\n";
        assert_eq!(rewrite(text), ("    x = 1;\n    y = 2;\n".to_string(), 1));
    }

    #[test]
    fn untagged_barriers_and_asm_with_operands_stay() {
        let text = "    /* keep */\n    asm(\"\");\n    /* FAKEMATCH: pin */\n    asm(\"\" : \"+r\"(x));\n";
        assert_eq!(rewrite(text), (text.to_string(), 0));
    }
}
