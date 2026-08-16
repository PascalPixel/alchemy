use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode, Stdio};

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum Group {
    Assets,
    Check,
    Compiler,
    Decomp,
    Make,
    Metrics,
    Overlay,
    Search,
    Semantic,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum Target {
    Binary(&'static str),
    /// A binary that hosts several former tools as subcommands: (path, subcommand).
    ///
    /// Consolidation target. `tools/` shipped 115 executables, which nothing
    /// could enumerate, so agents repeatedly rebuilt work that already existed.
    /// Folding a family of tools into one binary keeps every capability while
    /// leaving one entry point to discover.
    Sub(&'static str, &'static str),
}

impl Target {
    /// The binary path, whichever shape this is.
    pub fn path(self) -> &'static str {
        match self {
            Target::Binary(path) => path,
            Target::Sub(path, _) => path,
        }
    }

    /// Arguments the dispatcher must prepend before the caller's own.
    pub fn prefix(self) -> Option<&'static str> {
        match self {
            Target::Binary(_) => None,
            Target::Sub(_, subcommand) => Some(subcommand),
        }
    }
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct Entry {
    pub name: &'static str,
    pub target: Target,
}

const CHECK: &[Entry] = &[
    Entry {
        name: "architecture",
        target: Target::Binary("tools/architecture/target/release/architecture"),
    },
    Entry {
        name: "cache_key_lint",
        target: Target::Binary("tools/target/release/cache-key-lint"),
    },
    Entry {
        name: "check_commit_progress",
        target: Target::Binary("tools/check-commit-progress/target/release/check-commit-progress"),
    },
    Entry {
        name: "check_publication",
        target: Target::Binary("tools/check-publication/target/release/check-publication"),
    },
    Entry {
        name: "check_sanctum",
        target: Target::Binary("tools/check-sanctum/target/release/check-sanctum"),
    },
    Entry {
        name: "core_retained_audit",
        target: Target::Binary("tools/core-retained-audit/target/release/core-retained-audit"),
    },
    Entry {
        name: "documented",
        target: Target::Binary("tools/documented/target/release/documented"),
    },
    Entry {
        name: "no_asm_c",
        target: Target::Binary("tools/target/release/no-asm-c"),
    },
    Entry {
        name: "source_citations",
        target: Target::Binary("tools/source-citations/target/release/source-citations"),
    },
];

const ASSETS: &[Entry] = &[
    Entry {
        name: "bl_site_symbols",
        target: Target::Sub("tools/dev/target/release/dev", "bl-symbols"),
    },
    Entry {
        name: "export_asset",
        target: Target::Binary("tools/export-asset/target/release/export-asset"),
    },
    Entry {
        name: "extract_resource",
        target: Target::Binary("tools/extract-resource/target/release/extract-resource"),
    },
    Entry {
        name: "import_asset",
        target: Target::Binary("tools/import-asset/target/release/import-asset"),
    },
    Entry {
        name: "tilemap",
        target: Target::Sub("tools/assets/target/release/assets", "tilemap"),
    },
];

const COMPILER: &[Entry] = &[
    Entry {
        name: "compiler_corpus_regression",
        target: Target::Binary(
            "tools/compiler-corpus-regression/target/release/compiler-corpus-regression",
        ),
    },
    Entry {
        name: "mode_cohort",
        target: Target::Binary("tools/mode-cohort/target/release/mode-cohort"),
    },
    Entry {
        name: "mode_sweep",
        target: Target::Binary("tools/mode-sweep/target/release/mode-sweep"),
    },
];

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum NonPublicKind {
    InternalDiagnostic,
    SelfTestSupport,
    BenchAuxiliary,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct NonPublicTarget {
    /// The immediate directory under `tools/`, not the Cargo package name.
    pub crate_name: &'static str,
    pub binary: &'static str,
    pub kind: NonPublicKind,
    pub self_test: bool,
}

/// Every Cargo binary that is intentionally outside the public dispatcher.
/// Public binaries are the entries above; this table is the complete and
/// enforced remainder.  Keep auxiliary binaries here rather than allowing
/// them to become accidental commands when a manifest gains a new target.
const NON_PUBLIC: &[NonPublicTarget] = &[
    NonPublicTarget {
        crate_name: "route-dump",
        binary: "route-dump",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "alchemy-lints",
        binary: "alchemy-lints",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: false,
    },
    NonPublicTarget {
        crate_name: "zlib",
        binary: "alchemy-zlib",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "candidate-explain",
        binary: "candidate-explain",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "rtl",
        binary: "rtl",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "candidate-show",
        binary: "candidate-show",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "jobs",
        binary: "jobs",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "lang-ban",
        binary: "lang-ban",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: false,
    },
    NonPublicTarget {
        crate_name: "overlay-disasm",
        binary: "parity_dump",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: false,
    },
    NonPublicTarget {
        crate_name: "rtl-align",
        binary: "rtl-align",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "rtl-schedule",
        binary: "rtl-schedule",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "thumb-disasm",
        binary: "thumb-disasm",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "kind2-resources",
        binary: "kind2-resources",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "late-runtime-data",
        binary: "late-runtime-data",
        kind: NonPublicKind::InternalDiagnostic,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "dispatch",
        binary: "dispatch",
        kind: NonPublicKind::SelfTestSupport,
        self_test: false,
    },
    NonPublicTarget {
        crate_name: "alchemy-selftest",
        binary: "alchemy-selftest",
        kind: NonPublicKind::SelfTestSupport,
        self_test: false,
    },
    NonPublicTarget {
        crate_name: "cache-entry",
        binary: "cache-entry",
        kind: NonPublicKind::SelfTestSupport,
        self_test: true,
    },
    NonPublicTarget {
        crate_name: "decomp-targets",
        binary: "decomp-targets",
        kind: NonPublicKind::SelfTestSupport,
        self_test: false,
    },
    NonPublicTarget {
        crate_name: "self-test",
        binary: "self-test",
        kind: NonPublicKind::SelfTestSupport,
        self_test: false,
    },
    NonPublicTarget {
        crate_name: "compiler-corpus-regression",
        binary: "compiler-corpus-regression-bench",
        kind: NonPublicKind::BenchAuxiliary,
        self_test: false,
    },
    NonPublicTarget {
        crate_name: "integrate-matches",
        binary: "integrate-matches-bench",
        kind: NonPublicKind::BenchAuxiliary,
        self_test: false,
    },
];

const DECOMP: &[Entry] = &[
    Entry {
        name: "decomp_diagnose",
        target: Target::Binary("tools/decomp-diagnose/target/release/decomp-diagnose"),
    },
    Entry {
        name: "discover",
        target: Target::Binary("tools/discover/target/release/discover"),
    },
    Entry {
        name: "integrate_matches",
        target: Target::Binary("tools/integrate-matches/target/release/integrate-matches"),
    },
    Entry {
        name: "remaining_survey",
        target: Target::Binary("tools/remaining-survey/target/release/remaining-survey"),
    },
];

const MAKE: &[Entry] = &[
    Entry {
        name: "archive_asset",
        target: Target::Binary("tools/archive-asset/target/release/archive-asset"),
    },
    Entry {
        name: "audio_engine_data",
        target: Target::Sub("tools/data/target/release/data", "audio-engine"),
    },
    Entry {
        name: "audio_wave",
        target: Target::Sub("tools/assets/target/release/assets", "audio-wave"),
    },
    Entry {
        name: "battle_effect_data",
        target: Target::Sub("tools/data/target/release/data", "battle-effect"),
    },
    Entry {
        name: "build_asm",
        target: Target::Binary("tools/build-asm/target/release/build-asm"),
    },
    Entry {
        name: "build_assets",
        target: Target::Binary("tools/build-assets/target/release/build-assets"),
    },
    Entry {
        name: "build_claimed",
        target: Target::Binary("tools/build-claimed/target/release/build-claimed"),
    },
    Entry {
        name: "build_full",
        target: Target::Binary("tools/build-full/target/release/build-full"),
    },
    Entry {
        name: "build_rom",
        target: Target::Binary("tools/build-rom/target/release/build-rom"),
    },
    Entry {
        name: "build_semantic",
        target: Target::Binary("tools/build-semantic/target/release/build-semantic"),
    },
    Entry {
        name: "byte_henkan",
        target: Target::Binary("tools/byte-henkan/target/release/byte-henkan"),
    },
    Entry {
        name: "byte_value_regions",
        target: Target::Binary("tools/target/release/byte-value-regions"),
    },
    Entry {
        name: "character_catalog",
        target: Target::Sub("tools/assets/target/release/assets", "character-catalog"),
    },
    Entry {
        name: "chiiki_map_resources",
        target: Target::Binary("tools/map-resources/target/release/chiiki-map-resources"),
    },
    Entry {
        name: "early_runtime_data",
        target: Target::Binary("tools/early-runtime-data/target/release/early-runtime-data"),
    },
    Entry {
        name: "encounter_data",
        target: Target::Sub("tools/assets/target/release/assets", "encounter-data"),
    },
    Entry {
        name: "executable_gap_sources",
        target: Target::Binary(
            "tools/executable-gap-sources/target/release/executable-gap-sources",
        ),
    },
    Entry {
        name: "f0_archive",
        target: Target::Binary("tools/f0-archive/target/release/f0-archive"),
    },
    Entry {
        name: "gba_header",
        target: Target::Binary("tools/gba-header/target/release/gba-header"),
    },
    Entry {
        name: "indexed_still",
        target: Target::Sub("tools/assets/target/release/assets", "indexed-still"),
    },
    Entry {
        name: "kind1_map_grid",
        target: Target::Binary("tools/kind1-map-grid/target/release/kind1-map-grid"),
    },
    Entry {
        name: "late_runtime_residual",
        target: Target::Binary("tools/late-runtime-residual/target/release/late-runtime-residual"),
    },
    Entry {
        name: "localization_font",
        target: Target::Binary("tools/localization-font/target/release/localization-font"),
    },
    Entry {
        name: "localization_tables",
        target: Target::Binary("tools/localization-tables/target/release/localization-tables"),
    },
    Entry {
        name: "map_container_components",
        target: Target::Binary(
            "tools/map-container-components/target/release/map-container-components",
        ),
    },
    Entry {
        name: "message_archive",
        target: Target::Binary("tools/message-archive/target/release/message_archive"),
    },
    Entry {
        name: "music",
        target: Target::Binary("tools/music/target/release/music"),
    },
    Entry {
        name: "music_residuals",
        target: Target::Binary("tools/music-residuals/target/release/music-residuals"),
    },
    Entry {
        name: "namae_nyuuryoku",
        target: Target::Binary("tools/namae-nyuuryoku/target/release/namae-nyuuryoku"),
    },
    Entry {
        name: "pairtable",
        target: Target::Sub("tools/assets/target/release/assets", "pairtable"),
    },
    Entry {
        name: "resource_01c",
        target: Target::Binary("tools/resource-01c/target/release/resource-01c"),
    },
    Entry {
        name: "resource_3ce",
        target: Target::Sub("tools/resource/target/release/resource", "3ce"),
    },
    Entry {
        name: "resource_5",
        target: Target::Sub("tools/resource/target/release/resource", "5"),
    },
    Entry {
        name: "resource_byte_canvases",
        target: Target::Binary(
            "tools/resource-byte-canvases/target/release/resource-byte-canvases",
        ),
    },
    Entry {
        name: "resource_d1_d3",
        target: Target::Sub("tools/resource/target/release/resource", "d1-d3"),
    },
    Entry {
        name: "resource_directory",
        target: Target::Binary("tools/resource-directory/target/release/resource-directory"),
    },
    Entry {
        name: "runtime_support_data",
        target: Target::Sub("tools/data/target/release/data", "runtime-support"),
    },
    Entry {
        name: "sentou_gamen_data",
        target: Target::Sub("tools/data/target/release/data", "battle-screen"),
    },
    Entry {
        name: "sentou_hyouji",
        target: Target::Sub("tools/data/target/release/data", "battle-display"),
    },
    Entry {
        name: "sentou_kouka_runtime",
        target: Target::Sub("tools/data/target/release/data", "battle-runtime"),
    },
    Entry {
        name: "sentou_menu_data",
        target: Target::Sub("tools/data/target/release/data", "battle-menu"),
    },
    Entry {
        name: "sentou_resources",
        target: Target::Sub("tools/resource/target/release/resource", "sentou"),
    },
    Entry {
        name: "simple_resources",
        target: Target::Binary("tools/simple-resources/target/release/simple-resources"),
    },
    Entry {
        name: "skip_sprite_archive",
        target: Target::Binary("tools/skip-sprite-archive/target/release/skip-sprite-archive"),
    },
    Entry {
        name: "staff_roll",
        target: Target::Sub("tools/assets/target/release/assets", "staff-roll"),
    },
    Entry {
        name: "static_sprite_series",
        target: Target::Binary("tools/static-sprite-series/target/release/static-sprite-series"),
    },
    Entry {
        name: "title_resources",
        target: Target::Sub("tools/resource/target/release/resource", "title"),
    },
    Entry {
        name: "tokushu_map_resources",
        target: Target::Binary("tools/map-resources/target/release/tokushu-map-resources"),
    },
    Entry {
        name: "wordstream",
        target: Target::Sub("tools/assets/target/release/assets", "wordstream"),
    },
];

const METRICS: &[Entry] = &[
    Entry {
        name: "audit_residuals",
        target: Target::Sub("tools/dev/target/release/dev", "residuals"),
    },
    Entry {
        name: "compare_roms",
        target: Target::Binary("tools/compare-roms/target/release/compare-roms"),
    },
    Entry {
        name: "coverage_map",
        target: Target::Binary("tools/coverage-map/target/release/coverage-map"),
    },
    Entry {
        name: "dashboard_server",
        target: Target::Binary("tools/dashboard-server/target/release/dashboard-server"),
    },
    Entry {
        name: "full_c_history",
        target: Target::Binary("tools/full-c-history/target/release/full-c-history"),
    },
    Entry {
        name: "full_c_progress",
        target: Target::Binary("tools/full-c-progress/target/release/full-c-progress"),
    },
];

const OVERLAY: &[Entry] = &[
    Entry {
        name: "exact_reading_list",
        target: Target::Binary("tools/exact-reading-list/target/release/exact-reading-list"),
    },
    Entry {
        name: "overlay_adopt",
        target: Target::Sub("tools/overlay/target/release/overlay", "adopt"),
    },
    Entry {
        name: "overlay_call_order_check",
        target: Target::Sub("tools/overlay/target/release/overlay", "call-order-check"),
    },
    Entry {
        name: "overlay_candidate_rank",
        target: Target::Sub("tools/overlay/target/release/overlay", "candidate-rank"),
    },
    Entry {
        name: "overlay_certify",
        target: Target::Sub("tools/overlay/target/release/overlay", "certify"),
    },
    Entry {
        name: "overlay_disasm",
        target: Target::Binary("tools/overlay-disasm/target/release/overlay-disasm"),
    },
    Entry {
        name: "overlay_driver",
        target: Target::Binary("tools/overlay-driver/target/release/overlay-driver"),
    },
    Entry {
        name: "overlay_entry",
        target: Target::Sub("tools/overlay/target/release/overlay", "entry"),
    },
    Entry {
        name: "overlay_gaps",
        target: Target::Sub("tools/overlay/target/release/overlay", "gaps"),
    },
    Entry {
        name: "overlay_inventory",
        target: Target::Sub("tools/overlay/target/release/overlay", "inventory"),
    },
    Entry {
        name: "overlay_mode_cohort",
        target: Target::Sub("tools/overlay/target/release/overlay", "mode-cohort"),
    },
    Entry {
        name: "overlay_show",
        target: Target::Sub("tools/overlay/target/release/overlay", "show"),
    },
    Entry {
        name: "overlay_showcase",
        target: Target::Binary("tools/overlay-showcase/target/release/overlay-showcase"),
    },
    Entry {
        name: "overlay_twins",
        target: Target::Sub("tools/overlay/target/release/overlay", "twins"),
    },
    Entry {
        name: "overlay_unindexed",
        target: Target::Sub("tools/overlay/target/release/overlay", "unindexed"),
    },
];

const SEARCH: &[Entry] = &[
    Entry {
        name: "alchemy_permuter",
        target: Target::Sub("tools/dev/target/release/dev", "permute"),
    },
    Entry {
        name: "decomp_constraints",
        target: Target::Binary("tools/decomp-constraints/target/release/decomp-constraints"),
    },
    Entry {
        name: "search_compiler_modes",
        target: Target::Binary("tools/search-compiler-modes/target/release/search-compiler-modes"),
    },
    Entry {
        name: "shape_sweep",
        target: Target::Binary("tools/shape-sweep/target/release/shape-sweep"),
    },
];

const SEMANTIC: &[Entry] = &[
    Entry {
        name: "semantic_owner_scope",
        target: Target::Binary("tools/semantic-owner-scope/target/release/semantic-owner-scope"),
    },
    Entry {
        name: "semantic_queue",
        target: Target::Binary("tools/semantic-queue/target/release/semantic-queue"),
    },
    Entry {
        name: "semantic_superseded",
        target: Target::Binary("tools/semantic-superseded/target/release/semantic-superseded"),
    },
];

impl Group {
    pub fn parse(name: &str) -> Option<Self> {
        match name {
            "assets" => Some(Self::Assets),
            "check" => Some(Self::Check),
            "compiler" => Some(Self::Compiler),
            "decomp" => Some(Self::Decomp),
            "make" => Some(Self::Make),
            "metrics" => Some(Self::Metrics),
            "overlay" => Some(Self::Overlay),
            "search" => Some(Self::Search),
            "semantic" => Some(Self::Semantic),
            _ => None,
        }
    }

    pub fn name(self) -> &'static str {
        match self {
            Self::Assets => "assets",
            Self::Check => "check",
            Self::Compiler => "compiler",
            Self::Decomp => "decomp",
            Self::Make => "make",
            Self::Metrics => "metrics",
            Self::Overlay => "overlay",
            Self::Search => "search",
            Self::Semantic => "semantic",
        }
    }

    pub fn entries(self) -> &'static [Entry] {
        match self {
            Self::Assets => ASSETS,
            Self::Check => CHECK,
            Self::Compiler => COMPILER,
            Self::Decomp => DECOMP,
            Self::Make => MAKE,
            Self::Metrics => METRICS,
            Self::Overlay => OVERLAY,
            Self::Search => SEARCH,
            Self::Semantic => SEMANTIC,
        }
    }
}

