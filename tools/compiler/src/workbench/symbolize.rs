use regex::Regex;
use std::{collections::BTreeMap, path::Path};

#[derive(Debug, Clone, Copy, Eq, PartialEq)]
pub struct SymbolizeStats {
    pub jump_tables: usize,
    pub table_entries: usize,
    pub references: usize,
}

#[derive(Debug, Clone)]
struct JumpTable {
    start: usize,
    end: usize,
    address: u64,
    targets: Vec<u64>,
    loader: usize,
}

pub fn symbolize(
    source: &Path,
    listing: &Path,
    relocatable: bool,
) -> Result<(String, SymbolizeStats), String> {
    let source_text = std::fs::read_to_string(source)
        .map_err(|error| format!("{}: {error}", source.display()))?;
    let listing_text = std::fs::read_to_string(listing)
        .map_err(|error| format!("{}: {error}", listing.display()))?;
    symbolize_text(source, &source_text, &listing_text, relocatable)
}

fn symbolize_text(
    source: &Path,
    source_text: &str,
    listing_text: &str,
    relocatable: bool,
) -> Result<(String, SymbolizeStats), String> {
    let hex = Regex::new(r"(?i)([0-9a-f]{8})").unwrap();
    let stem = source
        .file_stem()
        .and_then(|value| value.to_str())
        .unwrap_or("");
    let base = hex
        .captures(stem)
        .and_then(|capture| u64::from_str_radix(&capture[1], 16).ok())
        .ok_or_else(|| format!("cannot derive image address from {}", source.display()))?;
    let lines: Vec<&str> = source_text.lines().collect();
    let listing_row = Regex::new(r"^\s*(\d+)\s+([0-9A-Fa-f]{4,8})\s+").unwrap();
    let mut line_address = BTreeMap::<usize, u64>::new();
    for row in listing_text.lines() {
        let Some(capture) = listing_row.captures(row) else {
            continue;
        };
        let line = capture[1]
            .parse::<usize>()
            .map_err(|error| error.to_string())?;
        let address = u64::from_str_radix(&capture[2], 16).map_err(|error| error.to_string())?;
        line_address.insert(line, address);
    }
    let mut address_line = BTreeMap::<u64, usize>::new();
    for (&line, &address) in &line_address {
        address_line.entry(address).or_insert(line);
    }

    let mov_pc = Regex::new(r"\bmov\s+pc,\s*r\d+").unwrap();
    let word = Regex::new(r"\.4byte\s+(0x[0-9A-Fa-f]+)").unwrap();
    let pc_load = Regex::new(r"\[pc,\s*#([0-9]+)\]").unwrap();
    let mut tables = Vec::new();
    let mut index = 0;
    while index < lines.len() {
        if !mov_pc.is_match(lines[index]) {
            index += 1;
            continue;
        }
        let mut cursor = index + 1;
        while cursor < lines.len() && !word.is_match(lines[cursor]) {
            cursor += 1;
        }
        let start = cursor;
        let mut targets = Vec::new();
        while cursor < lines.len() {
            let Some(capture) = word.captures(lines[cursor]) else {
                break;
            };
            targets.push(
                u64::from_str_radix(capture[1].trim_start_matches("0x"), 16)
                    .map_err(|error| error.to_string())?,
            );
            cursor += 1;
        }
        if targets.is_empty() {
            return Err(format!("no jump table after source line {}", index + 1));
        }
        let address = base
            + line_address
                .get(&(start + 1))
                .copied()
                .ok_or_else(|| format!("listing omits source line {}", start + 1))?;
        let loader = (index.saturating_sub(24)..index)
            .rev()
            .find(|candidate| pc_load.is_match(lines[*candidate]))
            .ok_or_else(|| format!("no PC-relative jump-table load before line {}", index + 1))?;
        tables.push(JumpTable {
            start,
            end: cursor,
            address,
            targets,
            loader,
        });
        index = cursor;
    }

    let mut inserts = BTreeMap::<usize, Vec<String>>::new();
    let mut rewrites = BTreeMap::<usize, String>::new();
    for (table_index, table) in tables.iter().enumerate() {
        let ordinal = table_index + 1;
        let table_label = format!(".Lm2c_jtbl_{ordinal}");
        let pointer_label = format!(".Lm2c_jtbl_ptr_{ordinal}");
        inserts
            .entry(table.start)
            .or_default()
            .push(format!("{table_label}:"));
        rewrites.insert(
            table.loader,
            pc_load
                .replace(lines[table.loader], pointer_label.as_str())
                .into_owned(),
        );
        for (source_index, line) in lines.iter().enumerate().take(table.end).skip(table.start) {
            let target = table.targets[source_index - table.start];
            let target_label = format!(".Lm2c_{target:08x}");
            let mut replacement = target_label.clone();
            if !relocatable {
                replacement.push_str(&format!(" + 0x{base:08x}"));
            }
            rewrites.insert(
                source_index,
                word.replace(line, format!(".4byte {replacement}"))
                    .into_owned(),
            );
            let target_line = address_line
                .get(&target.saturating_sub(base))
                .copied()
                .ok_or_else(|| format!("no source line for jump target 0x{target:08x}"))?;
            inserts
                .entry(target_line - 1)
                .or_default()
                .push(format!("{target_label}:"));
        }
        let pointer_text = format!("0x{:08x}", table.address);
        for (source_index, line) in lines.iter().enumerate() {
            if (table.start..table.end).contains(&source_index) || !line.contains(&pointer_text) {
                continue;
            }
            let mut replacement = table_label.clone();
            if !relocatable {
                replacement.push_str(&format!(" + 0x{base:08x}"));
            }
            rewrites.insert(source_index, line.replace(&pointer_text, &replacement));
            inserts
                .entry(source_index)
                .or_default()
                .push(format!("{pointer_label}:"));
        }
    }

    for (source_index, line) in lines.iter().enumerate() {
        if rewrites.contains_key(&source_index) {
            continue;
        }
        let Some(capture) = pc_load.captures(line) else {
            continue;
        };
        let instruction = line_address
            .get(&(source_index + 1))
            .copied()
            .ok_or_else(|| format!("listing omits PC load on source line {}", source_index + 1))?;
        let displacement = capture[1]
            .parse::<u64>()
            .map_err(|error| error.to_string())?;
        let pool_address = ((instruction + 4) & !3) + displacement;
        let pool_line = address_line
            .get(&pool_address)
            .copied()
            .ok_or_else(|| format!("no source line for literal pool offset 0x{pool_address:x}"))?;
        let pool_label = format!(".Lm2c_pool_{pool_address:04x}");
        inserts
            .entry(pool_line - 1)
            .or_default()
            .push(format!("{pool_label}:"));
        rewrites.insert(
            source_index,
            pc_load.replace(line, pool_label.as_str()).into_owned(),
        );
    }

    let mut output = String::new();
    for (source_index, line) in lines.iter().enumerate() {
        if let Some(labels) = inserts.get(&source_index) {
            let mut emitted = Vec::<&str>::new();
            for label in labels {
                if emitted.iter().any(|old| *old == label) {
                    continue;
                }
                emitted.push(label);
                output.push_str(label);
                output.push('\n');
            }
        }
        output.push_str(
            rewrites
                .get(&source_index)
                .map(String::as_str)
                .unwrap_or(line),
        );
        output.push('\n');
    }
    Ok((
        output,
        SymbolizeStats {
            jump_tables: tables.len(),
            table_entries: tables.iter().map(|table| table.targets.len()).sum(),
            references: rewrites.len(),
        },
    ))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn labels_jump_table_targets_and_pointer_pool() {
        let source = Path::new("08000000.s");
        let asm = ".thumb\nldr r2, [pc, #8]\nmov pc, r2\n.4byte 0x08000008\nnop\n.align 2\n.4byte 0x08000004\n";
        let listing = "1 0000  asm\n2 0000 0000 asm\n3 0002 0000 asm\n4 0004 00000000 asm\n5 0008 0000 asm\n6 000a 00 asm\n7 000c 00000000 asm\n";
        let (output, stats) = symbolize_text(source, asm, listing, true).unwrap();
        assert_eq!(stats.jump_tables, 1);
        assert_eq!(stats.table_entries, 1);
        assert!(output.contains("ldr r2, .Lm2c_jtbl_ptr_1"));
        assert!(output.contains(".4byte .Lm2c_08000008"));
        assert!(output.contains(".Lm2c_08000008:\nnop"));
        assert!(output.contains(".Lm2c_jtbl_ptr_1:\n.4byte .Lm2c_jtbl_1"));
    }
}
