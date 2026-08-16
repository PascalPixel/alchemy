//! Routing-necessity measurement for overlay owners.
//!
//! NOT part of the legacy port. The ported pipeline is main-image shaped: it
//! slices the ROM at an owner's address and links candidates at `ROM_BASE`.
//! Overlay owners live at EWRAM addresses inside compressed assets, so neither
//! applies, and this module answers a narrower question through the real
//! overlay compile path instead.
//!
//! The question is "is this owner's routing load-bearing": compile it once as
//! routed, once with those same flags removed, and compare the emitted bytes.
//! No reference bytes are needed, because the two compiles are compared with
//! each other rather than with the cartridge.
//!
//! Doing this outside `compile_overlay_c` does not work. Two earlier attempts
//! approximated it with a bare `cpp`/`cc1` pair over a hand-written baseline
//! and both reported ~450 owners as unrouted no-ops; deleting those entries
//! broke `resource_3ce` every time. The overlay path links the owner at its own
//! address and resolves its undefined symbols into generated stubs, and a
//! pool-placement mode's effect only appears once the owner is placed. The
//! approximation cannot see it.

use std::path::{Path, PathBuf};

use alchemy_plan::plan::CompilerFlagMutations;
use alchemy_routing::routing::{cflags_for_source, cflags_for_target, CompilerTarget};
use overlay_disasm::compile::{compile_overlay_mutated, Compiled};

/// One overlay owner and the routed flags it carries over the baseline.
#[derive(Debug, Clone)]
pub struct OverlayOwner {
    pub source: String,
    pub overlay: String,
    pub extras: Vec<String>,
}

/// What the two compiles said about one owner.
#[derive(Debug, Clone)]
pub struct OverlayVerdict {
    pub source: String,
    pub extras: Vec<String>,
    /// `true` when removing the routed flags changes the emitted bytes.
    pub load_bearing: bool,
    pub routed_len: usize,
    pub stripped_len: usize,
    pub first_difference: Option<usize>,
    pub note: Option<String>,
}

fn overlay_of(source: &str) -> Option<String> {
    let name = Path::new(source).file_name()?.to_str()?;
    let rest = name.strip_prefix("resource_")?;
    let (id, _) = rest.split_once("_c_")?;
    Some(format!("resource_{id}"))
}

