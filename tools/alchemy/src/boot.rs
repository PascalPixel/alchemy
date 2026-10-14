//! The boot test: each ☀️ edition's matching ROM and its modern ROM (`make
//! modern`) run in mGBA under an ARM debugger, from a blank save to the file
//! menu and 600 VBlanks past it. The matching ROM is the cartridge's bytes,
//! so it is the reference: the modern ROM must reach the same milestones on
//! the same frames, show the same palette, VRAM and OAM at the file menu,
//! and still be running, with little lag, 600 VBlanks later.
//!
//! The emulator runs on its own high-level BIOS (`useBios=0`), so no BIOS
//! file is read (L1, L3). Everything a run writes stays in its directory
//! under out/boot: the copy of the ROM, the blank save beside it, the
//! emulator's settings and the memory dumps.
//!
//! The debugger reads the ELF's symbols. The ELFs have no debug info, so a
//! milestone is a function's entry address (`*Name`): without the `*`, gdb
//! guesses where the prologue ends, and the modern and matching code differ
//! there. The debugger runs a generated command file, `gdb -batch -x
//! boot.gdb`: an error in a command file ends the run with a nonzero status,
//! where a list of `-ex` commands carries on past it, and once passed with no
//! emulator running. Each stop is checked against the breakpoint it waited
//! for, since a signal (a crash) also ends a `continue`, and `$ok` is set
//! last.

use crate::targets::{decomp_target, DecompTarget};
use std::fs;
use std::net::{SocketAddr, TcpStream};
use std::path::{Path, PathBuf};
use std::process::{Child, Command, ExitStatus, Stdio};
use std::time::{Duration, Instant};

const USAGE: &str = "usage: alchemy boot [--target tbs-EDITION]... [--emulator COMMAND] [--gdb COMMAND] [--timeout SECONDS]
Boot each ☀️ edition (all six by default) twice in mGBA under an ARM
debugger: its matching ROM (out/tbs-xx, make compare-tbs) and its modern ROM
(out/modern, make modern). Each run starts from a fresh directory,
out/boot/<edition>-<build>, holding a copy of the ROM, its blank save and the
emulator's settings, and follows the boot to the file menu and 600 VBlanks
past it. An edition passes when both ROMs reach every milestone in order,
the modern ROM's milestone lines and its palette, VRAM and OAM at the file
menu equal the matching ROM's, and both still run with little lag 600
VBlanks later.
  --emulator  mGBA, whose GDB stub -g opens on port 2345: $BOOT_EMULATOR, else mgba
  --gdb       an ARM debugger: $BOOT_GDB, else arm-none-eabi-gdb
  --timeout   the seconds one run may take: $BOOT_TIMEOUT, else 300
CI runs it, and make boot-tbs here; make verify and make land never do.";

const OUTPUT: &str = "out/boot";
/// mGBA's GDB stub listens here when it starts with -g.
const PORT: u16 = 2345;
const DEFAULT_EMULATOR: &str = "mgba";
const DEFAULT_GDB: &str = "arm-none-eabi-gdb";
const DEFAULT_TIMEOUT: &str = "300";

/// The functions whose entries the boot passes, in order. The debugger
/// prints each with the frame count, and Event_SetDestination with its
/// scene and entrance.
const ENTRIES: [&str; 5] = [
    "System_Initialize",
    "Game_ResetForNewGame",
    "Title_ShowSplashScreen",
    "Title_ShowAnimatedSplash",
    "Event_SetDestination",
];
/// The file menu scans the save's record flags as it opens: the second call
/// after a reset.
const FILE_MENU: &str = "SaveState_ScanRecordFlags";
/// Liveness is counted in VBlank interrupts after the file menu.
const VBLANK: &str = "System_VBlankHandler";
const VBLANKS: u32 = 600;
/// The variables the lines print.
const VARIABLES: [&str; 3] = ["gFrameCount", "gGameState", "gLagFrameCount"];

/// What a boot from a blank save reaches, in order: each milestone's name
/// and what its line carries after the frame. The frames are the matching
/// ROM's, which the modern ROM must keep: in mGBA 0.10.5, 0, 10, 10, 132,
/// 254 and 361 in all six editions.
const MILESTONES: [(&str, &str); 6] = [
    ("System_Initialize", ""),
    ("Game_ResetForNewGame", ""),
    ("Title_ShowSplashScreen", ""),
    ("Title_ShowAnimatedSplash", ""),
    ("Event_SetDestination(1, 1)", ""),
    ("file menu", "scene 1 entrance 1"),
];

/// How far the modern ROM's frame count may run ahead of or behind the
/// matching ROM's over the 600 VBlanks, and how many lag frames either may
/// count. No two emulator runs stop on the same instruction 600 VBlanks on:
/// measured, the modern ROMs advanced 587 frames and the matching ones 586,
/// with no lag.
const ADVANCE_TOLERANCE: i64 = 4;
const LAG_LIMIT: i64 = 4;

/// DISPCNT's forced blank: the screen shows nothing while it is set.
const FORCED_BLANK: u32 = 0x80;

/// The memory the file menu shows, dumped by both runs and compared.
const DISPLAY: [(&str, &str, u32, u32); 3] = [
    ("palette RAM", "menu-pal.bin", 0x0500_0000, 0x0500_0400),
    ("VRAM", "menu-vram.bin", 0x0600_0000, 0x0601_8000),
    ("OAM", "menu-oam.bin", 0x0700_0000, 0x0700_0400),
];

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Build {
    Matching,
    Modern,
}

const BUILDS: [Build; 2] = [Build::Matching, Build::Modern];

