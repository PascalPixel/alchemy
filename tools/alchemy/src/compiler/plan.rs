use crate::compiler::bundle::{
    compiler_command, validate_agbcc_bundle, validate_bundle, validate_game_arm_bundle,
};

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

use crate::compiler::routing::{
    bundle, cflags_for_family, driver_for_family, family_for_source, family_include_flags,
    include_flag,
    CompilerFamily, CompilerTarget,
};
pub type Result<T> = std::result::Result<T, String>;
#[derive(Debug, Clone)]
pub struct SourceToAssemblyPlanOptions {
    pub target: CompilerTarget,
    pub routing_source: String,
    pub input: String,
    pub output: String,
    pub support_flags: Vec<String>,
    pub preprocessor_flags: Vec<String>,
    pub preprocessed_output: Option<String>,
    pub dumpbase: Option<String>,
    /// An analysis-only route that replaces the one routing_source would take.
    pub family: Option<CompilerFamily>,
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
            family: None,
        }
    }
}
pub fn inferred_preprocessed_output(output: &str) -> String {
    let extension = extname(output);
    if extension.is_empty() {
        format!("{output}.i")
    } else {
        format!("{}.i", &output[..output.len() - extension.len()])
    }
}
fn diagnostic_flag(flag: &str) -> bool {
    flag == "-g"
        || flag.strip_prefix("-I").is_some_and(|path| !path.is_empty())
        || flag.strip_prefix("-d").is_some_and(|letters| {
            !letters.is_empty()
                && letters.chars().all(|c| c.is_ascii_alphabetic())
                && letters != "umpbase"
        })
        || flag
            .strip_prefix("-fsched-verbose=")
            .is_some_and(|level| !level.is_empty() && level.chars().all(|c| c.is_ascii_digit()))
}
pub fn source_to_assembly_plan(options: &SourceToAssemblyPlanOptions) -> Result<Vec<Vec<String>>> {
    if let Some(flag) = options
        .support_flags
        .iter()
        .find(|flag| !diagnostic_flag(flag))
    {
        return Err(format!(
            "compiler flag is not an include path or diagnostic: {flag}"
        ));
    }
    let family = options
        .family
        .unwrap_or_else(|| family_for_source(options.target, &options.routing_source));
    let mut flags = cflags_for_family(options.target, family);
    flags.extend(options.support_flags.iter().cloned());
    let dumpbase = options
        .dumpbase
        .clone()
        .unwrap_or_else(|| basename(&options.routing_source).to_string());
    let mut steps = Vec::new();
    if let Some(driver) = driver_for_family(family) {
        if family == CompilerFamily::GameArm {
            validate_game_arm_bundle()?;
        } else {
            validate_agbcc_bundle()?;
        }
        let compiler_input = options
            .preprocessed_output
            .clone()
            .unwrap_or_else(|| inferred_preprocessed_output(&options.output));
        let mut preprocessor_flags = family_include_flags(family);
        preprocessor_flags.extend(options.preprocessor_flags.iter().cloned());
        steps.push(direct_preprocessor_command_for_target_with_flags(
            options.target,
            &options.input,
            &compiler_input,
            family,
            &preprocessor_flags,
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
        steps.push(compiler_command(&arguments)?);
    }
    Ok(steps)
}
fn direct_preprocessor_command_for_target_with_flags(
    target: CompilerTarget,
    input: &str,
    output: &str,
    family: CompilerFamily,
    flags: &[String],
) -> Result<Vec<String>> {
    validate_bundle()?;
    let mut command = vec![
        bundle()
            .join("cpp0")
            .to_string_lossy()
            .into_owned(),
        "-lang-c".into(),
        "-nostdinc".into(),
        "-Acpu(arm)".into(),
        "-Amachine(arm)".into(),
        "-D__CHAR_UNSIGNED__".into(),
        "-D__OPTIMIZE__".into(),
        "-D__ARM_ARCH_4T__".into(),
        "-D__APCS_32__".into(),
        "-D__ARMEL__".into(),
        "-Darm_elf".into(),
        "-D__ELF__".into(),
        include_flag(target),
    ];
    command.extend(direct_preprocessor_defines(family).iter().map(|flag| flag.to_string()));
    command.extend(flags.iter().cloned());
    command.push(input.to_string());
    command.push(output.to_string());
    Ok(command)
}

pub(crate) fn direct_preprocessor_defines(family: CompilerFamily) -> Vec<&'static str> {
    let mut defines = vec!["-D__GNUC__=2"];
    if family == CompilerFamily::GameArm {
        defines.extend(["-D__GNUC_MINOR__=95", "-D__GNUC_PATCHLEVEL__=2"]);
    } else {
        defines.extend(["-D__GNUC_MINOR__=9", "-D__GNUC_PATCHLEVEL__=0"]);
    }
    if family.is_arm() {
        defines.extend(["-Darm", "-D__arm__"]);
    } else {
        defines.extend(["-D__THUMBEL__", "-Dthumb", "-D__thumb__"]);
    }
    defines
}

#[cfg(test)]
mod direct_preprocessor_tests {
    use super::*;

    #[test]
    fn direct_compilers_receive_their_version_and_instruction_set() {
        let arm = direct_preprocessor_defines(CompilerFamily::GameArm);
        assert!(arm.contains(&"-D__GNUC_MINOR__=95"));
        assert!(arm.contains(&"-D__GNUC_PATCHLEVEL__=2"));
        assert!(arm.contains(&"-D__arm__"));
        assert!(!arm.iter().any(|flag| flag.to_ascii_lowercase().contains("thumb")));
        for family in [CompilerFamily::Agbcc, CompilerFamily::AgbccFlash] {
            let thumb = direct_preprocessor_defines(family);
            assert!(thumb.contains(&"-D__GNUC_MINOR__=9"));
            assert!(thumb.contains(&"-D__GNUC_PATCHLEVEL__=0"));
            assert!(thumb.contains(&"-D__thumb__"));
            assert!(!thumb.contains(&"-D__arm__"));
        }
    }
}
