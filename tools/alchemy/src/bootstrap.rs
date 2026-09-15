//! Installed dependencies survive output cleanup; imports never approve hashes.
use crate::compiler::{
    bundle::validate_installation,
    routing::{bundle, root},
};
use fs2::FileExt;
use std::{fs, path::Path, process::Command};

const FILES: [&str; 6] = ["xgcc", "cpp0", "tradcpp0", "cc1", "as", "agbcc/old_agbcc"];
const BINUTILS_SHA256: &str = "fd7d227c0dd15cf5448385e56b8ad8313cd491839834b57c0c086ac7b7819a15";
const GAS_CFLAGS: &str = "-O2 -std=gnu89 -D_POSIX_C_SOURCE=200809L -DNSIG=32 -Wno-implicit-int -Wno-implicit-function-declaration -Wno-int-conversion -Wno-incompatible-pointer-types -fcommon";
const USAGE: &str = "usage: alchemy bootstrap [--check | --build | --from BUNDLE]\nBuild and install missing compiler dependencies from pinned sources.\n--check validates without building; --build rebuilds source dependencies.\n--from installs an already admitted distribution.\nInstalled executables live in tools/compilers; temporary build work lives in out/.";

pub fn run(args: &[String]) -> Result<(), String> {
    match args {
        [] if !bundle().exists() => build_from_sources()?,
        [flag] if flag == "--build" => build_from_sources()?,
        _ if args.is_empty() || args == ["--check"] => {
            crate::compiler::bundle::validate_bundle(
                crate::compiler::routing::CompilerTarget::Tbs,
            )?;
            crate::compiler::bundle::validate_bundle(
                crate::compiler::routing::CompilerTarget::Tla,
            )?;
            crate::compiler::bundle::validate_agbcc_bundle()?;
        }
        [help] if help == "--help" || help == "-h" => {
            println!("{USAGE}");
            return Ok(());
        }
        [flag, source] if flag == "--from" => {
            with_install_lock(|| install(Path::new(source), &bundle(), validate_installation))?;
        }
        _ => return Err(USAGE.into()),
    }
    println!("compiler toolchain ready: {}", bundle().display());
    Ok(())
}

fn with_install_lock(action: impl FnOnce() -> Result<(), String>) -> Result<(), String> {
    crate::compiler::bundle::ensure_canonical_bundle_root()?;
    let installation = bundle();
    let parent = installation
        .parent()
        .ok_or("compiler installation has no parent")?;
    fs::create_dir_all(&parent).map_err(|e| e.to_string())?;
    let lock = fs::OpenOptions::new()
        .create(true)
        .truncate(false)
        .read(true)
        .write(true)
        .open(parent.join(".compiler.lock"))
        .map_err(|e| e.to_string())?;
    FileExt::lock_exclusive(&lock).map_err(|e| e.to_string())?;
    action()
}

fn execute(command: &mut Command, log: &fs::File, label: &str) -> Result<(), String> {
    let status = command
        .stdout(log.try_clone().map_err(|e| e.to_string())?)
        .stderr(log.try_clone().map_err(|e| e.to_string())?)
        .status()
        .map_err(|e| format!("{label}: {e}"))?;
    if status.success() {
        Ok(())
    } else {
        Err(format!(
            "{label} failed; see {}",
            root().join("out/compilers/build/bootstrap.log").display()
        ))
    }
}

fn verify_archive(path: &Path) -> Result<(), String> {
    let bytes = fs::read(path).map_err(|e| e.to_string())?;
    if crate::compiler::sha256::hex(&bytes) != BINUTILS_SHA256 {
        return Err("GNU binutils 2.10 source archive has an unexpected digest".into());
    }
    Ok(())
}