impl Build {
    fn name(self) -> &'static str {
        match self {
            Build::Matching => "matching",
            Build::Modern => "modern",
        }
    }

    /// The ROM the emulator runs: out/tbs-xx/tbs-xx.gba, or the modern
    /// build's out/modern/tbs-xx.gba.
    fn rom(self, target: DecompTarget) -> PathBuf {
        match self {
            Build::Matching => Path::new(target.output_dir).join(format!("{}.gba", target.id)),
            Build::Modern => Path::new("out/modern").join(format!("{}.gba", target.id)),
        }
    }

    /// The ELF whose symbols the debugger reads.
    fn elf(self, target: DecompTarget) -> PathBuf {
        let directory = match self {
            Build::Matching => PathBuf::from(target.output_dir),
            Build::Modern => Path::new("out/modern").join(target.id.as_str()),
        };
        directory.join(format!("{}.elf", target.id))
    }

    /// What builds the ROM, for the message when it is missing.
    fn make(self) -> &'static str {
        match self {
            Build::Matching => "make compare-tbs",
            Build::Modern => "make modern",
        }
    }
}

struct Options {
    emulator: Vec<String>,
    gdb: Vec<String>,
    timeout: Duration,
}

/// A command from an option or the environment: a program and any leading
/// arguments, split at spaces.
fn command(text: &str, what: &str) -> Result<Vec<String>, String> {
    let words: Vec<String> = text.split_whitespace().map(str::to_owned).collect();
    if words.is_empty() {
        return Err(format!("the {what} command is empty"));
    }
    Ok(words)
}

fn setting(variable: &str, default: &str) -> String {
    std::env::var(variable)
        .ok()
        .filter(|value| !value.trim().is_empty())
        .unwrap_or_else(|| default.to_owned())
}

fn seconds(text: &str) -> Result<Duration, String> {
    match text.parse::<u64>() {
        Ok(seconds) if seconds > 0 => Ok(Duration::from_secs(seconds)),
        _ => Err(format!(
            "the timeout is a whole number of seconds, not {text:?}"
        )),
    }
}

pub fn run(args: &[String]) -> Result<(), String> {
    let mut targets = Vec::new();
    let mut emulator = setting("BOOT_EMULATOR", DEFAULT_EMULATOR);
    let mut gdb = setting("BOOT_GDB", DEFAULT_GDB);
    let mut timeout = setting("BOOT_TIMEOUT", DEFAULT_TIMEOUT);
    let mut args = args.iter();
    while let Some(arg) = args.next() {
        match arg.as_str() {
            "-h" | "--help" => {
                println!("{USAGE}");
                return Ok(());
            }
            "--target" => {
                let target = decomp_target(Some(args.next().ok_or(USAGE)?))?;
                if !target.id.as_str().starts_with("tbs-") {
                    return Err(format!(
                        "{}: the boot test runs ☀️'s editions only",
                        target.id
                    ));
                }
                targets.push(target);
            }
            "--emulator" => emulator = args.next().ok_or(USAGE)?.clone(),
            "--gdb" => gdb = args.next().ok_or(USAGE)?.clone(),
            "--timeout" => timeout = args.next().ok_or(USAGE)?.clone(),
            _ => return Err(USAGE.into()),
        }
    }
    if targets.is_empty() {
        for edition in ["ja", "en", "de", "es", "fr", "it"] {
            targets.push(decomp_target(Some(&format!("tbs-{edition}")))?);
        }
    }
    let options = Options {
        emulator: command(&emulator, "emulator")?,
        gdb: command(&gdb, "debugger")?,
        timeout: seconds(&timeout)?,
    };
    let root = crate::compiler::routing::root();
    // Every input before the first run: a renamed milestone or a stale ROM
    // is named at once, not after minutes of emulation.
    for target in &targets {
        for build in BUILDS {
            inputs(root, *target, build)?;
        }
    }
    let mut failures = Vec::new();
    for target in &targets {
        let mut runs = Vec::new();
        for build in BUILDS {
            let directory = root
                .join(OUTPUT)
                .join(format!("{}-{}", target.id, build.name()));
            let rom = root.join(build.rom(*target));
            let elf = root.join(build.elf(*target));
            let trace = boot(&directory, &rom, &elf, &options)?;
            println!("{} {}: {}", target.id, build.name(), trace.summary());
            let named = |problem: &String| format!("{} {}: {problem}", target.id, build.name());
            failures.extend(judge(&trace).iter().map(named));
            runs.push((trace, directory));
        }
        let [(matching, at_matching), (modern, at_modern)] = &runs[..] else {
            unreachable!("one run for each of the two builds");
        };
        let mut problems = compare(matching, modern);
        problems.extend(compare_display(at_matching, at_modern));
        failures.extend(
            problems
                .iter()
                .map(|problem| format!("{} modern: {problem}", target.id)),
        );
    }
    if failures.is_empty() {
        println!(
            "boot: {} editions reach the file menu and run on, each modern ROM as its matching ROM",
            targets.len()
        );
        Ok(())
    } else {
        Err(format!("the boot test failed:\n{}", failures.join("\n")))
    }
}

