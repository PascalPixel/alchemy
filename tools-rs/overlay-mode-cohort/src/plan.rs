//! `configsOf`, `loadCandidates` and `addressFromSource`.

use crate::collate::collate;
use crate::json::as_number;
use crate::options::{Options, Scope};
use crate::spec::{address_in_basename, candidate_spec};
use mode_sweep::Config;
use search_compiler_modes::{canonical_json, parse_json, resolve_path, Json};

#[derive(Debug, Clone)]
pub struct Candidate {
    pub id: String,
    pub overlay: String,
    pub entry: f64,
    pub offset: f64,
    pub span_bytes: f64,
    /// Always absolute: `loadCandidates` runs `resolve()` on the spec path.
    pub source: String,
}

/// `configsOf(options)`.
///
/// PORT NOTE -- THE DEDUPE IS A `Map`, NOT A `Set`, AND THE DISTINCTION MATTERS.
/// `keyed.set(canonicalJson(config), config)` keeps INSERTION ORDER of first
/// appearance while overwriting the VALUE of a repeat. Singles come first,
/// then pairs, then triples, so a configuration produced by two phases keeps
/// the singles slot and the triples object. Reproduced exactly with a `Vec` of
/// pairs; a `HashMap` would lose the order and a `Vec` with plain `push` would
/// lose the overwrite.
pub fn configs_of(options: &Options) -> Vec<Config> {
    let all = mode_sweep::single_configs();
    // Families are the useful first diagnostic: a hit that only appears under
    // one historical family across unrelated owners is a hypothesis; a pile of
    // arbitrary one-off flags is not.
    let base: Vec<Config> = match options.scope {
        Scope::Families => all
            .into_iter()
            .filter(|config| {
                config.ids.is_empty() || config.ids[0].starts_with("compiler-")
            })
            .collect(),
        Scope::Singles => all,
    };
    let pairs = if options.pairs == 0.0 {
        Vec::new()
    } else {
        // PORT NOTE: `pairConfigs(options.pairs)` is called with the raw number.
        // The guard above already rejected NaN and negatives, so the cast is
        // safe here and only here.
        mode_sweep::pair_configs(Some(options.pairs as usize))
    };
    // Triples stay evidence-gated: `tripleConfigs` only emits combinations
    // where at least two constituents carry the caller-supplied single/pair
    // evidence, so cubic blind permutation is impossible by construction.
    let triples = if options.triple_seeds.is_empty() {
        Vec::new()
    } else {
        // `Number.POSITIVE_INFINITY` for 0 means "no cap", which is `None`.
        let limit = if options.triples == 0.0 {
            None
        } else {
            Some(options.triples as usize)
        };
        mode_sweep::triple_configs(&options.triple_seeds, limit)
    };
    let mut keyed: Vec<(String, Config)> = Vec::new();
    for config in base.into_iter().chain(pairs).chain(triples) {
        let key = canonical_json(&config.to_json());
        match keyed.iter_mut().find(|(existing, _)| *existing == key) {
            Some(slot) => slot.1 = config,
            None => keyed.push((key, config)),
        }
    }
    keyed.into_iter().map(|(_, config)| config).collect()
}

/// `loadCandidates(options)`.
pub fn load_candidates(options: &Options) -> Result<Vec<Candidate>, String> {
    let text = std::fs::read(&options.inventory)
        .map_err(|error| format!("{}: {error}", options.inventory))?;
    // `readFileSync(path, "utf8")` replaces invalid sequences rather than
    // throwing, exactly like `from_utf8_lossy`.
    let document = parse_json(&String::from_utf8_lossy(&text))?;
    let functions = match document.get("functions") {
        Some(Json::Array(items)) => items.clone(),
        // `document.functions ?? []`.
        _ => Vec::new(),
    };
    // `new Map(rows.map(...))`: a repeated id keeps its FIRST position and its
    // LAST value.
    let mut rows: Vec<(String, Json)> = Vec::new();
    for row in functions {
        let id = match row.get("id") {
            Some(Json::String(id)) => id.to_lowercase(),
            // `row.id.toLowerCase()` on a non-string throws in JavaScript. That
            // is a malformed-inventory case; it is surfaced as an error rather
            // than skipped so the two mirrors both fail.
            _ => return Err("inventory row is missing a string id".into()),
        };
        match rows.iter_mut().find(|(existing, _)| *existing == id) {
            Some(slot) => slot.1 = row,
            None => rows.push((id, row)),
        }
    }
    let cwd = std::env::current_dir().map_err(|error| error.to_string())?;
    let mut found: Vec<Candidate> = Vec::new();
    for raw in &options.candidates {
        let spec = candidate_spec(raw)?;
        let row = match rows.iter().find(|(id, _)| *id == spec.id) {
            Some((_, row)) => row,
            None => {
                return Err(format!(
                    "candidate owner is missing from inventory: {}",
                    spec.id
                ))
            }
        };
        let source = resolve_path(&cwd, &spec.source);
        if !std::path::Path::new(&source).exists() {
            return Err(format!("candidate source does not exist: {}", spec.source));
        }
        found.push(Candidate {
            // `{ ...row, source }` keeps the INVENTORY's id, which is the
            // original-case one, not the lowercased lookup key.
            id: match row.get("id") {
                Some(Json::String(id)) => id.clone(),
                _ => spec.id.clone(),
            },
            overlay: match row.get("overlay") {
                Some(Json::String(overlay)) => overlay.clone(),
                _ => String::new(),
            },
            entry: as_number(row.get("entry")).unwrap_or(f64::NAN),
            offset: as_number(row.get("offset")).unwrap_or(f64::NAN),
            span_bytes: as_number(row.get("span_bytes")).unwrap_or(f64::NAN),
            source,
        });
    }
    if let Some(duplicate) = first_duplicate(&found) {
        return Err(format!("duplicate candidate owner: {duplicate}"));
    }
    // `localeCompare`, NOT `str::cmp`. Owner ids contain `_` and `:`, and the
    // collator orders both before the digits and letters that `str::cmp` would
    // interleave them with.
    found.sort_by(|left, right| collate(&left.id, &right.id));
    Ok(found)
}

