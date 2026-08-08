// Where does a code overlay actually start executing?
//
// A code overlay does not begin at offset 0 with real code. It begins with an
// eight-byte veneer -- `ldr r4,[pc,#0]` then `bx r4` -- followed by the absolute
// address of the ENTRY DRIVER. Everything the overlay does is reachable from that
// one function, so if the driver is not owned by C, the overlay is not really
// decompiled no matter how many leaf functions have been matched. This tool
// resolves each overlay's driver and reports who owns it.
//
// Rust port of tools/overlay/overlay_driver.ts. Byte-for-byte stdout parity with
// the TypeScript is the contract; see the PORT NOTEs for the three deliberate
// divergences (image production is bridged, the ownership tables are cached, and
// a self-test failure prints a plain message instead of a JS stack trace).
//
//   overlay-driver resource_380      # one overlay
//   overlay-driver --all             # every overlay, with owner status
//   overlay-driver --all --unowned   # only the drivers nobody owns
//   overlay-driver --self-test

mod json_scan;
mod overlay_image;

use std::collections::HashMap;
use std::path::{Path, PathBuf};

/// `assets/code/*_overlay.s` assembles to a THUMB image based here.
pub const OVERLAY_BASE: i64 = 0x0200_0000;
/// The link script offsets every overlay by this much before the header word is
/// written, so the header word is `OVERLAY_BASE + BASE_SHIFT + offset`.
pub const BASE_SHIFT: i64 = 0x8000;
/// `ldr r4,[pc,#0]` -- the first halfword of the entry veneer.
pub const ENTRY_VENEER_LOAD: u16 = 0x4c00;
/// `bx r4` -- the second halfword of the entry veneer.
pub const ENTRY_VENEER_BRANCH: u16 = 0x4720;

pub fn repo_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("crate lives two levels below the repository root")
        .to_path_buf()
}

fn read_u16_le(image: &[u8], at: usize) -> u16 {
    u16::from_le_bytes([image[at], image[at + 1]])
}

fn read_u32_le(image: &[u8], at: usize) -> u32 {
    u32::from_le_bytes([image[at], image[at + 1], image[at + 2], image[at + 3]])
}

/// Does this image open with the two-instruction entry veneer?
pub fn has_entry_veneer(image: &[u8]) -> bool {
    if image.len() < 8 {
        return false;
    }
    read_u16_le(image, 0) == ENTRY_VENEER_LOAD && read_u16_le(image, 2) == ENTRY_VENEER_BRANCH
}

/// Byte offset of the entry driver, or `None` when the header word does not land
/// inside the image.
pub fn driver_offset(image: &[u8]) -> Option<usize> {
    if image.len() < 8 {
        return None;
    }
    // PORT NOTE: JS reads this as an unsigned 32-bit value into a float64, so the
    // arithmetic below never wraps. i64 reproduces that; `as i32` would not.
    let word = read_u32_le(image, 4) as i64;
    let offset = word - (word & 1) - BASE_SHIFT - OVERLAY_BASE;
    if offset < 0 || offset + 2 > image.len() as i64 {
        return None;
    }
    Some(offset as usize)
}

/// Every reconstructed code overlay, by name, in sorted order.
pub fn overlay_names(root: &Path) -> Vec<String> {
    let directory = root.join("assets").join("code");
    let mut names: Vec<String> = std::fs::read_dir(&directory)
        .into_iter()
        .flatten()
        .flatten()
        .filter_map(|entry| {
            // PORT NOTE: readdirSync yields OS strings; the TypeScript regex would
            // see U+FFFD for undecodable bytes and reject them, and so do we.
            let name = entry.file_name().to_string_lossy().into_owned();
            is_overlay_source(&name).then(|| name.replace("_overlay.s", ""))
        })
        .collect();
    // PORT NOTE: `Array.prototype.sort` compares UTF-16 code units. Every name
    // here is `resource_` plus lowercase hex, so a byte sort is identical.
    names.sort();
    names
}

/// `/^resource_[0-9a-f]+_overlay\.s$/`, hand-rolled to keep the crate dependency-free.
fn is_overlay_source(name: &str) -> bool {
    let Some(rest) = name.strip_prefix("resource_") else {
        return false;
    };
    let Some(hex) = rest.strip_suffix("_overlay.s") else {
        return false;
    };
    !hex.is_empty() && hex.bytes().all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b))
}

/// `/^resource_[0-9a-f]+$/` -- the argument form.
fn is_overlay_argument(name: &str) -> bool {
    match name.strip_prefix("resource_") {
        Some(hex) => {
            !hex.is_empty() && hex.bytes().all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b))
        }
        None => false,
    }
}

