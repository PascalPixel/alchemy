//! Fresh source expansion with the exact production compiler and edition.
//! Expansion is returned in memory; saved intermediates and reports are never inputs.
use super::routing::{cflags_for_target_source, uses_agbcc_compiler};
use crate::targets::DecompTarget;
use std::path::Path;
use std::process::Command;

pub(crate) fn command(
    target: DecompTarget,
    source: &str,
    comments: bool,
) -> Result<Vec<String>, String> {
    let compiler = target.compiler;
    let mut flags = cflags_for_target_source(compiler, source);
    if uses_agbcc_compiler(compiler, source) {
        flags.extend([
            "-nostdinc".into(),
            "-mthumb".into(),
            super::routing::include_flag(compiler),
            "-D__GNUC_MINOR__=9".into(),
        ]);
    }
    flags.push(format!("-D{}=1", target.edition_define));
    flags.extend(["-w".into(), "-E".into(), "-x".into(), "c".into()]);
    if comments {
        flags.push("-C".into());
    }
    super::bundle::compiler_command_for_target(compiler, &flags)
}

pub(crate) fn fresh(root: &Path, target: DecompTarget, source: &str) -> Result<String, String> {
    expansion(root, target, source, &[])
}

/// Preserve active macro definitions for source-name validation, in memory.
#[cfg(test)]
pub(crate) fn fresh_definitions(
    root: &Path,
    target: DecompTarget,
    source: &str,
) -> Result<String, String> {
    expansion(root, target, source, &["-dD"])
}

fn expansion(
    root: &Path,
    target: DecompTarget,
    source: &str,
    extra: &[&str],
) -> Result<String, String> {
    let mut command = command(target, source, true)?;
    let imports = crate::build_text::fresh_c_imports(root, target)?;
    if let Some(directory) = &imports {
        command.insert(1, format!("-I{}", directory.path().display()));
    }
    command.extend(extra.iter().map(|flag| flag.to_string()));
    command.push(source.into());
    let output = Command::new(&command[0])
        .args(&command[1..])
        .current_dir(root)
        .output()
        .map_err(|error| format!("{source}: preprocessing: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "{source}: preprocessing: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    String::from_utf8(output.stdout).map_err(|error| format!("{source}: preprocessing: {error}"))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::targets::{target_for, DecompTargetId};
    #[test]
    fn approved_preprocessor_preserves_macro_tags_and_ownership() {
        let work = tempfile::tempdir().unwrap();
        let input = work.path().join("fixture.c");
        std::fs::write(&input, "#define MAKE(name) void name(void) { /* FAKEMATCH: test expansion. */ }\nMAKE(First)\nvoid Plain(void) {}\n").unwrap();
        let expanded = fresh(
            work.path(),
            target_for(DecompTargetId::TbsEn),
            input.to_str().unwrap(),
        )
        .unwrap();
        assert!(
            expanded.contains("First(void) { /* FAKEMATCH:"),
            "{expanded}"
        );
        assert!(expanded.contains(input.to_str().unwrap()));
        assert!(!input.with_extension("i").exists());
        assert!(!input.with_extension("s").exists());
    }
    #[test]
    fn fresh_expansion_observes_header_edits_and_edition_selection() {
        let work = tempfile::tempdir().unwrap();
        let input = work.path().join("fixture.c");
        let header = work.path().join("helper.h");
        std::fs::write(&input, "#include \"helper.h\"\n#if defined(TBS_EDITION_EN)\nvoid English(void) { Help(1); }\n#else\nvoid Japanese(void) { Help(2); }\n#endif\n").unwrap();
        std::fs::write(&header, "static inline int Help(int x) { return x; }\n").unwrap();
        std::fs::write(
            input.with_extension("i"),
            "/* FAKEMATCH: stale output must not be read. */",
        )
        .unwrap();
        let target = target_for(DecompTargetId::TbsEn);
        let expand = |target| fresh(work.path(), target, input.to_str().unwrap()).unwrap();
        assert!(
            super::super::steering::analyze(&expand(target), input.to_str().unwrap())
                .unwrap()
                .steered
                .is_empty()
        );
        std::fs::write(
            &header,
            "static inline int Help(int x) { /* FAKEMATCH: current source. */ return x; }\n",
        )
        .unwrap();
        let changed =
            super::super::steering::analyze(&expand(target), input.to_str().unwrap()).unwrap();
        assert!(changed.steered.contains("English"));
        assert!(!changed.steered.contains("Japanese"));
        let japanese = super::super::steering::analyze(
            &expand(target_for(DecompTargetId::TbsJa)),
            input.to_str().unwrap(),
        )
        .unwrap();
        assert!(japanese.steered.contains("Japanese"));
        assert!(!japanese.steered.contains("English"));
    }
}
