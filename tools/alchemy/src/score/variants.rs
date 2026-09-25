//! `alchemy score --variants DIR`: every `.c` in a directory scored against
//! one owner in parallel, each in a private work directory, then ranked.
//!
//! Each variant is an ordinary `alchemy score` of that file with the other
//! arguments passed through, run as a child of this executable so its route,
//! reference and caches are exactly the single-file score's.

use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;

pub const USAGE: &str = "usage: alchemy score --variants DIR [--owner OWNER] [score options]\n\
Scores every DIR/*.c in parallel, each with its own --work under out/score/variants/, and prints them\n\
ranked by differing halfwords, then size delta, then halfword edits. Other options pass through to\n\
each score (--owner, --target, --size, --unit ...).";

#[derive(Debug, Default, PartialEq)]
pub struct Scored {
    pub candidate: Option<usize>,
    pub reference: Option<usize>,
    pub differing: Option<usize>,
    pub edits: Option<usize>,
    pub error: Option<String>,
}

impl Scored {
    /// The first score block of a child's output: sizes, differing
    /// halfwords and the halfword edit count.
    pub fn parse(stdout: &str, stderr: &str, success: bool) -> Self {
        let mut scored = Self::default();
        for line in stdout.lines() {
            for field in line.split_whitespace() {
                let Some((key, value)) = field.split_once('=') else {
                    continue;
                };
                let slot = match key {
                    "candidate" => &mut scored.candidate,
                    "reference" => &mut scored.reference,
                    "differing_halfwords" => &mut scored.differing,
                    "halfword_edits" => &mut scored.edits,
                    _ => continue,
                };
                if slot.is_none() {
                    *slot = value.parse().ok();
                }
            }
        }
        if scored.differing.is_none() {
            let detail = if stderr.trim().is_empty() {
                stdout
            } else {
                stderr
            };
            let mut lines = detail
                .lines()
                .map(str::trim)
                .filter(|line| !line.is_empty());
            let first = lines.clone().next();
            scored.error = Some(
                lines
                    .find(|line| line.to_ascii_lowercase().contains("error"))
                    .or(first)
                    .unwrap_or(if success { "no score" } else { "failed" })
                    .to_string(),
            );
        }
        scored
    }

    fn delta(&self) -> Option<i64> {
        Some(self.candidate? as i64 - self.reference? as i64)
    }

    fn rank(&self) -> (bool, usize, u64, usize) {
        (
            self.error.is_some(),
            self.differing.unwrap_or(usize::MAX),
            self.delta().map_or(u64::MAX, i64::unsigned_abs),
            self.edits.unwrap_or(usize::MAX),
        )
    }
}

pub fn run(root: &Path, arguments: &[String]) -> Result<(), String> {
    if arguments.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    let (directory, passthrough) = split_arguments(arguments)?;
    let directory = Path::new(&directory)
        .canonicalize()
        .map_err(|error| format!("{directory}: {error}"))?;
    let mut sources = std::fs::read_dir(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?
        .filter_map(|entry| entry.ok().map(|entry| entry.path()))
        .filter(|path| path.extension().is_some_and(|extension| extension == "c"))
        .collect::<Vec<_>>();
    sources.sort();
    if sources.is_empty() {
        return Err(format!("{}: no .c variants", directory.display()));
    }
    let results = rank(root, &sources, &passthrough, &work_root(root, &directory))?;
    print!("{}", table(&directory, &results));
    Ok(())
}

/// Scores each source in parallel, each in `work/<stem>` with `passthrough`
/// options, best first: fewest differing halfwords, then size delta, then
/// halfword edits, failures last.
pub fn rank(
    root: &Path,
    sources: &[PathBuf],
    passthrough: &[String],
    work: &Path,
) -> Result<Vec<(PathBuf, Scored)>, String> {
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let results = Mutex::new(Vec::new());
    let next = AtomicUsize::new(0);
    let workers = crate::parallel::workers(sources.len());
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let index = next.fetch_add(1, Ordering::Relaxed);
                let Some(source) = sources.get(index) else {
                    break;
                };
                let stem = source
                    .file_stem()
                    .map(|stem| stem.to_string_lossy().into_owned())
                    .unwrap_or_default();
                let scored = score_one(&executable, root, source, &work.join(&stem), passthrough);
                results.lock().unwrap().push((source.clone(), scored));
            });
        }
    });
    let mut results = results.into_inner().unwrap();
    results.sort_by(|(a, left), (b, right)| left.rank().cmp(&right.rank()).then(a.cmp(b)));
    Ok(results)
}

/// DIR and everything else, which each score receives unchanged.
fn split_arguments(arguments: &[String]) -> Result<(String, Vec<String>), String> {
    let position = arguments
        .iter()
        .position(|arg| arg == "--variants")
        .ok_or(USAGE)?;
    let directory = arguments
        .get(position + 1)
        .ok_or("--variants requires a directory")?
        .clone();
    let mut rest = arguments[..position].to_vec();
    rest.extend_from_slice(&arguments[position + 2..]);
    if let Some(flag) = rest.iter().find(|arg| {
        matches!(
            arg.as_str(),
            "--work" | "--variants" | "--asm" | "--diff" | "--align" | "--first" | "--dump"
        )
    }) {
        return Err(format!(
            "{flag} does not apply to --variants: each variant gets a private work directory and one table row"
        ));
    }
    if let Some(positional) = rest.iter().enumerate().find_map(|(index, arg)| {
        let takes_value = index > 0
            && matches!(
                rest[index - 1].as_str(),
                "--owner"
                    | "--target"
                    | "--size"
                    | "--span"
                    | "--symbol"
                    | "--unit"
                    | "--instance"
                    | "--rom"
                    | "--patch"
            );
        (!arg.starts_with('-') && !takes_value).then_some(arg)
    }) {
        return Err(format!(
            "{positional}: --variants scores the files in its directory; pass no candidate"
        ));
    }
    Ok((directory, rest))
}

