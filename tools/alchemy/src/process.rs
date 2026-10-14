use std::path::Path;

struct Output {
    success: bool,
    stdout: Vec<u8>,
    stderr: Vec<u8>,
}

pub fn run<S: AsRef<str>>(command: &[S], cwd: impl AsRef<Path>) -> Result<String, String> {
    let program = command.first().ok_or("run: empty command")?.as_ref();
    let name = Path::new(program)
        .file_name()
        .and_then(|name| name.to_str())
        .unwrap_or(program);
    let args: Vec<&str> = command[1..].iter().map(AsRef::as_ref).collect();
    let output = spawn(program, &args, cwd.as_ref()).map_err(|error| format!("{name}: {error}"))?;
    let stdout = String::from_utf8_lossy(&output.stdout).into_owned();
    if output.success {
        return Ok(stdout);
    }
    let stderr = String::from_utf8_lossy(&output.stderr);
    let detail = if stderr.trim().is_empty() {
        stdout.trim()
    } else {
        stderr.trim()
    };
    if detail.is_empty() {
        Err(format!("{name} failed"))
    } else {
        Err(format!("{name} failed: {detail}"))
    }
}

#[cfg(not(target_os = "macos"))]
fn spawn(program: &str, args: &[&str], cwd: &Path) -> Result<Output, String> {
    #[cfg(target_os = "linux")]
    fixed_addresses::for_this_thread();
    let output = std::process::Command::new(program)
        .args(args)
        .current_dir(cwd)
        .output()
        .map_err(|error| error.to_string())?;
    Ok(Output {
        success: output.status.success(),
        stdout: output.stdout,
        stderr: output.stderr,
    })
}

/// Linux's counterpart of the macOS spawner below: the tools a build runs are
/// loaded at the same addresses every time. A thread's personality passes to
/// the programs it starts, so each thread that runs a tool asks once for
/// ADDR_NO_RANDOMIZE, as `setarch -R` does. Where the kernel refuses (a
/// container's default seccomp profile does), the tools run as the system
/// loads them and the build says so once.
#[cfg(target_os = "linux")]
mod fixed_addresses {
    use std::cell::Cell;
    use std::sync::Once;

    const ADDR_NO_RANDOMIZE: libc::c_ulong = 0x0004_0000;
    /// The persona that changes nothing and returns the current one.
    const QUERY: libc::c_ulong = 0xffff_ffff;

    thread_local! {
        static ASKED: Cell<Option<bool>> = const { Cell::new(None) };
    }
    static REFUSED: Once = Once::new();

    /// The persona to ask for, given the current one: none when it already
    /// keeps addresses fixed.
    pub(super) fn wanted(current: libc::c_ulong) -> Option<libc::c_ulong> {
        (current & ADDR_NO_RANDOMIZE == 0).then_some(current | ADDR_NO_RANDOMIZE)
    }

    fn ask() -> Result<(), std::io::Error> {
        let current = unsafe { libc::personality(QUERY) };
        if current == -1 {
            return Err(std::io::Error::last_os_error());
        }
        match wanted(current as libc::c_ulong) {
            Some(persona) if unsafe { libc::personality(persona) } == -1 => {
                Err(std::io::Error::last_os_error())
            }
            _ => Ok(()),
        }
    }

    /// Whether the programs this thread starts are loaded at fixed addresses.
    pub(super) fn for_this_thread() -> bool {
        ASKED.with(|asked| {
            if let Some(fixed) = asked.get() {
                return fixed;
            }
            let result = ask();
            if let Err(error) = &result {
                REFUSED.call_once(|| {
                    eprintln!(
                        "alchemy: the kernel refuses to load the build's tools at fixed addresses \
                         ({error}); they run with address randomisation"
                    );
                });
            }
            asked.set(Some(result.is_ok()));
            result.is_ok()
        })
    }
}

#[cfg(target_os = "macos")]
fn spawn(program: &str, args: &[&str], cwd: &Path) -> Result<Output, String> {
    address_stable::spawn(program, args, cwd)
}

#[cfg(target_os = "macos")]
mod address_stable {
    use super::Output;
    use std::ffi::{CStr, CString};
    use std::os::raw::{c_char, c_int, c_short};
    use std::path::Path;
    use std::ptr;
    use std::sync::atomic::{AtomicUsize, Ordering};

    #[repr(C)]
    struct Attr(*mut u8);
    #[repr(C)]
    struct FileActions(*mut u8);

    extern "C" {
        fn posix_spawnattr_init(attr: *mut Attr) -> c_int;
        fn posix_spawnattr_destroy(attr: *mut Attr) -> c_int;
        fn posix_spawnattr_setflags(attr: *mut Attr, flags: c_short) -> c_int;
        fn posix_spawn_file_actions_init(actions: *mut FileActions) -> c_int;
        fn posix_spawn_file_actions_destroy(actions: *mut FileActions) -> c_int;
        fn posix_spawn_file_actions_addopen(
            actions: *mut FileActions,
            fd: c_int,
            path: *const c_char,
            oflag: c_int,
            mode: u32,
        ) -> c_int;
        fn posix_spawn_file_actions_addchdir_np(
            actions: *mut FileActions,
            path: *const c_char,
        ) -> c_int;
        fn posix_spawnp(
            pid: *mut c_int,
            file: *const c_char,
            actions: *const FileActions,
            attr: *const Attr,
            argv: *const *const c_char,
            envp: *const *const c_char,
        ) -> c_int;
        fn waitpid(pid: c_int, status: *mut c_int, options: c_int) -> c_int;
        fn strerror(code: c_int) -> *const c_char;
        static environ: *const *const c_char;
    }

