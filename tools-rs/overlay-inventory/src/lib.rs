//! `tools/overlay/overlay_inventory.ts`, in Rust.
//!
//! Discovers ordinary Thumb functions inside Alchemy's canonical overlay
//! sources and groups relocation-normalized duplicates. Sibling crates supply
//! everything the TypeScript imported: `discover` for `Discovery`/`sx`,
//! `overlay-disasm` for `assemble_overlay`/`overlay_c_addresses`/`OVERLAY_BASE`,
//! `canonical-json` for the report writer. `wyhash` is kept local because it is
//! one small, self-contained, already-vetted Bun-compatible hash function.

mod wyhash;

use discover::{sx, Discovery, Mode};
use overlay_disasm::{assemble_overlay, overlay_c_addresses, OverlaySource, OVERLAY_BASE};
use serde_json::{Map, Value};
use std::collections::BTreeMap;
use std::fs::{self, read_dir};
use std::path::{Path, PathBuf};

pub struct Options {
    pub assets: PathBuf,
    pub output: PathBuf,
    pub top: usize,
}

/// Two levels above `CARGO_MANIFEST_DIR` (`tools-rs/overlay-inventory`) is the
/// repository root, matching `dirname(dirname(dirname(import.meta.url)))` in
/// the TypeScript (that has one extra `dirname` because it starts from the
/// file, not the crate directory).
pub fn root_from_exe(manifest_dir: &str) -> PathBuf {
    Path::new(manifest_dir)
        .parent()
        .and_then(Path::parent)
        .expect("tools-rs/overlay-inventory sits two levels below the repository root")
        .to_path_buf()
}

pub fn options_of(argv: &[String], root: &Path) -> Result<Options, String> {
    let mut options = Options {
        assets: root.join("assets/code"),
        output: root.join("out/decomp/overlays.json"),
        top: 20,
    };
    let mut index = 0usize;
    while index < argv.len() {
        let argument = &argv[index];
        if argument == "--assets" {
            index += 1;
            options.assets = PathBuf::from(argv.get(index).ok_or("--assets requires a value")?);
        } else if argument == "--output" || argument == "-o" {
            index += 1;
            options.output = PathBuf::from(argv.get(index).ok_or("--output requires a value")?);
        } else if argument == "--top" {
            index += 1;
            let raw = argv.get(index).ok_or("--top requires a value")?;
            options.top = raw
                .parse::<i64>()
                .ok()
                .filter(|value| *value >= 1)
                .ok_or("--top must be a positive integer")? as usize;
        } else if argument == "-h" || argument == "--help" {
            println!("usage: overlay-inventory [--assets DIR] [-o OUTPUT] [--top N] [--self-test]");
            std::process::exit(0);
        } else {
            return Err(format!("unrecognized argument: {argument}"));
        }
        index += 1;
    }
    Ok(options)
}

const FILLER_HALFWORD_BUDGET: i64 = 4;
const POOL_WORD_BUDGET: i64 = 64;

fn is_entry_shape(half: i64) -> bool {
    (half & 0xff00) == 0xb500
        || half == 0x4770
        || (half & 0xf800) == 0x2000
        || (half & 0xf800) == 0x4800
}

fn next_entry_after_return(discovery: &Discovery, last: i64) -> Option<i64> {
    let mut candidate = last + discovery.instructions.get(&last).unwrap().size;
    let mut fillers: i64 = 0;
    let mut pools: i64 = 0;
    while discovery.inside(candidate, 2) {
        if discovery.literal_slots.contains(&candidate) {
            pools += 1;
            if pools > POOL_WORD_BUDGET {
                return None;
            }
            candidate += 4;
            continue;
        }
        let half = discovery.u16(candidate);
        if is_entry_shape(half) {
            return if discovery.instructions.contains_key(&candidate) {
                None
            } else {
                Some(candidate)
            };
        }
        let aligns_pool = (candidate & 3) == 2 && discovery.literal_slots.contains(&(candidate + 2));
        if half == 0 || half == 0x46c0 || aligns_pool {
            fillers += 1;
            if fillers > FILLER_HALFWORD_BUDGET {
                return None;
            }
            candidate += 2;
            continue;
        }
        return None;
    }
    None
}

/// `discovery.walk_function` can panic (out-of-range reads, unknown entries)
/// where the TypeScript throws a catchable exception that `discoverOverlay`'s
/// `try { ... } catch {}` silently swallows. `catch_unwind` mirrors that, with
/// the panic hook suppressed so stderr stays byte-identical to the TS original
/// (which prints nothing on a caught throw).
fn walk_silently(discovery: &mut Discovery, entry: i64) {
    let previous_hook = std::panic::take_hook();
    std::panic::set_hook(Box::new(|_| {}));
    let result = std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| {
        discovery.walk_function(entry);
    }));
    std::panic::set_hook(previous_hook);
    let _ = result;
}

