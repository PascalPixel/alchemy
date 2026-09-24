//! What a retained overlay listing says about the bytes it spells.
//!
//! The listing is the one record of an overlay's layout: `AlchemyC_`,
//! `AlchemyData_` and `AlchemyRuntime_` placeholders reserve the bytes C and
//! the compiler runtime fill, `overlay_veneer` tables spell the fixed far-call
//! veneers, and a plain label such as `Scene_RunEntrySetup:` starts a
//! not-yet-C owner. An owner runs from its label to the next label,
//! placeholder or veneer table, or to the end of the listing, unless a
//! `.size Name, .-Name` line ends it sooner where code no owner claims yet
//! follows. Addresses come from sizing the listing's Thumb rows, and every
//! label that spells its own address is checked against that sizing.

use crate::overlay::assembly::OVERLAY_BASE;

/// What one stretch of a listing holds.
#[derive(Clone, Debug, Eq, PartialEq)]
pub enum Piece {
    /// An `AlchemyC_` placeholder: exact C fills it.
    C,
    /// An `AlchemyData_` placeholder: a unit's read-only data fills it.
    Data,
    /// An `AlchemyRuntime_` window: the container-built compiler runtime.
    Runtime,
    /// A table of `overlay_veneer` expansions: from the named `ENTRY.INC` or
    /// `IMPORT.INC`, or spelled inline in the listing when `None`.
    Veneers { include: Option<String> },
    /// A not-yet-C owner under its listing label.
    Owner { name: String },
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Item {
    pub piece: Piece,
    pub start: u32,
    pub end: u32,
}

impl Item {
    pub fn extent(&self) -> usize {
        (self.end - self.start) as usize
    }
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct Listing {
    /// Every placeholder, veneer table and owner, in address order.
    pub items: Vec<Item>,
    /// The address one past the listing's last byte.
    pub end: u32,
}

impl Listing {
    /// Each not-yet-C owner's name, entry and extent.
    pub fn owners(&self) -> impl Iterator<Item = (&str, u32, usize)> + '_ {
        self.items.iter().filter_map(|item| match &item.piece {
            Piece::Owner { name } => Some((name.as_str(), item.start, item.extent())),
            _ => None,
        })
    }
    /// Each `AlchemyC_` placeholder's entry and extent.
    pub fn placeholders(&self) -> impl Iterator<Item = (u32, usize)> + '_ {
        self.items
            .iter()
            .filter(|item| item.piece == Piece::C)
            .map(|item| (item.start, item.extent()))
    }
}

/// The address a reserved label spells, when the line is one.
fn addressed_label(line: &str) -> Option<(&str, u32)> {
    let label = line.strip_suffix(':')?;
    let (prefix, digits) = label.rsplit_once('_')?;
    if !matches!(
        prefix,
        "AlchemyC" | "AlchemyData" | "AlchemyRuntime" | "Overlay" | ".L"
    ) || digits.len() != 8
    {
        return None;
    }
    Some((prefix, u32::from_str_radix(digits, 16).ok()?))
}

/// A not-yet-C owner label: any other plain label.
fn owner_label(line: &str) -> Option<&str> {
    let name = line.strip_suffix(':')?;
    let mut characters = name.chars();
    let first = characters.next()?;
    ((first.is_ascii_alphabetic() || first == '_')
        && characters.all(|character| character.is_ascii_alphanumeric() || character == '_'))
    .then_some(name)
}

/// Whether a listing line is a not-yet-C owner label.
pub fn is_owner_label(line: &str) -> bool {
    let line = line.trim();
    addressed_label(line).is_none() && owner_label(line).is_some()
}

/// The veneers one `.irp` line expands `overlay_veneer` over.
fn irp_count(line: &str) -> Option<u32> {
    let arguments = line.strip_prefix(".irp")?;
    Some(arguments.split(',').count().saturating_sub(1) as u32)
}

/// The veneers an `ENTRY.INC` or `IMPORT.INC` body spells: `.irp` tables and
/// single `overlay_veneer` lines, nothing else.
fn include_veneers(path: &str, body: &str) -> Result<u32, String> {
    let (mut count, mut repeat) = (0, None);
    for line in body.lines().map(str::trim).filter(|line| !line.is_empty()) {
        if let Some(entries) = irp_count(line) {
            repeat = Some(entries);
        } else if line == ".endr" {
            repeat = None;
        } else if line.starts_with("overlay_veneer ") {
            count += repeat.unwrap_or(1);
        } else {
            return Err(format!("{path}: {line:?} is not an overlay_veneer table"));
        }
    }
    Ok(count)
}

