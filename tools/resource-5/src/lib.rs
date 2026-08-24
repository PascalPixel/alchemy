use canonical_json::canonical_json;
use serde_json::{Map, Value};
use std::collections::HashSet;
use std::fs;
use std::io::{self, Write};

pub const ADDRESS: usize = 0x0807_a828;
pub const END: usize = 0x0808_a000;
pub const ALIGNMENT: usize = 0x0808_962c;
pub const RESOURCE_SIZE: usize = END - ADDRESS;

pub type Result<T> = std::result::Result<T, String>;

fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(message.into())
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    value
        .as_object()
        .ok_or_else(|| format!("{label} must be an object"))
}

fn field<'a>(value: &'a Map<String, Value>, key: &str) -> Result<&'a Value> {
    value.get(key).ok_or_else(|| format!("missing field {key}"))
}

fn exact_keys(value: &Map<String, Value>, expected: &[&str], label: &str) -> Result<()> {
    let mut actual: Vec<&str> = value.keys().map(String::as_str).collect();
    let mut wanted = expected.to_vec();
    actual.sort_unstable();
    wanted.sort_unstable();
    if actual != wanted {
        return err(format!("{label} fields differ"));
    }
    Ok(())
}

fn sparse_keys(
    value: &Map<String, Value>,
    allowed: &[&str],
    required: &[&str],
    label: &str,
) -> Result<()> {
    if value.keys().any(|key| !allowed.contains(&key.as_str()))
        || required.iter().any(|key| !value.contains_key(*key))
    {
        return err(format!("{label} fields differ"));
    }
    Ok(())
}

fn list<'a>(value: &'a Value, size: usize, label: &str) -> Result<&'a Vec<Value>> {
    let entries = value
        .as_array()
        .ok_or_else(|| format!("{label} must contain {size} entries"))?;
    if entries.len() != size {
        return err(format!("{label} must contain {size} entries"));
    }
    Ok(entries)
}

fn integer(value: &Value, minimum: i128, maximum: i128, label: &str) -> Result<i128> {
    let number = value
        .as_i64()
        .map(i128::from)
        .or_else(|| value.as_u64().map(i128::from))
        .ok_or_else(|| format!("{label} must be an integer"))?;
    if !(minimum..=maximum).contains(&number) {
        return err(format!("{label} is outside {minimum}..{maximum}"));
    }
    Ok(number)
}

fn unsigned(value: &Value, maximum: u64, label: &str) -> Result<u64> {
    Ok(integer(value, 0, maximum as i128, label)? as u64)
}

fn s8(value: &Value, label: &str) -> Result<i8> {
    Ok(integer(value, -0x80, 0x7f, label)? as i8)
}
fn u8(value: &Value, label: &str) -> Result<u8> {
    Ok(unsigned(value, 0xff, label)? as u8)
}
fn s16(value: &Value, label: &str) -> Result<i16> {
    Ok(integer(value, -0x8000, 0x7fff, label)? as i16)
}
fn u16(value: &Value, label: &str) -> Result<u16> {
    Ok(unsigned(value, 0xffff, label)? as u16)
}
fn s32(value: &Value, label: &str) -> Result<i32> {
    Ok(integer(value, -0x8000_0000, 0x7fff_ffff, label)? as i32)
}
fn u32(value: &Value, label: &str) -> Result<u32> {
    Ok(unsigned(value, 0xffff_ffff, label)? as u32)
}

fn integer_or(
    value: &Map<String, Value>,
    key: &str,
    default: i128,
    minimum: i128,
    maximum: i128,
    label: &str,
) -> Result<i128> {
    value
        .get(key)
        .map_or(Ok(default), |value| integer(value, minimum, maximum, label))
}