pub fn discover_overlay(data: &[u8]) -> Discovery {
    let mut discovery = Discovery::new(data, OVERLAY_BASE);
    let scan_limit = data.len().min(0x400);
    let mut offset = 0usize;
    while offset + 2 <= scan_limit {
        let half = u16::from_le_bytes([data[offset], data[offset + 1]]);
        if (half & 0xff00) == 0xb500 {
            discovery.add_seed(OVERLAY_BASE + offset as i64, Mode::Thumb, "first-prologue");
            break;
        }
        offset += 2;
    }
    let mut offset = 0usize;
    while offset + 4 <= data.len() {
        let value = u32::from_le_bytes([
            data[offset],
            data[offset + 1],
            data[offset + 2],
            data[offset + 3],
        ]) as i64;
        let target = value & !1;
        if (value & 1) != 0 && discovery.inside(target, 2) {
            let first = discovery.u16(target);
            if (first & 0xff00) == 0xb500 || first == 0x4770 || (first & 0xf800) == 0x2000 {
                discovery.add_seed(target, Mode::Thumb, "local-function-pointer");
            }
        }
        offset += 4;
    }
    let mut walked: std::collections::BTreeSet<i64> = std::collections::BTreeSet::new();
    loop {
        let pending: Vec<i64> = discovery
            .function_entries()
            .into_iter()
            .filter(|entry| !walked.contains(entry))
            .collect();
        if pending.is_empty() {
            let mut added = false;
            for entry in discovery.function_entries() {
                let last = {
                    let info = discovery.function(entry).unwrap();
                    let mut addresses: Vec<i64> = info.instructions.iter().copied().collect();
                    addresses.sort_unstable();
                    match addresses.last().copied() {
                        Some(value) => value,
                        None => continue,
                    }
                };
                if discovery.instructions.get(&last).map(|item| item.kind) != Some("return") {
                    continue;
                }
                if let Some(candidate) = next_entry_after_return(&discovery, last) {
                    let source = format!("after-return:{last:x}");
                    added = discovery.add_seed(candidate, Mode::Thumb, &source) || added;
                }
            }
            if !added {
                break;
            }
            continue;
        }
        for entry in pending {
            walk_silently(&mut discovery, entry);
            walked.insert(entry);
        }
    }
    let jump_table_sites: Vec<i64> = discovery.jump_table_sites.keys().copied().collect();
    for site in &jump_table_sites {
        discovery.unresolved.remove(site);
    }
    for entry in discovery.function_entries() {
        if let Some(info) = discovery.function_mut(entry) {
            for site in &jump_table_sites {
                info.unresolved.remove(site);
            }
        }
    }
    discovery
}

fn synthetic_overlay(items: &[(usize, char, u32)]) -> Vec<u8> {
    let mut data = vec![0u8; 0x40];
    for (offset, width, value) in items {
        if *width == 'h' {
            let bytes = (*value as u16).to_le_bytes();
            data[*offset..*offset + 2].copy_from_slice(&bytes);
        } else {
            let bytes = value.to_le_bytes();
            data[*offset..*offset + 4].copy_from_slice(&bytes);
        }
    }
    data
}

