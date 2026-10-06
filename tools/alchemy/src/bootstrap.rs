use crate::compiler::{
    bundle::validate_installation,
    routing::{binutils_prefix, bundle, root},
};
use fs2::FileExt;
use std::{fs, path::Path, process::Command};

const FILES: [&str; 9] = [
    crate::compiler::bundle_data::SOURCE_RECORD,
    "xgcc",
    "cpp0",
    "tradcpp0",
    "cc1",
    "as",
    "agbcc/old_agbcc",
    "agbcc/agbcc_arm",
    "agbcc/camelot_arm",
];
const BINUTILS_SHA256: &str = "fd7d227c0dd15cf5448385e56b8ad8313cd491839834b57c0c086ac7b7819a15";
const MODERN_BINUTILS_VERSION: &str = "2.33.1";
const MODERN_BINUTILS_SHA256: &str =
    "ab66fc2d1c3ec0359b8e08843c9f33b63e8707efdff5e4cc5c200eae24722cbf";
const MODERN_BINUTILS_TOOLS: [&str; 5] = ["as", "ld", "nm", "objcopy", "objdump"];
const MODERN_BINUTILS_CFLAGS: &str = "-O2 -Wno-implicit-function-declaration -Wno-int-conversion -Wno-incompatible-function-pointer-types -Wno-deprecated-non-prototype";
const GAS_CFLAGS: &str = "-O2 -std=gnu89 -D_POSIX_C_SOURCE=200809L -DNSIG=32 -Wno-implicit-int -Wno-implicit-function-declaration -Wno-int-conversion -Wno-incompatible-pointer-types -fcommon";
const USAGE: &str = "usage: alchemy bootstrap [--check | --build | --binutils | --from BUNDLE]\nBuild and install missing compiler dependencies from pinned sources.\n--check validates without building; --build rebuilds source dependencies.\n--binutils installs only the pinned native assembler and linker tools.\n--from installs an already admitted distribution.\nInstalled executables live in tools/out/compilers; temporary build work lives in tools/out/compiler-build/.";

pub fn run(args: &[String]) -> Result<(), String> {
    let mut compiler_ready = true;
    match args {
        [] if !bundle().exists() => {
            build_from_sources()?;
            ensure_binutils()?;
        }
        [flag] if flag == "--build" => {
            build_from_sources()?;
            ensure_binutils()?;
        }
        [flag] if flag == "--binutils" => {
            ensure_binutils()?;
            compiler_ready = false;
        }
        _ if args.is_empty() || args == ["--check"] => {
            if args.is_empty() {
                ensure_binutils()?;
            }
            validate_binutils(&binutils_prefix())?;
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
            ensure_binutils()?;
        }
        _ => return Err(USAGE.into()),
    }
    if compiler_ready {
        println!("compiler toolchain ready: {}", bundle().display());
    } else {
        println!("binutils ready: {}", binutils_prefix().display());
    }
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
            root()
                .join("tools/out/compiler-build/build/bootstrap.log")
                .display()
        ))
    }
}

fn verify_archive(path: &Path) -> Result<(), String> {
    verify_digest(path, BINUTILS_SHA256, "binutils-2.10")
}

fn verify_digest(path: &Path, expected: &str, name: &str) -> Result<(), String> {
    let bytes = fs::read(path).map_err(|e| e.to_string())?;
    if crate::compiler::sha256::hex(&bytes) != expected {
        return Err(format!(
            "GNU {name} source archive has an unexpected digest"
        ));
    }
    Ok(())
}

fn build_from_sources() -> Result<(), String> {
    with_install_lock(|| {
        let toolchain = root().join("tools/out/compiler-build");
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
        fs::copy(
            root().join("agbcc/agbcc_arm"),
            stage.path().join("agbcc/agbcc_arm"),
        )
        .map_err(|e| e.to_string())?;
        build_camelot_arm(&toolchain, &log)?;
        fs::copy(
            toolchain.join("camelot_arm/gcc_arm/cc1"),
            stage.path().join("agbcc/camelot_arm"),
        )
        .map_err(|e| e.to_string())?;
        fs::write(
            stage
                .path()
                .join(crate::compiler::bundle_data::SOURCE_RECORD),
            crate::compiler::bundle_data::source_record(),
        )
        .map_err(|e| e.to_string())?;
        install(stage.path(), &bundle(), validate_installation)
    })
}

