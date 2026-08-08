//! The compilation-plan layer of `tools/lib/alchemy_gcc.ts`: the boundary
//! between compiler policy and process execution.
//!
//! Ported from `alchemy_gcc.ts` lines 3594-3790:
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
//! ORDER IS BEHAVIOUR, as in `alchemy-bundle`. gcc is later-flag-wins, so every
//! flag list here is an ordered `Vec` and `mutated_compiler_flags` appends the
//! additions *after* the filtered canonical list on purpose. There is no
//! `HashMap` or `HashSet` in this crate. A hash container in the mutation path
//! would compile, read fine, and silently emit a different compiler invocation.

use std::path::PathBuf;

use alchemy_bundle::bundle::{
    compiler_command_for_target, gcc2951_driver, gcc3_cflags, gcc3_driver, pret_early_thumb_driver,
    validate_agbcc_bundle, validate_bundle, validate_experimental_compiler,
};
use alchemy_bundle::bundle_data::{GCC2951_EXPECTED, GCC3_EXPECTED, PRET_EARLY_THUMB_EXPECTED};
use alchemy_routing::routing::{
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
    PretEarlyThumb,
    Gcc2951,
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
    PretEarlyThumb,
    Gcc2951,
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
            Self::PretEarlyThumb => "pret-early-thumb",
            Self::Gcc2951 => "gcc2951",
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
            "pret-early-thumb" => Self::PretEarlyThumb,
            "gcc2951" => Self::Gcc2951,
            "gcc3" => Self::Gcc3,
            _ => return None,
        })
    }

    /// Every member, in the declaration order of the TypeScript union. Callers
    /// that sweep the whole space (the corpus harness) rely on this order.
    pub const ALL: [CompilerFamily; 6] = [
        CompilerFamily::Routed,
        CompilerFamily::Gcc296,
        CompilerFamily::OldAgbcc,
        CompilerFamily::PretEarlyThumb,
        CompilerFamily::Gcc2951,
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
            ResolvedFamily::PretEarlyThumb => Self::PretEarlyThumb,
            ResolvedFamily::Gcc2951 => Self::Gcc2951,
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
        // `usesAgbccCompiler` is gs1-only, so a routed gs2 plan always resolves
        // to gcc296 and can never trip the gs1-only guard below.
        CompilerFamily::Routed => {
            if uses_agbcc_compiler(options.target, &options.routing_source) {
                ResolvedFamily::OldAgbcc
            } else {
                ResolvedFamily::Gcc296
            }
        }
        CompilerFamily::Gcc296 => ResolvedFamily::Gcc296,
        CompilerFamily::OldAgbcc => ResolvedFamily::OldAgbcc,
        CompilerFamily::PretEarlyThumb => ResolvedFamily::PretEarlyThumb,
        CompilerFamily::Gcc2951 => ResolvedFamily::Gcc2951,
        CompilerFamily::Gcc3 => ResolvedFamily::Gcc3,
    };
    if family != ResolvedFamily::Gcc296 && options.target != CompilerTarget::Gs1 {
        return Err(format!("{} is only approved for gs1", family.as_str()));
    }

    // PORT NOTE: the canonical-flag ternary chain is transcribed branch for
    // branch rather than tidied, because two of its arms are surprising and
    // tidying them would look like a correction:
    //
    //   * only the `routed` request consults `cflagsForTargetSource`. Asking
    //     explicitly for `gcc296` gets `cflagsForTarget`, i.e. the target's base
    //     flags WITHOUT the per-source routing additions. A caller that names
    //     the family it expected to be routed to therefore gets a different
    //     command line than `routed` would have produced for the same source.
    //   * `pret-early-thumb` and `gcc2951` both fall through to `AGBCC_CFLAGS`,
    //     which is the old-agbcc flag set, not their own. Only `gcc3` has its
    //     own list.
    //
    // Both are reported, not fixed. Changing either changes emitted assembly for
    // every experimental sweep already on record.
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
            ResolvedFamily::PretEarlyThumb => pret_early_thumb_driver(),
            ResolvedFamily::Gcc3 => gcc3_driver(),
            // The TypeScript's final ternary arm. gcc296 cannot reach here.
            ResolvedFamily::Gcc2951 | ResolvedFamily::Gcc296 => gcc2951_driver(),
        };
        match family {
            ResolvedFamily::OldAgbcc => validate_agbcc_bundle()?,
            ResolvedFamily::PretEarlyThumb => {
                validate_experimental_compiler(family.as_str(), &driver, PRET_EARLY_THUMB_EXPECTED)?
            }
            ResolvedFamily::Gcc3 => {
                validate_experimental_compiler(family.as_str(), &driver, GCC3_EXPECTED)?
            }
            ResolvedFamily::Gcc2951 | ResolvedFamily::Gcc296 => {
                validate_experimental_compiler(family.as_str(), &driver, GCC2951_EXPECTED)?
            }
        }
        compiler_input = options
            .preprocessed_output
            .clone()
            .unwrap_or_else(|| inferred_preprocessed_output(&options.output));
        // PORT NOTE: the minor-version selector is `gcc2951 ? 95 : gcc3 ? 0 : 9`,
        // so old-agbcc AND pret-early-thumb both preprocess as GNUC 2.9. See the
        // `-D__GNUC__=2` note on `direct_preprocessor_command` for why the gcc3
        // value of 0 is a defect that is nonetheless left in place.
        let gcc_minor = match family {
            ResolvedFamily::Gcc2951 => 95,
            ResolvedFamily::Gcc3 => 0,
            _ => 9,
        };
        steps.push(CompilerCommandStep {
            kind: StepKind::Preprocess,
            command: direct_preprocessor_command_with_minor(
                &options.input,
                &compiler_input,
                gcc_minor,
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
/// in `include/` guarded on `__GNUC__ >= 3` takes the wrong branch, and the 2.0
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
    validate_bundle(CompilerTarget::Gs1)?;
    Ok(vec![
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
        format!("-I{}", root().join("include").display()),
        input.to_string(),
        output.to_string(),
    ])
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

// ---------------------------------------------------------------------------
// Literal mirrors for the parity gate
// ---------------------------------------------------------------------------
//
// WHY THESE EXIST. [`crate::drift`] reads `tools/lib/alchemy_gcc.ts` and pulls
// the string literals out of each ported function, in source order, then
// compares them against the arrays below. The arrays are spelled in the
// TypeScript's own notation -- template literals included, delimiters stripped
// and nothing else normalised -- because the point is to quote the source, not
// to paraphrase it.
//
// They are not a second source of truth and they are not tautological: the unit
// tests at the bottom of this file tie each array back to what the Rust
// functions actually emit. An array can only satisfy the gate and the unit test
// at once if the port matches the TypeScript, which is the property we want.
//
// Order is load-bearing everywhere here. In the argv arrays it is the order gcc
// sees the flags in, and gcc is later-flag-wins. In
// `SOURCE_TO_ASSEMBLY_PLAN_LITERALS` it is the order the ternary chains test
// families in, which decides which driver a family resolves to.

/// String literals of `sourceToAssemblyPlan`, in source order.
pub const SOURCE_TO_ASSEMBLY_PLAN_LITERALS: [&str; 26] = [
    // `options.family ?? "routed"`, then the `requestedFamily === "routed"` test.
    "routed",
    "routed",
    "old-agbcc",
    "gcc296",
    // The gs1-only guard.
    "gcc296",
    "gs1",
    "${family} is only approved for gs1",
    // The canonical-flag ternary chain.
    "routed",
    "gcc3",
    "gcc296",
    // The non-gcc296 branch: driver selection, then validation.
    "gcc296",
    "old-agbcc",
    "pret-early-thumb",
    "gcc3",
    "old-agbcc",
    "pret-early-thumb",
    "gcc3",
    // The preprocess step and its gccMinor selector.
    "preprocess",
    "gcc2951",
    "gcc3",
    // The non-gcc296 compile step.
    "compile",
    "-dumpbase",
    "-o",
    // The gcc296 compile step.
    "compile",
    "-S",
    "-o",
];

/// String literals of `mutatedCompilerFlags`, in source order.
pub const MUTATED_COMPILER_FLAGS_LITERALS: [&str; 1] =
    ["compiler flag cannot be both added and removed: ${flag}"];

/// String literals of `inferredPreprocessedOutput`, in source order. The third
/// entry is the trailing-slash defect documented on
/// [`inferred_preprocessed_output`], quoted here exactly as written so the gate
/// notices if someone quietly repairs it upstream.
pub const INFERRED_PREPROCESSED_OUTPUT_LITERALS: [&str; 3] =
    ["", "${output}.i", "${output.slice(0, -extension.length)}.i"];

/// String literals of `directPreprocessorCommand`, in source order -- which for
/// this function is very nearly the emitted argv.
pub const DIRECT_PREPROCESSOR_LITERALS: [&str; 19] = [
    "cpp",
    "-lang-c",
    "-nostdinc",
    "-D__GNUC__=2",
    "-D__GNUC_MINOR__=${gccMinor}",
    "-D__GNUC_PATCHLEVEL__=0",
    "-Acpu(arm)",
    "-Amachine(arm)",
    "-D__CHAR_UNSIGNED__",
    "-D__OPTIMIZE__",
    "-D__ARM_ARCH_4T__",
    "-D__APCS_32__",
    "-D__ARMEL__",
    "-D__THUMBEL__",
    "-Darm_elf",
    "-D__ELF__",
    "-Dthumb",
    "-D__thumb__",
    "-I${join(ROOT, \"include\")}",
];

/// String literals of `directCompilerCommand`, in source order. The first two
/// belong to the cc1 flag filter, not to the argv.
pub const DIRECT_COMPILER_LITERALS: [&str; 6] =
    ["-nostdinc", "-I", "cc1", "-quiet", "-dumpbase", "-o"];

/// String literals of `directCompilerCommandForSource`, in source order. Both
/// `"gs1"` entries are the hard-wiring documented on
/// [`direct_compiler_command_for_source`].
pub const DIRECT_COMPILER_FOR_SOURCE_LITERALS: [&str; 4] = ["gs1", "-dumpbase", "gs1", "-o"];

#[cfg(test)]
mod tests {
    use super::*;

    fn strings(values: &[&str]) -> Vec<String> {
        values.iter().map(|value| (*value).to_string()).collect()
    }

    #[test]
    fn additions_land_after_the_canonical_list_so_they_win() {
        // gcc is later-flag-wins. If this ever reports `-O1` before `-O2`, the
        // mutation API has stopped meaning what every sweep assumed it meant.
        let canonical = strings(&["-O2", "-mthumb", "-fno-builtin"]);
        let mutations = CompilerFlagMutations {
            add_flags: strings(&["-O1"]),
            remove_flags: strings(&["-mthumb"]),
        };
        let flags = mutated_compiler_flags(&canonical, Some(&mutations)).expect("no conflict");
        assert_eq!(flags, strings(&["-O2", "-fno-builtin", "-O1"]));
    }

    #[test]
    fn no_mutations_is_the_canonical_list_unchanged() {
        let canonical = strings(&["-O2", "-mthumb"]);
        assert_eq!(
            mutated_compiler_flags(&canonical, None).expect("no conflict"),
            canonical
        );
        assert_eq!(
            mutated_compiler_flags(&canonical, Some(&CompilerFlagMutations::default()))
                .expect("no conflict"),
            canonical
        );
    }

    #[test]
    fn a_flag_that_is_both_added_and_removed_throws() {
        let mutations = CompilerFlagMutations {
            add_flags: strings(&["-O1"]),
            remove_flags: strings(&["-O1"]),
        };
        let error = mutated_compiler_flags(&[], Some(&mutations)).expect_err("must reject");
        assert_eq!(error, "compiler flag cannot be both added and removed: -O1");
    }

    #[test]
    fn the_conflict_check_runs_before_the_filter() {
        // PORT NOTE guard: the flag is absent from `canonical`, so a
        // filter-then-check implementation would happily return `["-O1"]`.
        let mutations = CompilerFlagMutations {
            add_flags: strings(&["-fzap"]),
            remove_flags: strings(&["-fzap"]),
        };
        assert!(mutated_compiler_flags(&strings(&["-O2"]), Some(&mutations)).is_err());
    }

    #[test]
    fn duplicate_removals_and_additions_behave_as_the_spread_does() {
        // `[...canonical.filter(...), ...added]` keeps duplicates in `added`,
        // and `removed` being a Set means a doubled removal removes once.
        let canonical = strings(&["-O2", "-O2", "-mthumb"]);
        let mutations = CompilerFlagMutations {
            add_flags: strings(&["-g", "-g"]),
            remove_flags: strings(&["-O2", "-O2"]),
        };
        let flags = mutated_compiler_flags(&canonical, Some(&mutations)).expect("no conflict");
        assert_eq!(flags, strings(&["-mthumb", "-g", "-g"]));
    }

    #[test]
    fn inferred_intermediate_replaces_the_extension() {
        assert_eq!(inferred_preprocessed_output("out/main.s"), "out/main.i");
        assert_eq!(inferred_preprocessed_output("out/main"), "out/main.i");
        assert_eq!(inferred_preprocessed_output("out/a.b.s"), "out/a.b.i");
        // A dotted directory does not count as an extension.
        assert_eq!(inferred_preprocessed_output("out.d/main"), "out.d/main.i");
        // A dotfile has no extension in node:path.
        assert_eq!(inferred_preprocessed_output("out/.main"), "out/.main.i");
    }

    #[test]
    fn inferred_intermediate_reproduces_the_trailing_slash_bug() {
        // PORT NOTE guard, see `inferred_preprocessed_output`. This is WRONG and
        // pinned deliberately: node:path finds `.c` inside `a.c/`, and the
        // slice then cuts characters that are not the extension.
        assert_eq!(inferred_preprocessed_output("out/a.c/"), "out/a..i");
        // The correct-looking answer, asserted as NOT what happens, so a
        // "cleanup" that produces it fails here and has to read the note.
        assert_ne!(inferred_preprocessed_output("out/a.c/"), "out/a.c/.i");
    }

    #[test]
    fn family_names_round_trip_and_cover_the_union() {
        let mut checked = 0;
        for family in CompilerFamily::ALL {
            assert_eq!(CompilerFamily::parse(family.as_str()), Some(family));
            checked += 1;
        }
        // FLOOR: the TypeScript union has exactly six members.
        assert_eq!(checked, 6);
        assert_eq!(CompilerFamily::parse("gcc297"), None);
        // Resolved families never spell themselves "routed".
        for family in [
            ResolvedFamily::Gcc296,
            ResolvedFamily::OldAgbcc,
            ResolvedFamily::PretEarlyThumb,
            ResolvedFamily::Gcc2951,
            ResolvedFamily::Gcc3,
        ] {
            assert_ne!(family.as_str(), "routed");
        }
    }

    #[test]
    fn non_gcc296_families_are_rejected_on_gs2() {
        for family in [
            CompilerFamily::OldAgbcc,
            CompilerFamily::PretEarlyThumb,
            CompilerFamily::Gcc2951,
            CompilerFamily::Gcc3,
        ] {
            let mut options = SourceToAssemblyPlanOptions::new(
                CompilerTarget::Gs2,
                "exact/main.c",
                "/tmp/in.c",
                "/tmp/out.s",
            );
            options.family = Some(family);
            let error = source_to_assembly_plan(&options).expect_err("gs2 must be refused");
            assert_eq!(
                error,
                format!("{} is only approved for gs1", family.as_str())
            );
        }
    }

    #[test]
    fn routed_gs2_never_reaches_the_gs1_only_guard() {
        // `usesAgbccCompiler` tests `target === "gs1"` first, so no gs2 source
        // can route to old-agbcc and trip the guard above. If routing ever
        // changes that, this test is where it surfaces.
        for source in ["exact/main.c", "exact/agb_flash.c", "semantic/whatever.c"] {
            assert!(!uses_agbcc_compiler(CompilerTarget::Gs2, source));
        }
    }

    #[test]
    fn the_gcc_minor_selector_covers_only_integers() {
        // PORT NOTE guard for ECMAScript ToString(Number): every value the tree
        // interpolates must render identically in both languages, which for
        // integers it does. A float here would print `1.0` in Rust and `1` in
        // JavaScript.
        for minor in [96i64, 95, 9, 0] {
            assert_eq!(format!("-D__GNUC_MINOR__={minor}"), {
                let mut text = String::from("-D__GNUC_MINOR__=");
                text.push_str(&minor.to_string());
                text
            });
            assert!(!minor.to_string().contains('.'));
        }
    }

    #[test]
    fn gcc3_cflags_are_the_only_family_specific_list() {
        // PORT NOTE guard: pret-early-thumb and gcc2951 deliberately share
        // AGBCC_CFLAGS. Pinned so the sharing cannot be "fixed" silently.
        let agbcc = agbcc_cflags();
        assert!(!agbcc.is_empty(), "AGBCC_CFLAGS must not be empty");
        assert_ne!(gcc3_cflags(), agbcc);
        assert!(gcc3_cflags().iter().any(|flag| flag == "-ffixed-r7"));
    }
}
