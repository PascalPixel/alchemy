//! Dry run of the merged-unit hypothesis: a proposed unit's C files joined
//! into one temporary source in ROM order (includes once, duplicate
//! declarations once), compiled with the game's own steps, and its sections
//! compared with today's objects as the build placed them: the code with the
//! unit's code, and its .rodata and .data with today's objects' own pieces
//! of those sections, concatenated in the same order. Nothing under games/ or
//! a linker script is touched; the sources and objects stay in
//! out/EDITION/obj/units.

use super::load::{Loaded, Piece};
use super::model::{Image, Lang, Unit};
use super::Judged;
use crate::targets::DecompTarget;
use object::{Object, ObjectSection, ObjectSymbol, RelocationFlags, RelocationTarget};
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write as _;
use std::fs;
use std::path::{Path, PathBuf};

/// The C source of one of today's objects.
fn source_of(root: &Path, object: &str) -> Option<PathBuf> {
    let path = root.join(format!("{}.c", object.trim_end_matches(".o")));
    path.exists().then_some(path)
}

/// Top-level items of a C file: preprocessor lines, declarations ending in
/// `;` and definitions ending in a closing brace at depth zero. Comments and
/// strings are skipped while counting braces.
pub fn items(text: &str) -> Vec<String> {
    let bytes = text.as_bytes();
    let mut out = Vec::new();
    let mut start = 0;
    let mut depth = 0i32;
    let mut i = 0;
    let mut line_start = true;
    let mut body = false;
    while i < bytes.len() {
        let c = bytes[i];
        if line_start && depth == 0 && c == b'#' && text[start..i].trim().is_empty() {
            let mut end = i;
            loop {
                let next = text[end..].find('\n').map_or(text.len(), |n| end + n);
                if next > 0 && bytes[next - 1] == b'\\' && next < text.len() {
                    end = next + 1;
                    continue;
                }
                end = next;
                break;
            }
            push(&mut out, &text[start..end]);
            start = end;
            i = end;
            continue;
        }
        if !c.is_ascii_whitespace() {
            line_start = false;
        }
        match c {
            b'\n' => line_start = true,
            b'/' if bytes.get(i + 1) == Some(&b'/') => {
                i = text[i..].find('\n').map_or(text.len(), |n| i + n);
                continue;
            }
            b'/' if bytes.get(i + 1) == Some(&b'*') => {
                i = text[i + 2..].find("*/").map_or(text.len(), |n| i + 2 + n + 2);
                continue;
            }
            b'"' | b'\'' => {
                let mut j = i + 1;
                while j < bytes.len() && bytes[j] != c {
                    j += if bytes[j] == b'\\' { 2 } else { 1 };
                }
                i = j + 1;
                continue;
            }
            b'{' => {
                if depth == 0 {
                    body = text[start..i].trim_end().ends_with(')');
                }
                depth += 1;
            }
            b'}' => {
                depth -= 1;
                if depth == 0 && body {
                    push(&mut out, &text[start..=i]);
                    start = i + 1;
                }
            }
            b';' if depth == 0 => {
                push(&mut out, &text[start..=i]);
                start = i + 1;
            }
            _ => {}
        }
        i += 1;
    }
    push(&mut out, &text[start..]);
    out
}

fn push(out: &mut Vec<String>, item: &str) {
    if !item.trim().is_empty() {
        out.push(item.to_string());
    }
}

fn normalised(item: &str) -> String {
    item.split_whitespace().collect::<Vec<_>>().join(" ")
}

/// Is the item a definition with a body (a function), never dropped?
fn is_definition(item: &str) -> bool {
    let trimmed = item.trim_end();
    trimmed.ends_with('}') && !trimmed.ends_with("};")
}

