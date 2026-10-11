pub const SOURCES: &[(&str, &str)] = &[
    ("agscc", "1027438bfe99b14f28d0c5b18a34813a22ad9551"),
];

pub const SOURCE_RECORD: &str = "SOURCE.tsv";

pub fn source_record() -> String {
    SOURCES
        .iter()
        .map(|(repo, commit)| format!("{repo}\t{commit}\n"))
        .collect()
}
