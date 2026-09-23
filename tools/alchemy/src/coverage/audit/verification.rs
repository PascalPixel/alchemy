//! Independent verification of the automatic executable count.
//!
//! `recon/<game>/metrics/audit-verification.json` records that one output of
//! the overlay audit was verified by someone other than the method's author.
//! It pins that output by the sha256 of its canonical interval serialization,
//! never by addresses, so it cannot become a second address ledger. Freshly
//! generated overlay intervals are authoritative only while they hash to the
//! recorded digest: any change to the method's output returns the count to
//! `?` until it is verified again. Readers recompute the digest from the
//! inventory they are handed, so a copied ledger or edited interval is never
//! scored. A record grants no credit.
use crate::compiler::sha256;
use crate::targets::DecompTarget;
use serde::Deserialize;
use serde_json::{json, Value};
use std::fmt::Write as _;
use std::path::Path;

/// The canonical serialization's name. Another spelling needs another name,
/// and records pinned under this one then stay pending until re-verified.
pub(super) const SERIALIZATION: &str = "alchemy-overlay-intervals-v1";

/// What a record verifies.
const SUBJECT: &str = "overlay executable intervals";

/// The committed record for a target's game.
pub(super) fn record_path(target: DecompTarget) -> String {
    format!("{}/metrics/audit-verification.json", target.recon_dir())
}

/// The canonical text of a candidate's overlay executable intervals, which
/// the verification digest covers: the serialization name and target, then
/// every overlay in id order with its disjoint intervals in address order,
/// each byte under the most specific kind the inventory publishes for it.
///
/// ```text
/// alchemy-overlay-intervals-v1
/// target tbs-en
/// overlay resource_370
/// 02000000 02000038 veneer
/// ```
pub(super) fn canonical_overlay_intervals(document: &Value) -> Result<String, String> {
    let target = document["target"]
        .as_str()
        .ok_or("audit report has no target")?;
    let mut overlays = document["overlays"]
        .as_array()
        .ok_or("audit report has no overlays")?
        .iter()
        .map(|row| {
            let id = row["id"]
                .as_str()
                .filter(|id| !id.is_empty())
                .ok_or("an audited overlay has no id")?;
            Ok((id, super::partition(&row["intervals"])?))
        })
        .collect::<Result<Vec<_>, String>>()?;
    overlays.sort_by(|left, right| left.0.cmp(right.0));
    if let Some(pair) = overlays.windows(2).find(|pair| pair[0].0 == pair[1].0) {
        return Err(format!("audit report lists {} twice", pair[0].0));
    }
    let mut text = format!("{SERIALIZATION}\ntarget {target}\n");
    for (id, spans) in overlays {
        let _ = writeln!(text, "overlay {id}");
        for (start, end, kind) in spans {
            let _ = writeln!(text, "{start:08x} {end:08x} {kind}");
        }
    }
    Ok(text)
}