/// `found.find((candidate, index) => found.findIndex(other => other.id === candidate.id) !== index)`.
///
/// PORT NOTE: this reports the SECOND occurrence, because the first one's own
/// `findIndex` returns its own index. The message therefore names the repeated
/// id, which reads the same either way, but the search order is preserved so a
/// three-way repeat names the same element as the TypeScript.
fn first_duplicate(found: &[Candidate]) -> Option<String> {
    for (index, candidate) in found.iter().enumerate() {
        let first = found
            .iter()
            .position(|other| other.id == candidate.id)
            .unwrap_or(index);
        if first != index {
            return Some(candidate.id.clone());
        }
    }
    None
}

/// `addressFromSource(candidate)`.
pub fn address_from_source(candidate: &Candidate) -> Result<f64, String> {
    let basename = std::path::Path::new(&candidate.source)
        .file_name()
        .map(|name| name.to_string_lossy().into_owned())
        .unwrap_or_default();
    let address = address_in_basename(&basename).unwrap_or(candidate.entry);
    if address != candidate.entry {
        return Err(format!(
            "{}: source address 0x{} does not match inventory entry 0x{}",
            candidate.id,
            js_hex(address),
            js_hex(candidate.entry)
        ));
    }
    Ok(address)
}

/// `value.toString(16)`.
///
/// PORT NOTE: `Number#toString(16)` on a non-integer emits a fractional hex
/// expansion and on NaN emits `"NaN"`. Both are reachable here: `entry` comes
/// out of a JSON file this tool does not write. The integer path is the only
/// one that matters for parity, but the fallbacks keep the message shaped like
/// JavaScript's rather than panicking.
fn js_hex(value: f64) -> String {
    if value.is_nan() {
        return "NaN".into();
    }
    if value.is_infinite() {
        return if value > 0.0 { "Infinity".into() } else { "-Infinity".into() };
    }
    if value.fract() != 0.0 {
        // Not worth reproducing digit-for-digit; it cannot survive the equality
        // test above unless both sides are the same non-integer, in which case
        // the message is never produced.
        return format!("{value}");
    }
    if value < 0.0 {
        format!("-{:x}", (-value) as i64)
    } else {
        format!("{:x}", value as i64)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::options::Parsed;

    fn options(argv: &[&str]) -> Options {
        match crate::options::options_of(&argv.iter().map(|a| a.to_string()).collect::<Vec<_>>())
            .expect("parsed")
        {
            Parsed::Options(options) => *options,
            Parsed::Help => panic!("help"),
        }
    }

    #[test]
    fn families_scope_keeps_the_routed_default_and_the_compiler_families() {
        let configs = configs_of(&options(&["-c", "a:0000=x.c"]));
        assert!(configs.len() >= 5, "got {} configurations", configs.len());
        assert!(configs.iter().any(|config| config.ids.is_empty()));
        assert!(configs
            .iter()
            .all(|config| config.ids.is_empty() || config.ids[0].starts_with("compiler-")));
    }

    #[test]
    fn singles_scope_is_a_superset_of_families_scope() {
        let families = configs_of(&options(&["-c", "a:0000=x.c"]));
        let singles = configs_of(&options(&["-c", "a:0000=x.c", "--scope", "singles"]));
        assert!(singles.len() > families.len());
        for config in &families {
            assert!(singles.contains(config));
        }
    }

    #[test]
    fn seeded_triples_appear_and_contain_their_seeds() {
        let configs = configs_of(&options(&[
            "-c",
            "a:0000=x.c",
            "--triple-seeds",
            "cse-two-insn-immediate-off,sched-low-dest-first",
        ]));
        let triples: Vec<&Config> = configs.iter().filter(|c| c.ids.len() == 3).collect();
        assert!(!triples.is_empty(), "seeded triple planning produced no triples");
        assert!(triples
            .iter()
            .any(|config| config.ids.iter().any(|id| id == "cse-two-insn-immediate-off")));
    }

    #[test]
    fn the_dedupe_keeps_first_appearance_order() {
        // Pairs cannot collide with singles, so the observable property is that
        // adding pairs never reorders the leading singles.
        let families = configs_of(&options(&["-c", "a:0000=x.c"]));
        let with_pairs = configs_of(&options(&["-c", "a:0000=x.c", "--pairs", "4"]));
        assert_eq!(&with_pairs[..families.len()], &families[..]);
        assert!(with_pairs.len() > families.len());
    }

    #[test]
    fn configurations_are_distinct_under_canonical_json() {
        let configs = configs_of(&options(&["-c", "a:0000=x.c", "--pairs", "8"]));
        let mut keys: Vec<String> = configs
            .iter()
            .map(|config| canonical_json(&config.to_json()))
            .collect();
        let total = keys.len();
        keys.sort();
        keys.dedup();
        assert_eq!(keys.len(), total);
    }

    #[test]
    fn hex_formatting_matches_javascripts_lowercase_form() {
        assert_eq!(js_hex(0x020007e0 as f64), "20007e0");
        assert_eq!(js_hex(f64::NAN), "NaN");
    }
}