/// The ownership tables, read at most once per process.
///
/// PORT NOTE: the TypeScript re-reads and re-parses `semantic/regions.json` (1 MB)
/// and `out/decomp/overlays.json` (84 MB) inside `ownerOf`, i.e. once per overlay.
/// Caching them changes runtime only -- neither file is written during a run, so
/// every answer is identical. This is the whole reason `--all` drops from ~21 s to
/// under two seconds.
#[derive(Default)]
struct Owners {
    manual: Option<HashMap<String, Vec<String>>>,
    inventory: Option<Option<HashMap<String, Vec<i64>>>>,
}

impl Owners {
    fn manual_regions(&mut self, root: &Path) -> &HashMap<String, Vec<String>> {
        self.manual.get_or_insert_with(|| {
            let bytes = std::fs::read(root.join("semantic").join("regions.json")).unwrap_or_default();
            let mut table: HashMap<String, Vec<String>> = HashMap::new();
            for record in json_scan::scan_records(&bytes, "manual_regions", "entry", "offset") {
                if let (Some(overlay), Some(entry)) = (record.overlay, record.text) {
                    table.entry(overlay).or_default().push(entry);
                }
            }
            table
        })
    }

    fn inventory(&mut self, root: &Path) -> Option<&HashMap<String, Vec<i64>>> {
        self.inventory
            .get_or_insert_with(|| {
                let path = root.join("out").join("decomp").join("overlays.json");
                if !path.exists() {
                    return None;
                }
                let bytes = std::fs::read(&path).unwrap_or_default();
                let mut table: HashMap<String, Vec<i64>> = HashMap::new();
                for record in json_scan::scan_records(&bytes, "functions", "entry", "offset") {
                    if let (Some(overlay), Some(offset)) = (record.overlay, record.number) {
                        table.entry(overlay).or_default().push(offset);
                    }
                }
                Some(table)
            })
            .as_ref()
    }

    /// Who owns the driver at `offset` inside `overlay`? The three sources are
    /// checked in decreasing order of strength: a hand-written semantic region, a
    /// byte-exact C reconstruction, then a bare inventory row.
    fn owner_of(&mut self, root: &Path, overlay: &str, offset: usize) -> &'static str {
        let address = format!("0x0{:x}", OVERLAY_BASE + offset as i64);
        if self
            .manual_regions(root)
            .get(overlay)
            .is_some_and(|entries| entries.contains(&address))
        {
            return "semantic-C";
        }
        // `address` carries the "0x" prefix; the exact-C filenames do not.
        let exact = root
            .join("exact")
            .join(format!("{overlay}_c_{}.c", &address[2..]));
        if exact.exists() {
            return "exact-C";
        }
        if self
            .inventory(root)
            .and_then(|rows| rows.get(overlay))
            .is_some_and(|offsets| offsets.contains(&(offset as i64)))
        {
            return "inventory row";
        }
        "UNOWNED"
    }
}

fn main() {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    let root = repo_root();
    if argv.iter().any(|a| a == "--self-test") {
        // PORT NOTE: an assertion failure in the TypeScript escapes `main` as an
        // uncaught exception (stack trace on stderr, exit 1). We print the message
        // and exit 1; the exit code matches, the stderr text does not.
        match self_test(&root) {
            Ok(()) => return,
            Err(message) => {
                eprintln!("{message}");
                std::process::exit(1);
            }
        }
    }
    std::process::exit(run(&root, &argv, &mut |line| println!("{line}")));
}

