//! `psynergy editions`: find one function of a linked build in the other
//! editions' ROMs by instruction shape and list its literal-pool words per
//! edition. The output is for people; nothing is stored.

use super::similar::{classify, images_in};
use psynergy::editions::{self, Found};
use psynergy::elf::Elf;
use psynergy::similar::{self, Interner};
use std::fmt::Write as _;
use std::fs;
use std::path::{Path, PathBuf};

pub const USAGE: &str =
    "usage: psynergy editions --build DIR [--image NAME] [--roms DIR | --rom FILE...] FUNCTION\n\
Finds FUNCTION (a symbol or hexadecimal address in DIR's linked images) in\n\
each other edition's ROM, in the cartridge image or a decoded resource, by\n\
its instructions with calls and literal-pool words masked, and prints every\n\
pool word per edition. same: probably a plain constant; moves-with-function:\n\
a pointer into the function's own image; differs: a relocated or link-time\n\
value (scene, resource, message or RAM id) that needs a real name.\n\
  --roms DIR   compare every GAME-*.gba there except DIR's own (default roms)\n\
  --rom FILE   compare this ROM (repeatable; replaces --roms)\n\
  --root DIR   where object paths' sources are looked up (default .)\n\
  --image NAME search only this linked image (an ELF stem such as resource_3cb)";

struct Target {
    name: String,
    address: u32,
    code: Vec<u8>,
}

fn target(build: &Path, root: &Path, image: Option<&str>, query: &str) -> Result<Target, String> {
    let address = u32::from_str_radix(query.trim_start_matches("0x"), 16).ok();
    let mut interner = Interner::default();
    for elf_path in images_in(build)?
        .into_iter()
        .filter(|p| image.is_none_or(|i| p.file_stem().is_some_and(|s| s == i)))
    {
        let data = fs::read(&elf_path).map_err(|e| format!("{}: {e}", elf_path.display()))?;
        let elf = Elf::parse(&data).map_err(|e| format!("{}: {e}", elf_path.display()))?;
        let map_path = elf_path.with_extension("map");
        let map =
            fs::read_to_string(&map_path).map_err(|e| format!("{}: {e}", map_path.display()))?;
        let image = similar::Image {
            build: "",
            elf: &elf,
            map: &map,
        };
        let found = similar::functions(&image, &mut interner, &|o| classify(root, o), &|_| true)
            .into_iter()
            .find(|f| f.name == query || Some(f.address) == address);
        if let Some(f) = found {
            let (section, offset) = elf
                .section_at(f.address)
                .ok_or_else(|| format!("{query}: no section holds {:08x}", f.address))?;
            let end = (offset + f.bytes as usize).min(section.bytes.len());
            return Ok(Target {
                name: f.name,
                address: f.address,
                code: section.bytes[offset..end].to_vec(),
            });
        }
    }
    Err(format!("no function {query} in {}", build.display()))
}

fn edition(path: &Path) -> String {
    path.file_stem()
        .map(|s| s.to_string_lossy().into_owned())
        .unwrap_or_default()
}