/// The bytes one row assembles to.
fn row_size(line: &str) -> Result<u32, String> {
    let mnemonic = line.split_whitespace().next().unwrap_or_default();
    Ok(match mnemonic {
        "" | ".syntax" | ".thumb" | ".global" | ".set" | ".thumb_func" => 0,
        ".4byte" => 4,
        ".2byte" => 2,
        ".byte" => 1,
        ".space" => crate::compiler::overlay::space_size(line)
            .and_then(|size| u32::try_from(size).ok())
            .ok_or_else(|| format!("unreadable row {line:?}"))?,
        "bl" => 4,
        directive if directive.starts_with('.') => {
            return Err(format!("unsupported listing directive {line:?}"))
        }
        _ => 2,
    })
}

/// Lays out one listing. `include` reads an included file by the path the
/// listing spells; `veneer_macro` is the game's shared macro file, which
/// assembles to nothing.
pub fn parse(
    text: &str,
    veneer_macro: &str,
    include: &mut dyn FnMut(&str) -> Result<String, String>,
) -> Result<Listing, String> {
    let lines = text.lines().map(str::trim).collect::<Vec<_>>();
    // Boundaries in listing order: (start, end or None for owners, piece).
    let mut pieces: Vec<(u32, Option<u32>, Piece)> = Vec::new();
    let mut address = OVERLAY_BASE as u32;
    let mut index = 0;
    while index < lines.len() {
        let line = lines[index];
        index += 1;
        if let Some((prefix, spelled)) = addressed_label(line) {
            if spelled != address {
                return Err(format!(
                    "{line} lies at 0x{address:08x}, not the address it spells"
                ));
            }
            let piece = match prefix {
                "AlchemyC" => Piece::C,
                "AlchemyData" => Piece::Data,
                "AlchemyRuntime" => Piece::Runtime,
                _ => continue,
            };
            let mut span = 0u32;
            while let Some(next) = lines.get(index) {
                if let Some(size) = crate::compiler::overlay::space_size(next) {
                    span += u32::try_from(size).map_err(|_| format!("{line} is too large"))?;
                } else if !(piece == Piece::C && next.starts_with(".L_") && next.ends_with(':')) {
                    break;
                } else if addressed_label(next).map(|(_, at)| at) != Some(address + span) {
                    return Err(format!(
                        "{next} inside {line} lies at 0x{:08x}",
                        address + span
                    ));
                }
                index += 1;
            }
            if span == 0 {
                return Err(format!("{line} reserves no bytes"));
            }
            pieces.push((address, Some(address + span), piece));
            address += span;
        } else if let Some(name) = owner_label(line) {
            pieces.push((address, None, Piece::Owner { name: name.into() }));
        } else if let Some(size) = line.strip_prefix(".size ") {
            let open = pieces.last_mut().filter(|(_, end, piece)| {
                end.is_none()
                    && matches!(piece, Piece::Owner { name }
                        if size.split_whitespace().collect::<String>() == format!("{name},.-{name}"))
            });
            let Some((_, end, _)) = open else {
                return Err(format!("{line:?} does not end the owner before it"));
            };
            *end = Some(address);
        } else if let Some(path) = line
            .strip_prefix(".include \"")
            .and_then(|rest| rest.strip_suffix('"'))
        {
            if path == veneer_macro {
                continue;
            }
            let body = include(path)?;
            let bytes = 8 * include_veneers(path, &body)?;
            if bytes == 0 {
                return Err(format!("{path} spells no veneers"));
            }
            pieces.push((
                address,
                Some(address + bytes),
                Piece::Veneers {
                    include: Some(path.into()),
                },
            ));
            address += bytes;
        } else if let Some(entries) = irp_count(line) {
            if lines
                .get(index)
                .map(|next| next.starts_with("overlay_veneer "))
                != Some(true)
                || lines.get(index + 1) != Some(&".endr")
            {
                return Err(format!("{line:?} is not an overlay_veneer table"));
            }
            index += 2;
            let bytes = 8 * entries;
            pieces.push((
                address,
                Some(address + bytes),
                Piece::Veneers { include: None },
            ));
            address += bytes;
        } else if line.starts_with("overlay_veneer ") {
            pieces.push((address, Some(address + 8), Piece::Veneers { include: None }));
            address += 8;
        } else {
            address += row_size(line)?;
        }
    }
    let end = address;
    let mut items = Vec::with_capacity(pieces.len());
    for (position, (start, fixed, piece)) in pieces.iter().enumerate() {
        let end = match fixed {
            Some(end) => *end,
            None => pieces.get(position + 1).map_or(end, |(next, _, _)| *next),
        };
        if end <= *start {
            let Piece::Owner { name } = piece else {
                unreachable!("fixed pieces reserve bytes")
            };
            return Err(format!(
                "owner label {name} at 0x{start:08x} marks no bytes"
            ));
        }
        items.push(Item {
            piece: piece.clone(),
            start: *start,
            end,
        });
    }
    let mut names = std::collections::BTreeSet::new();
    for (name, _, _) in (Listing {
        items: items.clone(),
        end,
    })
    .owners()
    {
        if !names.insert(name.to_string()) {
            return Err(format!("owner label {name} appears twice"));
        }
    }
    Ok(Listing { items, end })
}

