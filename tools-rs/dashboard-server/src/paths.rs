// The constant block at the head of `dashboard_server.ts`.
//
// BROKEN-TOOL CHECK (commit b3ab4841b): that commit moved this tool from
// `tools/` to `tools/metrics/` and moved `dashboard/` in beside it. The tool
// is NOT one of the five victims: every path here is derived from
// `dirname(import.meta.url)`, so `ROOT = dirname/../..` followed the move by
// construction, and `DASHBOARD = dirname/dashboard` did too. There is no
// literal `join(ROOT, "tools", ...)` segment anywhere in the file. Verified by
// running it both ways: `--self-test` exits 0 and a real run serves all ten
// routes. No corrected behaviour to port.

use std::path::{Path, PathBuf};

/// `ROOT = join(dirname(SOURCE), "..", "..")`.
///
/// PORT NOTE: the TypeScript derives this from the module URL. A Rust binary
/// has no module URL, so this uses `CARGO_MANIFEST_DIR`, which sits at the
/// same depth (`tools-rs/dashboard-server` versus `tools/metrics`). An
/// override exists only so the parity harness can point both mirrors at the
/// same tree without either writing to a tracked path.
pub fn root() -> PathBuf {
    if let Ok(override_root) = std::env::var("ALCHEMY_DASHBOARD_ROOT") {
        if !override_root.is_empty() {
            return PathBuf::from(override_root);
        }
    }
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("crate lives two levels below ROOT")
        .to_path_buf()
}

/// `DASHBOARD = join(dirname(SOURCE), "dashboard")`. The served assets still
/// live beside the TypeScript, because they are the same two files both
/// mirrors serve.
pub fn dashboard() -> PathBuf {
    root().join("tools").join("metrics").join("dashboard")
}

pub fn client() -> PathBuf {
    dashboard().join("client.ts")
}

pub fn styles() -> PathBuf {
    dashboard().join("styles.css")
}

pub fn font() -> PathBuf {
    root().join("assets").join("fonts").join("weyard.otf")
}

/// The TypeScript source, watched for restart.
pub fn source() -> PathBuf {
    root().join("tools").join("metrics").join("dashboard_server.ts")
}

/// `RESTART_FILES = [SOURCE, join(dirname(SOURCE), "coverage_map.ts")]`.
pub fn restart_files() -> Vec<PathBuf> {
    vec![source(), root().join("tools").join("metrics").join("coverage_map.ts")]
}

pub const COVERAGE_DIRECTORIES: [&str; 5] = ["asm", "assets", "metrics", "semantic", "exact"];

pub fn coverage_build_files() -> Vec<PathBuf> {
    vec![
        root().join("out").join("full").join("asm").join("manifest.json"),
        root().join("out").join("full").join("assets").join("manifest.json"),
    ]
}

/// `PAGE_FILES = [CLIENT, STYLES]`, in that order: the join order is the byte
/// order of the `?v=` cache buster.
pub fn page_files() -> Vec<PathBuf> {
    vec![client(), styles()]
}

/// `Number(Bun.env.ALCHEMY_DASHBOARD_PORT ?? 4649)`.
///
/// PORT NOTE: `Number("")` is 0 and `Number("abc")` is NaN, and `Bun.serve`
/// then picks a random free port for NaN. `??` is nullish, so an EMPTY
/// environment variable is NOT replaced by 4649 -- it becomes port 0. Both are
/// reproduced: an unparsable value yields `None`, which binds port 0, and the
/// operating system picks the port exactly as Bun would.
pub fn port() -> Option<u16> {
    let raw = match std::env::var("ALCHEMY_DASHBOARD_PORT") {
        Ok(value) => value,
        Err(_) => return Some(4649),
    };
    let number = js_to_number(&raw);
    if number.is_nan() {
        return None;
    }
    if !(0.0..=65535.0).contains(&number) {
        return None;
    }
    Some(number as u16)
}

/// `Number(string)`: trims JS whitespace, an empty result is 0, anything the
/// grammar rejects is NaN.
///
/// PORT NOTE: `str::trim` uses Unicode `White_Space`, which INCLUDES U+0085
/// and EXCLUDES U+FEFF. JS `Number` trims the reverse set. `is_js_space` from
/// the coverage-map crate already models the JS set, so it is used rather than
/// `trim`.
fn js_to_number(raw: &str) -> f64 {
    let text: &str = {
        let start = raw.find(|c| !coverage_map::js::is_js_space(c)).unwrap_or(raw.len());
        let end = raw.rfind(|c| !coverage_map::js::is_js_space(c)).map_or(start, |i| {
            i + raw[i..].chars().next().map_or(1, char::len_utf8)
        });
        &raw[start..end]
    };
    if text.is_empty() {
        return 0.0;
    }
    // PORT NOTE: `parse::<f64>()` accepts "inf", "NaN" and "1e5" but REJECTS a
    // leading "+"? (it accepts it) and ACCEPTS "infinity" spelled lowercase,
    // which `Number` also accepts only as "Infinity". The dashboard port is an
    // integer in practice; anything else falls to NaN, which binds port 0, so
    // the difference cannot change a served byte. Audited: this is the only
    // `parse::<f64>` in the crate, and there is no `from_str_radix` anywhere.
    if text.eq_ignore_ascii_case("nan") || text.eq_ignore_ascii_case("inf") {
        return f64::NAN;
    }
    text.parse::<f64>().unwrap_or(f64::NAN)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn root_reaches_the_repository_and_finds_the_typescript_oracle() {
        assert!(source().exists(), "the ported TypeScript must be reachable from root()");
        assert!(styles().exists(), "dashboard/styles.css must be reachable from root()");
    }

    #[test]
    fn number_of_empty_is_zero_not_the_default_port() {
        // `??` is nullish, so "" is kept and `Number("")` is 0. A port that
        // "simplifies" this to `unwrap_or(4649)` fails here.
        assert_eq!(js_to_number(""), 0.0);
        assert_eq!(js_to_number("  "), 0.0);
    }

    #[test]
    fn unparsable_ports_are_nan_and_never_a_number() {
        assert!(js_to_number("abc").is_nan());
        assert!(js_to_number("nan").is_nan());
        // `x.is_nan()` and not `x != x` written as a comparison chain: clippy
        // would rewrite a `!(x > 0.0)` here into `x <= 0.0`, which is FALSE
        // for NaN. Named predicate instead.
    }

    #[test]
    fn js_whitespace_trimming_differs_from_rust_trimming() {
        // U+00A0 is JS whitespace, so `Number("\u{a0}5")` is 5.
        assert_eq!(js_to_number("\u{a0}5"), 5.0);
        // U+0085 is Rust `White_Space` but NOT JS whitespace, so this is NaN.
        assert!(js_to_number("\u{85}5").is_nan());
    }
}