/// The ROM and ELF of one build exist, the matching ROM is the one rom.sha1
/// names, and the ELF defines every symbol the command file reads.
fn inputs(root: &Path, target: DecompTarget, build: Build) -> Result<(), String> {
    for path in [build.rom(target), build.elf(target)] {
        if !root.join(&path).is_file() {
            return Err(format!(
                "{} is missing: run {} first",
                path.display(),
                build.make()
            ));
        }
    }
    if build == Build::Matching {
        crate::coverage::calcrom::verified(root, target)?.map_err(|_| {
            format!(
                "{} is not the ROM rom.sha1 names: run make compare-tbs first",
                build.rom(target).display()
            )
        })?;
    }
    let elf = build.elf(target);
    let bytes = fs::read(root.join(&elf)).map_err(|error| format!("{}: {error}", elf.display()))?;
    let parsed =
        crate::elf::Elf::parse(&bytes).map_err(|error| format!("{}: {error}", elf.display()))?;
    let names: Vec<&str> = parsed
        .symbols
        .iter()
        .map(|symbol| symbol.name.as_str())
        .collect();
    let missing = missing_symbols(&names);
    if missing.is_empty() {
        return Ok(());
    }
    Err(format!(
        "{} defines no {}, which the boot test's command file names",
        elf.display(),
        missing.join(", ")
    ))
}

/// The symbols the command file reads that `names` lacks, in its order.
fn missing_symbols(names: &[&str]) -> Vec<&'static str> {
    ENTRIES
        .iter()
        .chain(&[FILE_MENU, VBLANK])
        .chain(&VARIABLES)
        .copied()
        .filter(|symbol| !names.contains(symbol))
        .collect()
}

/// The debugger's command file. Its dumps land in its working directory,
/// the run's own.
fn script() -> String {
    let frame = "*(int *)&gFrameCount";
    let scene = "*(short *)((char *)&gGameState + 0x1c0)";
    let entrance = "*(short *)((char *)&gGameState + 0x1c2)";
    let lag = "*(int *)&gLagFrameCount";
    let dispcnt = "*(unsigned short *)0x04000000";
    let bldcnt = "*(unsigned short *)0x04000050";
    let mut lines = vec![
        "set pagination off".to_owned(),
        "set confirm off".to_owned(),
        "set $ok = 0".to_owned(),
        "set tcp connect-timeout 30".to_owned(),
        format!("target remote 127.0.0.1:{PORT}"),
    ];
    for entry in ENTRIES {
        lines.push(if entry == "Event_SetDestination" {
            format!("dprintf *{entry},\"milestone {entry}(%d, %d) frame %d\\n\",$r0,$r1,{frame}")
        } else {
            format!("dprintf *{entry},\"milestone {entry} frame %d\\n\",{frame}")
        });
    }
    // Stop at the entry of `symbol` after passing it `passes` times, or end
    // the run where the target stopped instead.
    let stop = |symbol: &str, passes: u32| {
        [
            format!("break *{symbol}"),
            format!("ignore $bpnum {passes}"),
            "continue".to_owned(),
            format!("if ((unsigned)$pc & ~1) != ((unsigned)&{symbol} & ~1)"),
            format!("  printf \"stopped at %#x, not at {symbol}\\n\", (unsigned)$pc"),
            "  quit 1".to_owned(),
            "end".to_owned(),
        ]
    };
    lines.extend(stop(FILE_MENU, 1));
    lines.push(format!(
        "printf \"milestone file menu frame %d scene %d entrance %d\\n\",{frame},{scene},{entrance}"
    ));
    lines.push(format!(
        "printf \"display dispcnt %#x bldcnt %#x\\n\",{dispcnt},{bldcnt}"
    ));
    for (_, file, start, end) in DISPLAY {
        lines.push(format!(
            "dump binary memory {file} {start:#010x} {end:#010x}"
        ));
    }
    lines.push("delete".to_owned());
    lines.extend(stop(VBLANK, VBLANKS));
    lines.push(format!(
        "printf \"alive frame %d lag %d scene %d entrance %d dispcnt %#x\\n\",{frame},{lag},{scene},{entrance},{dispcnt}"
    ));
    lines.extend([
        "set $ok = 1".to_owned(),
        "printf \"verdict ok=%d\\n\", $ok".to_owned(),
        "kill".to_owned(),
        "quit !$ok".to_owned(),
    ]);
    lines.join("\n") + "\n"
}

/// Whether something accepts connections on `port` of this machine.
fn listening(port: u16) -> bool {
    let address = SocketAddr::from(([127, 0, 0, 1], port));
    TcpStream::connect_timeout(&address, Duration::from_millis(250)).is_ok()
}

/// Waits for `child` until `deadline`: its status, or None at the deadline,
/// when it is still running.
fn wait_until(child: &mut Child, deadline: Instant) -> Result<Option<ExitStatus>, String> {
    loop {
        if let Some(status) = child.try_wait().map_err(|error| error.to_string())? {
            return Ok(Some(status));
        }
        if Instant::now() >= deadline {
            return Ok(None);
        }
        std::thread::sleep(Duration::from_millis(50));
    }
}

/// Ends a child that is still running, and reaps it.
fn end(child: &mut Child) {
    let _ = child.kill();
    let _ = child.wait();
}

/// Starts `words` with `arguments` in `directory`, its output in `stdout`
/// and `stderr`, which may be one file.
fn spawn(
    words: &[String],
    arguments: &[String],
    directory: &Path,
    (stdout, stderr): (&Path, &Path),
    settings: Option<&Path>,
) -> Result<Child, String> {
    let open = |path: &Path, same: Option<&fs::File>| {
        match same {
            Some(file) => file.try_clone(),
            None => fs::File::create(path),
        }
        .map_err(|error| format!("{}: {error}", path.display()))
    };
    let out = open(stdout, None)?;
    let err = open(stderr, (stderr == stdout).then_some(&out))?;
    let mut command = Command::new(&words[0]);
    command
        .args(&words[1..])
        .args(arguments)
        .current_dir(directory)
        .stdin(Stdio::null())
        .stdout(out)
        .stderr(err);
    if let Some(settings) = settings {
        // The emulator keeps its settings in $XDG_CONFIG_HOME/mgba, and
        // plays no sound.
        command
            .env("XDG_CONFIG_HOME", settings)
            .env("SDL_AUDIODRIVER", "dummy");
    }
    command
        .spawn()
        .map_err(|error| format!("{}: {error}", words[0]))
}