pub fn self_test() -> Result<(), String> {
    let base = OVERLAY_BASE;
    let check = |label: &str, ok: bool| -> Result<(), String> {
        if ok {
            Ok(())
        } else {
            Err(format!("code-overlay inventory self-test: {label}"))
        }
    };
    let from_scan = |discovery: &Discovery, entry: i64| -> bool {
        match discovery.function(entry) {
            Some(info) => info.sources.iter().any(|source| source.starts_with("after-return:")),
            None => false,
        }
    };

    let pooled = discover_overlay(&synthetic_overlay(&[
        (0x00, 'h', 0xb500),
        (0x02, 'h', 0x4801),
        (0x04, 'h', 0x4901),
        (0x06, 'h', 0xbd00),
        (0x08, 'w', 0x0a0b0c0d),
        (0x0c, 'w', 0x12345678),
        (0x10, 'h', 0xb510),
        (0x12, 'h', 0x2000),
        (0x14, 'h', 0xbd10),
    ]));
    check("pool words must be stepped over", from_scan(&pooled, base + 0x10))?;
    check(
        "pool words are not function entries",
        pooled.function(base + 0x08).is_none() && pooled.function(base + 0x0c).is_none(),
    )?;

    let opaque = discover_overlay(&synthetic_overlay(&[
        (0x00, 'h', 0xb500),
        (0x02, 'h', 0xbd00),
        (0x04, 'w', 0xdeadbeef),
        (0x08, 'w', 0x0002b500),
    ]));
    check(
        "unexplained data must stop the scan",
        opaque.function(base + 0x08).is_none(),
    )?;
    check(
        "data must not yield extra functions",
        opaque.function_entries().len() == 1,
    )?;

    let unrecorded = discover_overlay(&synthetic_overlay(&[
        (0x00, 'h', 0xb500),
        (0x02, 'h', 0x4801),
        (0x04, 'h', 0xbd00),
        (0x06, 'h', 0x0000),
        (0x08, 'w', 0x0a0b0c0d),
        (0x0c, 'w', 0x11223344),
        (0x10, 'h', 0xb510),
        (0x12, 'h', 0xbd10),
    ]));
    check(
        "unrecorded words must stop the scan",
        unrecorded.function(base + 0x10).is_none(),
    )?;

    let padded = discover_overlay(&synthetic_overlay(&[
        (0x00, 'h', 0xb500),
        (0x02, 'h', 0x4801),
        (0x04, 'h', 0xbd00),
        (0x06, 'h', 0x0000),
        (0x08, 'w', 0x0a0b0c0d),
        (0x0c, 'h', 0x46c0),
        (0x0e, 'h', 0x46c0),
        (0x10, 'h', 0xb510),
        (0x12, 'h', 0xbd10),
    ]));
    check(
        "alignment and nop padding must be stepped over",
        from_scan(&padded, base + 0x10),
    )?;

    let disguised = discover_overlay(&synthetic_overlay(&[
        (0x00, 'h', 0xb500),
        (0x02, 'h', 0x4801),
        (0x04, 'h', 0xbd00),
        (0x06, 'h', 0x0000),
        (0x08, 'w', 0x0001b510),
        (0x0c, 'h', 0xb510),
        (0x0e, 'h', 0xbd10),
    ]));
    check(
        "a prologue-shaped pool word is data",
        disguised.function(base + 0x08).is_none(),
    )?;
    check(
        "the function behind a disguised pool word is found",
        from_scan(&disguised, base + 0x0c),
    )?;

    let prefix: [(usize, char, u32); 5] = [
        (0x00, 'h', 0xb500),
        (0x02, 'h', 0x4801),
        (0x04, 'h', 0xbd00),
        (0x06, 'h', 0x0000),
        (0x08, 'w', 0x0a0b0c0d),
    ];
    let tail: [(usize, char, u32); 2] = [(0x14, 'h', 0xb510), (0x16, 'h', 0xbd10)];
    let leaves: [(&str, &[(usize, char, u32)]); 3] = [
        ("bx lr", &[(0x0c, 'h', 0x4770)]),
        ("movs", &[(0x0c, 'h', 0x2001), (0x0e, 'h', 0x4770)]),
        (
            "pc-relative load",
            &[(0x0c, 'h', 0x4800), (0x0e, 'h', 0x4770), (0x10, 'w', 0x0a0b0c0d)],
        ),
    ];
    for (label, leaf) in leaves {
        let mut items: Vec<(usize, char, u32)> = prefix.to_vec();
        items.extend_from_slice(leaf);
        items.extend_from_slice(&tail);
        let widened = discover_overlay(&synthetic_overlay(&items));
        check(
            &format!("a leaf opening with {label} is an entry"),
            from_scan(&widened, base + 0x0c),
        )?;
        check(
            &format!("the function behind a {label} leaf is found"),
            from_scan(&widened, base + 0x14),
        )?;
    }

    let disguised_leaf = discover_overlay(&synthetic_overlay(&[
        (0x00, 'h', 0xb500),
        (0x02, 'h', 0x4801),
        (0x04, 'h', 0xbd00),
        (0x06, 'h', 0x0000),
        (0x08, 'w', 0x00002421),
        (0x0c, 'h', 0xb510),
        (0x0e, 'h', 0xbd10),
    ]));
    check(
        "a movs-shaped pool word is data",
        disguised_leaf.function(base + 0x08).is_none(),
    )?;
    check(
        "the function behind a movs-shaped pool word is found",
        from_scan(&disguised_leaf, base + 0x0c),
    )?;

    let indirect = discover_overlay(&synthetic_overlay(&[
        (0x00, 'h', 0xb500),
        (0x02, 'h', 0x4718),
        (0x04, 'h', 0xb510),
        (0x06, 'h', 0xbd10),
    ]));
    check(
        "an unresolved indirect branch seeds no successor",
        indirect.function(base + 0x04).is_none(),
    )?;

    println!("code-overlay inventory self-test passed");
    Ok(())
}

fn target_class(discovery: &Discovery, value: i64) -> String {
    let address = value & !1;
    if discovery.inside(address, 2) {
        return "overlay".to_string();
    }
    let bank = (value >> 24) & 0xff;
    if bank == 0x08 || bank == 0x09 {
        "rom".to_string()
    } else if bank == 0x02 || bank == 0x03 {
        "ram".to_string()
    } else if bank == 0x04 {
        "io".to_string()
    } else if value <= 0xffff {
        format!("constant:{value:x}")
    } else {
        "constant:wide".to_string()
    }
}