/// The body of `main`, with output captured so the self-test can drive it.
/// Returns the process exit code.
fn run(root: &Path, argv: &[String], out: &mut dyn FnMut(&str)) -> i32 {
    let want_all = argv.iter().any(|a| a == "--all");
    let only_unowned = argv.iter().any(|a| a == "--unowned");
    let names: Vec<String> = if want_all {
        overlay_names(root)
    } else {
        argv.iter()
            .filter(|a| is_overlay_argument(a))
            .cloned()
            .collect()
    };
    if names.is_empty() {
        out("usage: overlay-driver <resource_NNN> | --all [--unowned]");
        return 1;
    }

    let images = overlay_image::overlay_images(root, &names);
    let mut owners = Owners::default();
    let mut unowned = 0usize;
    let mut examined = 0usize;
    let mut missing: Vec<&str> = Vec::new();
    let mut code = 0;

    for (overlay, image) in names.iter().zip(images) {
        let Ok(image) = image else {
            missing.push(overlay);
            continue;
        };
        let Some(offset) = driver_offset(&image) else {
            out(&format!("  {overlay}  header word out of range"));
            continue;
        };
        examined += 1;
        let owner = owners.owner_of(root, overlay, offset);
        if owner == "UNOWNED" {
            unowned += 1;
        }
        if only_unowned && owner != "UNOWNED" {
            continue;
        }
        let head = read_u16_le(&image, offset);
        let note = if head & 0xfe00 == 0xb400 {
            ""
        } else {
            "  (does not open with push)"
        };
        let address = format!("{:x}", OVERLAY_BASE + offset as i64);
        out(&format!("  {overlay}  driver 0x{address}  {owner}{note}"));
    }

    if want_all {
        out(&format!(
            "\nentry drivers examined={examined} unowned={unowned}"
        ));
    }
    if !missing.is_empty() {
        out(&format!(
            "NO CODE-OVERLAY IMAGE — this is a FAILURE, not a pass: {}\n\
             \x20 Check the name against assets/code/*_overlay.s. Nothing was examined for\n\
             \x20 these, and `unowned=0` here does NOT mean their drivers are owned.",
            missing.join(" ")
        ));
        code = 1;
    }
    // A run that examined nothing cannot have proven anything. Never exit 0.
    if examined == 0 {
        code = 1;
    }
    code
}