fn write_u16(data: &mut [u8], offset: usize, value: u16) {
    data[offset..offset + 2].copy_from_slice(&value.to_le_bytes());
}
fn write_i16(data: &mut [u8], offset: usize, value: i16) {
    data[offset..offset + 2].copy_from_slice(&value.to_le_bytes());
}
fn write_u32(data: &mut [u8], offset: usize, value: u32) {
    data[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
}
fn write_i32(data: &mut [u8], offset: usize, value: i32) {
    data[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
}

const ITEM_KEYS: &[&str] = &[
    "name",
    "price",
    "type",
    "flags",
    "equip_mask",
    "icon",
    "primary_bonus",
    "secondary_bonus",
    "secondary_flags",
    "use_type",
    "description_message",
    "element",
    "effects",
    "action_id",
];

fn is_snake_case(name: &str) -> bool {
    let mut bytes = name.bytes();
    let Some(first) = bytes.next() else {
        return false;
    };
    if !first.is_ascii_lowercase() {
        return false;
    }

    let mut previous_was_underscore = false;
    for byte in bytes {
        if byte == b'_' {
            if previous_was_underscore {
                return false;
            }
            previous_was_underscore = true;
        } else if byte.is_ascii_lowercase() || byte.is_ascii_digit() {
            previous_was_underscore = false;
        } else {
            return false;
        }
    }
    !previous_was_underscore
}

fn item_definition<'a>(value: &'a Value, index: usize) -> Result<&'a Map<String, Value>> {
    let label = format!("items[{index}]");
    let item = object(value, &label)?;
    sparse_keys(item, ITEM_KEYS, &["name"], &label)?;

    let name = field(item, "name")?
        .as_str()
        .ok_or_else(|| format!("{label} name must be a string"))?;
    if !is_snake_case(name) {
        return err(format!("{label} name must be unique nonempty snake_case"));
    }

    if let Some(effects) = item.get("effects") {
        for (effect_index, effect) in list(effects, 4, "item effects")?.iter().enumerate() {
            list(effect, 2, &format!("{label}.effects[{effect_index}]"))?;
        }
    }
    Ok(item)
}

fn parse_gameplay_databases(value: &Value) -> Result<&Map<String, Value>> {
    let source = object(value, "gameplay database source")?;
    exact_keys(
        source,
        &[
            "format",
            "kind",
            "address",
            "size",
            "alignment_bytes",
            "item_name_message_base",
            "progression_groups",
            "level_experience",
            "inventory_counter_slots",
            "party_order",
            "items",
            "abilities",
            "combatants",
            "hero_growth",
            "summon_order",
            "summons",
            "classes",
            "class_family_matrix",
            "elemental_profiles",
            "signed_scale_curve",
            "djinn",
        ],
        "gameplay database source",
    )?;
    if field(source, "format")?.as_u64() != Some(5)
        || field(source, "kind")?.as_str() != Some("golden-sun-gameplay-databases")
        || field(source, "address")?.as_str() != Some("0x0807a828")
        || field(source, "size")?.as_str() != Some("0x0000f7d8")
        || field(source, "alignment_bytes")?.as_u64() != Some((END - ALIGNMENT) as u64)
        || field(source, "item_name_message_base")?.as_str() != Some("0x0182")
    {
        return err("gameplay database header differs");
    }
    list(
        field(source, "progression_groups")?,
        8,
        "progression_groups",
    )?;
    let experience = list(field(source, "level_experience")?, 8, "level_experience")?;
    for (index, row) in experience.iter().enumerate() {
        list(row, 99, &format!("level_experience[{index}]"))?;
    }
    list(
        field(source, "inventory_counter_slots")?,
        512,
        "inventory_counter_slots",
    )?;
    list(field(source, "party_order")?, 6, "party_order")?;
    let items = list(field(source, "items")?, 324, "items")?;
    let mut item_names = HashSet::with_capacity(items.len());
    for (index, raw) in items.iter().enumerate() {
        let item = item_definition(raw, index)?;
        let name = field(item, "name")?.as_str().unwrap();
        if !item_names.insert(name) {
            return err(format!("items[{index}] name {name:?} is not unique"));
        }
    }
    list(field(source, "abilities")?, 519, "abilities")?;
    list(field(source, "combatants")?, 165, "combatants")?;
    list(field(source, "hero_growth")?, 8, "hero_growth")?;
    list(field(source, "summon_order")?, 16, "summon_order")?;
    list(field(source, "summons")?, 16, "summons")?;
    list(field(source, "classes")?, 203, "classes")?;
    let matrix = list(
        field(source, "class_family_matrix")?,
        8,
        "class_family_matrix",
    )?;
    for (index, row) in matrix.iter().enumerate() {
        list(row, 4, &format!("class_family_matrix[{index}]"))?;
    }
    list(
        field(source, "elemental_profiles")?,
        44,
        "elemental_profiles",
    )?;
    list(
        field(source, "signed_scale_curve")?,
        5,
        "signed_scale_curve",
    )?;
    list(field(source, "djinn")?, 80, "djinn")?;
    Ok(source)
}