/// Lays out a listing on disk, reading its includes under `root`.
pub fn read(
    root: &std::path::Path,
    path: &std::path::Path,
    veneer_macro: &str,
) -> Result<Listing, String> {
    let text =
        std::fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    parse(&text, veneer_macro, &mut |include| {
        std::fs::read_to_string(root.join(include)).map_err(|error| format!("{include}: {error}"))
    })
    .map_err(|error| format!("{}: {error}", path.display()))
}

#[cfg(test)]
mod tests {
    use super::*;

    const MACRO: &str = "SRC/SYSTEM/OVERLAY.INC";

    fn layout(text: &str) -> Result<Listing, String> {
        parse(text, MACRO, &mut |path| match path {
            "SRC/A/ENTRY.INC" => Ok(".irp T, 1, 2, 3\n\toverlay_veneer \\T\n.endr\n".into()),
            _ => Err(format!("{path}: missing")),
        })
    }

    #[test]
    fn owners_run_to_the_next_boundary() {
        let listing = layout(
            ".syntax unified\n.include \"SRC/SYSTEM/OVERLAY.INC\"\n\t.thumb\n\t.set sub_02000100, 0x02000100\n\
             Overlay_02000000:\n\t.include \"SRC/A/ENTRY.INC\"\nAlchemyC_02000018:\n\t.space 0x8\n.L_02000020:\n\t.space 0x4\n\
             \t.2byte 0x0000\nScene_Run:\n\tpush\t{lr}\n\tbl\tsub_02000100\n.L_0200002c:\n\tpop\t{r0}\n\t.4byte 0x1\n\
             Func_02000032:\n\tbx\tlr\n\t.size Func_02000032, .-Func_02000032\n\tbx\tlr\n\
             AlchemyRuntime_02000036:\n\t.space 0x2\n",
        )
        .unwrap();
        assert_eq!(listing.end, 0x0200_0038);
        assert_eq!(
            listing.owners().collect::<Vec<_>>(),
            [
                ("Scene_Run", 0x0200_0026, 12),
                ("Func_02000032", 0x0200_0032, 2)
            ]
        );
        assert_eq!(
            listing.placeholders().collect::<Vec<_>>(),
            [(0x0200_0018, 12)]
        );
        assert_eq!(
            listing.items[0],
            Item {
                piece: Piece::Veneers {
                    include: Some("SRC/A/ENTRY.INC".into())
                },
                start: 0x0200_0000,
                end: 0x0200_0018
            }
        );
    }

    #[test]
    fn misplaced_labels_and_empty_owners_refuse() {
        assert!(layout("\tmovs\tr0, #0\nAlchemyC_02000000:\n\t.space 0x2\n")
            .unwrap_err()
            .contains("not the address it spells"));
        assert!(layout("Name:\nAlchemyC_02000000:\n\t.space 0x2\n")
            .unwrap_err()
            .contains("marks no bytes"));
        assert!(layout("Name:\n\tbx\tlr\nName:\n\tbx\tlr\n")
            .unwrap_err()
            .contains("appears twice"));
        assert!(layout("\t.align 2\n").unwrap_err().contains("unsupported"));
        assert!(layout("Name:\n\tbx\tlr\n\t.size Other, .-Other\n")
            .unwrap_err()
            .contains("does not end the owner"));
    }
}
