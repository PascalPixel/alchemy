use crate::compiler::bundle::{
    compiler_command_for_target, validate_agbcc_bundle, validate_bundle,
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
    bundle_for, cflags_for_family, driver_for_family, family_for_source, family_include_flags,
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
        validate_agbcc_bundle()?;
        let compiler_input = options
            .preprocessed_output
            .clone()
            .unwrap_or_else(|| inferred_preprocessed_output(&options.output));
        let gcc_minor = 9;
        let mut preprocessor_flags = family_include_flags(family);
        preprocessor_flags.extend(options.preprocessor_flags.iter().cloned());
        steps.push(direct_preprocessor_command_for_target_with_minor_and_flags(
            options.target,
            &options.input,
            &compiler_input,
            gcc_minor,
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
        steps.push(compiler_command_for_target(options.target, &arguments)?);
    }
    Ok(steps)
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
        bundle_for(target)
            .join("cpp0")
            .to_string_lossy()
            .into_owned(),
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