fn structural_veneer(discovery: &Discovery, entry: i64) -> bool {
    if !discovery.inside(entry, 8) {
        return false;
    }
    let pointer = discovery.u32(entry);
    if (pointer & 1) == 0 {
        return false;
    }
    let bank = (pointer >> 24) & 0xff;
    if bank != 0x08 && bank != 0x09 {
        return false;
    }
    let load = discovery.u16(entry + 4);
    let branch = discovery.u16(entry + 6);
    if (load & 0xf8ff) != 0x4800 {
        return false;
    }
    if (branch & 0xff87) != 0x4700 {
        return false;
    }
    ((load >> 8) & 7) == ((branch >> 3) & 0xf)
}

fn data_walk(discovery: &Discovery, addresses: &[i64]) -> bool {
    addresses.iter().any(|address| {
        discovery.instructions.get(address).map(|item| item.size) == Some(2)
            && discovery.u16(*address) >= 0xe800
    })
}

fn fingerprint(discovery: &Discovery, addresses: &[i64]) -> String {
    let mut tokens: Vec<String> = Vec::new();
    for &address in addresses {
        let instruction = discovery.instructions.get(&address).unwrap();
        let half = discovery.u16(address);
        if instruction.size == 4 && (half & 0xf800) == 0xf000 {
            let low = discovery.u16(address + 2);
            let offset = sx(((half & 0x7ff) << 12) | ((low & 0x7ff) << 1), 23);
            tokens.push(format!("bl:{}", target_class(discovery, address + 4 + offset)));
        } else if (half & 0xf800) == 0xe000 {
            tokens.push(format!("b:{}", half >> 11));
        } else if (half & 0xf000) == 0xd000 && ((half >> 8) & 0xf) < 0xe {
            tokens.push(format!("bc:{}", (half >> 8) & 0xf));
        } else if (half & 0xf800) == 0x4800 {
            let literal = ((address + 4) & !3) + ((half & 0xff) << 2);
            let kind = if discovery.inside(literal, 4) {
                target_class(discovery, discovery.u32(literal))
            } else {
                "external".to_string()
            };
            tokens.push(format!("ldr-lit:r{}:{}", (half >> 8) & 7, kind));
        } else if (half & 0xf800) == 0xa000 {
            tokens.push(format!("adr:r{}", (half >> 8) & 7));
        } else {
            tokens.push(format!("{half:04x}"));
        }
    }
    let joined = tokens.join(" ");
    wyhash::base36(wyhash::bun_hash(joined.as_bytes()))
}

fn converted_placeholders(source: &Path) -> Result<BTreeMap<i64, i64>, String> {
    let mut placeholders: BTreeMap<i64, i64> = BTreeMap::new();
    let text = fs::read_to_string(source).map_err(|error| error.to_string())?;
    let lower = text.to_ascii_lowercase();
    let lines: Vec<&str> = text.split('\n').collect();
    for (line_index, raw_line) in lines.iter().enumerate() {
        let candidate = raw_line.trim_end_matches('\r');
        let candidate_lower = lower
            .split('\n')
            .nth(line_index)
            .map(|line| line.trim_end_matches('\r'))
            .unwrap_or("");
        if !candidate_lower.starts_with("alchemyc_") {
            continue;
        }
        let trimmed = candidate.trim_end();
        if !trimmed.ends_with(':') {
            continue;
        }
        let head = &candidate_lower[..candidate_lower.len() - 0]; // placeholder, unused branch below replaced
        let _ = head;
        let hex_part = &trimmed[..trimmed.len() - 1];
        let prefix_len = "AlchemyC_".len();
        if hex_part.len() != prefix_len + 8 {
            continue;
        }
        let (prefix, hex) = hex_part.split_at(prefix_len);
        if !prefix.eq_ignore_ascii_case("alchemyc_") {
            continue;
        }
        if !hex.chars().all(|c| c.is_ascii_hexdigit()) {
            continue;
        }
        let address = i64::from_str_radix(hex, 16).map_err(|error| error.to_string())?;
        if placeholders.contains_key(&address) {
            return Err(format!(
                "duplicate overlay C placeholder: {}:0x{:x}",
                source.display(),
                address
            ));
        }
        let mut size: i64 = 0;
        for raw in lines.iter().skip(line_index + 1) {
            let line = raw.trim_end_matches('\r').trim();
            if line.is_empty() {
                continue;
            }
            if is_local_label(line) {
                continue;
            }
            match parse_space_directive(line) {
                Some(value) => size += value,
                None => break,
            }
        }
        if size == 0 {
            return Err(format!(
                "overlay C placeholder has no space: {}:0x{:x}",
                source.display(),
                address
            ));
        }
        placeholders.insert(address, size);
    }
    Ok(placeholders)
}

fn is_local_label(line: &str) -> bool {
    if !line.starts_with(".L_") || !line.ends_with(':') {
        return false;
    }
    let body = &line[3..line.len() - 1];
    !body.is_empty()
        && body
            .chars()
            .all(|c| c.is_ascii_alphanumeric() || c == '_' || c == '.' || c == '$')
}

