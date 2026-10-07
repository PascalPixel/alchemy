use super::routing::{
    cflags_for_target_source, family_for_source, family_include_flags, uses_agbcc_compiler,
};
use crate::targets::DecompTarget;
use std::path::Path;
use std::process::Command;

pub(crate) fn command(
    target: DecompTarget,
    source: &str,
    comments: bool,
) -> Result<Vec<String>, String> {
    super::bundle::compiler_command_for_target(target.compiler, &flags(target, source, comments))
}

fn flags(target: DecompTarget, source: &str, comments: bool) -> Vec<String> {
    let compiler = target.compiler;
    let mut flags = cflags_for_target_source(compiler, source);
    if uses_agbcc_compiler(compiler, source) {
        flags.extend([
            "-nostdinc".into(),
            "-mthumb".into(),
            super::routing::include_flag(compiler),
            "-D__GNUC_MINOR__=9".into(),
        ]);
        // As the build plans it: game ARM C reads the shared modules too.
        flags.extend(family_include_flags(family_for_source(compiler, source)));
    }
    flags.push(format!("-D{}=1", target.edition_define));
    flags.extend(["-w".into(), "-E".into(), "-x".into(), "c".into()]);
    if comments {
        flags.push("-C".into());
    }
    flags
}

pub(crate) struct Expansion<'a> {
    root: &'a Path,
    target: DecompTarget,
    imports: Option<tempfile::TempDir>,
}

impl<'a> Expansion<'a> {
    pub(crate) fn new(root: &'a Path, target: DecompTarget) -> Result<Self, String> {
        Ok(Self {
            root,
            target,
            imports: crate::build_text::fresh_c_imports(root, target)?,
        })
    }

    pub(crate) fn fresh(&self, source: &str) -> Result<String, String> {
        self.expand(source, &[])
    }

    fn expand(&self, source: &str, extra: &[&str]) -> Result<String, String> {
        let mut command = command(self.target, source, true)?;
        if let Some(directory) = &self.imports {
            command.insert(1, format!("-I{}", directory.path().display()));
        }
        command.extend(extra.iter().map(|flag| flag.to_string()));
        command.push(source.into());
        let output = Command::new(&command[0])
            .args(&command[1..])
            .current_dir(self.root)
            .output()
            .map_err(|error| format!("{source}: preprocessing: {error}"))?;
        if !output.status.success() {
            return Err(format!(
                "{source}: preprocessing: {}",
                String::from_utf8_lossy(&output.stderr).trim()
            ));
        }
        String::from_utf8(output.stdout)
            .map_err(|error| format!("{source}: preprocessing: {error}"))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::targets::{target_for, DecompTargetId};

    #[test]
    fn game_arm_c_preprocesses_with_the_shared_modules_it_builds_with() {
        let target = target_for(DecompTargetId::TbsJa);
        let flags = flags(
            target,
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/BUILD_METATILES.C",
            false,
        );
        assert!(flags
            .iter()
            .any(|flag| flag.starts_with("-I") && flag.ends_with("games/COMMON/INCLUDE/GAME")));
        assert!(flags.contains(&"-DTBS_EDITION_JA=1".to_string()));
    }
}
