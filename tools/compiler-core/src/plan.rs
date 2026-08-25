//! Compilation policy for source-to-assembly plans.
//!
//! `routing_source` selects evidenced compiler policy; `input` is the file being
//! compiled. Candidate sources need both names. Flag order is behavior because
//! GCC is later-flag-wins: additions follow the filtered canonical list, and no
//! unordered container may enter this path.

use std::path::PathBuf;

use crate::bundle::{compiler_command_for_target, gcc3_cflags, gcc3_driver, validate_agbcc_bundle, validate_bundle, validate_experimental_compiler};
use crate::bundle_data::GCC3_EXPECTED;
use crate::routing::{agbcc_cflags, agbcc_driver, bundle, cflags_for_source, cflags_for_target, cflags_for_target_source, root, uses_agbcc_compiler, CompilerTarget};

use crate::nodepath::{basename, extname};

/// Error text is user-facing and retained exactly.
pub type Result<T> = std::result::Result<T, String>;

/// `Routed` derives the family from routing evidence.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CompilerFamily {
    Routed,
    Gcc296,
    OldAgbcc,
    Gcc3,
}

/// A resolved family cannot be `Routed`.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ResolvedFamily {
    Gcc296,
    OldAgbcc,
    Gcc3,
}

impl CompilerFamily {
    /// User-facing spelling; changing it changes errors.
    pub fn as_str(self) -> &'static str {
        match self {
            Self::Routed => "routed",
            Self::Gcc296 => "gcc296",
            Self::OldAgbcc => "old-agbcc",
            Self::Gcc3 => "gcc3",
        }
    }

    pub fn parse(text: &str) -> Option<Self> {
        Some(match text {
            "routed" => Self::Routed,
            "gcc296" => Self::Gcc296,
            "old-agbcc" => Self::OldAgbcc,
            "gcc3" => Self::Gcc3,
            _ => return None,
        })
    }

    /// Declaration order is used by corpus sweeps.
    pub const ALL: [CompilerFamily; 4] = [CompilerFamily::Routed, CompilerFamily::Gcc296, CompilerFamily::OldAgbcc, CompilerFamily::Gcc3];
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

/// Ordered flag edits. Do not replace either vector with an unordered container.
#[derive(Debug, Clone, Default)]
pub struct CompilerFlagMutations {
    pub add_flags: Vec<String>,
    pub remove_flags: Vec<String>,
}

/// `None` carries the same defaulting meaning as an omitted option.
#[derive(Debug, Clone)]
pub struct SourceToAssemblyPlanOptions {
    pub target: CompilerTarget,
    pub routing_source: String,
    pub input: String,
    pub output: String,
    pub family: Option<CompilerFamily>,
    pub flags: Option<CompilerFlagMutations>,
    /// Preprocessor-only flags; old-agbcc rejects them on its `.i` compile step.
    pub preprocessor_flags: Vec<String>,
    /// Explicit old-agbcc intermediate; otherwise inferred beside the output.
    pub preprocessed_output: Option<String>,
    pub dumpbase: Option<String>,
}

impl SourceToAssemblyPlanOptions {
    pub fn new(target: CompilerTarget, routing_source: impl Into<String>, input: impl Into<String>, output: impl Into<String>) -> Self {
        Self { target, routing_source: routing_source.into(), input: input.into(), output: output.into(), family: None, flags: None, preprocessor_flags: Vec::new(), preprocessed_output: None, dumpbase: None }
    }
}

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

/// Filter canonical flags, then append additions: GCC's later flag wins.
/// Conflicts are rejected before filtering, including absent canonical flags.
pub fn mutated_compiler_flags(canonical: &[String], mutations: Option<&CompilerFlagMutations>) -> Result<Vec<String>> {
    let empty = CompilerFlagMutations::default();
    let mutations = mutations.unwrap_or(&empty);
    let added = &mutations.add_flags;
    let removed = &mutations.remove_flags;
    for flag in added {
        if removed.iter().any(|candidate| candidate == flag) {
            return Err(format!("compiler flag cannot be both added and removed: {flag}"));
        }
    }
    let mut out: Vec<String> = canonical.iter().filter(|flag| !removed.iter().any(|candidate| candidate == *flag)).cloned().collect();
    out.extend(added.iter().cloned());
    Ok(out)
}