const GROUPS: &[Group] = &[
    Group::Assets,
    Group::Check,
    Group::Compiler,
    Group::Decomp,
    Group::Make,
    Group::Metrics,
    Group::Overlay,
    Group::Search,
    Group::Semantic,
];

/// The native command registry consumed by the architecture gate.
pub fn groups() -> &'static [Group] {
    GROUPS
}

/// Flatten the registry without making callers know how the groups are stored.
pub fn all_entries() -> impl Iterator<Item = (Group, Entry)> {
    groups().iter().copied().flat_map(|group| {
        group
            .entries()
            .iter()
            .copied()
            .map(move |entry| (group, entry))
    })
}

pub fn non_public_targets() -> &'static [NonPublicTarget] {
    NON_PUBLIC
}

pub fn non_public_target(crate_name: &str, binary: &str) -> Option<NonPublicTarget> {
    NON_PUBLIC
        .iter()
        .copied()
        .find(|target| target.crate_name == crate_name && target.binary == binary)
}

fn target_identity(path: &str) -> Option<(&str, &str)> {
    let parts: Vec<_> = path.split('/').collect();
    match parts.as_slice() {
        ["tools", crate_name, "target", "release", binary] => Some((crate_name, binary)),
        ["tools", "target", "release", binary] => Some((binary, binary)),
        _ => None,
    }
}

