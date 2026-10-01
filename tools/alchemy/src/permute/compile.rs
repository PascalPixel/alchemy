//! Compile candidates exactly as the build compiles a source file, and
//! assemble the target listing exactly as the build assembles a listing.

use crate::build_rom::preprocessor_only;
use crate::compiler::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use crate::compiler::routing::{
    assembly_command, compiler_assembly_command, is_arm, prefer_installed_binutils, root,
};
use crate::targets::DecompTarget;
use psynergy::process::run;
use std::fs;
use std::path::{Path, PathBuf};

pub struct Toolchain {
    pub target: DecompTarget,
    /// The source whose whole-file compiler route applies.
    pub route: String,
    /// The candidate's file name, the draft's own.
    pub file_name: String,
    /// The draft's directory, for its local includes.
    pub include: PathBuf,
    /// This research run's current catalog, shared by all candidates.
    pub message_imports: String,
}

impl Toolchain {
    fn plan(&self, input: &Path) -> Result<Vec<Vec<String>>, String> {
        let mut options = SourceToAssemblyPlanOptions::new(
            self.target.compiler,
            self.route.clone(),
            input.to_string_lossy().into_owned(),
            input.with_extension("s").to_string_lossy().into_owned(),
        );
        options.preprocessor_flags = vec![format!("-D{}=1", self.target.edition_define)];
        options.preprocessed_output =
            Some(input.with_extension("i").to_string_lossy().into_owned());
        options.support_flags = vec![
            format!("-I{}", self.include.display()),
            format!(
                "-I{}",
                input.parent().expect("candidate directory").display()
            ),
        ];
        let mut steps = source_to_assembly_plan(&options)?;
        steps.push(compiler_assembly_command(
            &input.with_extension("s").to_string_lossy(),
            &input.with_extension("o").to_string_lossy(),
            is_arm(self.target.compiler, &self.route),
        ));
        Ok(steps)
    }

    fn write(&self, text: &str, directory: &Path) -> Result<PathBuf, String> {
        fs::create_dir_all(directory)
            .map_err(|error| format!("{}: {error}", directory.display()))?;
        crate::build_text::write_c_imports(directory, &self.message_imports)?;
        let input = directory.join(&self.file_name);
        fs::write(&input, text).map_err(|error| format!("{}: {error}", input.display()))?;
        Ok(input)
    }

    /// The object the routed compiler and assembler make from `text`.
    pub fn compile(&self, text: &str, directory: &Path) -> Result<Vec<u8>, String> {
        let input = self.write(text, directory)?;
        let object = input.with_extension("o");
        let _ = fs::remove_file(&object);
        for step in self.plan(&input)? {
            run(&step, root())?;
        }
        fs::read(&object).map_err(|error| format!("{}: {error}", object.display()))
    }

    /// The translation unit the compiler sees, for its declarations.
    pub fn preprocess(&self, text: &str, directory: &Path) -> Result<String, String> {
        let input = self.write(text, directory)?;
        let preprocessed = input.with_extension("i");
        let steps = self.plan(&input)?;
        let command = preprocessor_only(&steps, &input.to_string_lossy(), &preprocessed)?;
        run(&command, root())?;
        fs::read_to_string(&preprocessed)
            .map_err(|error| format!("{}: {error}", preprocessed.display()))
    }
}

/// Assemble `listing` as the build assembles listings, with the build
/// directory on the include path as the build has it.
pub fn assemble(listing: &Path, directory: &Path, build: &Path) -> Result<Vec<u8>, String> {
    prefer_installed_binutils();
    fs::create_dir_all(directory).map_err(|error| format!("{}: {error}", directory.display()))?;
    let object = directory.join("listing.o");
    let _ = fs::remove_file(&object);
    let mut step = assembly_command(&listing.to_string_lossy(), &object.to_string_lossy());
    step.insert(1, format!("-I{}", build.display()));
    run(&step, root())?;
    fs::read(&object).map_err(|error| format!("{}: {error}", object.display()))
}