fn parse_space_directive(line: &str) -> Option<i64> {
    let rest = strip_ci_prefix(line, ".space")?;
    let rest = rest.strip_prefix(char::is_whitespace)?;
    let rest = rest.trim_start();
    let token: String = rest
        .chars()
        .take_while(|c| c.is_ascii_hexdigit() || *c == 'x' || *c == 'X')
        .collect();
    if token.is_empty() {
        return None;
    }
    if let Some(hex) = token.strip_prefix("0x").or_else(|| token.strip_prefix("0X")) {
        i64::from_str_radix(hex, 16).ok()
    } else {
        token.parse::<i64>().ok()
    }
}

fn strip_ci_prefix<'a>(line: &'a str, prefix: &str) -> Option<&'a str> {
    if line.len() < prefix.len() {
        return None;
    }
    if line[..prefix.len()].eq_ignore_ascii_case(prefix) {
        Some(&line[prefix.len()..])
    } else {
        None
    }
}

struct FunctionRow {
    id: String,
    overlay: String,
    entry: i64,
    offset: i64,
    instruction_count: i64,
    instruction_offsets: Vec<i64>,
    code_bytes: i64,
    span_bytes: i64,
    calls: i64,
    returns: i64,
    starts_with_prologue: bool,
    unresolved: i64,
    unresolved_sites: Vec<i64>,
    jump_tables: i64,
    fingerprint: String,
    seed_sources: Vec<String>,
    contained_by: Vec<String>,
    structural_veneer: bool,
    data_walk: bool,
}

impl FunctionRow {
    fn to_json(&self) -> Value {
        let mut map = Map::new();
        map.insert("id".into(), Value::String(self.id.clone()));
        map.insert("overlay".into(), Value::String(self.overlay.clone()));
        map.insert("entry".into(), Value::from(self.entry));
        map.insert("offset".into(), Value::from(self.offset));
        map.insert("instruction_count".into(), Value::from(self.instruction_count));
        map.insert(
            "instruction_offsets".into(),
            Value::Array(self.instruction_offsets.iter().map(|v| Value::from(*v)).collect()),
        );
        map.insert("code_bytes".into(), Value::from(self.code_bytes));
        map.insert("span_bytes".into(), Value::from(self.span_bytes));
        map.insert("calls".into(), Value::from(self.calls));
        map.insert("returns".into(), Value::from(self.returns));
        map.insert("starts_with_prologue".into(), Value::from(self.starts_with_prologue));
        map.insert("unresolved".into(), Value::from(self.unresolved));
        map.insert(
            "unresolved_sites".into(),
            Value::Array(self.unresolved_sites.iter().map(|v| Value::from(*v)).collect()),
        );
        map.insert("jump_tables".into(), Value::from(self.jump_tables));
        map.insert("fingerprint".into(), Value::String(self.fingerprint.clone()));
        map.insert(
            "seed_sources".into(),
            Value::Array(self.seed_sources.iter().map(|v| Value::String(v.clone())).collect()),
        );
        map.insert(
            "contained_by".into(),
            Value::Array(self.contained_by.iter().map(|v| Value::String(v.clone())).collect()),
        );
        map.insert("structural_veneer".into(), Value::from(self.structural_veneer));
        map.insert("data_walk".into(), Value::from(self.data_walk));
        Value::Object(map)
    }
}

