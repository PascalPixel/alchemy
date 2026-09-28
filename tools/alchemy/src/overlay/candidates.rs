//! `alchemy overlay try OWNER [FILE...]`: score candidate sources for an
//! open trial.
//!
//! The trial's unit first binds every name the trial source and the
//! candidates use, so a candidate may call a service or local function the
//! first draft did not; then each file is an ordinary `alchemy score`
//! against the owner, run in parallel and ranked as `score --variants`
//! ranks a directory. `--install` copies the best candidate over the trial
//! source.

use crate::compiler::source_paths::SourceOwner;
use crate::overlay::trial::{overlay_owner, Trial};
use crate::score::variants::{rank, table, Scored};
use std::fs;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy overlay try OWNER [FILE...] [--install]\n\
Scores FILEs (default: the trial source) against OWNER's open trial, in parallel, ranked by\n\
differing halfwords, size delta and halfword edits. The trial unit first binds every name the\n\
trial source and the FILEs use. --install copies the best FILE over the trial source.\n\
For one file's full diff afterwards: alchemy score FILE --owner OWNER --diff.";

#[derive(Debug, Default, PartialEq)]
struct Options {
    owner: String,
    files: Vec<String>,
    install: bool,
}

fn parse(arguments: &[String]) -> Result<Option<Options>, String> {
    let mut options = Options::default();
    for argument in arguments {
        match argument.as_str() {
            "--install" => options.install = true,
            "-h" | "--help" => return Ok(None),
            flag if flag.starts_with('-') => return Err(format!("unknown option {flag}\n{USAGE}")),
            _ if options.owner.is_empty() => options.owner = argument.clone(),
            file => options.files.push(file.to_string()),
        }
    }
    if options.owner.is_empty() {
        return Err(USAGE.into());
    }
    Ok(Some(options))
}

/// Whether a score is an exact match of the complete owner.
fn exact(scored: &Scored) -> bool {
    scored.error.is_none()
        && scored.differing == Some(0)
        && scored.candidate.is_some()
        && scored.candidate == scored.reference
}

/// Candidate files, absolute: relative ones are read from the working
/// directory the command ran in.
fn resolve(files: &[String]) -> Result<Vec<PathBuf>, String> {
    let here = std::env::current_dir().map_err(|error| error.to_string())?;
    files
        .iter()
        .map(|file| {
            let path = here.join(file);
            path.canonicalize()
                .map_err(|error| format!("{file}: {error}"))
        })
        .collect()
}

pub fn run(root: &Path, arguments: &[String]) -> Result<i32, String> {
    let Some(options) = parse(arguments)? else {
        println!("{USAGE}");
        return Ok(0);
    };
    let owner: SourceOwner = overlay_owner(&options.owner)?;
    let trial = Trial::require(root, owner)?;
    let own = root.join(&trial.source);
    let files = if options.files.is_empty() {
        vec![own.clone()]
    } else {
        resolve(&options.files)?
    };
    let texts = files
        .iter()
        .map(|file| {
            fs::read_to_string(file).map_err(|error| format!("{}: {error}", file.display()))
        })
        .collect::<Result<Vec<_>, _>>()?;
    let symbols = trial.rebind(root, &texts)?;
    let work = root.join("out/trial").join(owner.legacy_stem()).join("try");
    let results = rank(root, &files, &["--owner".into(), owner.id()], &work)?;
    println!("trial={} symbols={symbols}", trial.owner);
    print!("{}", table(root, &results));
    let Some((best, scored)) = results.first() else {
        return Ok(1);
    };
    if options.install && scored.error.is_none() {
        let own = own.canonicalize().map_err(|error| error.to_string())?;
        if *best != own {
            fs::copy(best, &own).map_err(|error| format!("{}: {error}", own.display()))?;
            trial.rebind(root, &[])?;
            println!("installed {} as {}", best.display(), trial.source);
        }
    }
    if exact(scored) {
        println!(
            "exact: {}{}",
            best.display(),
            if options.install || files == [own] {
                format!("; alchemy overlay adopt {} --apply", trial.owner)
            } else {
                "; pass --install, then alchemy overlay adopt --apply".into()
            }
        );
        return Ok(0);
    }
    Ok(1)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn args(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| item.to_string()).collect()
    }

    #[test]
    fn parses_owner_files_and_install() {
        let options = parse(&args(&["resource_3ca:02000194", "a.c", "b.c", "--install"]))
            .unwrap()
            .unwrap();
        assert_eq!(options.owner, "resource_3ca:02000194");
        assert_eq!(options.files, ["a.c", "b.c"]);
        assert!(options.install);
        assert!(parse(&args(&["resource_3ca:02000194"]))
            .unwrap()
            .unwrap()
            .files
            .is_empty());
        assert!(parse(&args(&[])).is_err());
        assert!(parse(&args(&["x", "--keep"])).is_err());
        assert!(parse(&args(&["-h"])).unwrap().is_none());
    }

    #[test]
    fn only_a_complete_zero_difference_is_exact() {
        let scored = |candidate, reference, differing| Scored {
            candidate: Some(candidate),
            reference: Some(reference),
            differing: Some(differing),
            edits: Some(0),
            error: None,
        };
        assert!(exact(&scored(554, 554, 0)));
        assert!(!exact(&scored(552, 554, 0)));
        assert!(!exact(&scored(554, 554, 1)));
        assert!(!exact(&Scored {
            error: Some("failed".into()),
            ..scored(554, 554, 0)
        }));
    }

    #[test]
    fn relative_candidates_resolve_from_the_working_directory() {
        let directory = tempfile::tempdir().unwrap();
        let file = directory.path().join("a.c");
        fs::write(&file, "\n").unwrap();
        let absolute = file.to_string_lossy().into_owned();
        assert_eq!(
            resolve(&[absolute]).unwrap(),
            [file.canonicalize().unwrap()]
        );
        assert!(resolve(&["/nonexistent/candidate.c".into()]).is_err());
    }
}
