use std::fs;
use std::path::Path;

use remaining_survey::{buckets, classify, render};

fn main() {
    let root = Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap();
    let manifest_path = root.join("out/full/asm/manifest.json");
    let raw = fs::read_to_string(&manifest_path)
        .unwrap_or_else(|e| panic!("cannot read {}: {e}", manifest_path.display()));
    let manifest: serde_json::Value =
        serde_json::from_str(&raw).expect("manifest.json is not valid JSON");

    let mut buckets = buckets();
    let regions = manifest["regions"].as_array().cloned().unwrap_or_default();
    for region in regions {
        if region["retention"].as_str() != Some("c_candidate") {
            continue;
        }
        let Some(source) = region["source"].as_str() else { continue };
        // PORT NOTE: `region.source` is relative and the TS resolves it against the
        // process CWD (`existsSync(region.source)`), not against the repo root.
        // Kept as-is so the two agree when run from the same directory.
        let path = Path::new(source);
        if !path.exists() {
            continue;
        }
        let bytes = fs::read(path).unwrap_or_else(|e| panic!("cannot read {source}: {e}"));
        let text = String::from_utf8_lossy(&bytes);
        let stem = Path::new(source)
            .file_name()
            .map(|n| n.to_string_lossy().to_string())
            .unwrap_or_default();
        let stem = stem.strip_suffix(".s").unwrap_or(&stem).to_string();
        buckets[classify(&text)].stems.push(stem);
    }

    print!("{}", render(&buckets));
}