pub fn run(root: &Path, args: &[String]) -> Result<(), String> {
    if args.iter().any(|a| a == "--self-test") {
        return self_test();
    }
    let options = options_of(args, root)?;
    let suffix = "_overlay.s";
    let mut overlays: Vec<(String, PathBuf)> = read_dir(&options.assets)
        .map_err(|error| error.to_string())?
        .filter_map(|entry| entry.ok())
        .filter_map(|entry| {
            let name = entry.file_name().to_string_lossy().to_string();
            if name.starts_with("resource_") && name.ends_with(suffix) {
                Some((name[..name.len() - suffix.len()].to_string(), entry.path()))
            } else {
                None
            }
        })
        .filter(|(_, path)| fs::metadata(path).map(|m| m.len() > 0).unwrap_or(false))
        .collect();
    overlays.sort_by(|a, b| a.0.cmp(&b.0));
    if overlays.is_empty() {
        return Err(format!("no overlay sources under {}", options.assets.display()));
    }

    let mut functions: Vec<FunctionRow> = Vec::new();
    let mut decoded_bytes: i64 = 0;
    let mut instruction_bytes: i64 = 0;
    let mut unresolved: i64 = 0;
    let mut jump_tables: i64 = 0;
    let mut converted_functions: i64 = 0;
    let mut converted_instruction_bytes: i64 = 0;
    let mut converted_span_bytes: i64 = 0;
    let mut converted_internal_entries: i64 = 0;
    let mut undiscovered_converted_functions: i64 = 0;

    for (name, path) in &overlays {
        let source = OverlaySource::path(path.clone());
        let data = assemble_overlay(&source, OVERLAY_BASE)?;
        let converted = overlay_c_addresses(&source)?;
        let placeholders = converted_placeholders(path)?;
        let missing: Vec<i64> = converted
            .iter()
            .copied()
            .filter(|address| !placeholders.contains_key(address))
            .collect();
        let orphaned: Vec<i64> = placeholders
            .keys()
            .copied()
            .filter(|address| !converted.contains(address))
            .collect();
        if !missing.is_empty() || !orphaned.is_empty() {
            let mut parts: Vec<String> = Vec::new();
            if !missing.is_empty() {
                let list = missing
                    .iter()
                    .map(|a| format!("0x{a:x}"))
                    .collect::<Vec<_>>()
                    .join(",");
                parts.push(format!("missing placeholders={list}"));
            }
            if !orphaned.is_empty() {
                let list = orphaned
                    .iter()
                    .map(|a| format!("0x{a:x}"))
                    .collect::<Vec<_>>()
                    .join(",");
                parts.push(format!("orphaned placeholders={list}"));
            }
            return Err(format!(
                "overlay C/source mismatch: {name}: {}",
                parts.join(" ")
            ));
        }
        converted_functions += converted.len() as i64;
        converted_span_bytes += placeholders.values().sum::<i64>();
        let converted_spans: Vec<(i64, i64)> = placeholders.iter().map(|(a, b)| (*a, *b)).collect();
        decoded_bytes += data.len() as i64;
        let discovery = discover_overlay(&data);
        instruction_bytes += discovery.instructions.values().map(|item| item.size).sum::<i64>();
        converted_instruction_bytes += discovery
            .instructions
            .iter()
            .filter(|(address, _)| {
                converted_spans
                    .iter()
                    .any(|(start, size)| *start <= **address && **address < start + size)
            })
            .map(|(_, item)| item.size)
            .sum::<i64>();
        unresolved += discovery.unresolved.len() as i64;
        jump_tables += discovery.jump_tables.len() as i64;

        let mut discovered_converted: std::collections::BTreeSet<i64> = std::collections::BTreeSet::new();
        let mut entries = discovery.function_entries();
        entries.sort_unstable();
        for entry in entries {
            let info = discovery.function(entry).unwrap();
            let mut addresses: Vec<i64> = info.instructions.iter().copied().collect();
            addresses.sort_unstable();
            if addresses.is_empty() || addresses[0] != entry {
                continue;
            }
            let owner = converted_spans
                .iter()
                .find(|(start, size)| *start <= entry && entry < start + size)
                .map(|(start, _)| *start);
            if let Some(owner) = owner {
                if entry == owner {
                    discovered_converted.insert(entry);
                } else {
                    converted_internal_entries += 1;
                }
                continue;
            }
            let mut end = addresses
                .iter()
                .map(|address| address + discovery.instructions.get(address).unwrap().size)
                .max()
                .unwrap();
            for &address in &addresses {
                let half = discovery.u16(address);
                if (half & 0xf800) != 0x4800 {
                    continue;
                }
                let literal = ((address + 4) & !3) + ((half & 0xff) << 2);
                if discovery.inside(literal, 4) {
                    end = end.max(literal + 4);
                }
            }
            let offset = entry - OVERLAY_BASE;
            let code_bytes = addresses
                .iter()
                .map(|address| discovery.instructions.get(address).unwrap().size)
                .sum::<i64>();
            let returns = addresses
                .iter()
                .filter(|address| discovery.instructions.get(address).unwrap().kind == "return")
                .count() as i64;
            let mut unresolved_sites: Vec<i64> = info.unresolved.iter().copied().collect();
            unresolved_sites.sort_unstable();
            let unresolved_sites: Vec<i64> = unresolved_sites.iter().map(|s| s - OVERLAY_BASE).collect();
            let jump_tables_in_fn = discovery
                .jump_table_sites
                .keys()
                .filter(|site| info.instructions.contains(site))
                .count() as i64;
            let mut seed_sources: Vec<String> = info.sources.iter().cloned().collect();
            seed_sources.sort();
            functions.push(FunctionRow {
                id: format!("{name}:{offset:04x}"),
                overlay: name.clone(),
                entry,
                offset,
                instruction_count: addresses.len() as i64,
                instruction_offsets: addresses.iter().map(|address| address - entry).collect(),
                code_bytes,
                span_bytes: end - entry,
                calls: (info.callees.len() + info.external_callees.len()) as i64,
                returns,
                starts_with_prologue: (discovery.u16(entry) & 0xff00) == 0xb500,
                unresolved: info.unresolved.len() as i64,
                unresolved_sites,
                jump_tables: jump_tables_in_fn,
                fingerprint: fingerprint(&discovery, &addresses),
                seed_sources,
                contained_by: Vec::new(),
                structural_veneer: structural_veneer(&discovery, entry),
                data_walk: data_walk(&discovery, &addresses),
            });
        }
        undiscovered_converted_functions += converted
            .iter()
            .filter(|address| !discovered_converted.contains(address))
            .count() as i64;
    }

    let mut instruction_owners: BTreeMap<String, BTreeMap<i64, Vec<String>>> = BTreeMap::new();
    for function in &functions {
        let owners = instruction_owners.entry(function.overlay.clone()).or_default();
        for offset in &function.instruction_offsets {
            let address = function.entry + offset;
            owners.entry(address).or_default().push(function.id.clone());
        }
    }
    for function in &mut functions {
        let mut owners = instruction_owners
            .get(&function.overlay)
            .and_then(|table| table.get(&function.entry))
            .cloned()
            .unwrap_or_default();
        owners.retain(|id| id != &function.id);
        owners.sort();
        function.contained_by = owners;
    }

    let veneers = functions.iter().filter(|f| f.structural_veneer).count() as i64;
    let data_walks = functions.iter().filter(|f| f.data_walk).count() as i64;
    let ordinary: Vec<&FunctionRow> = functions
        .iter()
        .filter(|f| {
            f.unresolved == 0
                && f.jump_tables == 0
                && f.code_bytes >= 8
                && f.span_bytes - f.code_bytes <= 64
                && !f.structural_veneer
                && !f.data_walk
        })
        .collect();
    let tiny = functions.iter().filter(|f| f.code_bytes <= 6).count() as i64;
    let contained = functions.iter().filter(|f| !f.contained_by.is_empty()).count() as i64;
    let returning = functions.iter().filter(|f| f.returns > 0).count() as i64;
    let span_suspects: Vec<&FunctionRow> = functions
        .iter()
        .filter(|f| {
            f.returns == 0
                && f.starts_with_prologue
                && !f.structural_veneer
                && !f.data_walk
                && f.contained_by.is_empty()
        })
        .collect();
    for function in &span_suspects {
        eprintln!(
            "warning: {} walked {} bytes with no return — verify its span",
            function.id, function.code_bytes
        );
    }
    let ordinary_prologue_return = ordinary
        .iter()
        .filter(|f| f.contained_by.is_empty() && f.starts_with_prologue && f.returns > 0)
        .count() as i64;

    let mut groups: BTreeMap<String, Vec<&FunctionRow>> = BTreeMap::new();
    let mut group_order: Vec<String> = Vec::new();
    for function in &ordinary {
        if !groups.contains_key(&function.fingerprint) {
            group_order.push(function.fingerprint.clone());
        }
        groups.entry(function.fingerprint.clone()).or_default().push(function);
    }
    struct Family {
        fingerprint: String,
        count: i64,
        overlays: i64,
        code_bytes: i64,
        members: Vec<String>,
    }
    let mut families: Vec<Family> = group_order
        .into_iter()
        .map(|fingerprint| {
            let members = groups.get(&fingerprint).unwrap();
            let overlay_set: std::collections::BTreeSet<&str> =
                members.iter().map(|m| m.overlay.as_str()).collect();
            Family {
                fingerprint: fingerprint.clone(),
                count: members.len() as i64,
                overlays: overlay_set.len() as i64,
                code_bytes: members.iter().map(|m| m.code_bytes).sum(),
                members: members.iter().map(|m| m.id.clone()).collect(),
            }
        })
        .collect();
    families.sort_by(|a, b| b.count.cmp(&a.count).then(b.code_bytes.cmp(&a.code_bytes)));

    let duplicate_families = families.iter().filter(|f| f.count > 1).count() as i64;
    let duplicate_functions = families.iter().filter(|f| f.count > 1).map(|f| f.count).sum::<i64>();

    let mut totals = Map::new();
    totals.insert("overlays".into(), Value::from(overlays.len() as i64));
    totals.insert("decoded_bytes".into(), Value::from(decoded_bytes));
    totals.insert("converted_functions".into(), Value::from(converted_functions));
    totals.insert("unconverted_discoveries".into(), Value::from(functions.len() as i64));
    totals.insert("tiny_unconverted_discoveries".into(), Value::from(tiny));
    totals.insert("contained_unconverted_discoveries".into(), Value::from(contained));
    totals.insert("returning_unconverted_discoveries".into(), Value::from(returning));
    totals.insert(
        "ordinary_prologue_return_discoveries".into(),
        Value::from(ordinary_prologue_return),
    );
    totals.insert("structural_veneer_discoveries".into(), Value::from(veneers));
    totals.insert("data_walk_discoveries".into(), Value::from(data_walks));
    totals.insert("converted_internal_entries".into(), Value::from(converted_internal_entries));
    totals.insert(
        "undiscovered_converted_functions".into(),
        Value::from(undiscovered_converted_functions),
    );
    totals.insert("functions".into(), Value::from(functions.len() as i64));
    totals.insert("ordinary_unconverted_discoveries".into(), Value::from(ordinary.len() as i64));
    totals.insert("ordinary_functions".into(), Value::from(ordinary.len() as i64));
    totals.insert("instruction_bytes".into(), Value::from(instruction_bytes));
    totals.insert(
        "converted_instruction_bytes".into(),
        Value::from(converted_instruction_bytes),
    );
    totals.insert("converted_span_bytes".into(), Value::from(converted_span_bytes));
    totals.insert("unresolved".into(), Value::from(unresolved));
    totals.insert("jump_tables".into(), Value::from(jump_tables));
    totals.insert("duplicate_families".into(), Value::from(duplicate_families));
    totals.insert("duplicate_functions".into(), Value::from(duplicate_functions));

    let families_json: Vec<Value> = families
        .iter()
        .map(|family| {
            let mut map = Map::new();
            map.insert("fingerprint".into(), Value::String(family.fingerprint.clone()));
            map.insert("count".into(), Value::from(family.count));
            map.insert("overlays".into(), Value::from(family.overlays));
            map.insert("code_bytes".into(), Value::from(family.code_bytes));
            map.insert(
                "members".into(),
                Value::Array(family.members.iter().map(|m| Value::String(m.clone())).collect()),
            );
            Value::Object(map)
        })
        .collect();

    let mut report = Map::new();
    report.insert("format".into(), Value::from(1));
    report.insert(
        "generated_at".into(),
        Value::String(iso_now()),
    );
    report.insert("totals".into(), Value::Object(totals));
    report.insert("families".into(), Value::Array(families_json));
    report.insert(
        "functions".into(),
        Value::Array(functions.iter().map(|f| f.to_json()).collect()),
    );
    let report_value = Value::Object(report);

    if let Some(parent) = options.output.parent() {
        fs::create_dir_all(parent).map_err(|error| error.to_string())?;
    }
    let text = format!("{}\n", canonical_json::canonical_json(&report_value));
    fs::write(&options.output, text).map_err(|error| error.to_string())?;

    println!(
        "overlays={} converted_functions={} unconverted_discoveries={} ordinary_unconverted_discoveries={} tiny_unconverted_discoveries={} contained_unconverted_discoveries={} returning_unconverted_discoveries={} ordinary_prologue_return_discoveries={} structural_veneer_discoveries={} data_walk_discoveries={}",
        overlays.len(),
        converted_functions,
        functions.len(),
        ordinary.len(),
        tiny,
        contained,
        returning,
        ordinary_prologue_return,
        veneers,
        data_walks
    );
    println!(
        "decoded_bytes={decoded_bytes} instruction_bytes={instruction_bytes} converted_instruction_bytes={converted_instruction_bytes} converted_span_bytes={converted_span_bytes} converted_internal_entries={converted_internal_entries} undiscovered_converted_functions={undiscovered_converted_functions} unresolved={unresolved} jump_tables={jump_tables}"
    );
    println!("duplicate_families={duplicate_families} duplicate_functions={duplicate_functions}");
    for family in families.iter().filter(|f| f.count > 1).take(options.top) {
        let sample = family.members.iter().take(12).cloned().collect::<Vec<_>>().join(",");
        let tail = if family.count > 12 { ",..." } else { "" };
        println!(
            "{}\tcount={}\toverlays={}\tbytes={}\t{}{}",
            family.fingerprint, family.count, family.overlays, family.code_bytes, sample, tail
        );
    }
    println!("report={}", options.output.display());

    Ok(())
}

