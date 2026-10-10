pub const SOURCES: &[(&str, &str)] = &[
    ("agscc", "a5b70c21a48df6b5e14514d756036073d5bf1725"),
];

pub const SOURCE_RECORD: &str = "SOURCE.tsv";

pub fn source_record() -> String {
    SOURCES
        .iter()
        .map(|(repo, commit)| format!("{repo}\t{commit}\n"))
        .collect()
}