fn build_item_definition(value: &Value, index: usize) -> Result<Vec<u8>> {
    let item = item_definition(value, index)?;
    let mut result = vec![0; 44];
    write_u16(
        &mut result,
        0,
        integer_or(item, "price", 0, 0, 0xffff, "item price")? as u16,
    );
    result[2] = integer_or(item, "type", 0, 0, 0xff, "item type")? as u8;
    result[3] = integer_or(item, "flags", 0, 0, 0xff, "item flags")? as u8;
    write_u16(
        &mut result,
        4,
        integer_or(item, "equip_mask", 0, 0, 0xffff, "item equip_mask")? as u16,
    );
    write_u16(
        &mut result,
        6,
        integer_or(item, "icon", 0, 0, 0xffff, "item icon")? as u16,
    );
    write_i16(
        &mut result,
        8,
        integer_or(
            item,
            "primary_bonus",
            0,
            -0x8000,
            0x7fff,
            "item primary_bonus",
        )? as i16,
    );
    result[10] = integer_or(
        item,
        "secondary_bonus",
        0,
        -0x80,
        0x7f,
        "item secondary_bonus",
    )? as i8 as u8;
    result[11] = integer_or(item, "secondary_flags", 0, 0, 0xff, "item secondary_flags")? as u8;
    result[12] = integer_or(item, "use_type", 0, 0, 0xff, "item use_type")? as u8;
    write_u16(
        &mut result,
        14,
        integer_or(
            item,
            "description_message",
            0,
            0,
            0xffff,
            "item description_message",
        )? as u16,
    );
    result[20] = integer_or(item, "element", 4, 0, 0xff, "item element")? as u8;
    if let Some(effects) = item.get("effects") {
        for (effect_index, raw) in list(effects, 4, "item effects")?.iter().enumerate() {
            let effect = list(raw, 2, &format!("items[{index}].effects[{effect_index}]"))?;
            let offset = 24 + effect_index * 4;
            result[offset] = u8(&effect[0], "item effect kind")?;
            result[offset + 1] = s8(&effect[1], "item effect amount")? as u8;
        }
    }
    write_u16(
        &mut result,
        40,
        integer_or(item, "action_id", 0, 0, 0xffff, "item action_id")? as u16,
    );
    Ok(result)
}

fn build_ability(value: &Value, index: usize) -> Result<Vec<u8>> {
    let ability = value
        .as_array()
        .filter(|values| values.len() <= 9)
        .ok_or_else(|| format!("abilities[{index}] must contain at most 9 values"))?;
    let value = |index: usize, maximum: i128, label: &str| {
        ability
            .get(index)
            .map_or(Ok(0), |value| integer(value, 0, maximum, label))
    };
    let mut result = vec![0; 16];
    for (offset, field) in [(0, 0), (1, 1), (2, 2), (3, 3), (8, 5), (9, 6)] {
        result[offset] = value(field, 0xff, "ability byte")? as u8;
    }
    write_u16(
        &mut result,
        4,
        value(4, 0xffff, "ability animation")? as u16,
    );
    write_u16(&mut result, 10, value(7, 0xffff, "ability power")? as u16);
    write_u16(&mut result, 12, value(8, 0xffff, "ability chance")? as u16);
    Ok(result)
}

fn build_combatant(value: &Value, index: usize) -> Result<Vec<u8>> {
    let item = value
        .as_array()
        .filter(|values| values.len() <= 37)
        .ok_or_else(|| format!("combatants[{index}] must contain at most 37 values"))?;
    let value = |index: usize, default: i128, maximum: i128, label: &str| {
        item.get(index)
            .map_or(Ok(default), |value| integer(value, 0, maximum, label))
    };
    let mut result = vec![0; 84];
    match value(0, 0, 2, "combatant name slot")? {
        0 => result[0..14].fill(0x20),
        1 => {}
        2 => result[0] = 0xff,
        _ => unreachable!(),
    }
    result[15] = value(1, 0, 0xff, "combatant level")? as u8;
    for (offset, field) in [
        (16, 2),
        (18, 3),
        (20, 4),
        (22, 5),
        (24, 6),
        (28, 9),
        (54, 20),
        (76, 33),
        (78, 34),
        (80, 35),
        (82, 36),
    ] {
        write_u16(
            &mut result,
            offset,
            value(field, 0, 0xffff, "combatant halfword")? as u16,
        );
    }
    result[26] = value(7, 0, 0xff, "combatant luck")? as u8;
    result[27] = value(8, 1, 0xff, "combatant turns")? as u8;
    for (field, offset, label) in [
        (10, 40, "combatant initial ability"),
        (21, 56, "combatant secondary ability"),
        (29, 68, "combatant battle trait"),
    ] {
        for slot in 0..4 {
            write_u16(
                &mut result,
                offset + slot * 2,
                value(field + slot, 0, 0xffff, label)? as u16,
            );
        }
    }
    for (field, offset, label) in [
        (14, 48, "combatant initial ability count"),
        (25, 64, "combatant secondary ability count"),
    ] {
        for slot in 0..4 {
            result[offset + slot] = value(field + slot, 0, 0xff, label)? as u8;
        }
    }
    result[52] = value(18, 0, 0xff, "combatant elemental profile")? as u8;
    result[53] = value(19, 0, 0xff, "combatant behavior")? as u8;
    Ok(result)
}