/// One run: a fresh directory, mGBA on a copy of the ROM, the debugger on
/// the ELF, and the record of what the debugger printed. An Err is a run
/// that could not start or end cleanly; a boot that goes wrong is a Trace
/// the judge refuses.
fn boot(directory: &Path, rom: &Path, elf: &Path, options: &Options) -> Result<Trace, String> {
    let shown = |error: std::io::Error| format!("{}: {error}", directory.display());
    if directory.exists() {
        fs::remove_dir_all(directory).map_err(shown)?;
    }
    let settings = directory.join("settings");
    fs::create_dir_all(&settings).map_err(shown)?;
    let name = rom
        .file_name()
        .ok_or_else(|| format!("{}: no file name", rom.display()))?;
    let copy = directory.join(name);
    fs::copy(rom, &copy).map_err(|error| format!("{}: {error}", rom.display()))?;
    fs::write(directory.join("boot.gdb"), script()).map_err(shown)?;
    if listening(PORT) {
        return Err(format!(
            "something already listens on 127.0.0.1:{PORT}, the port of mGBA's GDB stub: another emulator or boot run?"
        ));
    }
    let mut arguments: Vec<String> = [
        "-g",
        "-C",
        "mute=1",
        "-C",
        "audioSync=0",
        "-C",
        "videoSync=0",
        "-C",
        "useBios=0",
    ]
    .iter()
    .map(|word| (*word).to_owned())
    .collect();
    arguments.push(copy.to_string_lossy().into_owned());
    let log = directory.join("mgba.log");
    let mut emulator = spawn(
        &options.emulator,
        &arguments,
        directory,
        (&log, &log),
        Some(&settings),
    )?;
    let arguments = ["-nx", "-batch", "-x", "boot.gdb"]
        .iter()
        .map(|word| (*word).to_owned())
        .chain([elf.to_string_lossy().into_owned()])
        .collect::<Vec<_>>();
    let (printed, errors) = (directory.join("gdb.out"), directory.join("gdb.err"));
    let mut debugger = match spawn(
        &options.gdb,
        &arguments,
        directory,
        (&printed, &errors),
        None,
    ) {
        Ok(debugger) => debugger,
        Err(error) => {
            end(&mut emulator);
            return Err(error);
        }
    };
    let status = wait_until(&mut debugger, Instant::now() + options.timeout);
    if !matches!(status, Ok(Some(_))) {
        end(&mut debugger);
    }
    // Whether the emulator had ended by itself, before it is ended.
    let emulator_status = emulator.try_wait().ok().flatten();
    if emulator_status.is_none() {
        end(&mut emulator);
    }
    let status = status?;
    let freed = Instant::now() + Duration::from_secs(10);
    while listening(PORT) {
        if Instant::now() >= freed {
            return Err(format!(
                "127.0.0.1:{PORT} is still open after the emulator ended"
            ));
        }
        std::thread::sleep(Duration::from_millis(100));
    }
    let mut record = fs::read_to_string(&printed).map_err(shown)?;
    if !record.is_empty() && !record.ends_with('\n') {
        record.push('\n');
    }
    for line in fs::read_to_string(&errors).map_err(shown)?.lines() {
        record += &format!("gdb-stderr {line}\n");
    }
    if let Some(early) = emulator_status {
        record += &format!("emulator-exit {}\n", code(Some(early)));
    }
    record += &format!("gdb-exit {}\n", code(status));
    fs::write(directory.join("trace.txt"), &record).map_err(shown)?;
    Ok(Trace::parse(&record))
}

/// An exit for the record: its code, "signal", or "timeout" for a process
/// that was still running.
fn code(status: Option<ExitStatus>) -> String {
    match status {
        None => "timeout".to_owned(),
        Some(status) => status
            .code()
            .map_or_else(|| "signal".to_owned(), |code| code.to_string()),
    }
}

/// A milestone line: its name, its frame, and what follows the frame.
#[derive(Debug, Clone, PartialEq, Eq)]
struct Milestone {
    name: String,
    frame: i64,
    rest: String,
}

impl Milestone {
    fn parse(text: &str) -> Option<Milestone> {
        let (name, after) = text.rsplit_once(" frame ")?;
        let (frame, rest) = after.split_once(' ').unwrap_or((after, ""));
        Some(Milestone {
            name: name.to_owned(),
            frame: frame.parse().ok()?,
            rest: rest.to_owned(),
        })
    }
}

/// The state 600 VBlanks after the file menu.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
struct Alive {
    frame: i64,
    lag: i64,
    scene: i64,
    entrance: i64,
    dispcnt: u32,
}

/// What one run recorded, read from its trace.txt: the debugger's output,
/// then each line it wrote to stderr (`gdb-stderr`), the emulator's exit
/// when it had ended by itself (`emulator-exit`) and the debugger's exit
/// (`gdb-exit`).
#[derive(Debug, Default)]
struct Trace {
    /// Every `milestone` line, in order.
    milestones: Vec<String>,
    /// DISPCNT and BLDCNT at the file menu.
    display: Option<(u32, u32)>,
    alive: Option<Alive>,
    verdict: bool,
    /// What says the boot went wrong: a signal, a stop at the wrong place,
    /// anything on the debugger's stderr.
    errors: Vec<String>,
    /// The debugger's exit: a code, "signal" or "timeout".
    exit: Option<String>,
    /// The emulator's exit, when it had ended by itself.
    emulator: Option<String>,
}