/// One merged source from several, in order: includes once (local ones
/// made absolute), any repeated declaration, typedef or macro once.
pub fn merge(sources: &[(PathBuf, String)]) -> String {
    let mut seen: BTreeSet<String> = BTreeSet::new();
    let mut out = String::from("/* Dry run of a proposed translation unit: analysis only, never source. */\n");
    for (path, text) in sources {
        let _ = writeln!(out, "\n/* ---- {} ---- */", path.file_name().map(|n| n.to_string_lossy()).unwrap_or_default());
        let directory = path.parent().unwrap_or(Path::new("."));
        for item in items(text) {
            let mut item = item;
            let trimmed = item.trim_start();
            if let Some(rest) = trimmed.strip_prefix("#include") {
                let name = rest.trim();
                if let Some(local) = name.strip_prefix('"').and_then(|n| n.split('"').next()) {
                    let beside = directory.join(local);
                    if beside.exists() {
                        let leading = &item[..item.len() - trimmed.len()];
                        item = format!("{leading}#include \"{}\"", beside.display());
                    }
                }
            }
            let key = normalised(&strip_comments(&item));
            let directive = key.starts_with('#') && !key.starts_with("#include") && !key.starts_with("#define") && !key.starts_with("#undef");
            if !directive && !is_definition(&item) && !key.is_empty() && !seen.insert(key) {
                continue;
            }
            out.push_str(&item);
        }
        out.push('\n');
    }
    out
}

fn strip_comments(text: &str) -> String {
    let mut out = String::new();
    let mut rest = text;
    while !rest.is_empty() {
        if let Some(i) = rest.find("/*") {
            let j = rest.find("//").filter(|&j| j < i);
            if let Some(j) = j {
                out.push_str(&rest[..j]);
                rest = rest[j..].find('\n').map_or("", |n| &rest[j + n..]);
                continue;
            }
            out.push_str(&rest[..i]);
            rest = rest[i + 2..].find("*/").map_or("", |n| &rest[i + 2 + n + 2..]);
        } else if let Some(j) = rest.find("//") {
            out.push_str(&rest[..j]);
            rest = rest[j..].find('\n').map_or("", |n| &rest[j + n..]);
        } else {
            out.push_str(rest);
            break;
        }
    }
    out
}

/// Where each byte of a merged section belongs: today's pieces of that
/// section, in order, each at its own alignment.
struct Layout {
    spans: Vec<(u32, u32, u32)>, // merged offset, address, size
}

impl Layout {
    fn new(pieces: &[&Piece], align: u32) -> Layout {
        let mut spans = Vec::new();
        let mut offset = 0;
        for p in pieces {
            offset = (offset + align - 1) & !(align - 1);
            spans.push((offset, p.address, p.size));
            offset += p.size;
        }
        Layout { spans }
    }

    fn size(&self) -> u32 {
        self.spans.last().map_or(0, |(o, _, s)| o + s)
    }

    fn address(&self, offset: u32) -> Option<u32> {
        self.spans.iter().find(|(o, _, s)| offset >= *o && offset <= o + s).map(|(o, a, _)| a + (offset - o))
    }
}

/// A byte of the image against the merged object's: a place the image
/// holds no bytes for (a NOLOAD section, such as an overlay's .data kept
/// with its bss) has nothing to judge.
fn differs(expected: Option<u8>, got: Option<u8>) -> bool {
    expected.is_some() && got != expected
}

pub struct Outcome {
    pub verdict: String,
    pub detail: String,
}

fn compare(root: &Path, target: DecompTarget, loaded: &Loaded, main: &Loaded, unit: &Unit, id: &str) -> Outcome {
    let image = &loaded.image;
    let objects = unit.objects(image);
    let mut sources = Vec::new();
    for object in &objects {
        let Some(path) = source_of(root, object) else {
            return Outcome { verdict: "no-source".into(), detail: format!("{object} has no C source") };
        };
        let text = fs::read(&path).map(|b| String::from_utf8_lossy(&b).into_owned()).unwrap_or_default();
        sources.push((path, text));
    }
    let family = crate::compiler::routing::family_for_source(target.compiler, &sources[0].0.to_string_lossy());
    compare_source(root, target, loaded, main, &objects, &merge(&sources), id, family)
}

