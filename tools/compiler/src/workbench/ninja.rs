use shell_escape::escape;
use std::{
    borrow::Cow,
    ffi::OsStr,
    path::{Path, PathBuf},
};

pub struct NinjaPlan {
    pub executable: PathBuf,
    pub source: PathBuf,
    pub reference_asm: PathBuf,
    pub output: PathBuf,
    pub stem: String,
    pub symbol: String,
    pub m2c: PathBuf,
    pub m2c_macros: PathBuf,
    pub headers: Vec<PathBuf>,
}

impl NinjaPlan {
    pub fn candidate_dir(&self) -> PathBuf {
        self.output.join("candidate")
    }
    pub fn candidate_assembly(&self) -> PathBuf {
        self.candidate_dir().join(format!("{}.s", self.stem))
    }
    pub fn candidate_context(&self) -> PathBuf {
        self.candidate_dir().join(format!("{}.i", self.stem))
    }
    pub fn candidate_object(&self) -> PathBuf {
        self.candidate_dir().join(format!("{}.o", self.stem))
    }
    pub fn target_object(&self) -> PathBuf {
        self.output.join("target/target.o")
    }
    pub fn target_listing(&self) -> PathBuf {
        self.output.join("target/target.lst")
    }
    pub fn symbolized_asm(&self) -> PathBuf {
        self.output.join("m2c/symbolized.s")
    }
    pub fn m2c_source(&self) -> PathBuf {
        self.output.join(format!("m2c/{}.c", self.stem))
    }
    pub fn structural_report(&self) -> PathBuf {
        self.output.join("structural.json")
    }
    pub fn m2c_probe_report(&self) -> PathBuf {
        self.output.join("m2c-compile.json")
    }
    pub fn m2c_build_dir(&self) -> PathBuf {
        self.output.join("m2c/build")
    }