fn build_hero_growth(value: &Value, index: usize) -> Result<Vec<u8>> {
    let growth = object(value, &format!("hero_growth[{index}]"))?;
    exact_keys(
        growth,
        &[
            "hp",
            "pp",
            "attack",
            "defense",
            "agility",
            "luck",
            "elemental_levels",
            "class_id",
            "initial_abilities",
        ],
        &format!("hero_growth[{index}]"),
    )?;
    let mut result = vec![0; 180];
    for (offset, key) in [
        (80, "hp"),
        (92, "pp"),
        (104, "attack"),
        (116, "defense"),
        (128, "agility"),
    ] {
        for (slot, entry) in list(field(growth, key)?, 6, &format!("hero growth {key}"))?
            .iter()
            .enumerate()
        {
            write_u16(
                &mut result,
                offset + slot * 2,
                u16(entry, &format!("hero growth {key}"))?,
            );
        }
    }
    for (slot, entry) in list(field(growth, "luck")?, 6, "hero growth luck")?
        .iter()
        .enumerate()
    {
        result[140 + slot] = u8(entry, "hero growth luck")?;
    }
    for (slot, entry) in list(
        field(growth, "elemental_levels")?,
        4,
        "hero growth elemental_levels",
    )?
    .iter()
    .enumerate()
    {
        result[146 + slot] = u8(entry, "hero growth elemental level")?;
    }
    result[150] = u8(field(growth, "class_id")?, "hero growth class_id")?;
    for (slot, entry) in list(
        field(growth, "initial_abilities")?,
        13,
        "hero growth initial_abilities",
    )?
    .iter()
    .enumerate()
    {
        write_u16(
            &mut result,
            152 + slot * 2,
            u16(entry, "hero growth initial ability")?,
        );
    }
    Ok(result)
}

fn build_class(value: &Value, index: usize) -> Result<Vec<u8>> {
    let item = object(value, &format!("classes[{index}]"))?;
    sparse_keys(
        item,
        &[
            "family",
            "djinn_requirements",
            "stat_multipliers",
            "abilities",
            "traits",
        ],
        &[],
        &format!("classes[{index}]"),
    )?;
    let mut result = vec![0; 84];
    write_i32(
        &mut result,
        0,
        integer_or(item, "family", 0, -0x8000_0000, 0x7fff_ffff, "class family")? as i32,
    );
    if let Some(requirements) = item.get("djinn_requirements") {
        for (slot, entry) in list(requirements, 4, "class djinn_requirements")?
            .iter()
            .enumerate()
        {
            result[4 + slot] = u8(entry, "class djinn requirement")?;
        }
    }
    if let Some(multipliers) = item.get("stat_multipliers") {
        for (slot, entry) in list(multipliers, 6, "class stat_multipliers")?
            .iter()
            .enumerate()
        {
            result[8 + slot] = u8(entry, "class stat multiplier")?;
        }
    }
    if let Some(abilities) = item.get("abilities") {
        for (slot, raw) in list(abilities, 16, "class abilities")?.iter().enumerate() {
            let ability = list(raw, 2, "class ability")?;
            result[16 + slot * 4] = u8(&ability[0], "class ability id")?;
            result[17 + slot * 4] = u8(&ability[1], "class ability level")?;
        }
    }
    if let Some(traits) = item.get("traits") {
        for (slot, entry) in list(traits, 4, "class traits")?.iter().enumerate() {
            result[80 + slot] = u8(entry, "class trait")?;
        }
    }
    Ok(result)
}

