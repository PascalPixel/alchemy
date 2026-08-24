//! The compilation-plan layer: the boundary between compiler policy and
//! process execution.
//!
//! Formerly the compilation-plan section of the TypeScript compiler module:
//!
//! * `CompilerFamily`, `CompilerFlagMutations`, `SourceToAssemblyPlanOptions`,
//!   `CompilerCommandStep`, `SourceToAssemblyPlan`
//! * `mutatedCompilerFlags`, `inferredPreprocessedOutput`
//! * `sourceToAssemblyPlan`
//! * `directPreprocessorCommand`, `directCompilerCommand`,
//!   `directCompilerCommandForSource`
//!
//! The original's own framing, kept because it explains the two-name design:
//! a compilation plan is the canonical boundary between compiler policy and
//! process execution. `routing_source` identifies the translation unit whose
//! evidenced compiler family/flags must be used; `input` is the file that is
//! actually compiled. Keeping those names separate is essential for candidate
//! and corpus work, where a temporary source must compile exactly as the
//! eventual installed source would.
//!
//! ORDER IS BEHAVIOUR, as in the compiler bundle. gcc is later-flag-wins, so every
//! flag list here is an ordered `Vec` and `mutated_compiler_flags` appends the
//! additions *after* the filtered canonical list on purpose. There is no
//! `HashMap` or `HashSet` in this crate. A hash container in the mutation path
//! would compile, read fine, and silently emit a different compiler invocation.

use std::path::PathBuf;

use crate::bundle::{
    compiler_command_for_target, gcc3_cflags, gcc3_driver, validate_agbcc_bundle, validate_bundle,
    validate_experimental_compiler,
};
use crate::bundle_data::GCC3_EXPECTED;
use crate::routing::{
    agbcc_cflags, agbcc_driver, bundle, cflags_for_source, cflags_for_target,
    cflags_for_target_source, root, uses_agbcc_compiler, CompilerTarget,
};

use crate::nodepath::{basename, extname};

/// Every fallible operation here corresponds to a `throw new Error(...)` in the
/// TypeScript; the message text is reproduced verbatim because callers print it.
pub type Result<T> = std::result::Result<T, String>;

// ---------------------------------------------------------------------------
// Families
// ---------------------------------------------------------------------------

/// `CompilerFamily`. `Routed` is the request to derive the family from the
/// routing tables rather than name one.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CompilerFamily {
    Routed,
    Gcc296,
    OldAgbcc,
    Gcc3,
}

/// `Exclude<CompilerFamily, "routed">`, the resolved family a plan carries.
///
/// PORT NOTE: TypeScript expresses this as a subtraction on a union; Rust gets
/// a second enum so "routed" is simply not representable in a finished plan.
/// That is the same guarantee, moved from the type checker into the shape.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ResolvedFamily {
    Gcc296,
    OldAgbcc,
    Gcc3,
}

impl CompilerFamily {
    /// The string the TypeScript union member is spelled with. Used in error
    /// messages, which callers print, so the exact text matters.
    pub fn as_str(self) -> &'static str {
        match self {
            Self::Routed => "routed",
            Self::Gcc296 => "gcc296",
            Self::OldAgbcc => "old-agbcc",
            Self::Gcc3 => "gcc3",
        }
    }

    /// Parse the union member text. `None` for anything else, matching the fact
    /// that TypeScript would reject the literal at compile time.
    pub fn parse(text: &str) -> Option<Self> {
        Some(match text {
            "routed" => Self::Routed,
            "gcc296" => Self::Gcc296,
            "old-agbcc" => Self::OldAgbcc,
            "gcc3" => Self::Gcc3,
            _ => return None,
        })
    }

    /// Every member, in the declaration order of the TypeScript union. Callers
    /// that sweep the whole space (the corpus harness) rely on this order.
    pub const ALL: [CompilerFamily; 4] = [
        CompilerFamily::Routed,
        CompilerFamily::Gcc296,
        CompilerFamily::OldAgbcc,
        CompilerFamily::Gcc3,
    ];
}