/// Preserves the pinned trailing-slash bug: `a.c/` becomes `a..i`. Fixing it
/// would change a cache-visible intermediate name.
pub fn inferred_preprocessed_output(output: &str) -> String {
    let extension = extname(output);
    if extension.is_empty() {
        format!("{output}.i")
    } else {
        format!("{}.i", &output[..output.len() - extension.len()])
    }
}

pub fn source_to_assembly_plan(options: &SourceToAssemblyPlanOptions) -> Result<SourceToAssemblyPlan> {
    let requested_family = options.family.unwrap_or(CompilerFamily::Routed);
    let family = match requested_family {
        // Shared audio owners use old-agbcc in both games; other GS2 owners use 2.96.
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

    // Only Routed includes per-source flags. Explicit Gcc296 intentionally uses
    // target-base flags; changing that would invalidate recorded sweeps.
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
    let dumpbase = options.dumpbase.clone().unwrap_or_else(|| basename(&options.routing_source).to_string());
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
            ResolvedFamily::Gcc3 => validate_experimental_compiler(family.as_str(), &driver, GCC3_EXPECTED)?,
            ResolvedFamily::Gcc296 => unreachable!("gcc296 does not use a separate driver"),
        }
        compiler_input = options.preprocessed_output.clone().unwrap_or_else(|| inferred_preprocessed_output(&options.output));
        // old-agbcc identifies as 2.9; the preserved gcc3 defect identifies as 2.0.
        let gcc_minor = match family {
            ResolvedFamily::Gcc3 => 0,
            _ => 9,
        };
        steps.push(CompilerCommandStep { kind: StepKind::Preprocess, command: direct_preprocessor_command_for_target_with_minor_and_flags(options.target, &options.input, &compiler_input, gcc_minor, &options.preprocessor_flags)? });
        let mut command = vec![driver.to_string_lossy().into_owned(), compiler_input.clone(), "-dumpbase".to_string(), dumpbase.clone()];
        command.extend(flags.iter().cloned());
        command.push("-o".to_string());
        command.push(options.output.clone());
        steps.push(CompilerCommandStep { kind: StepKind::Compile, command });
    } else {
        let mut arguments = flags.clone();
        arguments.extend(options.preprocessor_flags.iter().cloned());
        arguments.push("-S".to_string());
        arguments.push("-o".to_string());
        arguments.push(options.output.clone());
        arguments.push(options.input.clone());
        steps.push(CompilerCommandStep { kind: StepKind::Compile, command: compiler_command_for_target(options.target, &arguments)? });
    }

    Ok(SourceToAssemblyPlan { target: options.target, requested_family, family, routing_source: options.routing_source.clone(), input: options.input.clone(), output: options.output.clone(), compiler_input, flags, steps })
}

/// Direct hot-search preprocessing, defaulting the reported GCC minor to 96.
pub fn direct_preprocessor_command(input: &str, output: &str) -> Result<Vec<String>> {
    direct_preprocessor_command_with_minor(input, output, 96)
}

/// GCC3's preserved defect reports 2.0 because `__GNUC__=2` is fixed here and
/// the minor is 0; changing it changes prior sweep input. `i64` also keeps JS
/// number interpolation and Rust display equivalent for all supported values.
pub fn direct_preprocessor_command_with_minor(input: &str, output: &str, gcc_minor: i64) -> Result<Vec<String>> {
    direct_preprocessor_command_with_minor_and_flags(input, output, gcc_minor, &[])
}

fn direct_preprocessor_command_with_minor_and_flags(input: &str, output: &str, gcc_minor: i64, flags: &[String]) -> Result<Vec<String>> {
    direct_preprocessor_command_for_target_with_minor_and_flags(CompilerTarget::Gs1, input, output, gcc_minor, flags)
}

