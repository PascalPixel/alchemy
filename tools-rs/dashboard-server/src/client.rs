// `/client.js`.
//
// THE CLIENT-SIDE PROBLEM, stated rather than decided.
//
// The TypeScript serves this route by calling `Bun.build` on
// `tools/metrics/dashboard/client.ts` at request time, caching the result
// against the entry point's mtime. A Rust binary has no TypeScript compiler
// and, per the standing language rule, must not acquire one.
//
// What `client.ts` actually is: 175 lines, of which lines 1-19 are pure type
// aliases and the rest is plain DOM code. It uses NO TypeScript construct with
// runtime semantics -- no enum, no decorator, no parameter property, no
// namespace. `Bun.build` on it is therefore a type ERASER, not a compiler: the
// output is 132 lines of the same code with annotations removed and a single
// `// tools/metrics/dashboard/client.ts` banner prepended.
//
// That erased output is checked in beside this file as `client_bundle.js` and
// served verbatim, so this route reaches BYTE parity with Bun rather than
// merely behavioural parity.
//
// DRIFT is the only real hazard: nothing forces `client_bundle.js` to keep
// tracking `client.ts`. `CLIENT_SOURCE_SHA1` below pins the oracle's bytes and
// the test at the foot of this file fails loudly the moment the TypeScript
// changes. That is a gate that CAN fail, and the test proves it by also
// checking a perturbed input.

use crate::paths;

/// The type-erased client, byte-identical to `Bun.build`'s output.
const BUNDLE: &str = include_str!("client_bundle.js");

/// sha1 of `tools/metrics/dashboard/client.ts` at the time `client_bundle.js`
/// was derived from it.
pub const CLIENT_SOURCE_SHA1: &str = "106fb15e35e3b05cb906b2b44195398b7c8cba43";

/// `bundledClient()`.
///
/// PORT NOTE: the TypeScript memoises against `String(mtime(CLIENT))` and
/// rebuilds when it changes. There is no build here to memoise, and the
/// benchmark must not be flattered by a cache the oracle pays for, so this
/// returns the embedded bytes directly. The oracle's error path -- a failed
/// build becoming a 500 with the build log as the body -- is kept reachable
/// through the `Result`, because a missing client would otherwise be a silent
/// empty 200.
pub fn bundled_client() -> Result<String, String> {
    if BUNDLE.is_empty() {
        return Err("dashboard client build failed".to_string());
    }
    Ok(BUNDLE.to_string())
}

pub fn source_sha1() -> Option<String> {
    let bytes = std::fs::read(paths::client()).ok()?;
    Some(coverage_map::sha1::sha1_hex(&bytes))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_embedded_bundle_satisfies_every_predicate_the_oracle_self_test_asserts() {
        let client = bundled_client().unwrap();
        assert!(client.contains("EventSource"));
        assert!(client.contains("createElement"));
        assert!(client.contains("closest(\"g[aria-label]\")"));
        assert!(client.contains("hover-tooltip"));
        assert!(!client.contains("data-byte-leaf"));
        assert!(!client.contains("data-folder-depth"));
        assert!(!client.contains("legendbar"));
        assert!(!client.contains("titlebar"));
    }

    #[test]
    fn the_bundle_carries_no_typescript_syntax() {
        // If a future regeneration accidentally captured the source instead of
        // the erased output, these would appear.
        assert!(!BUNDLE.contains("type Snapshot"));
        assert!(!BUNDLE.contains(": Promise<"));
        assert!(!BUNDLE.contains("as Snapshot"));
    }

    #[test]
    fn the_drift_gate_can_actually_fail() {
        let actual = source_sha1().expect("client.ts must be readable from root()");
        assert_eq!(
            actual, CLIENT_SOURCE_SHA1,
            "client.ts changed; regenerate src/client_bundle.js and update CLIENT_SOURCE_SHA1"
        );
        // Prove the comparison is not vacuous: a perturbed input must NOT
        // match. A gate that cannot fail is the defect it polices.
        let perturbed = coverage_map::sha1::sha1_hex(b"not the client");
        assert_ne!(perturbed, CLIENT_SOURCE_SHA1);
        assert_eq!(actual.len(), 40);
    }
}