/// A merged source compiled with the edition's own steps, its sections compared
/// with `objects`' pieces as today's build placed them in `loaded`. The
/// merged copy lives under out/, so it takes the compiler its sources route to.
#[allow(clippy::too_many_arguments)]
pub fn compare_source(root: &Path, target: DecompTarget, loaded: &Loaded, main: &Loaded, objects: &[&str], text: &str, id: &str, family: crate::compiler::routing::CompilerFamily) -> Outcome {
    let image = &loaded.image;
    let fail = |verdict: &str, detail: String| Outcome { verdict: verdict.to_string(), detail };
    let directory = root.join(target.output_dir).join("obj/units");
    if let Err(e) = fs::create_dir_all(&directory) {
        return fail("error", e.to_string());
    }
    let source = directory.join(format!("{id}.c"));
    let object = directory.join(format!("{id}.o"));
    if let Err(e) = fs::write(&source, text) {
        return fail("error", e.to_string());
    }
    let steps = match crate::build_rom::c_steps_routed(target, &source, &object, Some(family)) {
        Ok(steps) => steps,
        Err(e) => return fail("error", e),
    };
    let numbered = regex::Regex::new(r":\d+: ").expect("pattern");
    for step in &steps {
        if let Err(e) = crate::process::run(step, root) {
            let _ = fs::write(directory.join(format!("{id}.log")), &e);
            let first = e
                .lines()
                .filter(|l| numbered.is_match(l) && !l.contains("warning"))
                .take(2)
                .map(|l| l.rsplit_once(".C:").map_or(l, |(_, tail)| tail).to_string())
                .collect::<Vec<_>>()
                .join(" / ");
            return fail("compile-error", if first.is_empty() { e.lines().next().unwrap_or("").to_string() } else { first });
        }
    }
    let Ok(bytes) = fs::read(&object) else {
        return fail("error", "no object".into());
    };
    let Ok(file) = object::File::parse(&*bytes) else {
        return fail("error", "unreadable object".into());
    };
    // Today's pieces of each section, in the unit's object order.
    let mut layouts: BTreeMap<String, Layout> = BTreeMap::new();
    for name in [".text", ".rodata", ".data"] {
        let pieces: Vec<&Piece> = objects
            .iter()
            .flat_map(|o| loaded.pieces.iter().filter(move |p| p.object == *o && p.section == name && p.size > 0))
            .collect();
        layouts.insert(name.to_string(), Layout::new(&pieces, 4));
    }
    let symbol_address = |index: object::SymbolIndex| -> Option<u32> {
        let symbol = file.symbol_by_index(index).ok()?;
        if symbol.is_undefined() {
            let name = symbol.name().ok()?;
            let find = |l: &Loaded| l.elf.symbols.iter().find(|s| s.name == name && s.section != 0).map(|s| s.value);
            return find(loaded).or_else(|| find(main));
        }
        let section = file.section_by_index(symbol.section_index()?).ok()?;
        let layout = layouts.get(section.name().ok()?)?;
        let thumb = u32::from(matches!(symbol.flags(), object::SymbolFlags::Elf { st_info, .. } if st_info & 0xf == 13 || st_info & 0xf == 15 || (st_info & 0xf == 2 && symbol.address() & 1 == 1)));
        layout.address(symbol.address() as u32).map(|a| a | thumb)
    };
    let mut report = Vec::new();
    let mut matched = true;
    for section in file.sections() {
        let name = section.name().unwrap_or("");
        let Some(layout) = layouts.get(name) else {
            if section.size() > 0 && (name.starts_with(".rodata") || name.starts_with(".data") || name.starts_with(".text")) {
                report.push(format!("{name} {}B has no counterpart today", section.size()));
                matched = false;
            }
            continue;
        };
        let mut data = section.data().unwrap_or(&[]).to_vec();
        let expected_size = layout.size();
        if data.len() as u32 != expected_size && !(data.len() as u32 == (expected_size + 3) & !3 && name == ".text") {
            report.push(format!("{name} {}B, today {}B", data.len(), expected_size));
            matched = false;
        }
        let mut masked = 0;
        let mut unresolved = BTreeSet::new();
        let mut hidden: BTreeSet<u32> = BTreeSet::new();
        for (offset, relocation) in section.relocations() {
            let offset = offset as usize;
            let RelocationFlags::Elf { r_type } = relocation.flags() else {
                continue;
            };
            // A section symbol (a static's place) carries its offset in the
            // addend, which the merged layout moves with the piece it lands in.
            let section_symbol = match relocation.target() {
                RelocationTarget::Symbol(index) => file.symbol_by_index(index).ok().filter(|s| s.kind() == object::SymbolKind::Section).and_then(|s| s.section_index()).and_then(|i| file.section_by_index(i).ok()).and_then(|s| s.name().ok().map(str::to_string)),
                _ => None,
            };
            if let (Some(target_section), Some(slot)) = (section_symbol, data.get(offset..offset + 4)) {
                let resolved = match r_type {
                    object::elf::R_ARM_ABS32 => {
                        let addend = u32::from_le_bytes([slot[0], slot[1], slot[2], slot[3]]);
                        layouts.get(&target_section).and_then(|l| l.address(addend)).map(|a| a.to_le_bytes().to_vec())
                    }
                    object::elf::R_ARM_THM_PC22 if target_section == name => {
                        let (high, low) = (u16::from_le_bytes([slot[0], slot[1]]), u16::from_le_bytes([slot[2], slot[3]]));
                        let field = (((u32::from(high & 0x7ff) << 12) | (u32::from(low & 0x7ff) << 1)) << 9) as i32 >> 9;
                        let at = (4 + i64::from(field)) as u32;
                        match (layout.address(at), layout.address(offset as u32)) {
                            (Some(target), Some(place)) => {
                                let delta = target.wrapping_sub(place + 4) as i32 >> 1;
                                let high = 0xf000 | ((delta >> 11) & 0x7ff) as u16;
                                let low = 0xf800 | (delta & 0x7ff) as u16;
                                Some([high.to_le_bytes(), low.to_le_bytes()].concat())
                            }
                            _ => None,
                        }
                    }
                    _ => None,
                };
                match resolved {
                    Some(bytes) => data[offset..offset + 4].copy_from_slice(&bytes),
                    None => {
                        masked += 1;
                        hidden.extend(offset as u32..offset as u32 + 4);
                    }
                }
                continue;
            }
            let target = match relocation.target() {
                RelocationTarget::Symbol(index) => symbol_address(index),
                _ => None,
            };
            let place = layout.address(offset as u32);
            let (Some(target), Some(place), Some(slot)) = (target, place, data.get_mut(offset..offset + 4)) else {
                if let RelocationTarget::Symbol(index) = relocation.target() {
                    unresolved.insert(file.symbol_by_index(index).ok().and_then(|s| s.name().ok().map(str::to_string)).unwrap_or_default());
                }
                masked += 1;
                hidden.extend(offset as u32..offset as u32 + 4);
                continue;
            };
            match r_type {
                object::elf::R_ARM_ABS32 => {
                    let addend = u32::from_le_bytes([slot[0], slot[1], slot[2], slot[3]]);
                    slot.copy_from_slice(&target.wrapping_add(addend).to_le_bytes());
                }
                object::elf::R_ARM_THM_PC22 => {
                    let delta = (target & !1).wrapping_sub(place + 4) as i32 >> 1;
                    let high = 0xf000 | ((delta >> 11) & 0x7ff) as u16;
                    let low = 0xf800 | (delta & 0x7ff) as u16;
                    slot[..2].copy_from_slice(&high.to_le_bytes());
                    slot[2..].copy_from_slice(&low.to_le_bytes());
                }
                _ => {
                    masked += 1;
                    hidden.extend(offset as u32..offset as u32 + 4);
                }
            }
        }
        // The bytes as today's build placed them.
        let mut differ = 0;
        let mut first: Option<u32> = None;
        for (o, a, s) in &layout.spans {
            for k in 0..*s {
                let expected = loaded.elf.section_at(a + k).and_then(|(sec, at)| sec.bytes.get(at).copied());
                let got = if hidden.contains(&(o + k)) { expected } else { data.get((o + k) as usize).copied() };
                if differs(expected, got) {
                    differ += 1;
                    first.get_or_insert(a + k);
                }
            }
        }
        if layout.spans.windows(2).any(|w| w[1].1 > ((w[0].1 + w[0].2 + 3) & !3)) {
            report.push(format!("{name} pieces lie apart in the image"));
        }
        if differ > 0 {
            matched = false;
            let at = first.unwrap_or(0);
            let function = image.functions.iter().find(|f| f.start <= at && at < f.end.max(f.start + 4)).map_or_else(String::new, |f| format!(" in {}", f.name));
            report.push(format!("{name} {differ}B differ, first at {at:08x}{function}"));
        } else {
            report.push(format!("{name} {}B identical", data.len()));
        }
        if masked > 0 {
            report.push(format!("{name} {masked} relocations masked{}", if unresolved.is_empty() { String::new() } else { format!(" ({})", unresolved.into_iter().take(3).collect::<Vec<_>>().join(",")) }));
        }
    }
    Outcome { verdict: if matched { "pass".into() } else { "differ".into() }, detail: report.join("; ") }
}

