//! The `--self-test` body, mirroring the TypeScript's `selfTest()`.
//!
//! The in-image bias rewrite is the only step between the compiler's assembly
//! and the assembler, so it gets its own test rather than riding on a byte
//! comparison somewhere downstream. Both directions matter: a row that must be
//! biased and, just as much, the four `.word` shapes that must NOT be.

use alchemy_bundle::sha256;

use crate::compile::{bias_in_image_label_words, self_digest};
use crate::regex::Regex;

const ASSEMBLY: [&str; 10] = [
    "\t.text",
    ".L2:",
    "\t.word\t.L4",
    "\t.word\t.L4-.L2",
    "\t.word\tData_02000240",
    "\t.word\t265",
    "\t.word\t.Lelsewhere",
    "\t.word\t.L2",
    ".L4:",
    "\tbx\tlr",
];

pub fn self_test() -> Result<String, String> {
    let assembly = ASSEMBLY.join("\n");
    let result = bias_in_image_label_words(&assembly);
    if result.biased != 2 {
        return Err(format!(
            "bias self-test: expected 2 biased words, got {}",
            result.biased
        ));
    }
    let lines: Vec<&str> = result.text.split('\n').collect();
    if lines[2] != "\t.word\t.L4 + 0x8000" {
        return Err(format!("bias self-test: jump-table row not biased: {}", lines[2]));
    }
    if lines[7] != "\t.word\t.L2 + 0x8000" {
        return Err(format!(
            "bias self-test: table-base pool word not biased: {}",
            lines[7]
        ));
    }
    for index in [3usize, 4, 5, 6] {
        if lines[index] != ASSEMBLY[index] {
            return Err(format!(
                "bias self-test: row {index} must be left alone: {}",
                lines[index]
            ));
        }
    }
    if bias_in_image_label_words("\t.word\t.L4").biased != 0 {
        return Err(
            "bias self-test: a label with no definition in the file must not be biased".to_string(),
        );
    }

    // The cache key must move with THIS SOURCE. Asserted on synthetic input so
    // unrelated source progress cannot break it: the digest of the real source
    // must be a stable 64-hex string, and any edit at all must produce a
    // different one.
    let digest = self_digest();
    if !Regex::new(r"^[0-9a-f]{64}$", "").is_match(&digest) {
        return Err(format!("self-digest is not a sha256: {digest}"));
    }
    if self_digest() != digest {
        return Err("self-digest must be stable within a run".to_string());
    }
    if crate::compile::edited_self_digest() == digest {
        return Err("self-digest must change when the source changes".to_string());
    }
    // And it must never silently degrade to a constant: an empty read is fatal.
    if sha256::hex(&[]) == digest {
        return Err("self-digest collapsed to the empty digest".to_string());
    }

    Ok("self-test=ok (including source-keyed cache digest)".to_string())
}