/// The output lines that say the target stopped where the boot never does.
const STOPPED: [&str; 3] = [
    "Program received signal",
    "Program terminated",
    "stopped at ",
];

fn number(text: &str) -> Option<i64> {
    match text.strip_prefix("0x") {
        Some(hex) => i64::from_str_radix(hex, 16).ok(),
        None => text.parse().ok(),
    }
}

/// The values of `keys` in a line of `key value` pairs.
fn fields(text: &str, keys: &[&str]) -> Option<Vec<i64>> {
    let words: Vec<&str> = text.split_whitespace().collect();
    keys.iter()
        .map(|key| {
            let at = words.iter().position(|word| word == key)?;
            number(words.get(at + 1)?)
        })
        .collect()
}

impl Trace {
    fn parse(record: &str) -> Trace {
        let mut trace = Trace::default();
        for line in record.lines() {
            if let Some(milestone) = line.strip_prefix("milestone ") {
                trace.milestones.push(milestone.to_owned());
            } else if let Some(display) = line.strip_prefix("display ") {
                if let Some(values) = fields(display, &["dispcnt", "bldcnt"]) {
                    trace.display = Some((values[0] as u32, values[1] as u32));
                }
            } else if let Some(alive) = line.strip_prefix("alive ") {
                let keys = ["frame", "lag", "scene", "entrance", "dispcnt"];
                if let Some(values) = fields(alive, &keys) {
                    trace.alive = Some(Alive {
                        frame: values[0],
                        lag: values[1],
                        scene: values[2],
                        entrance: values[3],
                        dispcnt: values[4] as u32,
                    });
                }
            } else if line == "verdict ok=1" {
                trace.verdict = true;
            } else if let Some(error) = line.strip_prefix("gdb-stderr ") {
                trace.errors.push(error.trim().to_owned());
            } else if let Some(exit) = line.strip_prefix("gdb-exit ") {
                trace.exit = Some(exit.to_owned());
            } else if let Some(exit) = line.strip_prefix("emulator-exit ") {
                trace.emulator = Some(exit.to_owned());
            } else if STOPPED.iter().any(|text| line.contains(text)) {
                trace.errors.push(line.trim().to_owned());
            }
        }
        trace
    }

    /// One line for the log: the milestones with their frames, and the
    /// state 600 VBlanks on.
    fn summary(&self) -> String {
        let mut parts: Vec<String> = self
            .milestones
            .iter()
            .map(|text| match Milestone::parse(text) {
                Some(milestone) => format!("{} {}", milestone.name, milestone.frame),
                None => text.clone(),
            })
            .collect();
        if let Some(alive) = self.alive {
            parts.push(format!(
                "{VBLANKS} VBlanks on frame {} lag {}",
                alive.frame, alive.lag
            ));
        }
        if parts.is_empty() {
            "no milestone".to_owned()
        } else {
            parts.join(", ")
        }
    }
}

/// What is wrong with one run on its own.
fn judge(trace: &Trace) -> Vec<String> {
    let mut problems = Vec::new();
    match trace.exit.as_deref() {
        Some("0") => {}
        Some("timeout") => {
            problems.push("the run timed out, and the debugger was stopped".to_owned())
        }
        Some(code) => problems.push(format!("the debugger exited with {code}")),
        None => problems.push("the record holds no exit of the debugger".to_owned()),
    }
    problems.extend(
        trace
            .errors
            .iter()
            .map(|line| format!("the debugger printed: {line}")),
    );
    // The frame of the last milestone that came as expected; the loop ends
    // at the first that did not.
    let mut previous = 0;
    let mut reached = 0;
    for (at, (name, rest)) in MILESTONES.iter().enumerate() {
        let Some(text) = trace.milestones.get(at) else {
            problems.push(match at.checked_sub(1) {
                Some(before) => format!(
                    "milestone {name} never came; the boot stopped after {}",
                    MILESTONES[before].0
                ),
                None => format!("milestone {name} never came; the boot reached no milestone"),
            });
            break;
        };
        let Some(milestone) = Milestone::parse(text) else {
            problems.push(format!(
                "milestone {name} expected, the debugger printed {text:?}"
            ));
            break;
        };
        if milestone.name != *name {
            problems.push(format!(
                "milestone {name} expected, {} came",
                milestone.name
            ));
            break;
        }
        if milestone.rest != *rest {
            problems.push(format!(
                "milestone {name} came with {:?}, not {rest:?}",
                milestone.rest
            ));
        }
        if milestone.frame < previous {
            problems.push(format!(
                "milestone {name} came on frame {}, after frame {previous}",
                milestone.frame
            ));
        }
        previous = milestone.frame;
        reached += 1;
    }
    if reached == MILESTONES.len() {
        // previous is the file menu's frame.
        if previous <= 0 {
            problems.push(format!(
                "the file menu came on frame {previous}: the frame count never ran"
            ));
        }
        for extra in &trace.milestones[MILESTONES.len()..] {
            problems.push(format!("unexpected milestone {extra}"));
        }
        match trace.display {
            Some((dispcnt, _)) if dispcnt & FORCED_BLANK != 0 => problems.push(format!(
                "the file menu is in forced blank, DISPCNT {dispcnt:#x}"
            )),
            Some(_) => {}
            None => problems.push("no DISPCNT or BLDCNT at the file menu".to_owned()),
        }
        match trace.alive {
            None => problems.push(format!("no state {VBLANKS} VBlanks after the file menu")),
            Some(alive) => {
                if alive.lag > LAG_LIMIT {
                    problems.push(format!(
                        "{} lag frames {VBLANKS} VBlanks after the file menu",
                        alive.lag
                    ));
                }
                if alive.dispcnt & FORCED_BLANK != 0 {
                    problems.push(format!(
                        "forced blank {VBLANKS} VBlanks after the file menu, DISPCNT {:#x}",
                        alive.dispcnt
                    ));
                }
            }
        }
    }
    if !trace.verdict {
        problems.push("the command file never reached its verdict".to_owned());
    }
    if !problems.is_empty() {
        if let Some(exit) = &trace.emulator {
            problems.push(format!("the emulator had ended by itself, with {exit}"));
        }
    }
    problems
}

