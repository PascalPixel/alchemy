//! Generate canonical GCC allocation dumps for a Golden Sun owner.

use crate::compiler::{
    routing::{cflags_for_target_source, CompilerTarget},
    source_paths::{SourceOwner, SourcePaths},
};
use psynergy::process::run as run_tool;
use std::fs;
use std::path::Path;
use std::process::ExitCode;

#[cfg(test)]
mod tests {
    #[test]
    fn invalid_owners_and_route_overrides_are_rejected_before_compilation() {
        for args in [
            vec!["../../elsewhere"],
            vec!["080bbb0c", "-fno-cse-follow-jumps"],
            vec!["080bbb0c", "source.c", "extra"],
        ] {
            let args = args.into_iter().map(str::to_string).collect::<Vec<_>>();
            assert!(super::run(&args).is_err());
        }
        assert!(super::run(&["--help".into()]).is_ok());
    }
    #[test]
    fn overlay_owners_resolve_a_candidate_rather_than_being_refused() {
        let repo = crate::compiler::routing::root();
        let main = super::SourceOwner::parse_argument("080bbb0c").expect("main owner");
        assert_eq!(
            super::default_source(&repo, main).expect("main default"),
            "games/gs1/recon/en/main/080bbb0c.c"
        );
        let overlay =
            super::SourceOwner::parse_argument("resource_3ba:02002910").expect("overlay owner");
        let path = super::default_source(&repo, overlay).expect("overlay default");
        assert!(path.ends_with(".c"), "{path}");
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
    let owner = parsed.address_stem();
    let repo = crate::compiler::routing::root();
    let source = match args.get(1) {
        Some(path) => path.clone(),
        None => default_source(&repo, parsed)?,
    };
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
    // Bind through the parsed owner rather than the file name: a named overlay
    // source carries no resource stem to infer an overlay from.
    fs::write(
        &bindings,
        crate::compiler::source_bindings::production_bindings(
            &repo,
            &SourcePaths::load_for_game(&repo, CompilerTarget::Gs1.as_str())?
                .symbol_bindings(parsed.overlay_id().as_deref()),
            Some(&src),
        )?,
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

/// Where a candidate lives when the caller names only an owner: the adopted
/// source if one is registered, otherwise the reconstruction draft.
fn default_source(repo: &Path, owner: SourceOwner) -> Result<String, String> {
    let stem = owner.address_stem();
    let Some(overlay) = owner.overlay_id() else {
        return Ok(format!("games/gs1/recon/en/main/{stem}.c"));
    };
    let paths = SourcePaths::load_for_game(repo, CompilerTarget::Gs1.as_str())?;
    Ok(paths.mapped_relative_path(owner).map_or_else(
        || format!("games/gs1/recon/en/overlays/{overlay}_c_{stem}.c"),
        |path| path.to_string_lossy().into_owned(),
    ))
}
/// The compiler runs through the shared tool executor so its dumps come from
/// the same address-stable invocation production compiles use.
fn checked(program: &Path, args: &[String], cwd: &Path) -> Result<(), String> {
    let mut command = vec![program.to_string_lossy().into_owned()];
    command.extend(args.iter().cloned());
    run_tool(&command, cwd).map(|_| ())
}