pub fn public_target(crate_name: &str, binary: &str) -> bool {
    all_entries().any(|(_, entry)| {
        let path = entry.target.path();
        target_identity(path) == Some((crate_name, binary))
    })
}

pub fn should_self_test(crate_name: &str, binary: &str) -> bool {
    if public_target(crate_name, binary) {
        return true;
    }
    non_public_target(crate_name, binary).is_some_and(|target| target.self_test)
}

fn usage(group: Group) -> String {
    let mut output = format!("usage: {} <subcommand> [args...]\n", group.name());
    for entry in group.entries() {
        output.push_str("  ");
        output.push_str(entry.name);
        output.push('\n');
    }
    output
}

fn find_entry(group: Group, name: &str) -> Option<Entry> {
    group
        .entries()
        .iter()
        .copied()
        .find(|entry| entry.name == name)
}

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("dispatch must live under tools")
        .parent()
        .expect("tools must live under the repository root")
        .to_path_buf()
}

#[derive(Debug, Eq, PartialEq)]
struct ResolvedTarget {
    manifest: PathBuf,
    binary: String,
}

fn resolve_target(repository: &Path, target: Target) -> Result<ResolvedTarget, String> {
    let path = target.path();
    let components: Vec<_> = Path::new(path).components().collect();
    let (manifest, binary) = match components.as_slice() {
        [std::path::Component::Normal(tools), std::path::Component::Normal(target), std::path::Component::Normal(release), std::path::Component::Normal(binary)]
            if *tools == std::ffi::OsStr::new("tools")
                && *target == std::ffi::OsStr::new("target")
                && *release == std::ffi::OsStr::new("release") =>
        {
            (repository.join("tools/Cargo.toml"), binary)
        }
        [std::path::Component::Normal(tools), std::path::Component::Normal(crate_name), std::path::Component::Normal(target), std::path::Component::Normal(release), std::path::Component::Normal(binary)]
            if *tools == std::ffi::OsStr::new("tools")
                && *target == std::ffi::OsStr::new("target")
                && *release == std::ffi::OsStr::new("release") =>
        {
            (
                repository.join("tools").join(crate_name).join("Cargo.toml"),
                binary,
            )
        }
        _ => return Err(format!("{path} is not a native release binary path")),
    };
    let binary = binary
        .to_str()
        .ok_or_else(|| format!("{path} has a non-UTF-8 binary name"))?;
    Ok(ResolvedTarget {
        manifest,
        binary: binary.to_string(),
    })
}

