use object::{Object, ObjectSection, ObjectSymbol, RelocationFlags, RelocationTarget};
use std::fs;
use std::path::{Path, PathBuf};

const TABLES: [(&str, &str); 3] = [
    ("SceneId_", "src/field/scenes.s"),
    ("ResourceId_", "src/system/resource/directory.s"),
    ("Msg", "text/messages.s"),
];

fn table(name: &str) -> Option<&'static str> {
    let [scene, resource, message] = TABLES;
    if name.starts_with(scene.0) {
        Some(scene.1)
    } else if name.starts_with(resource.0) {
        Some(resource.1)
    } else if ags::text::message_name(name) {
        Some(message.1)
    } else {
        None
    }
}

pub(crate) fn check(game: &Path, objects: &[(PathBuf, PathBuf)]) -> Result<(), String> {
    let mut problems = Vec::new();
    for (source, object) in objects {
        let bytes = fs::read(object).map_err(|error| format!("{}: {error}", object.display()))?;
        let ids = read(&bytes).map_err(|error| format!("{}: {error}", object.display()))?;
        problems.extend(violations(game, source, &ids));
    }
    if problems.is_empty() {
        Ok(())
    } else {
        Err(format!(
            "ids come only from their tables and are used whole:\n  {}\n\
             define each id by naming its row in the table, and load the id itself",
            problems.join("\n  ")
        ))
    }
}

#[derive(Default)]
struct Ids {
    defined: Vec<String>,
    uses: Vec<Use>,
}

struct Use {
    name: String,
    site: String,
    addend: Result<i64, u32>,
}

fn in_place(data: &[u8], offset: usize, kind: u32) -> Option<i64> {
    let width = data_width(kind)?;
    let place = data.get(offset..offset + width)?;
    Some(match width {
        4 => i64::from(i32::from_le_bytes(place.try_into().ok()?)),
        2 => i64::from(i16::from_le_bytes(place.try_into().ok()?)),
        _ => i64::from(place[0] as i8),
    })
}

fn data_width(kind: u32) -> Option<usize> {
    match kind {
        object::elf::R_ARM_ABS32 => Some(4),
        object::elf::R_ARM_ABS16 => Some(2),
        object::elf::R_ARM_ABS8 => Some(1),
        _ => None,
    }
}

fn read(bytes: &[u8]) -> Result<Ids, String> {
    let file = object::File::parse(bytes).map_err(|error| error.to_string())?;
    let mut ids = Ids::default();
    for symbol in file.symbols() {
        let Ok(name) = symbol.name() else {
            continue;
        };
        if !symbol.is_undefined() && table(name).is_some() {
            ids.defined.push(name.to_owned());
        }
    }
    for section in file.sections() {
        let data = section.data().map_err(|error| error.to_string())?;
        for (offset, relocation) in section.relocations() {
            let RelocationTarget::Symbol(index) = relocation.target() else {
                continue;
            };
            let symbol = file
                .symbol_by_index(index)
                .map_err(|error| error.to_string())?;
            let name = symbol.name().map_err(|error| error.to_string())?;
            if table(name).is_none() {
                continue;
            }
            let RelocationFlags::Elf { r_type } = relocation.flags() else {
                return Err(format!("{name} has a relocation that is not ELF"));
            };
            let addend = if relocation.has_implicit_addend() {
                in_place(data, offset as usize, r_type)
            } else {
                data_width(r_type).map(|_| relocation.addend())
            };
            ids.uses.push(Use {
                name: name.to_owned(),
                site: format!("{}+{offset:#x}", section.name().unwrap_or("?")),
                addend: addend.ok_or(r_type),
            });
        }
    }
    Ok(ids)
}

