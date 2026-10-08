pub const SOURCES: &[(&str, &str)] = &[
    ("agbcc", "da598c1d918402c42c0c0d7128ba14567f3175e9"),
    ("agscc", "629474adaee19177f64f9f04a3e56ee606c9b235"),
];

pub const SOURCE_RECORD: &str = "SOURCE.tsv";

pub fn source_record() -> String {
    SOURCES
        .iter()
        .map(|(repo, commit)| format!("{repo}\t{commit}\n"))
        .collect()
}