fn iso_now() -> String {
    // Matches `new Date().toISOString()`: millisecond-precision UTC.
    let now = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap();
    let millis = now.as_millis() as i64;
    format_iso(millis)
}

fn format_iso(millis_since_epoch: i64) -> String {
    let secs = millis_since_epoch.div_euclid(1000);
    let ms = millis_since_epoch.rem_euclid(1000);
    let days = secs.div_euclid(86_400);
    let secs_of_day = secs.rem_euclid(86_400);
    let (year, month, day) = civil_from_days(days);
    let hour = secs_of_day / 3600;
    let minute = (secs_of_day % 3600) / 60;
    let second = secs_of_day % 60;
    format!("{year:04}-{month:02}-{day:02}T{hour:02}:{minute:02}:{second:02}.{ms:03}Z")
}

// Howard Hinnant's civil_from_days algorithm.
fn civil_from_days(z: i64) -> (i64, i64, i64) {
    let z = z + 719_468;
    let era = if z >= 0 { z } else { z - 146_096 } / 146_097;
    let doe = (z - era * 146_097) as i64;
    let yoe = (doe - doe / 1460 + doe / 36524 - doe / 146_096) / 365;
    let y = yoe + era * 400;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let d = doy - (153 * mp + 2) / 5 + 1;
    let m = if mp < 10 { mp + 3 } else { mp - 9 };
    let y = if m <= 2 { y + 1 } else { y };
    (y, m, d)
}