fn self_test(root: &Path) -> Result<(), String> {
    fn check(condition: bool, message: &str) -> Result<(), String> {
        if condition {
            Ok(())
        } else {
            Err(format!("self-test failed: {message}"))
        }
    }

    let mut image = vec![0u8; 0x80];
    image[0..2].copy_from_slice(&ENTRY_VENEER_LOAD.to_le_bytes());
    image[2..4].copy_from_slice(&ENTRY_VENEER_BRANCH.to_le_bytes());
    let entry = (OVERLAY_BASE + BASE_SHIFT + 0x40) as u32;
    image[4..8].copy_from_slice(&(entry | 1).to_le_bytes());

    check(has_entry_veneer(&image), "veneer not recognised")?;
    check(
        driver_offset(&image) == Some(0x40),
        "thumb-bit driver offset",
    )?;
    image[4..8].copy_from_slice(&entry.to_le_bytes());
    check(
        driver_offset(&image) == Some(0x40),
        "even driver offset",
    )?;
    image[4..8].copy_from_slice(&((OVERLAY_BASE + BASE_SHIFT + 0x4000) as u32).to_le_bytes());
    check(
        driver_offset(&image).is_none(),
        "out-of-range word must not resolve",
    )?;
    check(
        driver_offset(&[0u8; 4]).is_none(),
        "a four-byte image has no header word",
    )?;

    let mut owners = Owners::default();
    check(
        owners.owner_of(root, "resource_3c9", 0x71c) == "exact-C",
        "known exact-C driver lost its owner",
    )?;

    // The refusal path matters more than the happy path: an unknown overlay name
    // must be a hard failure, not a quiet `examined=0` pass.
    let self_path = std::env::current_exe().map_err(|e| e.to_string())?;
    let status = |name: &str| -> i32 {
        std::process::Command::new(&self_path)
            .arg(name)
            .stdout(std::process::Stdio::null())
            .stderr(std::process::Stdio::null())
            .status()
            .ok()
            .and_then(|s| s.code())
            .unwrap_or(-1)
    };
    check(
        status("resource_ffffff") != 0,
        "unknown code overlay must fail",
    )?;
    let first = overlay_names(root);
    let first = first.first().ok_or("no code overlays on disk")?;
    check(status(first) == 0, "a real code overlay must succeed")?;

    println!("code-overlay driver self-test passed (including unknown-code-overlay refusal)");
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn synthetic(entry_word: u32, length: usize) -> Vec<u8> {
        let mut image = vec![0u8; length];
        image[0..2].copy_from_slice(&ENTRY_VENEER_LOAD.to_le_bytes());
        image[2..4].copy_from_slice(&ENTRY_VENEER_BRANCH.to_le_bytes());
        image[4..8].copy_from_slice(&entry_word.to_le_bytes());
        image
    }

    #[test]
    fn the_veneer_needs_both_halfwords_and_eight_bytes() {
        let image = synthetic((OVERLAY_BASE + BASE_SHIFT) as u32, 0x20);
        assert!(has_entry_veneer(&image));
        assert!(!has_entry_veneer(&image[..7]));
        let mut wrong = image.clone();
        wrong[2] = 0x21;
        assert!(!has_entry_veneer(&wrong));
    }

    #[test]
    fn the_thumb_bit_is_stripped_not_subtracted_blindly() {
        let even = synthetic((OVERLAY_BASE + BASE_SHIFT + 0x40) as u32, 0x80);
        let odd = synthetic((OVERLAY_BASE + BASE_SHIFT + 0x41) as u32, 0x80);
        assert_eq!(driver_offset(&even), Some(0x40));
        assert_eq!(driver_offset(&odd), Some(0x40));
    }

    #[test]
    fn a_word_landing_outside_the_image_resolves_to_nothing() {
        assert_eq!(driver_offset(&synthetic(0, 0x80)), None);
        // The last legal offset needs two bytes of halfword to read. 0x7f is not
        // the first rejection: the thumb bit is stripped first, so it lands on
        // 0x7e and is accepted. 0x80 is the first offset with no room.
        assert_eq!(
            driver_offset(&synthetic((OVERLAY_BASE + BASE_SHIFT + 0x7e) as u32, 0x80)),
            Some(0x7e)
        );
        assert_eq!(
            driver_offset(&synthetic((OVERLAY_BASE + BASE_SHIFT + 0x7f) as u32, 0x80)),
            Some(0x7e)
        );
        assert_eq!(
            driver_offset(&synthetic((OVERLAY_BASE + BASE_SHIFT + 0x80) as u32, 0x80)),
            None
        );
        assert_eq!(driver_offset(&[0u8; 4]), None);
    }

    #[test]
    fn a_header_word_above_two_gigabytes_stays_positive() {
        // JS reads the word with readUInt32LE into a float64, so 0xf0000000 is
        // +4026531840 and the offset is a huge positive number -- rejected by the
        // length check, not by the `offset < 0` check. `as i32` would have made it
        // negative and taken the other branch; the answer is the same but the
        // reasoning must not rely on wrapping.
        assert_eq!(driver_offset(&synthetic(0xf000_0000, 0x80)), None);
    }

    #[test]
    fn overlay_name_patterns_are_anchored_and_hex_only() {
        assert!(is_overlay_source("resource_3c9_overlay.s"));
        assert!(!is_overlay_source("resource_3g9_overlay.s"));
        assert!(!is_overlay_source("resource__overlay.s"));
        assert!(!is_overlay_source("xresource_3c9_overlay.s"));
        assert!(!is_overlay_source("resource_3c9_overlay.s.bak"));
        // PORT NOTE: JS regexes are unanchored by default; these two carry
        // explicit ^ and $, so the Rust must reject prefixes and suffixes too.
        assert!(is_overlay_argument("resource_380"));
        assert!(!is_overlay_argument("--all"));
        assert!(!is_overlay_argument("resource_380x"));
        assert!(!is_overlay_argument("resource_3C9"));
    }

    #[test]
    fn the_push_note_matches_the_prologue_mask() {
        let push = |head: u16| head & 0xfe00 == 0xb400;
        assert!(push(0xb500)); // push {lr}
        assert!(push(0xb4f0)); // push {r4-r7}
        assert!(!push(0xb000)); // add sp, #imm
        assert!(!push(0xbd00)); // pop {pc}
    }

    #[test]
    fn the_owner_address_carries_a_padded_prefix_but_the_printed_one_does_not() {
        // ownerOf builds "0x0" + hex; the driver line builds "0x" + hex. The two
        // differ by exactly one zero and both forms are load-bearing (the exact-C
        // filename uses the padded one).
        let offset = 0x3f24usize;
        assert_eq!(format!("0x0{:x}", OVERLAY_BASE + offset as i64), "0x02003f24");
        assert_eq!(format!("0x{:x}", OVERLAY_BASE + offset as i64), "0x2003f24");
    }

    #[test]
    fn no_arguments_is_a_usage_message_and_a_failing_exit_code() {
        let mut lines = Vec::new();
        let code = run(&repo_root(), &[], &mut |line| lines.push(line.to_string()));
        assert_eq!(code, 1);
        assert_eq!(
            lines,
            ["usage: overlay-driver <resource_NNN> | --all [--unowned]"]
        );
    }

    #[test]
    fn an_unknown_overlay_is_a_hard_failure_not_an_empty_pass() {
        let mut lines = Vec::new();
        let code = run(&repo_root(), &["resource_ffffff".into()], &mut |line| {
            lines.push(line.to_string())
        });
        assert_eq!(code, 1);
        assert_eq!(lines.len(), 1);
        assert!(lines[0].starts_with("NO CODE-OVERLAY IMAGE"));
        assert!(lines[0].contains("resource_ffffff"));
    }
}