fn build_from_sources() -> Result<(), String> {
    if crate::compiler::bundle::host_key() != Some("darwin-arm64") {
        return Err("source bootstrap is currently admitted only for Apple Silicon macOS".into());
    }
    with_install_lock(|| {
        let toolchain = root().join("out/compilers");
        let build = toolchain.join("build");
        let sources = toolchain.join("sources");
        fs::create_dir_all(&build).map_err(|e| e.to_string())?;
        fs::create_dir_all(&sources).map_err(|e| e.to_string())?;
        let log = fs::File::create(build.join("bootstrap.log")).map_err(|e| e.to_string())?;
        println!(
            "building pinned compiler sources; log: {}",
            build.join("bootstrap.log").display()
        );
        execute(
            Command::new("make")
                .arg("compiler-sources")
                .current_dir(root()),
            &log,
            "compiler source build",
        )?;
        let archive = sources.join("binutils-2.10.tar.gz");
        if !archive.exists() {
            let download = sources.join("binutils-2.10.download");
            execute(
                Command::new("curl")
                    .args([
                        "--fail",
                        "--location",
                        "--silent",
                        "--show-error",
                        "https://ftp.gnu.org/gnu/binutils/binutils-2.10.tar.gz",
                        "--output",
                    ])
                    .arg(&download),
                &log,
                "GNU source download",
            )?;
            verify_archive(&download)?;
            fs::rename(download, &archive).map_err(|e| e.to_string())?;
        }
        verify_archive(&archive)?;
        let source = sources.join("binutils-2.10");
        let gas = build.join("gas");
        for directory in [&source, &gas] {
            if directory.exists() {
                fs::remove_dir_all(directory).map_err(|e| e.to_string())?;
            }
        }
        fs::create_dir(&gas).map_err(|e| e.to_string())?;
        execute(
            Command::new("tar")
                .arg("-xzf")
                .arg(&archive)
                .arg("-C")
                .arg(&sources),
            &log,
            "GNU source extraction",
        )?;
        execute(
            Command::new("../../sources/binutils-2.10/configure")
                .args([
                    "--target=arm-elf",
                    "--build=i686-unknown-linux-gnu",
                    "--host=i686-unknown-linux-gnu",
                    "--disable-nls",
                    "--disable-shared",
                ])
                .env("CC", "clang")
                .env("CFLAGS", GAS_CFLAGS)
                .current_dir(&gas),
            &log,
            "GAS configure",
        )?;
        execute(
            Command::new("make")
                .args(["MAKEINFO=true", "all-gas"])
                .current_dir(&gas),
            &log,
            "GAS source build",
        )?;
        let stage = tempfile::Builder::new()
            .prefix(".source-")
            .tempdir_in(&toolchain)
            .map_err(|e| e.to_string())?;
        fs::create_dir(stage.path().join("agbcc")).map_err(|e| e.to_string())?;
        for name in ["xgcc", "cpp0", "tradcpp0", "cc1"] {
            fs::copy(
                root().join("agscc/build/gcc").join(name),
                stage.path().join(name),
            )
            .map_err(|e| e.to_string())?;
        }
        fs::copy(gas.join("gas/as-new"), stage.path().join("as")).map_err(|e| e.to_string())?;
        fs::copy(
            root().join("agbcc/gcc/old_agbcc"),
            stage.path().join("agbcc/old_agbcc"),
        )
        .map_err(|e| e.to_string())?;
        install(stage.path(), &bundle(), validate_installation)
    })
}