/// Camelot's ARM compiler study for ☀️ game ARM C (AGENTS.md K1, K3; not
/// admitted, no source routed): the pinned agbcc gcc_arm with upstream GCC's
/// whole arm_adjust_cost rule set as agscc's 2000-07-31 arm.c has it, never a
/// subset chosen by scores. Anti and output dependences cost nothing, and a
/// call waits one cycle on a true dependence, even after a load.
const CAMELOT_ARM_CHANGELOG: &str = "";
const CAMELOT_ARM_ANCHOR: &str = "";
/// Upstream's rules, verbatim, in upstream's order before the load-after-store
/// rule both compilers share.
const CAMELOT_ARM_RULES: &str = "";
const CAMELOT_ARM_HEAD: &str = "";

fn camelot_arm_source(arm_c: &[u8]) -> Result<Vec<u8>, String> {
    let anchor = CAMELOT_ARM_ANCHOR.as_bytes();
    let found: Vec<usize> = arm_c
        .windows(anchor.len())
        .enumerate()
        .filter_map(|(at, window)| (window == anchor).then_some(at))
        .collect();
    let [at] = found[..] else {
        return Err("gcc_arm arm_adjust_cost is not the pinned source".into());
    };
    let split = at + CAMELOT_ARM_HEAD.len();
    let mut out = arm_c[..split].to_vec();
    out.extend_from_slice(CAMELOT_ARM_RULES.as_bytes());
    out.extend_from_slice(&arm_c[split..]);
    Ok(out)
}

fn build_camelot_arm(toolchain: &Path, log: &fs::File) -> Result<(), String> {
    let work = toolchain.join("camelot_arm");
    if work.exists() {
        fs::remove_dir_all(&work).map_err(|e| e.to_string())?;
    }
    fs::create_dir_all(&work).map_err(|e| e.to_string())?;
    let archive = toolchain.join("camelot_arm.tar");
    execute(
        Command::new("git")
            .env_remove("GIT_DIR")
            .env_remove("GIT_WORK_TREE")
            .env_remove("GIT_INDEX_FILE")
            .arg("-C")
            .arg(root().join("agbcc"))
            .args(["archive", "-o"])
            .arg(&archive)
            .args(["HEAD", "gcc_arm", "libiberty", "include", "config.if", "config.sub", "install.sh"]),
        log,
        "agbcc source export",
    )?;
    execute(
        Command::new("tar").arg("-xf").arg(&archive).arg("-C").arg(&work),
        log,
        "agbcc source extraction",
    )?;
    let arm_c = work.join("gcc_arm/config/arm/arm.c");
    let text = fs::read(&arm_c).map_err(|e| e.to_string())?;
    fs::write(&arm_c, camelot_arm_source(&text)?).map_err(|e| e.to_string())?;
    let changelog = work.join("gcc_arm/ChangeLog");
    let mut entry = format!("Camelot ARM compiler (AGENTS.md K1)\n\n{CAMELOT_ARM_CHANGELOG}\n").into_bytes();
    entry.extend(fs::read(&changelog).map_err(|e| e.to_string())?);
    fs::write(&changelog, entry).map_err(|e| e.to_string())?;
    execute(
        Command::new("./configure")
            .args(["--target=arm-elf", "--host=i386-linux-gnu"])
            .current_dir(work.join("gcc_arm")),
        log,
        "camelot_arm configure",
    )?;
    execute(
        Command::new("make").arg("cc1").current_dir(work.join("gcc_arm")),
        log,
        "camelot_arm build",
    )
}