/// The units to try: named ones, or the N best-supported units made of
/// several whole C objects, at least one from an overlay when one qualifies.
fn choose<'a>(judged: &'a [Judged], choice: &str, root: &Path) -> Result<Vec<(&'a Judged, usize)>, String> {
    if let Ok(n) = choice.parse::<usize>() {
        let mut candidates: Vec<(f64, &Judged, usize)> = Vec::new();
        for j in judged {
            let image = &j.loaded.image;
            for (k, u) in j.units.iter().enumerate() {
                let objects = u.objects(image);
                let functions = &image.functions[u.first..=u.last];
                let whole = objects.iter().all(|o| image.functions.iter().enumerate().all(|(i, f)| f.object != *o || (u.first <= i && i <= u.last)));
                if objects.len() >= 2
                    && objects.len() <= 8
                    && whole
                    && functions.iter().all(|f| f.lang == Lang::C)
                    && objects.iter().all(|o| source_of(root, o).is_some())
                {
                    candidates.push((u.confidence, j, k));
                }
            }
        }
        candidates.sort_by(|a, b| b.0.total_cmp(&a.0));
        let mut picked: Vec<(&Judged, usize)> = Vec::new();
        if let Some(&(_, j, k)) = candidates.iter().find(|(_, j, _)| j.loaded.image.name.starts_with("resource_")) {
            picked.push((j, k));
        }
        let mut folders = BTreeSet::new();
        for &(_, j, k) in &candidates {
            if picked.len() >= n {
                break;
            }
            let image = &j.loaded.image;
            let folder = super::model::folder(&image.functions[j.units[k].first].object).to_string();
            let overlay = j.loaded.image.name.starts_with("resource_");
            if overlay || picked.iter().any(|(pj, pk)| std::ptr::eq(*pj, j) && *pk == k) || !folders.insert(folder) {
                continue;
            }
            picked.push((j, k));
        }
        return Ok(picked);
    }
    let mut picked = Vec::new();
    for name in choice.split(',') {
        let (image, unit) = name.split_once(':').ok_or_else(|| format!("{name}: name a unit as IMAGE:UNIT, main:U0042"))?;
        let j = judged
            .iter()
            .enumerate()
            .find(|(i, j)| j.loaded.image.name == image || (image == "main" && *i == 0))
            .map(|(_, j)| j)
            .ok_or_else(|| format!("{image}: no such image"))?;
        let k = unit.trim_start_matches('U').parse::<usize>().map_err(|_| format!("{unit}: not a unit id"))?;
        if k == 0 || k > j.units.len() {
            return Err(format!("{name}: no such unit"));
        }
        picked.push((j, k - 1));
    }
    Ok(picked)
}

