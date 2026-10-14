//! The cartridge header fix-up, devkitPro's gbafix as pret vendors it in
//! tools/gbafix: the Nintendo logo, the fixed byte, the device type, the
//! cleared checksum field and the header's complement check, and, when they
//! are given, the title, game code, maker code and version, with -p padding
//! to a power of two. pret's header source reserves the logo as
//! `.space 156` and leaves the complement 0, and its Makefiles run
//! `gbafix ROM -p --silent` right after objcopy (L1). Each game's
//! `src/system/rom_head.s` does the same, and the Makefile fixes every ROM
//! it links, both games and all twelve editions; objcopy's zero fill pads
//! the image to the cartridge size instead of -p.

use std::fs;
use std::ops::Range;
use std::path::{Path, PathBuf};

const USAGE: &str =
    "usage: gbafix ROM [-p] [-t[TITLE]] [-cCODE] [-mMAKER] [-rVERSION] [--silent]
Fix a GBA ROM's cartridge header in place, as pret's gbafix does: write the
Nintendo logo at 0x04, the fixed byte 0x96 at 0xb2 and device type 0 at
0xb4, clear the checksum field at 0xbe and compute the complement check at
0xbd. A value may follow its option directly or as the next argument.
  -p          pad with 0xff to the next power of two (no minimum size)
  -t[TITLE]   set the 12-byte title; without a value, the file name's stem
  -cCODE      set the four-character game code
  -mMAKER     set the two-character maker code
  -rVERSION   set the version byte (decimal, 0x hexadecimal or 0 octal)
  --silent    print nothing when the ROM is fixed
The Makefile runs it, without -p, on every ROM it links, right
after objcopy.";

/// The cartridge header fills the first 0xc0 bytes of a GBA ROM.
const HEADER_SIZE: usize = 0xc0;
const LOGO_AT: Range<usize> = 0x04..0xa0;
const TITLE: Range<usize> = 0xa0..0xac;
const GAME_CODE: Range<usize> = 0xac..0xb0;
const MAKER_CODE: Range<usize> = 0xb0..0xb2;
const FIXED: usize = 0xb2;
const DEVICE_TYPE: usize = 0xb4;
const VERSION: usize = 0xbc;
const COMPLEMENT: usize = 0xbd;
const CHECKSUM: Range<usize> = 0xbe..0xc0;
const FIXED_VALUE: u8 = 0x96;
const PADDING: u8 = 0xff;

/// The Nintendo logo every GBA cartridge carries at 0x04..0xa0 of its
/// header. The BIOS compares it with its own copy before it starts a
/// cartridge, so every built image needs it. L1 keeps it here and nowhere
/// else in the repository, as pret keeps it only in its vendored gbafix; the
/// publication check rejects it in any other file. All twelve reference ROMs
/// carry these same 156 bytes.
const NINTENDO_LOGO: [u8; 156] = [
    0x24, 0xff, 0xae, 0x51, 0x69, 0x9a, 0xa2, 0x21, 0x3d, 0x84, 0x82, 0x0a, 0x84, 0xe4, 0x09, 0xad,
    0x11, 0x24, 0x8b, 0x98, 0xc0, 0x81, 0x7f, 0x21, 0xa3, 0x52, 0xbe, 0x19, 0x93, 0x09, 0xce, 0x20,
    0x10, 0x46, 0x4a, 0x4a, 0xf8, 0x27, 0x31, 0xec, 0x58, 0xc7, 0xe8, 0x33, 0x82, 0xe3, 0xce, 0xbf,
    0x85, 0xf4, 0xdf, 0x94, 0xce, 0x4b, 0x09, 0xc1, 0x94, 0x56, 0x8a, 0xc0, 0x13, 0x72, 0xa7, 0xfc,
    0x9f, 0x84, 0x4d, 0x73, 0xa3, 0xca, 0x9a, 0x61, 0x58, 0x97, 0xa3, 0x27, 0xfc, 0x03, 0x98, 0x76,
    0x23, 0x1d, 0xc7, 0x61, 0x03, 0x04, 0xae, 0x56, 0xbf, 0x38, 0x84, 0x00, 0x40, 0xa7, 0x0e, 0xfd,
    0xff, 0x52, 0xfe, 0x03, 0x6f, 0x95, 0x30, 0xf1, 0x97, 0xfb, 0xc0, 0x85, 0x60, 0xd6, 0x80, 0x25,
    0xa9, 0x63, 0xbe, 0x03, 0x01, 0x4e, 0x38, 0xe2, 0xf9, 0xa2, 0x34, 0xff, 0xbb, 0x3e, 0x03, 0x44,
    0x78, 0x00, 0x90, 0xcb, 0x88, 0x11, 0x3a, 0x94, 0x65, 0xc0, 0x7c, 0x63, 0x87, 0xf0, 0x3c, 0xaf,
    0xd6, 0x25, 0xe4, 0x8b, 0x38, 0x0a, 0xac, 0x72, 0x21, 0xd4, 0xf8, 0x07,
];

/// The header fields gbafix sets only when it is asked to.
#[derive(Debug, Default, Clone, PartialEq, Eq)]
struct Fields {
    pub title: Option<[u8; 12]>,
    pub game_code: Option<[u8; 4]>,
    pub maker_code: Option<[u8; 2]>,
    pub version: Option<u8>,
}