/// What differs between an edition's matching and modern runs: the
/// milestone lines and display registers exactly, the state 600 VBlanks on
/// within bounds.
fn compare(matching: &Trace, modern: &Trace) -> Vec<String> {
    let mut problems = Vec::new();
    if modern.milestones != matching.milestones {
        let at = modern
            .milestones
            .iter()
            .zip(&matching.milestones)
            .position(|(a, b)| a != b)
            .unwrap_or(modern.milestones.len().min(matching.milestones.len()));
        let line = |trace: &Trace| {
            trace
                .milestones
                .get(at)
                .map(|text| format!("milestone {text:?}"))
        };
        problems.push(format!(
            "{} where the matching ROM printed {}",
            line(modern).unwrap_or_else(|| "no milestone".to_owned()),
            line(matching).unwrap_or_else(|| "none".to_owned())
        ));
    }
    if let (Some(a), Some(b)) = (modern.display, matching.display) {
        if a != b {
            problems.push(format!(
                "DISPCNT {:#x} and BLDCNT {:#x} at the file menu, the matching ROM's {:#x} and {:#x}",
                a.0, a.1, b.0, b.1
            ));
        }
    }
    let menu = |trace: &Trace| {
        let text = trace.milestones.get(MILESTONES.len() - 1)?;
        Milestone::parse(text)
    };
    if let (Some(a), Some(b), Some(menu_a), Some(menu_b)) =
        (modern.alive, matching.alive, menu(modern), menu(matching))
    {
        let (advance_a, advance_b) = (a.frame - menu_a.frame, b.frame - menu_b.frame);
        if (advance_a - advance_b).abs() > ADVANCE_TOLERANCE {
            problems.push(format!(
                "{advance_a} frames in the {VBLANKS} VBlanks after the file menu, the matching ROM's {advance_b}"
            ));
        }
        if (a.scene, a.entrance) != (b.scene, b.entrance) {
            problems.push(format!(
                "scene {} entrance {} {VBLANKS} VBlanks after the file menu, the matching ROM's scene {} entrance {}",
                a.scene, a.entrance, b.scene, b.entrance
            ));
        }
    }
    problems
}

/// The palette RAM, VRAM and OAM each run dumped at the file menu, compared
/// byte for byte. A run that dumped none has failed already.
fn compare_display(matching: &Path, modern: &Path) -> Vec<String> {
    DISPLAY
        .iter()
        .filter_map(|(what, file, start, end)| {
            let (a, b) = (
                fs::read(modern.join(file)).ok()?,
                fs::read(matching.join(file)).ok()?,
            );
            differs(what, &a, &b, (end - start) as usize)
        })
        .collect()
}