fn describe(image: &Image, unit: &Unit) -> String {
    format!("{} functions, {} objects", unit.last - unit.first + 1, unit.objects(image).len())
}

pub fn run(root: &Path, target: DecompTarget, judged: &[Judged], choice: &str) -> Result<String, String> {
    crate::compiler::routing::prefer_installed_binutils();
    let mut out = String::from("image\tunit\tname\tsize\tverdict\tdetail\n");
    for (j, k) in choose(judged, choice, root)? {
        let image = &j.loaded.image;
        let unit = &j.units[k];
        let id = format!("{}-U{:04}", image.name, k + 1);
        let outcome = compare(root, target, &j.loaded, &judged[0].loaded, unit, &id);
        let _ = writeln!(out, "{}\tU{:04}\t{}\t{}\t{}\t{}", image.name, k + 1, unit.name, describe(image, unit), outcome.verdict, outcome.detail.replace('\t', " "));
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn items_split_at_top_level_and_keep_bodies_whole() {
        let text = "#include \"a.h\"\n#define X(a) \\\n  (a)\nextern int g; /* ; */\nstruct S { int a; };\nvoid f(void)\n{\n    if (g) { g = 1; }\n}\nstatic const char *s = \"};\";\n";
        let items: Vec<String> = items(text).into_iter().map(|i| normalised(&i)).collect();
        assert_eq!(items, [
            "#include \"a.h\"",
            "#define X(a) \\ (a)",
            "extern int g;",
            "/* ; */ struct S { int a; };",
            "void f(void) { if (g) { g = 1; } }",
            "static const char *s = \"};\";",
        ]);
    }

    #[test]
    fn merging_keeps_one_of_each_declaration_and_every_definition() {
        let a = (PathBuf::from("/nowhere/a.c"), "#include \"types.h\"\nextern int g;\nvoid f(void);\nvoid f(void)\n{\n}\n".to_string());
        let b = (PathBuf::from("/nowhere/b.c"), "#include \"types.h\"\nextern  int g;\nvoid f(void);\nvoid h(void)\n{\n    f();\n}\n".to_string());
        let merged = merge(&[a, b]);
        assert_eq!(merged.matches("#include \"types.h\"").count(), 1);
        assert_eq!(merged.matches("extern").count(), 1);
        assert_eq!(merged.matches("void f(void);").count(), 1);
        assert!(merged.contains("void h(void)"));
        assert!(merged.find("void f(void)\n{").unwrap() < merged.find("void h(void)").unwrap());
    }

    #[test]
    fn bytes_the_image_does_not_hold_are_not_judged() {
        assert!(!differs(None, Some(7)));
        assert!(!differs(Some(7), Some(7)));
        assert!(differs(Some(7), Some(0)));
        assert!(differs(Some(7), None));
    }

    #[test]
    fn a_layout_maps_merged_offsets_to_today_s_addresses() {
        let piece = |address, size| Piece { object: String::new(), path: String::new(), section: ".rodata".into(), address, size, region: 0 };
        let (a, b) = (piece(0x100, 6), piece(0x200, 4));
        let layout = Layout::new(&[&a, &b], 4);
        assert_eq!(layout.size(), 12);
        assert_eq!(layout.address(5), Some(0x105));
        assert_eq!(layout.address(8), Some(0x200));
        assert_eq!(layout.address(11), Some(0x203));
    }
}