impl ResolvedFamily {
    pub fn as_str(self) -> &'static str {
        CompilerFamily::from(self).as_str()
    }
}

impl From<ResolvedFamily> for CompilerFamily {
    fn from(family: ResolvedFamily) -> Self {
        match family {
            ResolvedFamily::Gcc296 => Self::Gcc296,
            ResolvedFamily::OldAgbcc => Self::OldAgbcc,
            ResolvedFamily::Gcc3 => Self::Gcc3,
        }
    }
}

// ---------------------------------------------------------------------------
// Options and plan
// ---------------------------------------------------------------------------

/// `CompilerFlagMutations`.
///
/// PORT NOTE: `removeFlags` becomes a `Set` in the TypeScript and is used only
/// for membership, so its iteration order is unobservable -- but it is still a
/// `Vec` here. The rule in this crate is that no unordered container exists at
/// all, because the one that leaks into the *ordered* list is unrecoverable and
/// silent. Linear search over a handful of flags is also faster.
#[derive(Debug, Clone, Default)]
pub struct CompilerFlagMutations {
    pub add_flags: Vec<String>,
    pub remove_flags: Vec<String>,
}

/// `SourceToAssemblyPlanOptions`. The `Option` fields are the TypeScript's
/// optional properties; `None` is `undefined`, which the defaulting below
/// treats exactly as `??` does.
#[derive(Debug, Clone)]
pub struct SourceToAssemblyPlanOptions {
    pub target: CompilerTarget,
    pub routing_source: String,
    pub input: String,
    pub output: String,
    pub family: Option<CompilerFamily>,
    pub flags: Option<CompilerFlagMutations>,
    /// Flags consumed by preprocessing rather than cc1. This distinction is
    /// load-bearing for old-agbcc: its compile step receives an already
    /// preprocessed `.i` file and rejects driver-level `-D` options.
    pub preprocessor_flags: Vec<String>,
    /// old_agbcc consumes preprocessed input. Supplying this makes intermediate
    /// ownership explicit; otherwise it is placed beside the assembly output.
    pub preprocessed_output: Option<String>,
    pub dumpbase: Option<String>,
}

impl SourceToAssemblyPlanOptions {
    /// The four required properties, with every optional one `undefined`.
    pub fn new(
        target: CompilerTarget,
        routing_source: impl Into<String>,
        input: impl Into<String>,
        output: impl Into<String>,
    ) -> Self {
        Self {
            target,
            routing_source: routing_source.into(),
            input: input.into(),
            output: output.into(),
            family: None,
            flags: None,
            preprocessor_flags: Vec::new(),
            preprocessed_output: None,
            dumpbase: None,
        }
    }
}

/// `CompilerCommandStep`.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CompilerCommandStep {
    pub kind: StepKind,
    pub command: Vec<String>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum StepKind {
    Preprocess,
    Compile,
}

impl StepKind {
    pub fn as_str(self) -> &'static str {
        match self {
            Self::Preprocess => "preprocess",
            Self::Compile => "compile",
        }
    }
}

/// `SourceToAssemblyPlan`.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SourceToAssemblyPlan {
    pub target: CompilerTarget,
    pub requested_family: CompilerFamily,
    pub family: ResolvedFamily,
    pub routing_source: String,
    pub input: String,
    pub output: String,
    pub compiler_input: String,
    pub flags: Vec<String>,
    pub steps: Vec<CompilerCommandStep>,
}

// ---------------------------------------------------------------------------
// mutatedCompilerFlags
// ---------------------------------------------------------------------------