/// A private work root per variant directory, so parallel lanes and
/// repeated sweeps never share a work directory or its cache.
fn work_root(root: &Path, directory: &Path) -> PathBuf {
    let name = directory
        .file_name()
        .map(|name| name.to_string_lossy().into_owned())
        .unwrap_or_else(|| "variants".into());
    let digest = crate::compiler::sha256::hex(directory.to_string_lossy().as_bytes());
    root.join("out/score/variants")
        .join(format!("{name}-{}", &digest[..8]))
}

fn score_one(
    executable: &Path,
    root: &Path,
    source: &Path,
    work: &Path,
    passthrough: &[String],
) -> Scored {
    let output = Command::new(executable)
        .current_dir(root)
        .arg("score")
        .arg(source)
        .args(passthrough)
        .arg("--work")
        .arg(work)
        .output();
    match output {
        Ok(output) => Scored::parse(
            &String::from_utf8_lossy(&output.stdout),
            &String::from_utf8_lossy(&output.stderr),
            output.status.success(),
        ),
        Err(error) => Scored {
            error: Some(error.to_string()),
            ..Scored::default()
        },
    }
}

pub fn table(directory: &Path, results: &[(PathBuf, Scored)]) -> String {
    let mut out = format!(
        "variants={} directory={}\n{:>4}  {:>9}  {:>6}  {:>5}  {}\n",
        results.len(),
        directory.display(),
        "rank",
        "differing",
        "delta",
        "edits",
        "variant"
    );
    for (rank, (source, scored)) in results.iter().enumerate() {
        let name = source
            .file_name()
            .map(|name| name.to_string_lossy().into_owned())
            .unwrap_or_default();
        let number = |value: Option<usize>| value.map_or("-".into(), |value| value.to_string());
        let delta = scored
            .delta()
            .map_or("-".into(), |delta| format!("{delta:+}"));
        let note = scored.error.as_deref().map_or(String::new(), |error| {
            let prefix = format!("{}/", directory.display());
            format!("  error: {}", error.replace(&prefix, ""))
        });
        out.push_str(&format!(
            "{:>4}  {:>9}  {:>6}  {:>5}  {name}{note}\n",
            rank + 1,
            number(scored.differing),
            delta,
            number(scored.edits),
        ));
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    fn args(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| item.to_string()).collect()
    }

    #[test]
    fn variants_directory_is_split_from_the_passthrough_score_options() {
        let (directory, rest) = split_arguments(&args(&[
            "--owner",
            "resource_3ba:02002910",
            "--variants",
            "sweep",
            "--target",
            "tbs",
        ]))
        .unwrap();
        assert_eq!(directory, "sweep");
        assert_eq!(
            rest,
            args(&["--owner", "resource_3ba:02002910", "--target", "tbs"])
        );
        for bad in [
            &["--variants", "sweep", "--work", "w"][..],
            &["--variants", "sweep", "--diff"],
            &["--variants", "sweep", "candidate.c"],
            &["--variants"],
        ] {
            assert!(split_arguments(&args(bad)).is_err(), "{bad:?}");
        }
    }

    #[test]
    fn parses_the_first_score_block_and_errors() {
        let stdout = "reference_from=rom\ncandidate=96 reference=100 differing_halfwords=12\ncompile=fresh\nbinary_similarity=90% halfword_edits=5 comparison_halfwords=50\ncandidate=4 reference=4 differing_halfwords=0\n";
        assert_eq!(
            Scored::parse(stdout, "", false),
            Scored {
                candidate: Some(96),
                reference: Some(100),
                differing: Some(12),
                edits: Some(5),
                error: None
            }
        );
        let failed = Scored::parse(
            "",
            "xgcc failed: v/b.c: In function `f':\nv/b.c:3: parse error before `;'\n",
            false,
        );
        assert_eq!(
            failed.error.as_deref(),
            Some("v/b.c:3: parse error before `;'")
        );
        let silent = Scored::parse("", "", false);
        assert_eq!(silent.error.as_deref(), Some("failed"));
    }

    #[test]
    fn ranks_by_difference_then_size_then_edits_with_failures_last() {
        let scored = |differing, candidate, edits| Scored {
            candidate: Some(candidate),
            reference: Some(100),
            differing: Some(differing),
            edits: Some(edits),
            error: None,
        };
        let failed = Scored {
            error: Some("failed".into()),
            ..Scored::default()
        };
        let mut rows = [
            ("a", failed),
            ("b", scored(4, 104, 3)),
            ("c", scored(4, 100, 9)),
            ("d", scored(0, 100, 0)),
            ("e", scored(4, 100, 2)),
        ];
        rows.sort_by(|(_, left), (_, right)| left.rank().cmp(&right.rank()));
        let order = rows.iter().map(|(name, _)| *name).collect::<String>();
        assert_eq!(order, "decba");
        let table = table(
            Path::new("/sweep"),
            &rows
                .into_iter()
                .map(|(name, scored)| (PathBuf::from(format!("/sweep/{name}.c")), scored))
                .collect::<Vec<_>>(),
        );
        assert!(
            table.contains("   1          0      +0      0  d.c\n"),
            "{table}"
        );
        assert!(
            table.contains("   4          4      +4      3  b.c\n"),
            "{table}"
        );
        assert!(
            table.contains("   5          -       -      -  a.c  error: failed\n"),
            "{table}"
        );
    }
}
