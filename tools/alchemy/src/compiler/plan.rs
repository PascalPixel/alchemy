//! Compilation policy for source-to-assembly plans.
//!
//! `routing_source` selects evidenced compiler policy; `input` is the file being
//! compiled. Candidate sources need both names. Compiler family and ordered
//! flags come only from routing; callers may supply local include paths or dumps.
use crate::compiler::bundle::{
    compiler_command_for_target, validate_agbcc_bundle, validate_bundle,
};
// POSIX filename semantics retained by the compiler argv contract.

pub fn basename(path: &str) -> &str {
    path.trim_end_matches('/').rsplit('/').next().unwrap_or("")
}

pub fn extname(path: &str) -> &str {
    let name = basename(path);
    match name.rfind('.') {
        Some(index) if index > 0 && name != ".." => &name[index..],
        _ => "",
    }
}

#[test]
fn posix_names_preserve_dotfiles_unicode_and_trailing_slashes() {
    for (path, base, ext) in [
        ("", "", ""),
        ("///", "", ""),
        ("a/.", ".", ""),
        ("a/..//", "..", ""),
        ("a/...", "...", "."),
        ("a/.c", ".c", ""),
        ("a/..c", "..c", ".c"),
        ("a/.c.s/", ".c.s", ".s"),
        ("a/b..", "b..", "."),
        ("日本/場面.c//", "場面.c", ".c"),
    ] {
        assert_eq!((basename(path), extname(path)), (base, ext));
    }
    assert_eq!(
        crate::compiler::plan::inferred_preprocessed_output("a.c/"),
        "a..i"
    );
}
use crate::compiler::routing::{
    agbcc_driver, bundle, cflags_for_target_source, include_flag, uses_agbcc_compiler,
    CompilerTarget,
};
pub type Result<T> = std::result::Result<T, String>;
#[derive(Debug, Clone)]
pub struct SourceToAssemblyPlanOptions {
    pub target: CompilerTarget,
    pub routing_source: String,
    pub input: String,
    pub output: String,
    pub support_flags: Vec<String>,
    /// Preprocessor-only flags; old-agbcc rejects them on its `.i` compile step.
    pub preprocessor_flags: Vec<String>,
    /// Explicit old-agbcc intermediate; otherwise inferred beside the output.
    pub preprocessed_output: Option<String>,
    pub dumpbase: Option<String>,
}
impl SourceToAssemblyPlanOptions {
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
            support_flags: Vec::new(),
            preprocessor_flags: Vec::new(),
            preprocessed_output: None,
            dumpbase: None,
        }
    }
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
pub fn source_to_assembly_plan(options: &SourceToAssemblyPlanOptions) -> Result<Vec<Vec<String>>> {
    if let Some(flag) = options.support_flags.iter().find(|flag| {
        !matches!(flag.as_str(), "-g" | "-dp" | "-dr" | "-dl" | "-dg" | "-da")
            && !flag.strip_prefix("-I").is_some_and(|path| !path.is_empty())
    }) {
        return Err(format!(
            "compiler flag is not an include path or diagnostic: {flag}"
        ));
    }
    let old_agbcc = uses_agbcc_compiler(options.target, &options.routing_source);
    let mut flags = cflags_for_target_source(options.target, &options.routing_source);
    flags.extend(options.support_flags.iter().cloned());
    let dumpbase = options
        .dumpbase
        .clone()
        .unwrap_or_else(|| basename(&options.routing_source).to_string());
    let mut steps = Vec::new();
    if old_agbcc {
        let driver = agbcc_driver();
        validate_agbcc_bundle()?;
        let compiler_input = options
            .preprocessed_output
            .clone()
            .unwrap_or_else(|| inferred_preprocessed_output(&options.output));
        // old-agbcc identifies as 2.9.
        let gcc_minor = 9;
        steps.push(direct_preprocessor_command_for_target_with_minor_and_flags(
            options.target,
            &options.input,
            &compiler_input,
            gcc_minor,
            &options.preprocessor_flags,
        )?);
        let mut command = vec![
            driver.to_string_lossy().into_owned(),
            compiler_input.clone(),
            "-dumpbase".to_string(),
            dumpbase.clone(),
        ];
        command.extend(flags.iter().cloned());
        command.push("-o".to_string());
        command.push(options.output.clone());
        steps.push(command);
    } else {
        let mut arguments = flags.clone();
        arguments.extend(options.preprocessor_flags.iter().cloned());
        arguments.push("-S".to_string());
        arguments.push("-o".to_string());
        arguments.push(options.output.clone());
        arguments.push(options.input.clone());
        steps.push(compiler_command_for_target(options.target, &arguments)?);
    }
    Ok(steps)
}
/// Direct hot-search preprocessing, defaulting the reported GCC minor to 96.
pub fn direct_preprocessor_command(input: &str, output: &str) -> Result<Vec<String>> {
    direct_preprocessor_command_for_target_with_minor_and_flags(
        CompilerTarget::Gs1,
        input,
        output,
        96,
        &[],
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
        bundle().join("cpp0").to_string_lossy().into_owned(),
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
        include_flag(target),
    ];
    command.extend(flags.iter().cloned());
    command.push(input.to_string());
    command.push(output.to_string());
    Ok(command)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn diagnostics_preserve_canonical_flags_and_reject_codegen_overrides() {
        for source in ["games/gs1/src/080bbb0c.c", "games/gs1/src/08006878.c"] {
            let mut options = SourceToAssemblyPlanOptions::new(
                CompilerTarget::Gs1,
                source,
                "source.c",
                "source.s",
            );
            let canonical = cflags_for_target_source(options.target, source);
            options.support_flags = vec!["-da".into(), "-Ilocal-headers".into()];
            let plan = source_to_assembly_plan(&options).unwrap();
            let command = plan.last().unwrap();
            assert!(command
                .windows(canonical.len())
                .any(|flags| flags == canonical));
            assert!(command.iter().any(|flag| flag == "-da"));
            assert!(command.iter().any(|flag| flag == "-Ilocal-headers"));
            for flag in ["-O0", "-fno-regmove", "-ffixed-r5", "-marm"] {
                options.support_flags = vec![flag.into()];
                assert!(source_to_assembly_plan(&options)
                    .unwrap_err()
                    .contains("not an include path or diagnostic"));
            }
        }
    }
    #[test]
    fn edition_define_stays_in_old_agbcc_preprocessor_step() {
        let mut options = SourceToAssemblyPlanOptions::new(
            CompilerTarget::Gs1,
            "games/gs1/src/08006878.c",
            "candidate.c",
            "candidate.s",
        );
        options.preprocessor_flags = vec!["-DGS1_EDITION_JA=1".into()];
        let plan = source_to_assembly_plan(&options).unwrap();
        assert!(plan[0]
            .iter()
            .any(|argument| argument == "-DGS1_EDITION_JA=1"));
        assert!(!plan[1]
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
        options.preprocessor_flags = vec!["-DGS2_EDITION_IT=1".into()];
        let plan = source_to_assembly_plan(&options).unwrap();
        assert_eq!(plan.len(), 1);
        assert!(plan[0]
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
        assert!(plan[1][0].ends_with("/agbcc/old_agbcc"));
        assert!(plan[0]
            .iter()
            .any(|argument| argument == "-DGS2_EDITION_JA=1"));
    }
}