/// `mutatedCompilerFlags(canonical, mutations = {})`.
///
/// ORDER IS THE WHOLE POINT: the canonical list is filtered in place and the
/// additions are appended, so an added `-O1` beats a canonical `-O2` under
/// gcc's later-flag-wins rule. Reordering these two spreads changes which
/// optimisation level the compiler actually uses.
///
/// PORT NOTE -- the add/remove conflict check reproduces the TypeScript's exact
/// asymmetry. It iterates `added` and asks whether `removed` contains it, so a
/// flag listed twice in `addFlags` is reported once per occurrence but only the
/// first reaches the `throw`; and because the check runs before the filter, a
/// conflicting flag that is not even in `canonical` still throws. Both fall out
/// of the original's structure, not from a decision made here.
pub fn mutated_compiler_flags(
    canonical: &[String],
    mutations: Option<&CompilerFlagMutations>,
) -> Result<Vec<String>> {
    let empty = CompilerFlagMutations::default();
    let mutations = mutations.unwrap_or(&empty);
    let added = &mutations.add_flags;
    let removed = &mutations.remove_flags;
    for flag in added {
        if removed.iter().any(|candidate| candidate == flag) {
            return Err(format!(
                "compiler flag cannot be both added and removed: {flag}"
            ));
        }
    }
    let mut out: Vec<String> = canonical
        .iter()
        .filter(|flag| !removed.iter().any(|candidate| candidate == *flag))
        .cloned()
        .collect();
    out.extend(added.iter().cloned());
    Ok(out)
}

/// `inferredPreprocessedOutput(output)`.
///
/// PORT NOTE -- BUG, REPRODUCED AS WRITTEN. `output.slice(0, -extension.length)`
/// assumes the extension node:path found sits at the very end of the string.
/// For an output path with a trailing slash it does not: `extname("a.c/")` is
/// `".c"`, so the slice cuts the last two characters of `"a.c/"` and yields
/// `"a."`, producing the intermediate `"a..i"` rather than anything under
/// `a.c/`. No caller in the tree passes a trailing slash as an assembly output
/// path, so this is latent rather than live -- and it is left alone because
/// "fix" here means choosing a different intermediate filename, which is
/// build-cache-visible. It is pinned by a unit test so it cannot change by
/// accident.
pub fn inferred_preprocessed_output(output: &str) -> String {
    let extension = extname(output);
    if extension.is_empty() {
        format!("{output}.i")
    } else {
        format!("{}.i", &output[..output.len() - extension.len()])
    }
}

// ---------------------------------------------------------------------------
// sourceToAssemblyPlan
// ---------------------------------------------------------------------------