/// The complement check the BIOS verifies: the bytes 0xa0..=0xbc and this
/// byte, plus 0x19, sum to zero.
fn complement(header: &[u8]) -> u8 {
    let sum = header[TITLE.start..COMPLEMENT]
        .iter()
        .fold(0x19u8, |sum, byte| sum.wrapping_add(*byte));
    0u8.wrapping_sub(sum)
}

/// Fixes `image` as gbafix fixes a ROM file: the header is read from the
/// image as it was, padding (with `pad`) follows the image's own size, and
/// the fixed header is written back over the first 0xc0 bytes, growing an
/// image too short to hold it.
fn fix(image: &mut Vec<u8>, fields: &Fields, pad: bool) {
    let mut header = [0; HEADER_SIZE];
    let read = image.len().min(HEADER_SIZE);
    header[..read].copy_from_slice(&image[..read]);
    header[LOGO_AT].copy_from_slice(&NINTENDO_LOGO);
    header[FIXED] = FIXED_VALUE;
    header[DEVICE_TYPE] = 0;
    if let Some(title) = &fields.title {
        header[TITLE].copy_from_slice(title);
    }
    if let Some(code) = &fields.game_code {
        header[GAME_CODE].copy_from_slice(code);
    }
    if let Some(maker) = &fields.maker_code {
        header[MAKER_CODE].copy_from_slice(maker);
    }
    if let Some(version) = fields.version {
        header[VERSION] = version;
    }
    header[CHECKSUM].fill(0);
    header[COMPLEMENT] = complement(&header);
    if pad && !image.is_empty() && !image.len().is_power_of_two() {
        image.resize(image.len().next_power_of_two(), PADDING);
    }
    if image.len() < HEADER_SIZE {
        image.resize(HEADER_SIZE, 0);
    }
    image[..HEADER_SIZE].copy_from_slice(&header);
}

/// Fixes the ROM file at `path` in place.
fn fix_file(path: &Path, fields: &Fields, pad: bool) -> Result<(), String> {
    let mut image = fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    fix(&mut image, fields, pad);
    fs::write(path, image).map_err(|error| format!("{}: {error}", path.display()))
}

/// A text field as gbafix copies it: its first bytes, zero-filled.
fn field<const N: usize>(text: &str) -> [u8; N] {
    let mut field = [0; N];
    let bytes = text.as_bytes();
    let length = bytes.len().min(N);
    field[..length].copy_from_slice(&bytes[..length]);
    field
}

/// A number as C's strtoul reads it in base 0, kept to its low byte.
fn version(text: &str) -> Result<u8, String> {
    let (digits, radix) = match text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        Some(hex) => (hex, 16),
        None if text.len() > 1 && text.starts_with('0') => (&text[1..], 8),
        None => (text, 10),
    };
    u64::from_str_radix(digits, radix)
        .map(|value| value as u8)
        .map_err(|_| format!("invalid version {text:?}\n{USAGE}"))
}

/// The title gbafix takes from the file name when -t has no value.
fn file_title(rom: &Path) -> String {
    let name = rom
        .file_name()
        .map(|name| name.to_string_lossy().into_owned())
        .unwrap_or_default();
    match name.rsplit_once('.') {
        Some((stem, _)) => stem.to_owned(),
        None => name,
    }
}

#[derive(Debug, PartialEq, Eq)]
struct Request {
    rom: PathBuf,
    fields: Fields,
    pad: bool,
    silent: bool,
}

fn parse(args: &[String]) -> Result<Option<Request>, String> {
    let mut args = args.iter().peekable();
    let rom = match args.next().map(String::as_str) {
        Some("-h" | "--help") => return Ok(None),
        Some(rom) if !rom.starts_with('-') => PathBuf::from(rom),
        _ => return Err(USAGE.into()),
    };
    let mut request = Request {
        rom,
        fields: Fields::default(),
        pad: false,
        silent: false,
    };
    while let Some(arg) = args.next() {
        let option = arg.get(..2).unwrap_or(arg);
        let attached = arg.get(2..).unwrap_or("");
        let mut value = || match (attached, args.peek()) {
            ("", Some(next)) if !next.starts_with('-') => Ok(args.next().unwrap().as_str()),
            ("", _) => Err(format!("{arg} needs a value\n{USAGE}")),
            (attached, _) => Ok(attached),
        };
        match option {
            "-h" if attached.is_empty() => return Ok(None),
            "-p" if attached.is_empty() => request.pad = true,
            "--" if attached == "help" => return Ok(None),
            "--" if attached == "silent" => request.silent = true,
            "-t" => {
                let title = value().map_or_else(|_| file_title(&request.rom), str::to_owned);
                request.fields.title = Some(field(&title));
            }
            "-c" => request.fields.game_code = Some(field(value()?)),
            "-m" => request.fields.maker_code = Some(field(value()?)),
            "-r" => request.fields.version = Some(version(value()?)?),
            _ => return Err(format!("invalid option: {arg}\n{USAGE}")),
        }
    }
    Ok(Some(request))
}

fn run(args: &[String]) -> Result<(), String> {
    let Some(request) = parse(args)? else {
        println!("{USAGE}");
        return Ok(());
    };
    fix_file(&request.rom, &request.fields, request.pad)?;
    if !request.silent {
        println!("ROM fixed!");
    }
    Ok(())
}


fn main() -> std::process::ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match run(&args) {
        Ok(()) => std::process::ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("gbafix: {error}");
            std::process::ExitCode::FAILURE
        }
    }
}