fn build_elemental_profile(value: &Value, index: usize) -> Result<Vec<u8>> {
    let item = object(value, &format!("elemental_profiles[{index}]"))?;
    exact_keys(
        item,
        &["element", "levels", "stats"],
        &format!("elemental_profiles[{index}]"),
    )?;
    let mut result = vec![0; 24];
    write_i32(
        &mut result,
        0,
        s32(field(item, "element")?, "elemental profile element")?,
    );
    for (slot, entry) in list(field(item, "levels")?, 4, "elemental profile levels")?
        .iter()
        .enumerate()
    {
        result[4 + slot] = u8(entry, "elemental profile level")?;
    }
    for (slot, raw) in list(field(item, "stats")?, 4, "elemental profile stats")?
        .iter()
        .enumerate()
    {
        let stat = list(raw, 2, "elemental profile stat")?;
        write_u16(
            &mut result,
            8 + slot * 4,
            u16(&stat[0], "elemental profile power")?,
        );
        write_u16(
            &mut result,
            10 + slot * 4,
            u16(&stat[1], "elemental profile resistance")?,
        );
    }
    Ok(result)
}

pub fn build_gameplay_databases(value: &Value) -> Result<Vec<u8>> {
    let source = parse_gameplay_databases(value)?;
    let mut result = Vec::with_capacity(RESOURCE_SIZE);
    result.extend(
        list(
            field(source, "progression_groups")?,
            8,
            "progression_groups",
        )?
        .iter()
        .map(|entry| u8(entry, "progression group"))
        .collect::<Result<Vec<_>>>()?,
    );
    let mut experience = vec![0; 8 * 99 * 4];
    for (hero, row) in list(field(source, "level_experience")?, 8, "level_experience")?
        .iter()
        .enumerate()
    {
        for (level, entry) in list(row, 99, &format!("level_experience[{hero}]"))?
            .iter()
            .enumerate()
        {
            write_u32(
                &mut experience,
                (hero * 99 + level) * 4,
                u32(entry, "level experience")?,
            );
        }
    }
    result.extend(experience);
    result.extend(
        list(
            field(source, "inventory_counter_slots")?,
            512,
            "inventory_counter_slots",
        )?
        .iter()
        .map(|entry| {
            if entry.is_null() {
                Ok(0)
            } else {
                Ok(u8(entry, "inventory counter slot")?
                    .checked_add(1)
                    .ok_or_else(|| "inventory counter slot is outside 0..0xfe".to_string())?)
            }
        })
        .collect::<Result<Vec<_>>>()?,
    );
    let mut party = vec![0; 24];
    for (index, entry) in list(field(source, "party_order")?, 6, "party_order")?
        .iter()
        .enumerate()
    {
        write_i32(&mut party, index * 4, s32(entry, "party order")?);
    }
    result.extend(party);
    result.extend(
        list(field(source, "items")?, 324, "items")?
            .iter()
            .enumerate()
            .map(|(index, value)| build_item_definition(value, index))
            .collect::<Result<Vec<_>>>()?
            .into_iter()
            .flatten(),
    );
    result.extend(
        list(field(source, "abilities")?, 519, "abilities")?
            .iter()
            .enumerate()
            .map(|(index, value)| build_ability(value, index))
            .collect::<Result<Vec<_>>>()?
            .into_iter()
            .flatten(),
    );
    result.extend(
        list(field(source, "combatants")?, 165, "combatants")?
            .iter()
            .enumerate()
            .map(|(index, value)| build_combatant(value, index))
            .collect::<Result<Vec<_>>>()?
            .into_iter()
            .flatten(),
    );
    result.extend(
        list(field(source, "hero_growth")?, 8, "hero_growth")?
            .iter()
            .enumerate()
            .map(|(index, value)| build_hero_growth(value, index))
            .collect::<Result<Vec<_>>>()?
            .into_iter()
            .flatten(),
    );
    result.extend(
        list(field(source, "summon_order")?, 16, "summon_order")?
            .iter()
            .map(|entry| u8(entry, "summon order"))
            .collect::<Result<Vec<_>>>()?,
    );
    for (index, raw) in list(field(source, "summons")?, 16, "summons")?
        .iter()
        .enumerate()
    {
        let summon = list(raw, 2, &format!("summons[{index}]"))?;
        let mut bytes = vec![0; 8];
        write_u32(&mut bytes, 0, u32(&summon[0], "summon ability_id")?);
        for (slot, entry) in list(&summon[1], 4, "summon djinn_cost")?.iter().enumerate() {
            bytes[4 + slot] = u8(entry, "summon Djinn cost")?;
        }
        result.extend(bytes);
    }
    result.extend(
        list(field(source, "classes")?, 203, "classes")?
            .iter()
            .enumerate()
            .map(|(index, value)| build_class(value, index))
            .collect::<Result<Vec<_>>>()?
            .into_iter()
            .flatten(),
    );
    let mut matrix = vec![0; 128];
    for (row_index, row) in list(
        field(source, "class_family_matrix")?,
        8,
        "class_family_matrix",
    )?
    .iter()
    .enumerate()
    {
        for (column, entry) in list(row, 4, &format!("class_family_matrix[{row_index}]"))?
            .iter()
            .enumerate()
        {
            write_i32(
                &mut matrix,
                (row_index * 4 + column) * 4,
                s32(entry, "class family matrix")?,
            );
        }
    }
    result.extend(matrix);
    result.extend(
        list(
            field(source, "elemental_profiles")?,
            44,
            "elemental_profiles",
        )?
        .iter()
        .enumerate()
        .map(|(index, value)| build_elemental_profile(value, index))
        .collect::<Result<Vec<_>>>()?
        .into_iter()
        .flatten(),
    );
    for (index, raw) in list(
        field(source, "signed_scale_curve")?,
        5,
        "signed_scale_curve",
    )?
    .iter()
    .enumerate()
    {
        let point = list(raw, 2, &format!("signed_scale_curve[{index}]"))?;
        let mut bytes = vec![0; 4];
        write_i16(&mut bytes, 0, s16(&point[0], "scale input")?);
        write_i16(&mut bytes, 2, s16(&point[1], "scale output")?);
        result.extend(bytes);
    }
    for (index, raw) in list(field(source, "djinn")?, 80, "djinn")?
        .iter()
        .enumerate()
    {
        let djinni = list(raw, 2, &format!("djinn[{index}]"))?;
        let mut bytes = vec![0; 12];
        write_u16(&mut bytes, 0, u16(&djinni[0], "Djinn name_message")?);
        for (slot, entry) in list(&djinni[1], 6, "Djinn stat_bonuses")?
            .iter()
            .enumerate()
        {
            bytes[4 + slot] = s8(entry, "Djinn stat bonus")? as u8;
        }
        result.extend(bytes);
    }
    let alignment = unsigned(
        field(source, "alignment_bytes")?,
        (END - ALIGNMENT) as u64,
        "alignment_bytes",
    )? as usize;
    result.extend(vec![0; alignment]);
    if result.len() != RESOURCE_SIZE {
        return err(format!(
            "gameplay databases rebuilt 0x{:x} bytes",
            result.len()
        ));
    }
    Ok(result)
}