fn install(
    source: &Path,
    destination: &Path,
    validate: impl Fn(&Path) -> Result<(), String>,
) -> Result<(), String> {
    let parent = destination
        .parent()
        .ok_or("toolchain installation has no parent")?;
    let stage = tempfile::Builder::new()
        .prefix(".install-")
        .tempdir_in(parent)
        .map_err(|e| e.to_string())?;
    fs::create_dir(stage.path().join("agbcc")).map_err(|e| e.to_string())?;
    for name in FILES {
        let input = source.join(name);
        let output = stage.path().join(name);
        fs::copy(&input, &output).map_err(|e| format!("cannot stage {}: {e}", input.display()))?;
        fs::set_permissions(
            &output,
            fs::metadata(&input)
                .map_err(|e| e.to_string())?
                .permissions(),
        )
        .map_err(|e| e.to_string())?;
    }
    validate(stage.path())?;
    // A replacement requires an explicit removal; never destroy an installed
    // compiler used by another worktree, even if the incoming bundle is valid.
    if destination.exists() {
        validate(destination)?;
        for name in FILES {
            if fs::read(stage.path().join(name)).map_err(|e| e.to_string())?
                != fs::read(destination.join(name)).map_err(|e| e.to_string())?
            {
                return Err(
                    "a different toolchain is already installed; bootstrap does not replace it"
                        .into(),
                );
            }
        }
        return Ok(());
    }
    fs::rename(stage.path(), destination).map_err(|e| e.to_string())?;
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    fn fixture(root: &Path) -> std::path::PathBuf {
        let p = root.join("bundle");
        fs::create_dir_all(p.join("agbcc")).unwrap();
        for name in FILES {
            fs::write(p.join(name), name).unwrap();
        }
        p
    }
    #[test]
    fn rejected_import_preserves_installation() {
        let temp = tempfile::tempdir().unwrap();
        let source = fixture(temp.path());
        let destination = temp.path().join("installed");
        fs::create_dir(&destination).unwrap();
        fs::write(destination.join("keep"), b"existing").unwrap();
        assert!(install(&source, &destination, |_| Err("unapproved".into())).is_err());
        assert_eq!(fs::read(destination.join("keep")).unwrap(), b"existing");
        assert_eq!(fs::read_dir(temp.path()).unwrap().count(), 2);
    }
    #[test]
    fn output_cleanup_preserves_toolchain_and_allows_rebootstrap() {
        let temp = tempfile::tempdir().unwrap();
        let source = fixture(temp.path());
        let parent = temp.path().join("toolchain");
        fs::create_dir(&parent).unwrap();
        let destination = parent.join("compilers");
        install(&source, &destination, |_| Ok(())).unwrap();
        let out = temp.path().join("out");
        fs::create_dir(&out).unwrap();
        fs::write(out.join("report"), b"disposable").unwrap();
        fs::remove_dir_all(&out).unwrap();
        install(&source, &destination, |_| Ok(())).unwrap();
        for name in FILES {
            assert_eq!(fs::read(destination.join(name)).unwrap(), name.as_bytes());
        }
        assert!(!crate::compiler::routing::bundle().starts_with(root().join("out")));
    }
    #[test]
    fn changed_bundle_cannot_replace_installation() {
        let temp = tempfile::tempdir().unwrap();
        let source = fixture(temp.path());
        let destination = temp.path().join("installed");
        install(&source, &destination, |_| Ok(())).unwrap();
        fs::write(source.join("cc1"), b"changed").unwrap();
        assert!(install(&source, &destination, |_| Ok(())).is_err());
        assert_eq!(fs::read(destination.join("cc1")).unwrap(), b"cc1");
    }
    #[test]
    fn modified_source_archives_are_rejected_before_extraction() {
        let temp = tempfile::tempdir().unwrap();
        let archive = temp.path().join("binutils-2.10.tar.gz");
        fs::write(&archive, b"modified archive").unwrap();
        assert!(verify_archive(&archive)
            .unwrap_err()
            .contains("unexpected digest"));
    }
    #[test]
    fn unapproved_executables_are_rejected_by_real_validation() {
        let temp = tempfile::tempdir().unwrap();
        let source = fixture(temp.path());
        #[cfg(unix)]
        {
            use std::os::unix::fs::PermissionsExt;
            for name in FILES {
                fs::set_permissions(source.join(name), fs::Permissions::from_mode(0o755)).unwrap();
            }
        }
        let destination = temp.path().join("installed");
        assert!(install(&source, &destination, validate_installation).is_err());
        assert!(!destination.exists());
        assert_eq!(fs::read_dir(temp.path()).unwrap().count(), 1);
    }
}
