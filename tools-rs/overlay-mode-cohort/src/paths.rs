//! `ROOT`, the self digest, and the `hash(...)` helper.

use alchemy_bundle::sha256;
use std::path::{Path, PathBuf};

/// `dirname(dirname(dirname(Bun.fileURLToPath(import.meta.url))))`.
///
/// The TypeScript lives at `tools/overlay/overlay_mode_cohort.ts`, so three
/// `dirname` calls land on the repository root. This crate lives at
/// `tools-rs/overlay-mode-cohort`, which is two levels deep, so it takes two.
///
/// PORT NOTE: commit `b3ab4841b` moved several tools into subfolders and
/// adjusted `ROOT` inconsistently, leaving five tools that exit 0 under
/// `--self-test` and fail with ENOENT on a real run. This tool is NOT one of
/// them: both paths were exercised before porting and both succeed. The depth
/// above is therefore a faithful port of a correct value, not a repair.
pub fn root() -> PathBuf {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR"));
    manifest
        .parent()
        .and_then(Path::parent)
        .expect("crate sits two levels below the repository root")
        .to_path_buf()
}

/// `hash(...parts)`: SHA-256 over each part followed by a NUL separator.
///
/// PORT NOTE: the separator is what makes this injective. Without it
/// `hash("ab", "c")` and `hash("a", "bc")` would collide, and the parts here
/// include arbitrary source bytes. The trailing NUL after the LAST part is also
/// reproduced: the loop emits one per part, including the final one.
pub fn hash(parts: &[&[u8]]) -> String {
    let mut message: Vec<u8> = Vec::new();
    for part in parts {
        message.extend_from_slice(part);
        message.push(0);
    }
    sha256::hex(&message)
}

/// The Rust mirror of `readFileSync(Bun.fileURLToPath(import.meta.url))`: the
/// tool's own source, folded into the compiler signature so a change to the
/// scoring logic invalidates every cached row.
///
/// PORT NOTE -- THIS IS DELIBERATELY NOT THE `.ts` FILE'S BYTES, AND THAT MAKES
/// THE TWO MIRRORS' CACHE KEYS DIFFER BY DESIGN. Hashing the TypeScript here
/// would mean a change to THIS file did not invalidate the cache, which is the
/// exact failure `tools-rs/cache-key-lint` exists to prevent, and sharing a key
/// space between two implementations that can disagree is how one stale
/// `overlay-c-v3` entry once made `main` look red across checkouts. The parity
/// harness therefore normalises `cache_key` and nothing else.
///
/// Note also what is NOT here: no `-vN` literal anywhere. The format number is
/// the numeric `FORMAT` constant the TypeScript already hashes, and every other
/// input is content.
pub fn self_digest() -> String {
    let sources: [&str; 12] = [
        include_str!("lib.rs"),
        include_str!("main.rs"),
        include_str!("aggregate.rs"),
        include_str!("collate.rs"),
        include_str!("json.rs"),
        include_str!("jsops.rs"),
        include_str!("options.rs"),
        include_str!("paths.rs"),
        include_str!("plan.rs"),
        include_str!("score.rs"),
        include_str!("spec.rs"),
        include_str!("verify.rs"),
    ];
    let parts: Vec<&[u8]> = sources.iter().map(|source| source.as_bytes()).collect();
    hash(&parts)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn root_holds_the_tool_this_crate_ports() {
        assert!(root().join("tools-rs/overlay-mode-cohort/Cargo.toml").exists());
    }

    #[test]
    fn the_nul_separator_keeps_the_hash_injective() {
        assert_ne!(hash(&[b"ab", b"c"]), hash(&[b"a", b"bc"]));
        // One part is not the same as that part concatenated with nothing.
        assert_ne!(hash(&[b"a"]), sha256::hex(b"a"));
        assert_eq!(hash(&[b"a"]), sha256::hex(b"a\0"));
    }

    #[test]
    fn the_self_digest_is_stable_and_content_derived() {
        let digest = self_digest();
        assert_eq!(digest.len(), 64);
        assert_eq!(digest, self_digest());
        // No hand-maintained version literal contributes to any cache key. The
        // scan skips comment lines on purpose: the PORT NOTE above NAMES the
        // `overlay-c-v3` incident, and a whole-file `contains` would flag the
        // warning as if it were the defect.
        // The needle is BUILT rather than written, so this test does not match
        // itself. `cache-key-lint`'s own pattern is `-v` followed by digits.
        let needle = format!("-{}", 'v');
        let key_forming_literals = include_str!("lib.rs")
            .lines()
            .chain(include_str!("paths.rs").lines())
            .map(str::trim)
            .filter(|line| !line.starts_with("//"))
            .filter(|line| {
                line.split(&needle)
                    .skip(1)
                    .any(|tail| tail.starts_with(|c: char| c.is_ascii_digit()))
            });
        assert_eq!(key_forming_literals.count(), 0);
    }
}
