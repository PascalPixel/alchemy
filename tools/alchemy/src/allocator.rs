//! Generate canonical GCC allocation dumps for a Golden Sun owner.

use crate::compiler::{
    routing::{cflags_for_target_source, CompilerTarget},
    source_paths::SourceOwner,
};
use std::fs;
use std::path::Path;
use std::process::{Command, ExitCode};

#[cfg(test)]
mod tests {
    #[test]
    fn invalid_owners_and_route_overrides_are_rejected_before_compilation() {
        for args in [
            vec!["../../elsewhere"],
            vec!["resource_3ba:02002910"],
            vec!["080bbb0c", "-fno-cse-follow-jumps"],
            vec!["080bbb0c", "source.c", "extra"],
        ] {
            let args = args.into_iter().map(str::to_string).collect::<Vec<_>>();
            assert!(super::run(&args).is_err());
        }
        assert!(super::run(&["--help".into()]).is_ok());
    }
}

pub fn entry(args: &[String]) -> ExitCode {
    match run(args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

fn run(args: &[String]) -> Result<(), String> {
    const USAGE: &str = "usage: alchemy build allocator MAIN_OWNER [candidate.c]";
    if args == ["--help"] || args == ["-h"] {
        println!("{USAGE}");
        return Ok(());
    }
    if args.is_empty() || args.len() > 2 || args.iter().any(|a| a.starts_with('-')) {
        return Err(USAGE.into());
    }
    let parsed = SourceOwner::parse_argument(&args[0])?;
    if !parsed.is_main() {
        return Err("allocator dump generation currently requires a main-ROM owner".into());
    }
    let owner = format!("{:08x}", parsed.address());
    let source = args
        .get(1)
        .cloned()
        .unwrap_or_else(|| format!("games/gs1/recon/en/main/{owner}.c"));
    let repo = crate::compiler::routing::root();
    let directory = repo.join("out/allocator");
    fs::create_dir_all(&directory).map_err(|e| e.to_string())?;
    let work = tempfile::Builder::new()
        .prefix(&format!("{owner}-"))
        .tempdir_in(&directory)
        .map_err(|e| e.to_string())?
        .keep();
    let src = repo.join(&source);
    let bundle = crate::compiler::routing::bundle();
    let mut cpp = crate::compiler::plan::direct_preprocessor_command(
        &src.to_string_lossy(),
        &work.join("in.i").to_string_lossy(),
    )
    .map_err(|e| e.to_string())?;
    let bindings = work.join("bindings.h");
    fs::write(
        &bindings,
        crate::candidate::source_symbol_bindings(&repo, &source, CompilerTarget::Gs1)?,
    )
    .map_err(|e| e.to_string())?;
    cpp.splice(
        1..1,
        ["-include".into(), bindings.to_string_lossy().into_owned()],
    );
    checked(Path::new(&cpp[0]), &cpp[1..], &repo)?;
    let mut cc1 = cflags_for_target_source(CompilerTarget::Gs1, &source);
    cc1.retain(|flag| flag != "-nostdinc" && !flag.starts_with("-I"));
    cc1.extend([
        "-quiet".into(),
        "-da".into(),
        "-o".into(),
        work.join("out.s").to_string_lossy().into_owned(),
        work.join("in.i").to_string_lossy().into_owned(),
    ]);
    checked(&bundle.join("cc1"), &cc1, &work)?;

    println!("allocator dumps kept in {}", work.display());
    println!("run: psynergy inspect allocator {}", work.display());
    Ok(())
}

fn checked(program: &Path, args: &[String], cwd: &Path) -> Result<(), String> {
    let output = Command::new(program)
        .args(args)
        .current_dir(cwd)
        .output()
        .map_err(|e| format!("{}: {e}", program.display()))?;
    if output.status.success() {
        Ok(())
    } else {
        Err(format!(
            "{} failed:\n{}",
            program.display(),
            String::from_utf8_lossy(&output.stderr)
        ))
    }
}