fn direct_preprocessor_command_for_target_with_minor_and_flags(target: CompilerTarget, input: &str, output: &str, gcc_minor: i64, flags: &[String]) -> Result<Vec<String>> {
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
        format!("-I{}", root().join("games").join(target.as_str()).join("include").display()),
    ];
    command.extend(flags.iter().cloned());
    command.push(input.to_string());
    command.push(output.to_string());
    Ok(command)
}

/// Omitting `source` routes by basename and silently misses path-keyed overlay
/// flags. Production callers must pass it explicitly; changing the default also
/// changes the assembly-visible dumpbase contract.
pub fn direct_compiler_command(input: &str, output: &str, dumpbase: &str, source: Option<&str>) -> Result<Vec<String>> {
    validate_bundle(CompilerTarget::Gs1)?;
    let source = source.unwrap_or(dumpbase);
    // cc1 receives includes through preprocessing, not driver-only include flags.
    let flags: Vec<String> = cflags_for_source(source).into_iter().filter(|flag| flag != "-nostdinc" && !flag.starts_with("-I")).collect();
    let mut out = vec![bundle().join("cc1").to_string_lossy().into_owned(), input.to_string(), "-quiet".to_string(), "-dumpbase".to_string(), dumpbase.to_string()];
    out.extend(flags);
    out.push("-o".to_string());
    out.push(output.to_string());
    Ok(out)
}

/// GS1-only. Routing GS2 here would silently use GS1 flags. The agbcc driver
/// also intentionally omits cc1's `-quiet`.
pub fn direct_compiler_command_for_source(source: &str, input: &str, output: &str, dumpbase: &str) -> Result<Vec<String>> {
    if !uses_agbcc_compiler(CompilerTarget::Gs1, source) {
        return direct_compiler_command(input, output, dumpbase, Some(source));
    }
    validate_agbcc_bundle()?;
    let mut out = vec![agbcc_driver().to_string_lossy().into_owned(), input.to_string(), "-dumpbase".to_string(), dumpbase.to_string()];
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
        let mut options = SourceToAssemblyPlanOptions::new(CompilerTarget::Gs1, "games/gs1/src/080000c0.c", "candidate.c", "candidate.s");
        options.family = Some(CompilerFamily::OldAgbcc);
        options.preprocessor_flags = vec!["-DGS1_EDITION_JA=1".into()];

        let plan = source_to_assembly_plan(&options).unwrap();
        assert!(plan.steps[0].command.iter().any(|argument| argument == "-DGS1_EDITION_JA=1"));
        assert!(!plan.steps[1].command.iter().any(|argument| argument.starts_with("-DGS1_EDITION_")));
    }

    #[test]
    fn edition_define_reaches_gcc296_driver() {
        let mut options = SourceToAssemblyPlanOptions::new(CompilerTarget::Gs2, "games/gs2/src/08120450.c", "candidate.c", "candidate.s");
        options.family = Some(CompilerFamily::Gcc296);
        options.preprocessor_flags = vec!["-DGS2_EDITION_IT=1".into()];

        let plan = source_to_assembly_plan(&options).unwrap();
        assert_eq!(plan.steps.len(), 1);
        assert!(plan.steps[0].command.iter().any(|argument| argument == "-DGS2_EDITION_IT=1"));
    }

    #[test]
    fn shared_gs2_audio_owner_routes_through_old_agbcc() {
        let mut options = SourceToAssemblyPlanOptions::new(CompilerTarget::Gs2, "games/gs2/src/081c28e0.c", "candidate.c", "candidate.s");
        options.preprocessor_flags = vec!["-DGS2_EDITION_JA=1".into()];

        let plan = source_to_assembly_plan(&options).unwrap();
        assert!(plan.steps[1].command[0].ends_with("/agbcc/old_agbcc"));
        assert!(plan.steps[0].command.iter().any(|argument| argument == "-DGS2_EDITION_JA=1"));
    }
}