pub fn run(arguments: &[String]) -> Result<String, String> {
    let mut image: Option<String> = None;
    let (mut build, mut roms_dir, mut roms, mut root, mut query) = (
        None,
        PathBuf::from("roms"),
        Vec::new(),
        PathBuf::from("."),
        None,
    );
    let mut i = 0;
    while i < arguments.len() {
        let flag = arguments[i].as_str();
        if !flag.starts_with("--") {
            if query.replace(flag.to_string()).is_some() {
                return Err(format!("more than one function given\n{USAGE}"));
            }
            i += 1;
            continue;
        }
        let value = arguments
            .get(i + 1)
            .ok_or_else(|| format!("{flag} needs a value"))?;
        match flag {
            "--build" => build = Some(PathBuf::from(value)),
            "--roms" => roms_dir = PathBuf::from(value),
            "--rom" => roms.push(PathBuf::from(value)),
            "--root" => root = PathBuf::from(value),
            "--image" => image = Some(value.clone()),
            _ => return Err(format!("unknown option {flag}\n{USAGE}")),
        }
        i += 2;
    }
    let build = build.ok_or_else(|| format!("--build is required\n{USAGE}"))?;
    let query = query.ok_or_else(|| format!("give a FUNCTION\n{USAGE}"))?;
    let own = build
        .file_name()
        .map(|n| n.to_string_lossy().into_owned())
        .unwrap_or_default();
    if roms.is_empty() {
        let game = own.split('-').next().unwrap_or("").to_string();
        let entries =
            fs::read_dir(&roms_dir).map_err(|e| format!("{}: {e}", roms_dir.display()))?;
        for entry in entries.flatten() {
            let path = entry.path();
            let stem = edition(&path);
            if path.extension().is_some_and(|e| e == "gba")
                && stem.starts_with(&format!("{game}-"))
                && stem != own
            {
                roms.push(path);
            }
        }
        roms.sort();
    }
    let t = target(&build, &root, image.as_deref(), &query)?;
    let shape = editions::shape(&t.code, t.address);
    let mut columns: Vec<(String, Found)> = vec![(
        own.clone(),
        Found {
            resource: None,
            address: t.address,
            pool: shape
                .pool
                .iter()
                .map(|o| editions::word(&t.code, *o as usize).unwrap_or(0))
                .collect(),
        },
    )];
    // Compare placements in one coordinate system: the build's own ROM, read
    // the same way as the others, when it is there.
    let own_rom = roms_dir.join(format!("{own}.gba"));
    if let Ok(rom) = fs::read(&own_rom) {
        if let Some(f) = editions::locate(&rom, &shape, t.address).first() {
            columns[0].1.address = f.address;
        }
    }
    let mut text = format!(
        "{} {:08x} {} bytes, {} pool words\n",
        t.name,
        t.address,
        t.code.len(),
        shape.pool.len()
    );
    for rom_path in &roms {
        let rom = fs::read(rom_path).map_err(|e| format!("{}: {e}", rom_path.display()))?;
        let found = editions::locate(&rom, &shape, t.address);
        let name = edition(rom_path);
        match found.len() {
            0 => {
                let near: Vec<String> = editions::nearest(&rom, &shape, 6)
                    .into_iter()
                    .take(3)
                    .filter(|(_, score)| *score > 0.0)
                    .map(|(at, score)| {
                        format!("{:08x} ({:.0}%)", 0x0800_0000 + at as u32, score * 100.0)
                    })
                    .collect();
                let hint = if near.is_empty() {
                    String::new()
                } else {
                    format!("; nearest by shared instruction runs: {}", near.join(", "))
                };
                let _ = writeln!(text, "{name}: not found (the shape differs){hint}");
            }
            n => {
                let f = &found[0];
                let place = f
                    .resource
                    .map_or("rom".to_string(), |r| format!("resource {r:03x}"));
                let more = if n > 1 {
                    format!(" (+{} more, first kept)", n - 1)
                } else {
                    String::new()
                };
                let _ = writeln!(text, "{name}: {place} at {:08x}{more}", f.address);
                columns.push((name, f.clone()));
            }
        }
    }
    let _ = write!(text, "offset");
    for (name, _) in &columns {
        let _ = write!(text, "\t{name}");
    }
    let _ = writeln!(text, "\tverdict");
    let addresses: Vec<u32> = columns.iter().map(|(_, f)| f.address).collect();
    for (k, offset) in shape.pool.iter().enumerate() {
        let values: Vec<u32> = columns.iter().map(|(_, f)| f.pool[k]).collect();
        let _ = write!(text, "+{offset:#05x}");
        for v in &values {
            let _ = write!(text, "\t{v:08x}");
        }
        let _ = writeln!(text, "\t{}", editions::verdict(&values, &addresses));
    }
    Ok(text)
}