    pub fn render(&self) -> Result<String, String> {
        let mut output = String::from("ninja_required_version = 1.10\n\n");
        let candidate_dir = self.candidate_dir();
        let target_object = self.target_object();
        let target_listing = self.target_listing();
        let symbolized_asm = self.symbolized_asm();
        let candidate_context = self.candidate_context();
        let candidate_object = self.candidate_object();
        let structural_report = self.structural_report();
        let m2c_source = self.m2c_source();
        let m2c_build_dir = self.m2c_build_dir();
        let m2c_probe_report = self.m2c_probe_report();
        let image = self
            .reference_asm
            .ancestors()
            .nth(4)
            .ok_or("reference assembly is not under games/gs1/asm")?
            .join("roms/gs1-en.gba");
        let steps = [
            (
                "compile_candidate",
                command(&[
                    self.executable.as_os_str(),
                    OsStr::new("__workbench-step"),
                    OsStr::new("compile"),
                    self.source.as_os_str(),
                    self.source.as_os_str(),
                    candidate_dir.as_os_str(),
                ])?,
                format!("GS1CC {}", self.stem),
            ),
            (
                "assemble_target",
                command(&[
                    self.executable.as_os_str(),
                    OsStr::new("__workbench-step"),
                    OsStr::new("target"),
                    self.reference_asm.as_os_str(),
                    OsStr::new(&self.symbol),
                    target_object.as_os_str(),
                    target_listing.as_os_str(),
                ])?,
                format!("ASSEMBLE {}", self.stem),
            ),
            (
                "symbolize_target",
                command(&[
                    self.executable.as_os_str(),
                    OsStr::new("__workbench-step"),
                    OsStr::new("symbolize"),
                    self.reference_asm.as_os_str(),
                    target_listing.as_os_str(),
                    OsStr::new(&self.symbol),
                    image.as_os_str(),
                    symbolized_asm.as_os_str(),
                ])?,
                format!("SYMBOLIZE {}", self.stem),
            ),
            (
                "run_m2c",
                command(&[
                    self.executable.as_os_str(),
                    OsStr::new("__workbench-step"),
                    OsStr::new("m2c"),
                    self.m2c.as_os_str(),
                    symbolized_asm.as_os_str(),
                    candidate_context.as_os_str(),
                    OsStr::new(&self.symbol),
                    m2c_source.as_os_str(),
                ])?,
                format!("M2C {}", self.stem),
            ),
            (
                "run_objdiff",
                command(&[
                    self.executable.as_os_str(),
                    OsStr::new("__workbench-step"),
                    OsStr::new("structural"),
                    target_object.as_os_str(),
                    candidate_object.as_os_str(),
                    OsStr::new(&self.symbol),
                    structural_report.as_os_str(),
                ])?,
                format!("OBJDIFF {}", self.stem),
            ),
            (
                "probe_m2c",
                command(&[
                    self.executable.as_os_str(),
                    OsStr::new("__workbench-step"),
                    OsStr::new("probe"),
                    m2c_source.as_os_str(),
                    self.source.as_os_str(),
                    m2c_build_dir.as_os_str(),
                    target_object.as_os_str(),
                    OsStr::new(&self.symbol),
                    m2c_probe_report.as_os_str(),
                ])?,
                format!("GS1CC M2C {}", self.stem),
            ),
        ];
        for (name, command, description) in steps {
            output.push_str(&format!(
                "rule {name}\n  command = {command}\n  description = {description}\n\n"
            ));
        }

        let headers = self
            .headers
            .iter()
            .map(PathBuf::as_path)
            .map(ninja_path)
            .collect::<Vec<_>>()
            .join(" ");
        output.push_str(&format!(
            "build {} {} {}: compile_candidate {} | {} {}\n",
            ninja_path(&self.candidate_object()),
            ninja_path(&self.candidate_assembly()),
            ninja_path(&self.candidate_context()),
            ninja_path(&self.source),
            ninja_path(&self.executable),
            headers,
        ));
        output.push_str(&format!(
            "build {} {}: assemble_target {} | {}\n",
            ninja_path(&self.target_object()),
            ninja_path(&self.target_listing()),
            ninja_path(&self.reference_asm),
            ninja_path(&self.executable),
        ));
        output.push_str(&format!(
            "build {}: symbolize_target {} {} | {}\n",
            ninja_path(&self.symbolized_asm()),
            ninja_path(&self.reference_asm),
            ninja_path(&self.target_listing()),
            ninja_path(&self.executable),
        ));
        output.push_str(&format!(
            "build {}: run_m2c {} {} | {} {} {}\n",
            ninja_path(&self.m2c_source()),
            ninja_path(&self.symbolized_asm()),
            ninja_path(&self.candidate_context()),
            ninja_path(&self.executable),
            ninja_path(&self.m2c),
            ninja_path(&self.m2c_macros),
        ));
        output.push_str(&format!(
            "build {}: run_objdiff {} {} | {}\n",
            ninja_path(&self.structural_report()),
            ninja_path(&self.target_object()),
            ninja_path(&self.candidate_object()),
            ninja_path(&self.executable),
        ));
        output.push_str(&format!(
            "build {}: probe_m2c {} {} | {}\n",
            ninja_path(&self.m2c_probe_report()),
            ninja_path(&self.m2c_source()),
            ninja_path(&self.target_object()),
            ninja_path(&self.executable),
        ));
        output.push_str(&format!("build score: phony {}\nbuild m2c: phony {} {}\nbuild all: phony score m2c\ndefault all\n", ninja_path(&self.structural_report()), ninja_path(&self.m2c_source()), ninja_path(&self.m2c_probe_report()),));
        Ok(output)
    }
}

fn command(parts: &[&OsStr]) -> Result<String, String> {
    parts
        .iter()
        .map(|part| {
            part.to_str()
                .ok_or("non-UTF-8 command path".to_string())
                .map(|value| escape(Cow::Borrowed(value)).into_owned().replace('$', "$$"))
        })
        .collect::<Result<Vec<_>, _>>()
        .map(|parts| parts.join(" "))
}

fn ninja_path(path: &Path) -> String {
    path.to_string_lossy()
        .replace('$', "$$")
        .replace(' ', "$ ")
        .replace(':', "$:")
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ninja_paths_escape_spaces_colons_and_dollars() {
        assert_eq!(ninja_path(&PathBuf::from("a b:c$d")), "a$ b$:c$$d");
    }

    #[test]
    fn ninja_commands_preserve_shell_quoted_dollars() {
        assert_eq!(command(&[OsStr::new("a b$c")]).unwrap(), "'a b$$c'");
    }
}