#[cfg(test)]
mod camelot_arm_tests {
    use super::*;
    #[test]
    fn camelot_arm_adds_upstreams_whole_rule_set_to_the_pinned_source() {
        assert!(CAMELOT_ARM_ANCHOR.starts_with(CAMELOT_ARM_HEAD));
        let pinned = fs::read(root().join("agbcc/gcc_arm/config/arm/arm.c")).unwrap();
        let changed = camelot_arm_source(&pinned).unwrap();
        assert_eq!(changed.len(), pinned.len() + CAMELOT_ARM_RULES.len());
        // Every rule upstream's arm_adjust_cost has before the shared
        // load-after-store rule, verbatim and in its order: the whole set.
        let upstream = fs::read_to_string(root().join("agscc/gcc/config/arm/arm.c")).unwrap();
        let body = &upstream[upstream.find("\narm_adjust_cost (").unwrap()..];
        let rules = &body[body.find(CAMELOT_ARM_HEAD).unwrap() + CAMELOT_ARM_HEAD.len()..];
        let shared = rules.find("  if ((i_pat = single_set (insn)) != NULL").unwrap();
        assert_eq!(&rules[..shared], CAMELOT_ARM_RULES);
        assert!(String::from_utf8(changed.clone()).unwrap().contains(&format!("{CAMELOT_ARM_HEAD}{CAMELOT_ARM_RULES}  if ((i_pat")));
        assert!(camelot_arm_source(&changed).is_err());
    }
}

fn ensure_binutils() -> Result<(), String> {
    let prefix = binutils_prefix();
    if validate_binutils(&prefix).is_ok() {
        return Ok(());
    }
    if prefix.exists() {
        return Err(format!(
            "{} is not a valid GNU binutils {MODERN_BINUTILS_VERSION} installation; remove it and run make bootstrap",
            prefix.display()
        ));
    }
    build_binutils(&prefix)
}

fn build_binutils(prefix: &Path) -> Result<(), String> {
    with_install_lock(|| {
        let toolchain = root().join("tools/out/compiler-build");
        let build = toolchain.join("build");
        let sources = toolchain.join("sources");
        fs::create_dir_all(&build).map_err(|e| e.to_string())?;
        fs::create_dir_all(&sources).map_err(|e| e.to_string())?;
        let log = fs::File::create(build.join("bootstrap.log")).map_err(|e| e.to_string())?;
        println!(
            "building GNU binutils {MODERN_BINUTILS_VERSION}; log: {}",
            build.join("bootstrap.log").display()
        );
        let name = format!("binutils-{MODERN_BINUTILS_VERSION}");
        let archive = sources.join(format!("{name}.tar.xz"));
        if !archive.exists() {
            let download = sources.join(format!("{name}.download"));
            execute(
                Command::new("curl")
                    .args(["--fail", "--location", "--silent", "--show-error"])
                    .arg(format!("https://ftp.gnu.org/gnu/binutils/{name}.tar.xz"))
                    .arg("--output")
                    .arg(&download),
                &log,
                "GNU source download",
            )?;
            verify_digest(&download, MODERN_BINUTILS_SHA256, &name)?;
            fs::rename(download, &archive).map_err(|e| e.to_string())?;
        }
        verify_digest(&archive, MODERN_BINUTILS_SHA256, &name)?;
        let source = sources.join(&name);
        let objects = build.join("binutils");
        for directory in [&source, &objects] {
            if directory.exists() {
                fs::remove_dir_all(directory).map_err(|e| e.to_string())?;
            }
        }
        fs::create_dir(&objects).map_err(|e| e.to_string())?;
        execute(
            Command::new("tar")
                .arg("-xJf")
                .arg(&archive)
                .arg("-C")
                .arg(&sources),
            &log,
            "GNU source extraction",
        )?;
        execute(
            Command::new(source.join("configure"))
                .args([
                    "--target=arm-none-eabi",
                    "--disable-nls",
                    "--disable-werror",
                    "--disable-gdb",
                    "--disable-sim",
                    "--disable-gold",
                    "--disable-gprof",
                    "--disable-plugins",
                    "--with-system-zlib",
                ])
                .arg(format!("--prefix={}", prefix.display()))
                .env("CFLAGS", MODERN_BINUTILS_CFLAGS)
                .current_dir(&objects),
            &log,
            "binutils configure",
        )?;
        let jobs = std::thread::available_parallelism().map_or(4, |n| n.get());
        execute(
            Command::new("make")
                .arg(format!("-j{jobs}"))
                .arg("MAKEINFO=true")
                .current_dir(&objects),
            &log,
            "binutils build",
        )?;
        let installed = execute(
            Command::new("make")
                .args(["install", "MAKEINFO=true"])
                .current_dir(&objects),
            &log,
            "binutils install",
        )
        .and_then(|()| validate_binutils(prefix));
        if installed.is_err() && prefix.exists() {
            fs::remove_dir_all(prefix).map_err(|e| e.to_string())?;
        }
        installed?;
        for directory in [&source, &objects] {
            fs::remove_dir_all(directory).map_err(|e| e.to_string())?;
        }
        Ok(())
    })
}