/// `sourceToAssemblyPlan(options)`.
pub fn source_to_assembly_plan(
    options: &SourceToAssemblyPlanOptions,
) -> Result<SourceToAssemblyPlan> {
    let requested_family = options.family.unwrap_or(CompilerFamily::Routed);
    let family = match requested_family {
        // The routed family follows owner evidence. The shared audio-engine
        // owners use old_agbcc in both games; GS2's remaining routed owners use
        // GCC 2.96.
        CompilerFamily::Routed => {
            if uses_agbcc_compiler(options.target, &options.routing_source) {
                ResolvedFamily::OldAgbcc
            } else {
                ResolvedFamily::Gcc296
            }
        }
        CompilerFamily::Gcc296 => ResolvedFamily::Gcc296,
        CompilerFamily::OldAgbcc => ResolvedFamily::OldAgbcc,
        CompilerFamily::Gcc3 => ResolvedFamily::Gcc3,
    };
    if family == ResolvedFamily::Gcc3 && options.target != CompilerTarget::Gs1 {
        return Err(format!("{} is only approved for gs1", family.as_str()));
    }

    // One arm here is surprising and is reported rather than fixed: only the
    // `routed` request consults `cflagsForTargetSource`. Asking explicitly for
    // `gcc296` gets `cflagsForTarget`, i.e. the target's base flags WITHOUT the
    // per-source routing additions, so a caller that names the family it
    // expected to be routed to gets a different command line than `routed`
    // would have produced for the same source. Changing it changes emitted
    // assembly for every sweep already on record.
    let canonical: Vec<String> = if requested_family == CompilerFamily::Routed {
        cflags_for_target_source(options.target, &options.routing_source)
    } else if family == ResolvedFamily::Gcc3 {
        gcc3_cflags()
    } else if family != ResolvedFamily::Gcc296 {
        agbcc_cflags()
    } else {
        cflags_for_target(options.target)
    };
    let flags = mutated_compiler_flags(&canonical, options.flags.as_ref())?;
    let dumpbase = options
        .dumpbase
        .clone()
        .unwrap_or_else(|| basename(&options.routing_source).to_string());
    let mut compiler_input = options.input.clone();
    let mut steps: Vec<CompilerCommandStep> = Vec::new();

    if family != ResolvedFamily::Gcc296 {
        let driver: PathBuf = match family {
            ResolvedFamily::OldAgbcc => agbcc_driver(),
            ResolvedFamily::Gcc3 => gcc3_driver(),
            // gcc296 never takes this branch; it is handled above.
            ResolvedFamily::Gcc296 => unreachable!("gcc296 does not use a separate driver"),
        };
        match family {
            ResolvedFamily::OldAgbcc => validate_agbcc_bundle()?,
            ResolvedFamily::Gcc3 => {
                validate_experimental_compiler(family.as_str(), &driver, GCC3_EXPECTED)?
            }
            ResolvedFamily::Gcc296 => unreachable!("gcc296 does not use a separate driver"),
        }
        compiler_input = options
            .preprocessed_output
            .clone()
            .unwrap_or_else(|| inferred_preprocessed_output(&options.output));
        // old-agbcc preprocesses as GNUC 2.9. See the `-D__GNUC__=2` note on
        // `direct_preprocessor_command` for why the gcc3 value of 0 is a defect
        // that is nonetheless left in place.
        let gcc_minor = match family {
            ResolvedFamily::Gcc3 => 0,
            _ => 9,
        };
        steps.push(CompilerCommandStep {
            kind: StepKind::Preprocess,
            command: direct_preprocessor_command_for_target_with_minor_and_flags(
                options.target,
                &options.input,
                &compiler_input,
                gcc_minor,
                &options.preprocessor_flags,
            )?,
        });
        let mut command = vec![
            driver.to_string_lossy().into_owned(),
            compiler_input.clone(),
            "-dumpbase".to_string(),
            dumpbase.clone(),
        ];
        command.extend(flags.iter().cloned());
        command.push("-o".to_string());
        command.push(options.output.clone());
        steps.push(CompilerCommandStep {
            kind: StepKind::Compile,
            command,
        });
    } else {
        let mut arguments = flags.clone();
        arguments.extend(options.preprocessor_flags.iter().cloned());
        arguments.push("-S".to_string());
        arguments.push("-o".to_string());
        arguments.push(options.output.clone());
        arguments.push(options.input.clone());
        steps.push(CompilerCommandStep {
            kind: StepKind::Compile,
            command: compiler_command_for_target(options.target, &arguments)?,
        });
    }

    Ok(SourceToAssemblyPlan {
        target: options.target,
        requested_family,
        family,
        routing_source: options.routing_source.clone(),
        input: options.input.clone(),
        output: options.output.clone(),
        compiler_input,
        flags,
        steps,
    })
}

// ---------------------------------------------------------------------------
// Direct (driver-free) command builders
// ---------------------------------------------------------------------------

/// `directPreprocessorCommand(input, output)`, i.e. with `gccMinor` defaulted
/// to 96.
///
/// Hot-search pipeline: invoke the approved preprocessor and cc1 directly,
/// saving one driver process per candidate. These arguments are the exact
/// subprocesses emitted by xgcc for CFLAGS; ordinary builds use the
/// source-aware command API.
pub fn direct_preprocessor_command(input: &str, output: &str) -> Result<Vec<String>> {
    direct_preprocessor_command_with_minor(input, output, 96)
}