/// The sha256 of [`canonical_overlay_intervals`].
pub(super) fn overlay_digest(document: &Value) -> Result<String, String> {
    Ok(sha256::hex(
        canonical_overlay_intervals(document)?.as_bytes(),
    ))
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
struct Record {
    format: u8,
    verified: Verified,
    date: String,
    verdict: String,
    runs: Vec<Run>,
    residual: Residual,
    credit: String,
}

/// The verified output: whose, what, its digest and its byte totals.
#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
struct Verified {
    target: String,
    subject: String,
    serialization: String,
    sha256: String,
    overlays: u64,
    decoded_bytes: u64,
    executable_bytes: u64,
    excluded_bytes: u64,
}

/// One independent verification run and what it concluded.
#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
struct Run {
    id: String,
    scope: String,
    result: String,
}

/// Bytes the verification could prove neither code nor data, each named by
/// its overlay and entry rather than as an interval.
#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
struct Residual {
    bytes: u64,
    counted: bool,
    reason: String,
    items: Vec<ResidualItem>,
}

#[derive(Debug, Deserialize)]
#[serde(deny_unknown_fields)]
struct ResidualItem {
    overlay: String,
    entry: String,
    bytes: u64,
}

impl Record {
    fn validate(&self) -> Result<(), String> {
        let verified = &self.verified;
        let hex =
            |text: &str| !text.is_empty() && text.bytes().all(|byte| byte.is_ascii_hexdigit());
        let date = self.date.len() == 10
            && self.date.bytes().enumerate().all(|(index, byte)| {
                if matches!(index, 4 | 7) {
                    byte == b'-'
                } else {
                    byte.is_ascii_digit()
                }
            });
        let residual_room = if self.residual.counted {
            verified.executable_bytes
        } else {
            verified.excluded_bytes
        };
        let mut runs = std::collections::BTreeSet::new();
        let problem = if self.format != 1 {
            "format is not 1".to_string()
        } else if verified.subject != SUBJECT {
            format!("verified.subject is not {SUBJECT:?}")
        } else if verified.serialization != SERIALIZATION {
            format!("verified.serialization is not {SERIALIZATION}")
        } else if verified.sha256.len() != 64
            || !hex(&verified.sha256)
            || verified
                .sha256
                .bytes()
                .any(|byte| byte.is_ascii_uppercase())
        {
            "verified.sha256 is not a lowercase sha256".into()
        } else if verified.executable_bytes + verified.excluded_bytes != verified.decoded_bytes {
            "verified executable and excluded bytes do not sum to the decoded bytes".into()
        } else if !date {
            "date is not YYYY-MM-DD".into()
        } else if self.verdict != "accepted" {
            "only an accepted verification is recorded".into()
        } else if self.credit != "none" {
            "a verification grants no credit".into()
        } else if self.runs.is_empty()
            || self.runs.iter().any(|run| {
                run.id.trim().is_empty()
                    || run.scope.trim().is_empty()
                    || run.result.trim().is_empty()
                    || !runs.insert(run.id.as_str())
            })
        {
            "runs must name distinct runs with their scope and result".into()
        } else if self.residual.reason.trim().is_empty()
            || self
                .residual
                .items
                .iter()
                .map(|item| item.bytes)
                .sum::<u64>()
                != self.residual.bytes
            || self.residual.items.iter().any(|item| {
                item.overlay.trim().is_empty()
                    || item.bytes == 0
                    || !item.entry.strip_prefix("0x").is_some_and(hex)
            })
        {
            "residual items must name an overlay, a 0x entry and bytes summing to residual.bytes"
                .into()
        } else if self.residual.bytes > residual_room {
            "residual bytes exceed the verified bytes they are counted in or excluded from".into()
        } else {
            return Ok(());
        };
        Err(problem)
    }

    /// A record whose digest matches must also state the matching totals:
    /// overlays, decoded, executable and excluded bytes.
    fn totals_match(&self, found: [Option<u64>; 4]) -> Result<(), String> {
        let recorded = &self.verified;
        let expected = [
            recorded.overlays,
            recorded.decoded_bytes,
            recorded.executable_bytes,
            recorded.excluded_bytes,
        ];
        if found != expected.map(Some) {
            return Err(format!(
                "the digest matches but the recorded totals {expected:?} (overlays, decoded, executable, excluded) differ from the audit's {found:?}"
            ));
        }
        Ok(())
    }
}

/// A candidate audit's totals, as its summary states them.
fn candidate_totals(document: &Value) -> [Option<u64>; 4] {
    let summary = &document["summary"];
    [
        document["overlays"]
            .as_array()
            .map(|rows| rows.len() as u64),
        summary["decoded_bytes"].as_u64(),
        summary["executable_bytes"].as_u64(),
        summary["excluded_bytes"].as_u64(),
    ]
}

/// A generated inventory's totals, counted from its own overlay rows and
/// intervals rather than from any total it states.
fn inventory_totals(inventory: &Value) -> Result<[Option<u64>; 4], String> {
    let overlays = inventory["overlays"]
        .as_array()
        .ok_or("the inventory has no overlays")?;
    let mut decoded = Some(0u64);
    let mut executable = 0u64;
    for row in overlays {
        decoded = decoded
            .zip(row["decoded_bytes"].as_u64())
            .map(|(total, bytes)| total + bytes);
        executable += super::partition(&row["intervals"])?
            .iter()
            .map(|(start, end, _)| (end - start) as u64)
            .sum::<u64>();
    }
    Ok([
        Some(overlays.len() as u64),
        decoded,
        Some(executable),
        decoded.and_then(|decoded| decoded.checked_sub(executable)),
    ])
}

/// The overlay provenance an inventory records. Readers require it verbatim
/// for the digest they recompute before scoring the inventory.
fn provenance(path: &str, digest: &str, record: Option<&Record>, verified: bool) -> Value {
    json!({
        "record": path,
        "serialization": SERIALIZATION,
        "overlay_sha256": digest,
        "verified_sha256": record.map(|record| record.verified.sha256.as_str()),
        "verified_on": record.map(|record| record.date.as_str()),
        "runs": record.map(|record| record.runs.iter().map(|run| run.id.as_str()).collect::<Vec<_>>()),
        "overlays": if verified { "verified" } else { "unverified" },
    })
}

/// A game's committed verification record, or `None` when there is none.
fn load(root: &Path, path: &str) -> Result<Option<Record>, String> {
    let text = match std::fs::read(root.join(path)) {
        Ok(text) => text,
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => return Ok(None),
        Err(error) => return Err(format!("{path}: {error}")),
    };
    let record: Record =
        serde_json::from_slice(&text).map_err(|error| format!("{path}: {error}"))?;
    record
        .validate()
        .map_err(|error| format!("{path}: {error}"))?;
    Ok(Some(record))
}

/// Whether a candidate's overlay intervals are the independently verified
/// output, and the provenance the inventory records either way.
pub(super) struct Overlays {
    pub provenance: Value,
    /// Why the intervals are not authoritative, when they are not.
    pub pending: Option<String>,
}

pub(super) fn overlays(
    root: &Path,
    target: DecompTarget,
    document: &Value,
) -> Result<Overlays, String> {
    let path = record_path(target);
    let digest = overlay_digest(document)?;
    let record = load(root, &path)?.filter(|record| record.verified.target == target.id.as_str());
    let pending = match &record {
        None => Some(format!(
            "no independent verification of {}'s overlay intervals is recorded in {path}",
            target.id
        )),
        Some(record) if record.verified.sha256 != digest => Some(format!(
            "the overlay intervals hash to {digest}, not the {} verified in {path}; a changed output counts only after independent re-verification",
            record.verified.sha256
        )),
        Some(record) => {
            record
                .totals_match(candidate_totals(document))
                .map_err(|error| format!("{path}: {error}"))?;
            None
        }
    };
    Ok(Overlays {
        provenance: provenance(&path, &digest, record.as_ref(), pending.is_none()),
        pending,
    })
}

/// Whether a generated inventory's overlays are the independently verified
/// output. The digest is recomputed from the inventory's own intervals, never
/// taken from what it states; its totals and the provenance the writer
/// records must then agree with the committed record. `Err` says why the
/// inventory is not authoritative.
pub(super) fn authenticate(
    root: &Path,
    target: DecompTarget,
    inventory: &Value,
) -> Result<(), String> {
    let path = record_path(target);
    let record = load(root, &path)?
        .filter(|record| record.verified.target == target.id.as_str())
        .ok_or_else(|| {
            format!(
                "no independent verification of {}'s overlay intervals is recorded in {path}",
                target.id
            )
        })?;
    let digest = overlay_digest(inventory)?;
    if digest != record.verified.sha256 {
        return Err(format!(
            "its overlay intervals hash to {digest}, not the {} verified in {path}",
            record.verified.sha256
        ));
    }
    record
        .totals_match(inventory_totals(inventory)?)
        .map_err(|error| format!("{path}: {error}"))?;
    let mut stated = inventory["verification"].clone();
    if let Some(fields) = stated.as_object_mut() {
        fields.remove("main");
    }
    if stated != provenance(&path, &digest, Some(&record), true) {
        return Err(format!(
            "it does not record the provenance of the digest verified in {path}"
        ));
    }
    Ok(())
}

#[cfg(test)]
pub(super) mod tests {
    use super::*;

    /// A synthetic candidate with two overlays listed out of id order and
    /// overlapping classified spans.
    pub(in crate::coverage::audit) fn document() -> Value {
        json!({
            "target": "tla-en",
            "overlays": [
                {"id": "resource_002", "decoded_bytes": 32, "intervals": [
                    {"start": 0x02000010, "end": 0x02000014, "kind": "thumb"},
                    {"start": 0x02000000, "end": 0x02000008, "kind": "veneer"}
                ]},
                {"id": "resource_001", "decoded_bytes": 32, "intervals": [
                    {"start": 0x02000000, "end": 0x0200000c, "kind": "thumb"},
                    {"start": 0x02000004, "end": 0x02000008, "kind": "literal_pool"}
                ]}
            ],
            "summary": {"decoded_bytes": 64, "executable_bytes": 24, "excluded_bytes": 40}
        })
    }

    pub(in crate::coverage::audit) fn record(digest: &str) -> Value {
        json!({
            "format": 1,
            "verified": {
                "target": "tla-en",
                "subject": SUBJECT,
                "serialization": SERIALIZATION,
                "sha256": digest,
                "overlays": 2,
                "decoded_bytes": 64,
                "executable_bytes": 24,
                "excluded_bytes": 40
            },
            "date": "2026-09-23",
            "verdict": "accepted",
            "runs": [{"id": "wf_synthetic", "scope": "every byte", "result": "acceptable"}],
            "residual": {
                "bytes": 4,
                "counted": false,
                "reason": "a lone bx lr nothing names",
                "items": [{"overlay": "resource_002", "entry": "0x0200000c", "bytes": 4}]
            },
            "credit": "none"
        })
    }

    #[test]
    fn canonical_intervals_are_ordered_partitioned_text() {
        let text = canonical_overlay_intervals(&document()).unwrap();
        assert_eq!(
            text,
            "alchemy-overlay-intervals-v1\n\
             target tla-en\n\
             overlay resource_001\n\
             02000000 02000004 thumb\n\
             02000004 02000008 literal_pool\n\
             02000008 0200000c thumb\n\
             overlay resource_002\n\
             02000000 02000008 veneer\n\
             02000010 02000014 thumb\n"
        );
        // The digest is the sha256 of exactly that text.
        assert_eq!(
            overlay_digest(&document()).unwrap(),
            "080e2d13b7aef67e030467a6f64700ba4687f678dd7ec46afd9720bc73c24648"
        );
    }

    #[test]
    fn the_digest_ignores_listing_order_but_not_bytes_or_kinds() {
        let digest = overlay_digest(&document()).unwrap();
        let mut reordered = document();
        reordered["overlays"].as_array_mut().unwrap().reverse();
        for row in reordered["overlays"].as_array_mut().unwrap() {
            row["intervals"].as_array_mut().unwrap().reverse();
        }
        assert_eq!(overlay_digest(&reordered).unwrap(), digest);
        let mut kind = document();
        kind["overlays"][0]["intervals"][0]["kind"] = json!("literal_pool");
        assert_ne!(overlay_digest(&kind).unwrap(), digest);
        let mut byte = document();
        byte["overlays"][0]["intervals"][0]["end"] = json!(0x02000016);
        assert_ne!(overlay_digest(&byte).unwrap(), digest);
        let mut target = document();
        target["target"] = json!("tla-ja");
        assert_ne!(overlay_digest(&target).unwrap(), digest);
        let mut twice = document();
        twice["overlays"][1]["id"] = json!("resource_002");
        assert!(overlay_digest(&twice).unwrap_err().contains("twice"));
    }

    #[test]
    fn a_record_verifies_without_listing_intervals_or_granting_credit() {
        let valid: Record = serde_json::from_value(record(&"a".repeat(64))).unwrap();
        valid.validate().unwrap();
        let mut intervals = record(&"a".repeat(64));
        intervals["verified"]["intervals"] = json!([{"start": 0, "end": 2}]);
        assert!(serde_json::from_value::<Record>(intervals).is_err());
        for (pointer, value) in [
            ("/verdict", json!("rejected")),
            ("/credit", json!("exact")),
            ("/date", json!("23 September")),
            ("/verified/sha256", json!("A".repeat(64))),
            ("/verified/excluded_bytes", json!(41)),
            ("/verified/serialization", json!("other")),
            ("/runs", json!([])),
            ("/residual/bytes", json!(6)),
            ("/residual/items/0/entry", json!("0200000c")),
        ] {
            let mut invalid = record(&"a".repeat(64));
            *invalid.pointer_mut(pointer).unwrap() = value;
            let invalid: Record = serde_json::from_value(invalid).unwrap();
            assert!(invalid.validate().is_err(), "{pointer}");
        }
        // Excluded residual bytes fit the excluded bytes; counted ones must
        // fit the executable bytes.
        let mut residual = record(&"a".repeat(64));
        residual["residual"]["bytes"] = json!(30);
        residual["residual"]["items"][0]["bytes"] = json!(30);
        let excluded: Record = serde_json::from_value(residual.clone()).unwrap();
        excluded.validate().unwrap();
        residual["residual"]["counted"] = json!(true);
        let counted: Record = serde_json::from_value(residual).unwrap();
        assert!(counted.validate().is_err());
    }

    /// The committed records parse, validate and name each game's canonical
    /// edition; they record a verification, never an interval list.
    #[test]
    fn committed_records_are_valid() {
        let root = crate::compiler::routing::root();
        for id in [
            crate::targets::DecompTargetId::TbsEn,
            crate::targets::DecompTargetId::TlaEn,
        ] {
            let target = crate::targets::target_for(id);
            let path = record_path(target);
            let record = load(root, &path)
                .unwrap()
                .unwrap_or_else(|| panic!("{path}"));
            assert_eq!(record.verified.target, target.id.as_str(), "{path}");
            let text = std::fs::read_to_string(root.join(&path)).unwrap();
            let value: Value = serde_json::from_str(&text).unwrap();
            let canonical = crate::compiler::canonical_json::canonical_json(&value);
            assert_eq!(
                text,
                format!("{canonical}\n"),
                "{path} is not canonical JSON"
            );
        }
    }
}