/// Every adopted overlay owner, whether or not it carries routed flags.
///
/// `routed_overlay_owners` answers "is this owner's routing load-bearing" and so
/// only needs owners that have routing. Promotion asks the opposite question --
/// "would adding this flag to the overlay baseline break an owner that does not
/// have it" -- and that population is every overlay owner, most of which are
/// currently flag-free.
pub fn all_overlay_owners(root: &Path) -> Result<Vec<OverlayOwner>, String> {
    let baseline: std::collections::BTreeSet<String> =
        cflags_for_target(CompilerTarget::Gs1).into_iter().collect();
    let directory = root.join("exact");
    let entries = std::fs::read_dir(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?;
    let mut owners = Vec::new();
    for entry in entries.filter_map(Result::ok) {
        let path = entry.path();
        if path.extension().and_then(|e| e.to_str()) != Some("c") {
            continue;
        }
        let Some(name) = path.file_name().and_then(|n| n.to_str()) else { continue };
        if !name.starts_with("resource_") {
            continue;
        }
        let source = format!("exact/{name}");
        let Some(overlay) = overlay_of(&source) else { continue };
        let extras: Vec<String> = cflags_for_source(&source)
            .into_iter()
            .filter(|flag| !baseline.contains(flag))
            .collect();
        owners.push(OverlayOwner { source, overlay, extras });
    }
    owners.sort_by(|a, b| a.source.cmp(&b.source));
    Ok(owners)
}

/// Compile one owner as routed, and again with `added` on top.
///
/// Bytes that change mean this owner would stop reproducing the cartridge if
/// `added` became part of the overlay baseline. No reference bytes are needed:
/// the owner is byte-exact as routed by construction, so "changed" is "broke".
pub fn evaluate_addition(
    owner: &OverlayOwner,
    added: &[String],
    root: &Path,
    work: &Path,
) -> OverlayVerdict {
    let source = root.join(&owner.source);
    let routed = compile_overlay_mutated(
        &source,
        &work.join("routed"),
        &owner.overlay,
        None,
        &CompilerFlagMutations { add_flags: Vec::new(), remove_flags: Vec::new() },
    );
    let promoted = compile_overlay_mutated(
        &source,
        &work.join("promoted"),
        &owner.overlay,
        None,
        &CompilerFlagMutations { add_flags: added.to_vec(), remove_flags: Vec::new() },
    );
    match (routed, promoted) {
        (Ok(Compiled { data: a, .. }), Ok(Compiled { data: b, .. })) => OverlayVerdict {
            source: owner.source.clone(),
            extras: owner.extras.clone(),
            load_bearing: a != b,
            routed_len: a.len(),
            stripped_len: b.len(),
            first_difference: first_difference(&a, &b),
            note: None,
        },
        (routed, promoted) => {
            let note = match (&routed, &promoted) {
                (Err(error), _) => format!("routed compile failed: {error}"),
                (_, Err(error)) => format!("promoted compile failed: {error}"),
                _ => unreachable!(),
            };
            OverlayVerdict {
                source: owner.source.clone(),
                extras: owner.extras.clone(),
                load_bearing: true,
                routed_len: 0,
                stripped_len: 0,
                first_difference: None,
                note: Some(note),
            }
        }
    }
}

/// Compile one owner as routed, and again with `stripped` removed.
///
/// `load_bearing` false means the owner does not need that flag: it is a
/// redundant grant that can be retired without reconstructing anything. This is
/// the per-flag question, which `evaluate` cannot answer because it strips an
/// owner's whole extras set at once and so cannot see a redundant flag sitting
/// beside a load-bearing one.
pub fn evaluate_removal(
    owner: &OverlayOwner,
    stripped: &[String],
    root: &Path,
    work: &Path,
) -> OverlayVerdict {
    let source = root.join(&owner.source);
    let routed = compile_overlay_mutated(
        &source,
        &work.join("routed"),
        &owner.overlay,
        None,
        &CompilerFlagMutations { add_flags: Vec::new(), remove_flags: Vec::new() },
    );
    let reduced = compile_overlay_mutated(
        &source,
        &work.join("reduced"),
        &owner.overlay,
        None,
        &CompilerFlagMutations {
            add_flags: Vec::new(),
            remove_flags: stripped.to_vec(),
        },
    );
    match (routed, reduced) {
        (Ok(Compiled { data: a, .. }), Ok(Compiled { data: b, .. })) => OverlayVerdict {
            source: owner.source.clone(),
            extras: owner.extras.clone(),
            load_bearing: a != b,
            routed_len: a.len(),
            stripped_len: b.len(),
            first_difference: first_difference(&a, &b),
            note: None,
        },
        (routed, reduced) => {
            let note = match (&routed, &reduced) {
                (Err(error), _) => format!("routed compile failed: {error}"),
                (_, Err(error)) => format!("reduced compile failed: {error}"),
                _ => unreachable!(),
            };
            OverlayVerdict {
                source: owner.source.clone(),
                extras: owner.extras.clone(),
                load_bearing: true,
                routed_len: 0,
                stripped_len: 0,
                first_difference: None,
                note: Some(note),
            }
        }
    }
}

/// Test every routed overlay owner one flag at a time.
///
/// Returns a verdict per (owner, flag) pair. Owners carrying a single flag are
/// skipped: `run` already answers them, and re-testing would double the cost of
/// the sweep for nothing.
pub fn run_per_flag(root: &Path, cache: &Path) -> Result<Vec<(String, OverlayVerdict)>, String> {
    let owners = routed_overlay_owners(root)?;
    let mut verdicts = Vec::new();
    let mut index = 0usize;
    for owner in owners.iter().filter(|o| o.extras.len() > 1) {
        for flag in &owner.extras {
            let work: PathBuf = cache.join(format!("flag-{index:05}"));
            index += 1;
            std::fs::create_dir_all(work.join("routed"))
                .map_err(|error| format!("{}: {error}", work.display()))?;
            std::fs::create_dir_all(work.join("reduced"))
                .map_err(|error| format!("{}: {error}", work.display()))?;
            let verdict =
                evaluate_removal(owner, std::slice::from_ref(flag), root, &work);
            verdicts.push((flag.clone(), verdict));
        }
    }
    Ok(verdicts)
}

/// Measure what adding `added` to every overlay owner would cost.
pub fn run_addition(
    root: &Path,
    cache: &Path,
    added: &[String],
) -> Result<Vec<OverlayVerdict>, String> {
    let owners = all_overlay_owners(root)?;
    let mut verdicts = Vec::with_capacity(owners.len());
    for (index, owner) in owners.iter().enumerate() {
        // An owner that already carries the flag cannot be broken by adding it.
        if added.iter().all(|flag| owner.extras.contains(flag)) && !added.is_empty() {
            continue;
        }
        let work: PathBuf = cache.join(format!("add-{index:04}"));
        std::fs::create_dir_all(work.join("routed"))
            .map_err(|error| format!("{}: {error}", work.display()))?;
        std::fs::create_dir_all(work.join("promoted"))
            .map_err(|error| format!("{}: {error}", work.display()))?;
        verdicts.push(evaluate_addition(owner, added, root, &work));
    }
    Ok(verdicts)
}

/// Every adopted overlay owner that carries routed flags beyond the baseline.
pub fn routed_overlay_owners(root: &Path) -> Result<Vec<OverlayOwner>, String> {
    let baseline: std::collections::BTreeSet<String> =
        cflags_for_target(CompilerTarget::Gs1).into_iter().collect();
    let directory = root.join("exact");
    let entries = std::fs::read_dir(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?;
    let mut owners = Vec::new();
    for entry in entries.filter_map(Result::ok) {
        let path = entry.path();
        if path.extension().and_then(|e| e.to_str()) != Some("c") {
            continue;
        }
        let Some(name) = path.file_name().and_then(|n| n.to_str()) else { continue };
        if !name.starts_with("resource_") {
            continue;
        }
        let source = format!("exact/{name}");
        let Some(overlay) = overlay_of(&source) else { continue };
        let extras: Vec<String> = cflags_for_source(&source)
            .into_iter()
            .filter(|flag| !baseline.contains(flag))
            .collect();
        if extras.is_empty() {
            continue;
        }
        owners.push(OverlayOwner { source, overlay, extras });
    }
    owners.sort_by(|a, b| a.source.cmp(&b.source));
    Ok(owners)
}

fn first_difference(a: &[u8], b: &[u8]) -> Option<usize> {
    let shared = a.len().min(b.len());
    for index in 0..shared {
        if a[index] != b[index] {
            return Some(index);
        }
    }
    if a.len() == b.len() {
        None
    } else {
        Some(shared)
    }
}

/// Compile one owner as routed and again with its routed flags removed.
pub fn evaluate(owner: &OverlayOwner, root: &Path, work: &Path) -> OverlayVerdict {
    let source = root.join(&owner.source);
    let routed = compile_overlay_mutated(
        &source,
        &work.join("routed"),
        &owner.overlay,
        None,
        &CompilerFlagMutations { add_flags: Vec::new(), remove_flags: Vec::new() },
    );
    let stripped = compile_overlay_mutated(
        &source,
        &work.join("stripped"),
        &owner.overlay,
        None,
        &CompilerFlagMutations {
            add_flags: Vec::new(),
            remove_flags: owner.extras.clone(),
        },
    );
    match (routed, stripped) {
        (Ok(Compiled { data: a, .. }), Ok(Compiled { data: b, .. })) => OverlayVerdict {
            source: owner.source.clone(),
            extras: owner.extras.clone(),
            load_bearing: a != b,
            routed_len: a.len(),
            stripped_len: b.len(),
            first_difference: first_difference(&a, &b),
            note: None,
        },
        // A compile failure is never reported as "routing unnecessary". An
        // owner we could not build is an owner we did not measure.
        (routed, stripped) => {
            let note = match (&routed, &stripped) {
                (Err(error), _) => format!("routed compile failed: {error}"),
                (_, Err(error)) => format!("stripped compile failed: {error}"),
                _ => unreachable!(),
            };
            OverlayVerdict {
                source: owner.source.clone(),
                extras: owner.extras.clone(),
                load_bearing: true,
                routed_len: 0,
                stripped_len: 0,
                first_difference: None,
                note: Some(note),
            }
        }
    }
}

/// Measure every routed overlay owner. Returns the verdicts in source order.
pub fn run(root: &Path, cache: &Path) -> Result<Vec<OverlayVerdict>, String> {
    let owners = routed_overlay_owners(root)?;
    let mut verdicts = Vec::with_capacity(owners.len());
    for (index, owner) in owners.iter().enumerate() {
        let work: PathBuf = cache.join(format!("owner-{index:04}"));
        std::fs::create_dir_all(work.join("routed"))
            .map_err(|error| format!("{}: {error}", work.display()))?;
        std::fs::create_dir_all(work.join("stripped"))
            .map_err(|error| format!("{}: {error}", work.display()))?;
        verdicts.push(evaluate(owner, root, &work));
    }
    Ok(verdicts)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn overlay_name_comes_from_the_owner_filename() {
        assert_eq!(
            overlay_of("exact/resource_3ce_c_02000b10.c").as_deref(),
            Some("resource_3ce")
        );
        assert_eq!(overlay_of("exact/080bbb0c.c"), None);
    }

    #[test]
    fn first_difference_reports_length_mismatch() {
        assert_eq!(first_difference(&[1, 2, 3], &[1, 2, 3]), None);
        assert_eq!(first_difference(&[1, 2, 3], &[1, 9, 3]), Some(1));
        assert_eq!(first_difference(&[1, 2], &[1, 2, 3]), Some(2));
    }
}
