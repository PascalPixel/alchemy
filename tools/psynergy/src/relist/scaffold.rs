//! Assembly scaffolds that hold not-yet-sourced bytes: `.incbin` data read
//! from the base image and `.space` reserved RAM, each in named sections
//! with global labels. A scaffold is read into ranges and labels by address
//! and written back with whatever labels and splits a relisting needs.

use std::collections::BTreeMap;

/// One scaffold item.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Item {
    Label(String),
    /// `.incbin FILE, OFFSET, SIZE`.
    Bytes {
        file: String,
        offset: u32,
        size: u32,
    },
    /// `.space SIZE`.
    Space(u32),
}

/// One `.section` of a scaffold: its name, the rest of its directive line,
/// and its items in order.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Section {
    pub name: String,
    pub flags: String,
    pub items: Vec<Item>,
}

impl Section {
    pub fn size(&self) -> u32 {
        self.items
            .iter()
            .map(|item| match item {
                Item::Bytes { size, .. } => *size,
                Item::Space(size) => *size,
                Item::Label(_) => 0,
            })
            .sum()
    }
}

/// A scaffold file: its leading comment lines and its sections.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Scaffold {
    pub header: Vec<String>,
    pub sections: Vec<Section>,
}

fn number(text: &str) -> Result<u32, String> {
    let text = text.trim();
    match text.strip_prefix("0x") {
        Some(hex) => u32::from_str_radix(hex, 16),
        None => text.parse(),
    }
    .map_err(|_| format!("not a number: {text}"))
}

/// Read a scaffold of `.section`, `.global`, labels, `.incbin` and `.space`
/// lines, with `@` comments before its first section.
pub fn parse(text: &str) -> Result<Scaffold, String> {
    let mut scaffold = Scaffold {
        header: Vec::new(),
        sections: Vec::new(),
    };
    for (index, line) in text.lines().enumerate() {
        let fail = |what: &str| format!("line {}: {what}: {line}", index + 1);
        let trimmed = line.trim();
        if trimmed.is_empty() {
            continue;
        }
        if trimmed.starts_with('@') {
            if !scaffold.sections.is_empty() {
                return Err(fail("a comment inside a section"));
            }
            scaffold.header.push(line.to_string());
            continue;
        }
        if let Some(rest) = trimmed.strip_prefix(".section") {
            let rest = rest.trim();
            let (name, flags) = rest.split_once(',').unwrap_or((rest, ""));
            scaffold.sections.push(Section {
                name: name.trim().to_string(),
                flags: flags.trim().to_string(),
                items: Vec::new(),
            });
            continue;
        }
        let section = scaffold
            .sections
            .last_mut()
            .ok_or_else(|| fail("an item before any section"))?;
        if trimmed.starts_with(".global") || trimmed.starts_with(".globl") {
            continue;
        }
        if let Some(name) = trimmed.strip_suffix(':') {
            section.items.push(Item::Label(name.to_string()));
        } else if let Some(rest) = trimmed.strip_prefix(".incbin") {
            let fields: Vec<&str> = rest.split(',').collect();
            let [file, offset, size] = fields.as_slice() else {
                return Err(fail("an .incbin without offset and size"));
            };
            section.items.push(Item::Bytes {
                file: file.trim().trim_matches('"').to_string(),
                offset: number(offset)?,
                size: number(size)?,
            });
        } else if let Some(rest) = trimmed.strip_prefix(".space") {
            section.items.push(Item::Space(number(rest)?));
        } else {
            return Err(fail("not a scaffold line"));
        }
    }
    Ok(scaffold)
}

/// Write a scaffold back in the form `parse` reads, every label global.
pub fn render(scaffold: &Scaffold) -> String {
    let mut text = String::new();
    for line in &scaffold.header {
        text.push_str(line);
        text.push('\n');
    }
    for section in &scaffold.sections {
        if section.flags.is_empty() {
            text.push_str(&format!("\t.section {}\n", section.name));
        } else {
            text.push_str(&format!("\t.section {},{}\n", section.name, section.flags));
        }
        for item in &section.items {
            match item {
                Item::Label(name) => text.push_str(&format!("\t.global {name}\n{name}:\n")),
                Item::Bytes { file, offset, size } => text.push_str(&format!(
                    "\t.incbin \"{file}\", 0x{offset:08x}, 0x{size:08x}\n"
                )),
                Item::Space(size) => text.push_str(&format!("\t.space 0x{size:08x}\n")),
            }
        }
    }
    text
}

/// A section laid out at its placed address: every label's address, and its
/// bytes as `(address, end, file, file offset)` runs or reserved runs.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Layout {
    pub name: String,
    pub flags: String,
    pub start: u32,
    pub end: u32,
    pub labels: BTreeMap<u32, Vec<String>>,
    /// Included runs: start, end, file and the file offset of `start`.
    pub bytes: Vec<(u32, u32, String, u32)>,
}

impl Layout {
    pub fn new(section: &Section, start: u32) -> Layout {
        let mut layout = Layout {
            name: section.name.clone(),
            flags: section.flags.clone(),
            start,
            end: start,
            labels: BTreeMap::new(),
            bytes: Vec::new(),
        };
        for item in &section.items {
            match item {
                Item::Label(name) => layout
                    .labels
                    .entry(layout.end)
                    .or_default()
                    .push(name.clone()),
                Item::Bytes { file, offset, size } => {
                    layout
                        .bytes
                        .push((layout.end, layout.end + size, file.clone(), *offset));
                    layout.end += size;
                }
                Item::Space(size) => layout.end += size,
            }
        }
        layout
    }