    const DISABLE_ASLR: c_short = 0x0100;
    const O_WRONLY: c_int = 0x0001;
    const O_CREAT: c_int = 0x0200;
    const O_TRUNC: c_int = 0x0400;

    static SERIAL: AtomicUsize = AtomicUsize::new(0);

    fn describe(code: c_int) -> String {
        unsafe { CStr::from_ptr(strerror(code)) }
            .to_string_lossy()
            .into_owned()
    }

    fn c(text: &str) -> Result<CString, String> {
        CString::new(text).map_err(|_| "argument contains a NUL byte".to_string())
    }

    pub(crate) fn spawn(program: &str, args: &[&str], cwd: &Path) -> Result<Output, String> {
        let serial = SERIAL.fetch_add(1, Ordering::Relaxed);
        let stem =
            std::env::temp_dir().join(format!("alchemy-run-{}-{serial}", std::process::id()));
        let stdout_path = stem.with_extension("out");
        let stderr_path = stem.with_extension("err");
        let result = spawn_to_files(program, args, cwd, &stdout_path, &stderr_path);
        let stdout = std::fs::read(&stdout_path).unwrap_or_default();
        let stderr = std::fs::read(&stderr_path).unwrap_or_default();
        let _ = std::fs::remove_file(&stdout_path);
        let _ = std::fs::remove_file(&stderr_path);
        let success = result?;
        Ok(Output {
            success,
            stdout,
            stderr,
        })
    }

    fn spawn_to_files(
        program: &str,
        args: &[&str],
        cwd: &Path,
        stdout_path: &Path,
        stderr_path: &Path,
    ) -> Result<bool, String> {
        let program_c = c(program)?;
        let argv_c: Vec<CString> = std::iter::once(program)
            .chain(args.iter().copied())
            .map(c)
            .collect::<Result<_, _>>()?;
        let mut argv: Vec<*const c_char> = argv_c.iter().map(|arg| arg.as_ptr()).collect();
        argv.push(ptr::null());
        let cwd_c = c(&cwd.to_string_lossy())?;
        let stdout_c = c(&stdout_path.to_string_lossy())?;
        let stderr_c = c(&stderr_path.to_string_lossy())?;
        let mut pid: c_int = 0;
        let mut status: c_int = 0;
        unsafe {
            let mut attr = Attr(ptr::null_mut());
            let mut actions = FileActions(ptr::null_mut());
            posix_spawnattr_init(&mut attr);
            posix_spawnattr_setflags(&mut attr, DISABLE_ASLR);
            posix_spawn_file_actions_init(&mut actions);
            posix_spawn_file_actions_addchdir_np(&mut actions, cwd_c.as_ptr());
            let flags = O_WRONLY | O_CREAT | O_TRUNC;
            posix_spawn_file_actions_addopen(&mut actions, 1, stdout_c.as_ptr(), flags, 0o600);
            posix_spawn_file_actions_addopen(&mut actions, 2, stderr_c.as_ptr(), flags, 0o600);
            let code = posix_spawnp(
                &mut pid,
                program_c.as_ptr(),
                &actions,
                &attr,
                argv.as_ptr(),
                environ,
            );
            posix_spawn_file_actions_destroy(&mut actions);
            posix_spawnattr_destroy(&mut attr);
            if code != 0 {
                return Err(describe(code));
            }
            if waitpid(pid, &mut status, 0) < 0 {
                return Err("waitpid failed".to_string());
            }
        }
        let exited_normally = status & 0x7f == 0;
        Ok(exited_normally && (status >> 8) & 0xff == 0)
    }
}

#[cfg(all(test, unix))]
mod tests {
    use super::*;

    #[test]
    fn preserves_arguments_and_stdout_without_shell_interpretation() {
        let cwd = std::env::temp_dir();
        assert_eq!(
            run(
                &["/bin/sh", "-c", "printf '%s' \"$1\"", "test", "a b; $HOME"],
                &cwd
            )
            .unwrap(),
            "a b; $HOME"
        );
        let expected = cwd.canonicalize().unwrap();
        assert_eq!(
            run(&["/bin/pwd", "-P"], &cwd).unwrap().trim(),
            expected.to_str().unwrap()
        );
    }

    #[cfg(target_os = "linux")]
    #[test]
    fn a_tool_runs_at_fixed_addresses_wherever_the_kernel_allows_it() {
        assert_eq!(fixed_addresses::wanted(0), Some(0x4_0000));
        assert_eq!(fixed_addresses::wanted(8), Some(0x4_0008));
        assert_eq!(fixed_addresses::wanted(0x4_0008), None);
        let fixed = fixed_addresses::for_this_thread();
        let persona = run(&["cat", "/proc/self/personality"], "/").unwrap();
        let persona = u64::from_str_radix(persona.trim(), 16).unwrap();
        assert_eq!(persona & 0x4_0000 != 0, fixed);
    }

    #[test]
    fn reports_failures_and_rejects_empty_commands() {
        let cwd = std::env::temp_dir();
        assert!(run::<&str>(&[], &cwd)
            .unwrap_err()
            .contains("empty command"));
        assert!(run(&["/nonexistent-alchemy-tool"], &cwd)
            .unwrap_err()
            .starts_with("nonexistent-alchemy-tool:"));
        for (script, expected) in [
            ("printf out; printf err >&2; exit 1", "sh failed: err"),
            ("printf out; printf ' ' >&2; exit 1", "sh failed: out"),
            ("exit 1", "sh failed"),
            ("kill -TERM $$", "sh failed"),
        ] {
            assert_eq!(run(&["/bin/sh", "-c", script], &cwd).unwrap_err(), expected);
        }
    }
}