/// `directPreprocessorCommand(input, output, gccMinor)`.
///
/// PORT NOTE -- `-D__GNUC__=2` IS HARD-CODED, and that is a real defect once
/// `sourceToAssemblyPlan` reaches here for the `gcc3` family. gcc3 preprocesses
/// with `__GNUC__=2 __GNUC_MINOR__=0`, which is gcc 2.0, not gcc 3.0: any header
/// in `games/gs1/include/` guarded on `__GNUC__ >= 3` takes the wrong branch, and the 2.0
/// combination is a version that never existed in this tree. It is reproduced
/// rather than corrected because every gcc3 sweep already on record was measured
/// through this preprocessor invocation, and changing the macro changes the
/// preprocessed text and therefore the assembly those results were scored on.
///
/// PORT NOTE -- `gccMinor` is a JavaScript number interpolated into a template
/// string, which is ECMAScript `ToString(Number)` and disagrees with Rust's
/// `Display` for non-integers (`1` vs `1.0`). This takes an `i64`, where the two
/// agree exactly, and pushes any float away from the boundary. The four values
/// the tree passes -- 96, 95, 9, 0 -- are all integers and are pinned by a test.
pub fn direct_preprocessor_command_with_minor(
    input: &str,
    output: &str,
    gcc_minor: i64,
) -> Result<Vec<String>> {
    direct_preprocessor_command_with_minor_and_flags(input, output, gcc_minor, &[])
}

fn direct_preprocessor_command_with_minor_and_flags(
    input: &str,
    output: &str,
    gcc_minor: i64,
    flags: &[String],
) -> Result<Vec<String>> {
    direct_preprocessor_command_for_target_with_minor_and_flags(
        CompilerTarget::Gs1,
        input,
        output,
        gcc_minor,
        flags,
    )
}

fn direct_preprocessor_command_for_target_with_minor_and_flags(
    target: CompilerTarget,
    input: &str,
    output: &str,
    gcc_minor: i64,
    flags: &[String],
) -> Result<Vec<String>> {
    validate_bundle(target)?;
    let mut command = vec![
        bundle().join("cpp").to_string_lossy().into_owned(),
        "-lang-c".into(),
        "-nostdinc".into(),
        "-D__GNUC__=2".into(),
        format!("-D__GNUC_MINOR__={gcc_minor}"),
        "-D__GNUC_PATCHLEVEL__=0".into(),
        "-Acpu(arm)".into(),
        "-Amachine(arm)".into(),
        "-D__CHAR_UNSIGNED__".into(),
        "-D__OPTIMIZE__".into(),
        "-D__ARM_ARCH_4T__".into(),
        "-D__APCS_32__".into(),
        "-D__ARMEL__".into(),
        "-D__THUMBEL__".into(),
        "-Darm_elf".into(),
        "-D__ELF__".into(),
        "-Dthumb".into(),
        "-D__thumb__".into(),
        format!(
            "-I{}",
            root()
                .join("games")
                .join(target.as_str())
                .join("include")
                .display()
        ),
    ];
    command.extend(flags.iter().cloned());
    command.push(input.to_string());
    command.push(output.to_string());
    Ok(command)
}

/// `directCompilerCommand(input, output, dumpbase, source = dumpbase)`.
///
/// PORT NOTE -- THE DEFAULT `source = dumpbase` IS A TRAP, reproduced as
/// written. `cflagsForSource` keys on `sourceKey(source)` (a repository-relative
/// path) for the overlay tables and on `overlayStem(source)` (a bare stem) for
/// the rest. `dumpbase` is a *basename*, so when a caller omits `source` the
/// stem-keyed routing still matches but every path-keyed overlay table misses
/// silently, and the candidate is compiled with base CFLAGS instead of its
/// evidenced overlay flags. There is no error; the assembly just differs. Every
/// call that cares passes `source` explicitly -- `direct_compiler_command_for_source`
/// below always does. Left alone: making the parameter required is a signature
/// change across the TypeScript tree, and silently substituting a full path
/// would change the `-dumpbase` argument, which appears in the emitted `.s`.
pub fn direct_compiler_command(
    input: &str,
    output: &str,
    dumpbase: &str,
    source: Option<&str>,
) -> Result<Vec<String>> {
    validate_bundle(CompilerTarget::Gs1)?;
    let source = source.unwrap_or(dumpbase);
    // cc1 is not the driver: it has no `-B` search path and no built-in include
    // handling, so the driver-level `-nostdinc` and `-I` flags are dropped here
    // and the include path arrives via the preprocessor step instead.
    let flags: Vec<String> = cflags_for_source(source)
        .into_iter()
        .filter(|flag| flag != "-nostdinc" && !flag.starts_with("-I"))
        .collect();
    let mut out = vec![
        bundle().join("cc1").to_string_lossy().into_owned(),
        input.to_string(),
        "-quiet".to_string(),
        "-dumpbase".to_string(),
        dumpbase.to_string(),
    ];
    out.extend(flags);
    out.push("-o".to_string());
    out.push(output.to_string());
    Ok(out)
}