fn validate_binutils(prefix: &Path) -> Result<(), String> {
    let tool = |name: &str| prefix.join("bin").join(format!("arm-none-eabi-{name}"));
    for name in MODERN_BINUTILS_TOOLS {
        if !tool(name).is_file() {
            return Err(format!("missing {}", tool(name).display()));
        }
    }
    let version = Command::new(tool("as"))
        .arg("--version")
        .output()
        .map_err(|e| format!("{}: {e}", tool("as").display()))?;
    let banner = String::from_utf8_lossy(&version.stdout);
    if !banner
        .lines()
        .next()
        .is_some_and(|line| line.ends_with(MODERN_BINUTILS_VERSION))
    {
        return Err(format!(
            "{} is not GNU binutils {MODERN_BINUTILS_VERSION}",
            tool("as").display()
        ));
    }
    let smoke = tempfile::tempdir().map_err(|e| e.to_string())?;
    let source = smoke.path().join("smoke.s");
    let object = smoke.path().join("smoke.o");
    fs::write(&source, "\t.thumb\n\t.global Smoke\nSmoke:\n\tbx lr\n")
        .map_err(|e| e.to_string())?;
    let mut assemble = crate::compiler::routing::assembly_command(
        &source.to_string_lossy(),
        &object.to_string_lossy(),
    );
    assemble[0] = tool("as").to_string_lossy().into_owned();
    for command in [
        assemble,
        vec![
            tool("nm").to_string_lossy().into_owned(),
            object.to_string_lossy().into_owned(),
        ],
    ] {
        let status = Command::new(&command[0])
            .args(&command[1..])
            .output()
            .map_err(|e| format!("{}: {e}", command[0]))?;
        if !status.status.success() {
            return Err(format!("binutils smoke test failed: {}", command.join(" ")));
        }
    }
    Ok(())
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
    if destination.exists() {
        let mut missing = Vec::new();
        for name in FILES {
            match fs::read(destination.join(name)) {
                Ok(existing)
                    if existing
                        == fs::read(stage.path().join(name)).map_err(|e| e.to_string())? => {}
                Ok(_) => {
                    return Err(
                        "a different toolchain is already installed; bootstrap does not replace it"
                            .into(),
                    )
                }
                Err(error) if error.kind() == std::io::ErrorKind::NotFound => missing.push(name),
                Err(error) => return Err(error.to_string()),
            }
        }
        let mut added = Vec::new();
        let complete = (|| {
            for name in missing {
                let path = destination.join(name);
                fs::create_dir_all(path.parent().expect("compiler parent"))
                    .map_err(|e| e.to_string())?;
                fs::copy(stage.path().join(name), &path).map_err(|e| e.to_string())?;
                added.push(path);
            }
            validate(destination)
        })();
        if complete.is_err() {
            for path in added {
                let _ = fs::remove_file(path);
            }
        }
        return complete;
    }
    fs::rename(stage.path(), destination).map_err(|e| e.to_string())?;
    Ok(())
}