#[derive(Debug, Eq, PartialEq)]
struct CommandPlan {
    target: ResolvedTarget,
    arguments: Vec<String>,
}

impl CommandPlan {
    fn cargo_arguments(&self) -> Vec<String> {
        let mut arguments = vec![
            "run".to_string(),
            "--offline".to_string(),
            "--quiet".to_string(),
            "--release".to_string(),
            "--manifest-path".to_string(),
            self.target.manifest.to_string_lossy().into_owned(),
            "--bin".to_string(),
            self.target.binary.clone(),
            "--".to_string(),
        ];
        arguments.extend(self.arguments.iter().cloned());
        arguments
    }
}

fn command_plan(
    entry: Entry,
    arguments: &[String],
    repository: &Path,
) -> Result<CommandPlan, String> {
    let mut combined = Vec::new();
    if let Some(subcommand) = entry.target.prefix() {
        combined.push(subcommand.to_string());
    }
    combined.extend_from_slice(arguments);
    Ok(CommandPlan {
        target: resolve_target(repository, entry.target)?,
        arguments: combined,
    })
}

fn run_child(entry: Entry, arguments: &[String], repository: &Path) -> ExitCode {
    let plan = match command_plan(entry, arguments, repository) {
        Ok(plan) => plan,
        Err(error) => {
            eprintln!("dispatch: could not plan {}: {error}", entry.target.path());
            return ExitCode::FAILURE;
        }
    };
    let child = Command::new("cargo")
        .args(plan.cargo_arguments())
        .current_dir(repository)
        .stdin(Stdio::inherit())
        .stdout(Stdio::inherit())
        .stderr(Stdio::inherit())
        .status();
    match child {
        Ok(status) => status
            .code()
            .map(|code| ExitCode::from(code.clamp(0, u8::MAX as i32) as u8))
            .unwrap_or(ExitCode::FAILURE),
        Err(error) => {
            eprintln!("dispatch: could not run {}: {error}", entry.target.path());
            ExitCode::FAILURE
        }
    }
}

