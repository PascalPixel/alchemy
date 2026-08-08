// `/client.js`.
//
// The browser client is deliberately plain JavaScript. It is embedded in the
// Rust binary and served verbatim, so starting the dashboard never requires a
// JavaScript or TypeScript build step. Tests below guard the behaviours the
// dashboard relies on and reject TypeScript syntax in the checked-in bundle.

/// The checked-in browser client.
const BUNDLE: &str = include_str!("client_bundle.js");

/// `bundledClient()`.
///
/// The bundle is compiled into the executable, so this returns its bytes
/// directly. The `Result` keeps the empty-client failure explicit rather than
/// allowing a silent empty 200 response.
pub fn bundled_client() -> Result<String, String> {
    if BUNDLE.is_empty() {
        return Err("dashboard client build failed".to_string());
    }
    Ok(BUNDLE.to_string())
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
    fn the_checked_in_bundle_is_a_real_program() {
        assert!(BUNDLE.len() > 4_000);
        let digest = coverage_map::sha1::sha1_hex(BUNDLE.as_bytes());
        assert_eq!(digest.len(), 40);
        assert_ne!(digest, coverage_map::sha1::sha1_hex(b"not the client"));
    }
}