pub fn run(args: Vec<String>) -> Result<()> {
    let [command, source] = args.as_slice() else {
        return err("usage: assets 5 (build-stdout|format) SOURCE");
    };
    let text = fs::read_to_string(source).map_err(|error| format!("{source}: {error}"))?;
    let value: Value = serde_json::from_str(&text).map_err(|error| format!("{source}: {error}"))?;
    match command.as_str() {
        "build-stdout" => io::stdout()
            .write_all(&build_gameplay_databases(&value)?)
            .map_err(|error| error.to_string()),
        "format" => fs::write(source, format!("{}\n", canonical_json(&value)))
            .map_err(|error| error.to_string()),
        _ => err("usage: assets 5 (build-stdout|format) SOURCE"),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;

    #[test]
    fn sparse_records_expand_to_binary_defaults() {
        let item = build_item_definition(&json!({"name":"no_item"}), 0).unwrap();
        assert_eq!(item.len(), 44);
        assert_eq!(item[20], 4);
        assert!(item[..20].iter().chain(&item[21..]).all(|byte| *byte == 0));

        assert_eq!(build_ability(&json!([]), 0).unwrap(), vec![0; 16]);
        assert_eq!(build_class(&json!({}), 0).unwrap(), vec![0; 84]);

        let combatant = build_combatant(&json!([]), 0).unwrap();
        assert_eq!(&combatant[..14], &[0x20; 14]);
        assert_eq!(combatant[27], 1);
        assert!(combatant[14..27]
            .iter()
            .chain(&combatant[28..])
            .all(|byte| *byte == 0));
    }

    #[test]
    fn sparse_records_still_reject_unknown_fields() {
        assert!(build_item_definition(&json!({"name":"no_item","unknown":0}), 0).is_err());
        assert!(build_ability(&json!({"unknown":0}), 0).is_err());
        assert!(build_ability(&json!([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]), 0).is_err());
        assert!(build_class(&json!({"unknown":0}), 0).is_err());
        assert!(build_combatant(&json!({"unknown":0}), 0).is_err());
        assert!(build_combatant(&json!(vec![0; 38]), 0).is_err());
    }
}
