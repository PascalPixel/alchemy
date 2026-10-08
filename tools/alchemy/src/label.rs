use crate::compiler::routing::root;

const USAGE: &str = r#"usage: alchemy label NAME ENGLISH_ADDRESS LENGTH [--game tbs|tla]
Finds LENGTH bytes at ENGLISH_ADDRESS (hexadecimal, 0x08......) of the
English ROM in each other edition's ROM, nearest the same address, and
splits that edition's recon/<game>/<lang>/rom.s incbin there with
".global NAME" and "NAME:". For data adopted C references that only the
English scaffold labelled. The game defaults to tbs."#;

pub(crate) fn find_all(haystack: &[u8], needle: &[u8]) -> Vec<usize> {
    if needle.is_empty() || needle.len() > haystack.len() {
        return Vec::new();
    }
    (0..=haystack.len() - needle.len())
        .step_by(2)
        .filter(|&i| haystack[i..i + needle.len()] == *needle)
        .collect()
}

pub(crate) fn split_at(text: &str, offset: u32, name: &str) -> Option<String> {
    if text.lines().any(|l| l == format!("{name}:")) {
        return None;
    }
    let mut out = Vec::new();
    let mut done = false;
    for line in text.lines() {
        let range = line
            .strip_prefix("\t.incbin \"baserom.gba\", 0x")
            .and_then(|r| r.split_once(", 0x"))
            .and_then(|(o, s)| Some((u32::from_str_radix(o, 16).ok()?, u32::from_str_radix(s.trim(), 16).ok()?)));
        match range {
            Some((start, size)) if !done && start <= offset && offset < start + size => {
                if offset > start {
                    out.push(format!("\t.incbin \"baserom.gba\", 0x{start:08x}, 0x{:08x}", offset - start));
                }
                out.push(format!("\t.global {name}"));
                out.push(format!("{name}:"));
                out.push(format!("\t.incbin \"baserom.gba\", 0x{offset:08x}, 0x{:08x}", start + size - offset));
                done = true;
            }
            _ => out.push(line.to_string()),
        }
    }
    done.then(|| out.join("\n") + "\n")
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let mut positional = Vec::new();
    let mut game = "tbs";
    let mut i = 0;
    while i < arguments.len() {
        match arguments[i].as_str() {
            "--game" => {
                i += 1;
                game = match arguments.get(i).map(String::as_str) {
                    Some("tbs") => "tbs",
                    Some("tla") => "tla",
                    _ => return Err(USAGE.into()),
                };
            }
            "-h" | "--help" => return Err(USAGE.into()),
            value => positional.push(value.to_string()),
        }
        i += 1;
    }
    let [name, address, length] = positional.as_slice() else {
        return Err(USAGE.into());
    };
    let address = u32::from_str_radix(address.trim_start_matches("0x"), 16).map_err(|_| USAGE.to_string())?;
    let offset = address.checked_sub(0x0800_0000).ok_or_else(|| USAGE.to_string())? as usize;
    let length: usize = length.parse().map_err(|_| USAGE.to_string())?;
    let root = root();
    let read = |edition: &str| {
        let path = root.join(format!("roms/{game}-{edition}.gba"));
        std::fs::read(&path).map_err(|e| format!("{}: {e}", path.display()))
    };
    let english = read("en")?;
    let needle = english.get(offset..offset + length).ok_or("address beyond the English ROM")?.to_vec();
    for lang in ["ja", "de", "es", "fr", "it"] {
        let hits = find_all(&read(lang)?, &needle);
        let Some(&at) = hits.iter().min_by_key(|&&h| h.abs_diff(offset)) else {
            println!("{game}-{lang}: bytes not found");
            continue;
        };
        let scaffold = root.join(format!("recon/{game}/{lang}/rom.s"));
        let text = std::fs::read_to_string(&scaffold).map_err(|e| e.to_string())?;
        match split_at(&text, at as u32, name) {
            Some(new) => {
                std::fs::write(&scaffold, new).map_err(|e| e.to_string())?;
                println!("{game}-{lang}: {name} at {at:#x}{}", if hits.len() > 1 { format!(" (nearest of {})", hits.len()) } else { String::new() });
            }
            None => println!("{game}-{lang}: {name} already labelled, or {at:#x} is not scaffold"),
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn splits_the_covering_incbin() {
        let text = "\t.section .rom.00000100, \"ax\"\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000040\n";
        let new = split_at(text, 0x120, "Table").unwrap();
        assert_eq!(new, "\t.section .rom.00000100, \"ax\"\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000020\n\t.global Table\nTable:\n\t.incbin \"baserom.gba\", 0x00000120, 0x00000020\n");
        assert!(split_at(&new, 0x120, "Table").is_none());
        assert_eq!(find_all(&[1, 2, 3, 1, 2, 3, 1, 2], &[1, 2]), [0, 6]);
    }
}