    /// The part of this section in `[from, to)`, named for its start.
    /// A label at the section's own end stays with its last slice.
    pub fn slice(&self, from: u32, to: u32, name: String) -> Layout {
        let last = if to == self.end { to + 1 } else { to };
        Layout {
            name,
            flags: self.flags.clone(),
            start: from,
            end: to,
            labels: self
                .labels
                .range(from..last)
                .map(|(address, names)| (*address, names.clone()))
                .collect(),
            bytes: self
                .bytes
                .iter()
                .filter(|(start, end, ..)| *start < to && *end > from)
                .map(|(start, end, file, offset)| {
                    let clipped = (*start).max(from);
                    (
                        clipped,
                        (*end).min(to),
                        file.clone(),
                        offset + (clipped - start),
                    )
                })
                .collect(),
        }
    }

    /// Items that lay this section out again: bytes or space split at every
    /// label.
    pub fn section(&self) -> Section {
        let mut items = Vec::new();
        let mut cursor = self.start;
        let mut marks: std::collections::BTreeSet<u32> = self
            .labels
            .keys()
            .map(|mark| (*mark).min(self.end))
            .collect();
        marks.insert(self.end);
        for mark in marks {
            while cursor < mark {
                match self
                    .bytes
                    .iter()
                    .find(|(start, end, ..)| *start <= cursor && cursor < *end)
                {
                    Some((start, end, file, offset)) => {
                        let stop = (*end).min(mark);
                        items.push(Item::Bytes {
                            file: file.clone(),
                            offset: offset + (cursor - start),
                            size: stop - cursor,
                        });
                        cursor = stop;
                    }
                    None => {
                        let stop = self
                            .bytes
                            .iter()
                            .map(|(start, ..)| *start)
                            .filter(|start| *start > cursor)
                            .min()
                            .unwrap_or(mark)
                            .min(mark);
                        items.push(Item::Space(stop - cursor));
                        cursor = stop;
                    }
                }
            }
            if let Some(names) = self.labels.get(&mark) {
                items.extend(names.iter().cloned().map(Item::Label));
            }
        }
        Section {
            name: self.name.clone(),
            flags: self.flags.clone(),
            items,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    const INCBIN: &str = "@ header\n\t.section .unidentified.08000100,\"a\"\n\t.global Head\nHead:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000020\n";

    #[test]
    fn a_scaffold_reads_and_writes_back_unchanged() {
        let scaffold = parse(INCBIN).unwrap();
        assert_eq!(scaffold.header, vec!["@ header"]);
        assert_eq!(
            scaffold.sections[0].items[1],
            Item::Bytes {
                file: "baserom.gba".into(),
                offset: 0x100,
                size: 0x20
            }
        );
        assert_eq!(render(&scaffold), INCBIN);
        let space = "\t.section .sym,\"aw\",%nobits\n\t.space 0x00000010\n\t.global gX\ngX:\n\t.space 0x00000004\n";
        assert_eq!(render(&parse(space).unwrap()), space);
    }

    #[test]
    fn a_label_splits_the_bytes_it_lands_in_and_slices_keep_offsets() {
        let scaffold = parse(INCBIN).unwrap();
        let mut layout = Layout::new(&scaffold.sections[0], 0x0800_0100);
        assert_eq!(layout.end, 0x0800_0120);
        layout
            .labels
            .entry(0x0800_0108)
            .or_default()
            .push("Data_08000108".into());
        let items = layout.section().items;
        assert_eq!(
            items,
            vec![
                Item::Label("Head".into()),
                Item::Bytes {
                    file: "baserom.gba".into(),
                    offset: 0x100,
                    size: 8
                },
                Item::Label("Data_08000108".into()),
                Item::Bytes {
                    file: "baserom.gba".into(),
                    offset: 0x108,
                    size: 0x18
                },
            ]
        );
        let tail = layout.slice(0x0800_0110, 0x0800_0120, ".unidentified.08000110".into());
        assert_eq!(
            tail.section().items,
            vec![Item::Bytes {
                file: "baserom.gba".into(),
                offset: 0x110,
                size: 0x10
            }]
        );
    }

    #[test]
    fn a_label_at_the_end_of_a_section_stays_with_it() {
        let space =
            parse("\t.section .sym,\"aw\",%nobits\n\t.space 0x00000010\n\t.global gEnd\ngEnd:\n")
                .unwrap();
        let layout = Layout::new(&space.sections[0], 0x0200_0000);
        let whole = layout.slice(0x0200_0000, 0x0200_0010, ".sym".into());
        assert_eq!(
            whole.section().items,
            vec![Item::Space(16), Item::Label("gEnd".into())]
        );
        let head = layout.slice(0x0200_0000, 0x0200_0008, ".sym".into());
        assert_eq!(head.section().items, vec![Item::Space(8)]);
    }

    #[test]
    fn a_label_in_reserved_space_splits_it() {
        let space = parse("\t.section .sym,\"aw\",%nobits\n\t.space 0x00000010\n").unwrap();
        let mut layout = Layout::new(&space.sections[0], 0x0200_0000);
        layout
            .labels
            .entry(0x0200_0004)
            .or_default()
            .push("Data_02000004".into());
        assert_eq!(
            layout.section().items,
            vec![
                Item::Space(4),
                Item::Label("Data_02000004".into()),
                Item::Space(12)
            ]
        );
    }
}