fn differs(what: &str, modern: &[u8], matching: &[u8], size: usize) -> Option<String> {
    if modern.len() != size || matching.len() != size {
        return Some(format!(
            "{what} dumps of {} and {} bytes, not {size}",
            modern.len(),
            matching.len()
        ));
    }
    let count = modern.iter().zip(matching).filter(|(a, b)| a != b).count();
    (count > 0).then(|| {
        format!(
            "{what} differs from the matching ROM's at the file menu in {count} of {size} bytes"
        )
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    // Recorded runs of tbs-en: trace.txt as `alchemy boot` writes it, less
    // the debugger's lines that give addresses. The illegal instruction is
    // an undefined Thumb instruction (0xde00) planted in the modern ROM over
    // Title_ShowAnimatedSplash's second instruction; the no-emulator run had
    // `true` for its emulator.
    const MATCHING: &str = include_str!("../tests/fixtures/boot/matching.txt");
    const MODERN: &str = include_str!("../tests/fixtures/boot/modern.txt");
    const ILLEGAL: &str = include_str!("../tests/fixtures/boot/illegal-instruction.txt");
    const NO_EMULATOR: &str = include_str!("../tests/fixtures/boot/no-emulator.txt");

    fn problems(record: &str) -> String {
        judge(&Trace::parse(record)).join("\n")
    }

    #[test]
    fn the_recorded_boots_pass() {
        let (matching, modern) = (Trace::parse(MATCHING), Trace::parse(MODERN));
        assert_eq!(judge(&matching), Vec::<String>::new());
        assert_eq!(judge(&modern), Vec::<String>::new());
        assert_eq!(compare(&matching, &modern), Vec::<String>::new());
        assert_eq!(matching.milestones.len(), MILESTONES.len());
        assert!(matching.alive.is_some() && modern.display.is_some());
    }

    #[test]
    fn a_planted_illegal_instruction_fails_after_its_last_milestone() {
        let found = problems(ILLEGAL);
        let stopped = "milestone Event_SetDestination(1, 1) never came; the boot stopped after Title_ShowAnimatedSplash";
        assert!(found.contains(stopped), "{found}");
        assert!(found.contains("SIGILL"), "{found}");
        assert!(found.contains("the debugger exited with 1"), "{found}");
        let found = compare(&Trace::parse(MATCHING), &Trace::parse(ILLEGAL)).join("\n");
        let expected = "no milestone where the matching ROM printed milestone \"Event_SetDestination(1, 1) frame 254\"";
        assert_eq!(found, expected);
    }

    #[test]
    fn a_run_with_no_emulator_fails() {
        let found = problems(NO_EMULATOR);
        assert!(
            found.contains("milestone System_Initialize never came"),
            "{found}"
        );
        assert!(found.contains("the debugger exited with 1"), "{found}");
        assert!(found.contains("the debugger printed: "), "{found}");
        assert!(
            found.contains("the command file never reached its verdict"),
            "{found}"
        );
    }

    #[test]
    fn a_renamed_milestone_fails_by_name() {
        // Missing from an ELF: named before any run.
        let mut names: Vec<&str> = ENTRIES
            .iter()
            .chain(&[FILE_MENU, VBLANK])
            .chain(&VARIABLES)
            .copied()
            .collect();
        assert_eq!(missing_symbols(&names), Vec::<&str>::new());
        names.retain(|name| *name != "Title_ShowSplashScreen");
        assert_eq!(missing_symbols(&names), vec!["Title_ShowSplashScreen"]);
        // Printed under another name.
        let renamed = MATCHING.replace(
            "milestone Title_ShowSplashScreen",
            "milestone Title_ShowLogo",
        );
        let found = problems(&renamed);
        assert!(
            found.contains("milestone Title_ShowSplashScreen expected, Title_ShowLogo came"),
            "{found}"
        );
        // Never printed.
        let dropped: String = MATCHING
            .lines()
            .filter(|line| !line.starts_with("milestone Title_ShowSplashScreen"))
            .map(|line| format!("{line}\n"))
            .collect();
        let found = problems(&dropped);
        assert!(
            found.contains(
                "milestone Title_ShowSplashScreen expected, Title_ShowAnimatedSplash came"
            ),
            "{found}"
        );
        let cut: String = MATCHING
            .lines()
            .take_while(|line| !line.starts_with("milestone file menu"))
            .map(|line| format!("{line}\n"))
            .collect();
        let found = problems(&cut);
        assert!(
            found.contains(
                "milestone file menu never came; the boot stopped after Event_SetDestination(1, 1)"
            ),
            "{found}"
        );
    }

    #[test]
    fn the_modern_rom_keeps_the_matching_frames_and_scene() {
        let matching = Trace::parse(MATCHING);
        let late = MODERN.replace(
            "Title_ShowAnimatedSplash frame 132",
            "Title_ShowAnimatedSplash frame 133",
        );
        assert_ne!(late, MODERN);
        assert_eq!(problems(&late), "");
        let found = compare(&matching, &Trace::parse(&late)).join("\n");
        let expected =
            "milestone \"Title_ShowAnimatedSplash frame 133\" where the matching ROM printed \
                        milestone \"Title_ShowAnimatedSplash frame 132\"";
        assert!(found.contains(expected), "{found}");
        let elsewhere = MODERN.replace("scene 1 entrance 1", "scene 2 entrance 1");
        assert!(
            problems(&elsewhere).contains("milestone file menu came with \"scene 2 entrance 1\"")
        );
        let early = MODERN.replace(
            "Event_SetDestination(1, 1) frame 254",
            "Event_SetDestination(1, 1) frame 4",
        );
        assert!(problems(&early).contains("came on frame 4, after frame 132"));
        let frozen = MATCHING
            .replace("file menu frame 361", "file menu frame 0")
            .replace(" frame 10\n", " frame 0\n")
            .replace(" frame 132\n", " frame 0\n")
            .replace(" frame 254\n", " frame 0\n");
        assert!(
            problems(&frozen).contains("the frame count never ran"),
            "{frozen}"
        );
    }

    #[test]
    fn liveness_has_bounds_not_equality() {
        let matching = Trace::parse(MATCHING);
        let alive = matching.alive.unwrap();
        let line = |frame: i64, lag: i64, dispcnt: u32| {
            format!(
                "alive frame {frame} lag {lag} scene {} entrance {} dispcnt {dispcnt:#x}",
                alive.scene, alive.entrance
            )
        };
        let original = line(alive.frame, alive.lag, alive.dispcnt);
        assert!(MATCHING.contains(&original), "{original}");
        let with = |replacement: String| MATCHING.replace(&original, &replacement);
        let near = with(line(
            alive.frame + ADVANCE_TOLERANCE,
            alive.lag,
            alive.dispcnt,
        ));
        assert_eq!(problems(&near), "");
        assert_eq!(
            compare(&matching, &Trace::parse(&near)),
            Vec::<String>::new()
        );
        let stalled = with(line(alive.frame - 100, alive.lag, alive.dispcnt));
        assert!(compare(&matching, &Trace::parse(&stalled))
            .join("\n")
            .contains("frames in the 600 VBlanks"));
        assert!(
            problems(&with(line(alive.frame, LAG_LIMIT + 1, alive.dispcnt))).contains("lag frames")
        );
        assert!(problems(&with(line(
            alive.frame,
            alive.lag,
            alive.dispcnt | FORCED_BLANK
        )))
        .contains("forced blank"));
        assert!(problems(&with(String::new())).contains("no state 600 VBlanks after the file menu"));
    }

    #[test]
    fn the_display_must_match_byte_for_byte() {
        let size = 0x400;
        let a = vec![0u8; size];
        let mut b = a.clone();
        assert_eq!(differs("OAM", &a, &b, size), None);
        b[7] = 1;
        let expected = "OAM differs from the matching ROM's at the file menu in 1 of 1024 bytes";
        assert_eq!(differs("OAM", &a, &b, size).as_deref(), Some(expected));
        assert!(differs("OAM", &a[..10], &b, size).is_some());
        let directory = tempfile::tempdir().unwrap();
        let (matching, modern) = (
            directory.path().join("matching"),
            directory.path().join("modern"),
        );
        for (path, vram) in [(&matching, 0u8), (&modern, 1u8)] {
            fs::create_dir_all(path).unwrap();
            for (_, file, start, end) in DISPLAY {
                let fill = if file == "menu-vram.bin" { vram } else { 0 };
                fs::write(path.join(file), vec![fill; (end - start) as usize]).unwrap();
            }
        }
        let expected =
            "VRAM differs from the matching ROM's at the file menu in 98304 of 98304 bytes";
        assert_eq!(compare_display(&matching, &modern), vec![expected]);
        let display = MODERN.replace("display dispcnt 0x", "display dispcnt 0x1");
        assert!(compare(&Trace::parse(MATCHING), &Trace::parse(&display))
            .join("\n")
            .contains("at the file menu, the matching ROM's"));
    }

    #[test]
    fn the_command_file_stops_on_errors_and_checks_every_stop() {
        let text = script();
        let lines: Vec<&str> = text.lines().collect();
        // $ok is cleared before the connection and set only at the end.
        let cleared = lines
            .iter()
            .position(|line| *line == "set $ok = 0")
            .unwrap();
        let connected = lines
            .iter()
            .position(|line| line.starts_with("target remote"))
            .unwrap();
        let set = lines
            .iter()
            .position(|line| *line == "set $ok = 1")
            .unwrap();
        assert!(cleared < connected && connected < set);
        assert_eq!(lines.last(), Some(&"quit !$ok"));
        for entry in ENTRIES {
            assert!(text.contains(&format!("dprintf *{entry},")), "{entry}");
        }
        for stop in [FILE_MENU, VBLANK] {
            assert!(text.contains(&format!("break *{stop}\n")), "{stop}");
            let check = format!("if ((unsigned)$pc & ~1) != ((unsigned)&{stop} & ~1)\n");
            assert!(text.contains(&check), "{stop}");
        }
        assert_eq!(text.matches("\ncontinue\n").count(), 2);
        assert_eq!(text.matches("\n  quit 1\n").count(), 2);
        for (_, file, ..) in DISPLAY {
            assert!(
                text.contains(&format!("dump binary memory {file} ")),
                "{file}"
            );
        }
    }

    #[test]
    fn a_run_ends_at_its_timeout() {
        let mut slow = Command::new("sleep")
            .arg("30")
            .stdin(Stdio::null())
            .spawn()
            .unwrap();
        let started = Instant::now();
        assert_eq!(
            wait_until(&mut slow, started + Duration::from_millis(300)).unwrap(),
            None
        );
        assert!(started.elapsed() < Duration::from_secs(5));
        end(&mut slow);
        assert!(slow.try_wait().unwrap().is_some());
        assert_eq!(code(None), "timeout");
        let mut quick = Command::new("true").stdin(Stdio::null()).spawn().unwrap();
        let status = wait_until(&mut quick, Instant::now() + Duration::from_secs(10)).unwrap();
        assert_eq!(code(status), "0");
    }

    #[test]
    fn a_whole_run_without_an_emulator_is_refused() {
        let directory = tempfile::tempdir().unwrap();
        let rom = directory.path().join("tbs-xx.gba");
        let elf = directory.path().join("tbs-xx.elf");
        fs::write(&rom, b"no ROM").unwrap();
        fs::write(&elf, b"no ELF").unwrap();
        let run = directory.path().join("run");
        let options = Options {
            emulator: vec!["true".to_owned()],
            gdb: vec!["false".to_owned()],
            timeout: Duration::from_secs(10),
        };
        if listening(PORT) {
            // An emulator already holds the port, and the run is refused.
            assert!(boot(&run, &rom, &elf, &options)
                .unwrap_err()
                .contains("already listens"));
            return;
        }
        let trace = boot(&run, &rom, &elf, &options).unwrap();
        assert_eq!(trace.exit.as_deref(), Some("1"));
        let found = judge(&trace).join("\n");
        assert!(
            found.contains("milestone System_Initialize never came"),
            "{found}"
        );
        assert!(run.join("boot.gdb").is_file() && run.join("trace.txt").is_file());
        assert!(run.join("settings").is_dir());
        assert_eq!(fs::read(run.join("tbs-xx.gba")).unwrap(), b"no ROM");
        // A second run starts afresh.
        fs::write(run.join("tbs-xx.sav"), b"old save").unwrap();
        boot(&run, &rom, &elf, &options).unwrap();
        assert!(!run.join("tbs-xx.sav").exists());
    }

    #[test]
    fn options_are_read_strictly() {
        assert!(seconds("0").is_err());
        assert!(seconds("ten").is_err());
        assert_eq!(seconds("90"), Ok(Duration::from_secs(90)));
        let words = command("/usr/games/mgba --flag", "emulator").unwrap();
        assert_eq!(words, vec!["/usr/games/mgba", "--flag"]);
        assert!(command("  ", "emulator").is_err());
        let refused = run(&["--target".to_owned(), "tla-en".to_owned()]).unwrap_err();
        assert!(refused.contains("☀️'s editions only"), "{refused}");
        let target = decomp_target(Some("tbs-en")).unwrap();
        let paths = |build: Build| (build.rom(target), build.elf(target));
        assert_eq!(
            paths(Build::Matching),
            (
                PathBuf::from("out/tbs-en/tbs-en.gba"),
                PathBuf::from("out/tbs-en/tbs-en.elf")
            )
        );
        assert_eq!(
            paths(Build::Modern),
            (
                PathBuf::from("out/modern/tbs-en.gba"),
                PathBuf::from("out/modern/tbs-en/tbs-en.elf")
            )
        );
    }
}