/// `directCompilerCommandForSource(source, input, output, dumpbase)`.
///
/// PORT NOTE: hard-wired to `"gs1"` in the TypeScript -- both the
/// `usesAgbccCompiler` test and the `cflagsForTargetSource` call. A gs2 source
/// routed through here would silently receive gs1 flags, which is why nothing
/// in the tree does; the hard-wiring is preserved rather than parameterised.
/// Note also that the agbcc branch omits `-quiet`, which the gcc296 branch
/// passes: old_agbcc is a driver-shaped binary, not a bare cc1.
pub fn direct_compiler_command_for_source(
    source: &str,
    input: &str,
    output: &str,
    dumpbase: &str,
) -> Result<Vec<String>> {
    if !uses_agbcc_compiler(CompilerTarget::Gs1, source) {
        return direct_compiler_command(input, output, dumpbase, Some(source));
    }
    validate_agbcc_bundle()?;
    let mut out = vec![
        agbcc_driver().to_string_lossy().into_owned(),
        input.to_string(),
        "-dumpbase".to_string(),
        dumpbase.to_string(),
    ];
    out.extend(cflags_for_target_source(CompilerTarget::Gs1, source));
    out.push("-o".to_string());
    out.push(output.to_string());
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn edition_define_stays_in_old_agbcc_preprocessor_step() {
        let mut options = SourceToAssemblyPlanOptions::new(
            CompilerTarget::Gs1,
            "games/gs1/src/080000c0.c",
            "candidate.c",
            "candidate.s",
        );
        options.family = Some(CompilerFamily::OldAgbcc);
        options.preprocessor_flags = vec!["-DGS1_EDITION_JA=1".into()];

        let plan = source_to_assembly_plan(&options).unwrap();
        assert!(plan.steps[0]
            .command
            .iter()
            .any(|argument| argument == "-DGS1_EDITION_JA=1"));
        assert!(!plan.steps[1]
            .command
            .iter()
            .any(|argument| argument.starts_with("-DGS1_EDITION_")));
    }

    #[test]
    fn edition_define_reaches_gcc296_driver() {
        let mut options = SourceToAssemblyPlanOptions::new(
            CompilerTarget::Gs2,
            "games/gs2/src/08120450.c",
            "candidate.c",
            "candidate.s",
        );
        options.family = Some(CompilerFamily::Gcc296);
        options.preprocessor_flags = vec!["-DGS2_EDITION_IT=1".into()];

        let plan = source_to_assembly_plan(&options).unwrap();
        assert_eq!(plan.steps.len(), 1);
        assert!(plan.steps[0]
            .command
            .iter()
            .any(|argument| argument == "-DGS2_EDITION_IT=1"));
    }

    #[test]
    fn shared_gs2_audio_owner_routes_through_old_agbcc() {
        let mut options = SourceToAssemblyPlanOptions::new(
            CompilerTarget::Gs2,
            "games/gs2/src/081c28e0.c",
            "candidate.c",
            "candidate.s",
        );
        options.preprocessor_flags = vec!["-DGS2_EDITION_JA=1".into()];

        let plan = source_to_assembly_plan(&options).unwrap();
        assert!(plan.steps[1].command[0].ends_with("/agbcc/old_agbcc"));
        assert!(plan.steps[0]
            .command
            .iter()
            .any(|argument| argument == "-DGS2_EDITION_JA=1"));
    }
}