pub fn run(group: Group, arguments: &[String]) -> ExitCode {
    let (subcommand, rest) = arguments
        .split_first()
        .map_or((None, &[][..]), |(first, rest)| {
            (Some(first.as_str()), rest)
        });

    if subcommand.is_none() || matches!(subcommand, Some("-h" | "--list" | "--help")) {
        print!("{}", usage(group));
        return ExitCode::SUCCESS;
    }
    if subcommand == Some("--self-test") {
        println!(
            "{} self-test ok ({} subcommands)",
            group.name(),
            group.entries().len()
        );
        return ExitCode::SUCCESS;
    }

    let Some(name) = subcommand else {
        unreachable!()
    };
    let Some(entry) = find_entry(group, name) else {
        eprintln!("unknown {} subcommand: {name}", group.name());
        eprintln!(
            "try one of: {}",
            group
                .entries()
                .iter()
                .map(|entry| entry.name)
                .collect::<Vec<_>>()
                .join(", ")
        );
        return ExitCode::from(2);
    };
    run_child(entry, rest, &root())
}

pub fn top_level_usage() -> &'static str {
    "usage: dispatch <assets|check|compiler|decomp|make|metrics|overlay|search|semantic> <subcommand> [args...]"
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn groups_and_entries_are_complete_and_sorted() {
        assert_eq!(Group::parse("assets"), Some(Group::Assets));
        assert_eq!(Group::parse("check"), Some(Group::Check));
        assert_eq!(Group::parse("compiler"), Some(Group::Compiler));
        assert_eq!(Group::parse("decomp"), Some(Group::Decomp));
        assert_eq!(Group::parse("make"), Some(Group::Make));
        assert_eq!(Group::parse("metrics"), Some(Group::Metrics));
        assert_eq!(Group::parse("overlay"), Some(Group::Overlay));
        assert_eq!(Group::parse("search"), Some(Group::Search));
        assert_eq!(Group::parse("semantic"), Some(Group::Semantic));
        for group in [
            Group::Assets,
            Group::Check,
            Group::Compiler,
            Group::Decomp,
            Group::Make,
            Group::Metrics,
            Group::Overlay,
            Group::Search,
            Group::Semantic,
        ] {
            let names: Vec<_> = group.entries().iter().map(|entry| entry.name).collect();
            let mut sorted = names.clone();
            sorted.sort_unstable();
            assert_eq!(names, sorted);
            assert!(names.windows(2).all(|pair| pair[0] != pair[1]));
        }
    }

    #[test]
    fn usage_preserves_the_native_dispatcher_shape() {
        assert_eq!(usage(Group::Assets).lines().count(), 6);
        assert_eq!(
            usage(Group::Check).lines().next(),
            Some("usage: check <subcommand> [args...]")
        );
        // 4, not 5: statement-order-sweep-main was deleted once shape-sweep's
        // descent driver absorbed its job.
        assert_eq!(usage(Group::Compiler).lines().count(), 4);
        assert_eq!(usage(Group::Decomp).lines().count(), 5);
        assert_eq!(usage(Group::Make).lines().count(), 50);
        assert_eq!(usage(Group::Metrics).lines().count(), 7);
        assert_eq!(usage(Group::Overlay).lines().count(), 16);
        assert_eq!(usage(Group::Search).lines().count(), 5);
        assert_eq!(usage(Group::Semantic).lines().count(), 4);
        assert!(usage(Group::Check).contains("  architecture"));
    }

    #[test]
    fn unknown_and_help_are_dispatcher_local() {
        assert!(find_entry(Group::Check, "compiler_corpus_regression").is_none());
        assert!(find_entry(Group::Check, "commands").is_none());
        assert!(find_entry(Group::Compiler, "mode_sweep").is_some());
        for diagnostic in [
            "candidate_explain",
            "candidate_show",
            "rtl_align",
            "rtl_insn",
            "rtl_schedule",
            "rtl_sexpr",
            "thumb_disasm",
        ] {
            assert!(find_entry(Group::Compiler, diagnostic).is_none());
        }
        assert!(find_entry(Group::Assets, "bl_site_symbols").is_some());
        assert!(find_entry(Group::Assets, "extract_resource").is_some());
        assert!(find_entry(Group::Decomp, "decomp_diagnose").is_some());
        assert!(find_entry(Group::Decomp, "discover").is_some());
        assert!(find_entry(Group::Make, "build_assets").is_some());
        assert!(find_entry(Group::Make, "tokushu_map_resources").is_some());
        assert!(find_entry(Group::Metrics, "coverage_map").is_some());
        assert!(find_entry(Group::Metrics, "full_c_progress").is_some());
        assert!(find_entry(Group::Overlay, "overlay_gaps").is_some());
        assert!(find_entry(Group::Overlay, "overlay_disasm").is_some());
        assert!(find_entry(Group::Overlay, "overlay_certify").is_some());
        assert!(find_entry(Group::Search, "shape_sweep").is_some());
        assert!(find_entry(Group::Search, "alchemy_permuter").is_some());
        assert!(find_entry(Group::Semantic, "semantic_queue").is_some());
        assert_eq!(non_public_targets().len(), 23);
        assert_eq!(
            non_public_target("candidate-explain", "candidate-explain").map(|target| target.kind),
            Some(NonPublicKind::InternalDiagnostic)
        );
        assert!(!public_target("candidate-explain", "candidate-explain"));
        assert!(should_self_test("candidate-explain", "candidate-explain"));
        assert!(!should_self_test(
            "compiler-corpus-regression",
            "compiler-corpus-regression-bench"
        ));
        assert!(should_self_test("build-rom", "build-rom"));
        assert!(should_self_test("discover", "discover"));
        assert_eq!(
            top_level_usage(),
            "usage: dispatch <assets|check|compiler|decomp|make|metrics|overlay|search|semantic> <subcommand> [args...]"
        );
    }

    #[test]
    fn resolves_standalone_target_to_its_crate_manifest() {
        assert_eq!(
            resolve_target(
                Path::new("/repo"),
                Target::Binary("tools/message-archive/target/release/message_archive")
            ),
            Ok(ResolvedTarget {
                manifest: PathBuf::from("/repo/tools/message-archive/Cargo.toml"),
                binary: "message_archive".to_string(),
            })
        );
    }

    #[test]
    fn resolves_shared_target_to_the_tools_workspace_manifest() {
        assert_eq!(
            resolve_target(
                Path::new("/repo"),
                Target::Binary("tools/target/release/cache-key-lint")
            ),
            Ok(ResolvedTarget {
                manifest: PathBuf::from("/repo/tools/Cargo.toml"),
                binary: "cache-key-lint".to_string(),
            })
        );
    }

    #[test]
    fn rejects_non_release_binary_paths() {
        let error = resolve_target(
            Path::new("/repo"),
            Target::Binary("tools/message-archive/debug/message_archive"),
        )
        .expect_err("debug targets must not be dispatched");
        assert_eq!(
            error,
            "tools/message-archive/debug/message_archive is not a native release binary path"
        );
    }

    #[test]
    fn command_plan_is_cargo_authoritative_and_preserves_arguments() {
        let plan = command_plan(
            Entry {
                name: "message_archive",
                target: Target::Binary("tools/message-archive/target/release/message_archive"),
            },
            &["--self-test".to_string(), "extra value".to_string()],
            Path::new("/repo"),
        )
        .expect("command plan should resolve");
        let expected: Vec<_> = [
            "run",
            "--offline",
            "--quiet",
            "--release",
            "--manifest-path",
            "/repo/tools/message-archive/Cargo.toml",
            "--bin",
            "message_archive",
            "--",
            "--self-test",
            "extra value",
        ]
        .into_iter()
        .map(String::from)
        .collect();
        assert_eq!(plan.cargo_arguments(), expected);
    }
}