fn violations(game: &Path, source: &Path, ids: &Ids) -> Vec<String> {
    let mut problems = Vec::new();
    for name in &ids.defined {
        let owner = game.join(table(name).expect("an id"));
        if source != owner {
            problems.push(format!(
                "{} defines {name}, which only {} may define",
                source.display(),
                owner.display()
            ));
        }
    }
    for Use { name, site, addend } in &ids.uses {
        match addend {
            Ok(0) => {}
            Ok(addend) => problems.push(format!(
                "{} {site}: {name} {} {:#x}; use the id whole",
                source.display(),
                if *addend < 0 { '-' } else { '+' },
                addend.unsigned_abs()
            )),
            Err(kind) => problems.push(format!(
                "{} {site}: {name} fills a place of relocation type {kind}, not a data word",
                source.display()
            )),
        }
    }
    problems
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::gate::elf::{build, Symbol, ABSOLUTE, SECTION, UNDEFINED};

    const GAME: &str = "games/tbs";

    fn problems(source: &str, object: &[u8]) -> Vec<String> {
        violations(
            Path::new(GAME),
            &Path::new(GAME).join(source),
            &read(object).unwrap(),
        )
    }

    #[test]
    fn only_each_kinds_table_defines_its_ids() {
        let object = build(
            ".rodata",
            0,
            &[0; 8],
            &[
                Symbol("SceneId_WorldMap", 2, ABSOLUTE, true),
                Symbol("ResourceId_WindowTiles", 0x13, ABSOLUTE, true),
                Symbol("MsgSaving", 0x1a, ABSOLUTE, true),
                Symbol("Msg_Show", 0, SECTION, true),
                Symbol("MsgWindowTiles", 0, UNDEFINED, true),
            ],
            &[],
        );
        for (table, name) in [
            ("src/field/scenes.s", "SceneId_WorldMap"),
            ("src/system/resource/directory.s", "ResourceId_WindowTiles"),
            ("text/messages.s", "MsgSaving"),
        ] {
            let found = problems(table, &object);
            assert_eq!(found.len(), 2, "{found:?}");
            assert!(
                found.iter().all(|problem| !problem.contains(name)),
                "{found:?}"
            );
        }
        assert_eq!(
            problems("src/field/rows.s", &object),
            [
                "games/tbs/src/field/rows.s defines SceneId_WorldMap, which only games/tbs/src/field/scenes.s may define",
                "games/tbs/src/field/rows.s defines ResourceId_WindowTiles, which only games/tbs/src/system/resource/directory.s may define",
                "games/tbs/src/field/rows.s defines MsgSaving, which only games/tbs/text/messages.s may define",
            ]
        );
    }

    #[test]
    fn ids_are_used_whole_in_data_words() {
        let mut place = Vec::new();
        for word in [0u32, 4, 0xffff_fffe, 0] {
            place.extend_from_slice(&word.to_le_bytes());
        }
        place.extend_from_slice(&[0, 0, 1, 0]);
        place.extend_from_slice(&[0, 0xff, 0, 0]);
        let object = build(
            ".text",
            0,
            &place,
            &[
                Symbol("SceneId_WorldMap", 0, UNDEFINED, true),
                Symbol("ResourceId_WindowTiles", 0, UNDEFINED, true),
                Symbol("MsgSaving", 0, UNDEFINED, true),
                Symbol("Random16", 0, UNDEFINED, true),
            ],
            &[
                (0x0, 1, object::elf::R_ARM_ABS32),
                (0x4, 2, object::elf::R_ARM_ABS32),
                (0x8, 3, object::elf::R_ARM_ABS32),
                (0xc, 4, object::elf::R_ARM_THM_PC22),
                (0x10, 1, object::elf::R_ARM_ABS16),
                (0x12, 2, object::elf::R_ARM_ABS16),
                (0x14, 3, object::elf::R_ARM_ABS8),
                (0x15, 3, object::elf::R_ARM_ABS8),
                (0xc, 3, object::elf::R_ARM_THM_PC22),
            ],
        );
        assert_eq!(
            problems("src/scene.s", &object),
            [
                "games/tbs/src/scene.s .text+0x4: ResourceId_WindowTiles + 0x4; use the id whole",
                "games/tbs/src/scene.s .text+0x8: MsgSaving - 0x2; use the id whole",
                "games/tbs/src/scene.s .text+0x12: ResourceId_WindowTiles + 0x1; use the id whole",
                "games/tbs/src/scene.s .text+0x15: MsgSaving - 0x1; use the id whole",
                "games/tbs/src/scene.s .text+0xc: MsgSaving fills a place of relocation type 10, not a data word",
            ]
        );
    }

    #[test]
    fn the_failure_names_the_rule_and_the_remedy() {
        let directory = std::env::temp_dir().join(format!("alchemy-ids-{}", std::process::id()));
        fs::create_dir_all(&directory).unwrap();
        let object = directory.join("ROWS.o");
        fs::write(
            &object,
            build(
                ".rodata",
                0,
                &[0; 4],
                &[Symbol("SceneId_WorldMap", 2, ABSOLUTE, true)],
                &[],
            ),
        )
        .unwrap();
        let source = Path::new(GAME).join("src/field/rows.s");
        let error = check(Path::new(GAME), &[(source.clone(), object.clone())]).unwrap_err();
        let table = Path::new(GAME).join("src/field/scenes.s");
        let passes = check(Path::new(GAME), &[(table, object)]);
        fs::remove_dir_all(&directory).unwrap();
        assert!(error.starts_with("ids come only from their tables and are used whole:\n  "));
        assert!(error.contains("rows.s defines SceneId_WorldMap"), "{error}");
        assert!(error.ends_with("load the id itself"), "{error}");
        passes.unwrap();
    }
}
