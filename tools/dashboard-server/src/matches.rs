// Advisory semantic-owner byte-match scores for the live coverage charts.
//
// A diagnosis already emits the candidate and expected byte streams. Reading
// those ignored files is much cheaper than compiling every semantic owner on
// each dashboard refresh, while a source-mtime check prevents an old result
// from colouring newly edited C.

use std::path::Path;

use coverage_map::boxtree::ByteMatchScores;

fn byte_mismatches(actual: &[u8], expected: &[u8]) -> usize {
    actual.len().abs_diff(expected.len())
        + actual
            .iter()
            .zip(expected)
            .filter(|(left, right)| left != right)
            .count()
}

fn match_fraction(actual: &[u8], expected: &[u8]) -> Option<f64> {
    if expected.is_empty() {
        return None;
    }
    let fraction = 1.0 - byte_mismatches(actual, expected) as f64 / expected.len() as f64;
    Some(fraction.clamp(0.0, 1.0))
}

fn evidence_is_fresh(source: &Path, actual: &Path, expected: &Path) -> bool {
    let modified = |path: &Path| {
        std::fs::metadata(path)
            .and_then(|metadata| metadata.modified())
            .ok()
    };
    let Some(source_time) = modified(source) else {
        return false;
    };
    [actual, expected]
        .iter()
        .all(|path| modified(path).is_some_and(|time| time >= source_time))
}

/// Load fresh ignored diagnosis evidence for flat semantic source owners.
/// Missing, empty, unreadable, or stale evidence deliberately means 0% and is
/// omitted from the map.
pub fn diagnostic_match_scores(root: &Path) -> ByteMatchScores {
    let semantic = root.join("semantic");
    let diagnoses = root.join("out").join("decomp").join("diagnose");
    let mut sources = match std::fs::read_dir(&semantic) {
        Ok(entries) => entries
            .flatten()
            .map(|entry| entry.path())
            .collect::<Vec<_>>(),
        Err(_) => return ByteMatchScores::new(),
    };
    sources.sort();

    let mut scores = ByteMatchScores::new();
    for source in sources {
        if source.extension().and_then(|extension| extension.to_str()) != Some("c") {
            continue;
        }
        let Some(stem) = source.file_stem().and_then(|name| name.to_str()) else {
            continue;
        };
        let directory = diagnoses.join(stem);
        let actual = directory.join(format!("{stem}.actual.bin"));
        let expected = directory.join(format!("{stem}.expected.bin"));
        if !evidence_is_fresh(&source, &actual, &expected) {
            continue;
        }
        let (Ok(actual_bytes), Ok(expected_bytes)) =
            (std::fs::read(&actual), std::fs::read(&expected))
        else {
            continue;
        };
        if let Some(fraction) = match_fraction(&actual_bytes, &expected_bytes) {
            scores.insert(stem.to_string(), fraction);
        }
    }
    scores
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::path::PathBuf;
    use std::time::Duration;

    fn scratch(name: &str) -> PathBuf {
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target")
            .join("fixtures")
            .join(format!("matches-{name}-{}", std::process::id()));
        std::fs::create_dir_all(root.join("semantic")).unwrap();
        std::fs::create_dir_all(root.join("out/decomp/diagnose/08001234")).unwrap();
        root
    }

    fn write_evidence(root: &Path, source: &[u8], actual: &[u8], expected: &[u8]) {
        std::fs::write(root.join("semantic/08001234.c"), source).unwrap();
        std::thread::sleep(Duration::from_millis(2));
        let directory = root.join("out/decomp/diagnose/08001234");
        std::fs::write(directory.join("08001234.actual.bin"), actual).unwrap();
        std::fs::write(directory.join("08001234.expected.bin"), expected).unwrap();
    }

    #[test]
    fn positional_agreement_drives_the_fraction() {
        let root = scratch("fraction");
        write_evidence(&root, b"source", &[1, 2, 9, 4], &[1, 2, 3, 4]);
        assert_eq!(diagnostic_match_scores(&root).get("08001234"), Some(&0.75));
    }

    #[test]
    fn stale_evidence_is_not_displayed() {
        let root = scratch("stale");
        write_evidence(&root, b"old", &[1, 2], &[1, 2]);
        std::thread::sleep(Duration::from_millis(2));
        std::fs::write(root.join("semantic/08001234.c"), b"new").unwrap();
        assert!(!diagnostic_match_scores(&root).contains_key("08001234"));
    }

    #[test]
    fn size_debt_cannot_produce_a_negative_colour_fraction() {
        assert_eq!(match_fraction(&[0; 8], &[1; 2]), Some(0.0));
        assert_eq!(match_fraction(&[], &[]), None);
    }
}
