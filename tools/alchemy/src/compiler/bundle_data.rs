/// The compiler sources every installed toolchain is built from, as
/// `make compiler-source-check` pins them: stock pret/agbcc and agscc.
/// Executable bytes vary with the host and its own C compiler, so a
/// toolchain is admitted by the source it was built from on any system;
/// the twelve editions judge what it produces (Pascal, 2026-10-05).
pub const SOURCES: &[(&str, &str)] = &[
    ("agbcc", "da598c1d918402c42c0c0d7128ba14567f3175e9"),
    ("agscc", "c7a493c13e16734a73d15a1162f1aadd18beae0a"),
];

/// The record a source build leaves beside the executables it installs.
pub const SOURCE_RECORD: &str = "SOURCE.tsv";

pub fn source_record() -> String {
    SOURCES
        .iter()
        .map(|(repo, commit)| format!("{repo}\t{commit}\n"))
        .collect()
}
