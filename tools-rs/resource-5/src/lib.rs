//! Native port of `tools/make/resource_5.ts`.
//!
//! Resource 5 is the large gameplay-database block.  The JSON document is
//! deliberately kept as the public interchange format; this crate owns the
//! standalone export, build, verify, and self-test commands while the legacy
//! TypeScript builder remains available to the asset-build compatibility path.

use canonical_json::canonical_json;
use serde_json::{json, Map, Value};
use std::fs;
use std::path::Path;

pub const ROM_BASE: usize = 0x0800_0000;
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

fn read_u16(data: &[u8], offset: usize) -> u16 {
    u16::from_le_bytes([data[offset], data[offset + 1]])
}
fn read_i16(data: &[u8], offset: usize) -> i16 {
    i16::from_le_bytes([data[offset], data[offset + 1]])
}
fn read_u32(data: &[u8], offset: usize) -> u32 {
    u32::from_le_bytes([
        data[offset],
        data[offset + 1],
        data[offset + 2],
        data[offset + 3],
    ])
}
fn read_i32(data: &[u8], offset: usize) -> i32 {
    i32::from_le_bytes([
        data[offset],
        data[offset + 1],
        data[offset + 2],
        data[offset + 3],
    ])
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

fn all_zero(data: &[u8], start: usize, end: usize, label: &str) -> Result<()> {
    if data[start..end].iter().any(|byte| *byte != 0) {
        return err(format!(
            "{label} has a nonzero reserved byte at +0x{:x}",
            data[start..end].iter().position(|byte| *byte != 0).unwrap() + start
        ));
    }
    Ok(())
}

fn name_slot(data: &[u8]) -> Result<&'static str> {
    if data == [0x20; 14] {
        Ok("space")
    } else if data == [0; 14] {
        Ok("zero")
    } else if data[0] == 0xff && data[1..].iter().all(|byte| *byte == 0) {
        Ok("sentinel")
    } else {
        err("combatant name slot has an unsupported fill")
    }
}

fn build_name_slot(value: &Value) -> Result<Vec<u8>> {
    let name = value
        .as_str()
        .ok_or_else(|| "combatant name_slot must be space, zero, or sentinel".to_string())?;
    match name {
        "space" => Ok(vec![0x20; 14]),
        "zero" => Ok(vec![0; 14]),
        "sentinel" => {
            let mut result = vec![0; 14];
            result[0] = 0xff;
            Ok(result)
        }
        _ => err("combatant name_slot must be space, zero, or sentinel"),
    }
}

fn parse_document(value: &Value) -> Result<&Map<String, Value>> {
    let source = object(value, "resource 5 source")?;
    exact_keys(
        source,
        &[
            "format",
            "kind",
            "address",
            "size",
            "alignment_bytes",
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
        "resource 5 source",
    )?;
    if field(source, "format")?.as_u64() != Some(1)
        || field(source, "kind")?.as_str() != Some("golden-sun-gameplay-databases")
        || field(source, "address")?.as_str() != Some("0x0807a828")
        || field(source, "size")?.as_str() != Some("0x0000f7d8")
        || field(source, "alignment_bytes")?.as_u64() != Some((END - ALIGNMENT) as u64)
    {
        return err("resource 5 header differs");
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
    list(field(source, "items")?, 324, "items")?;
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

fn export_item(data: &[u8]) -> Result<Value> {
    all_zero(data, 5, 6, "item")?;
    all_zero(data, 7, 8, "item")?;
    all_zero(data, 13, 14, "item")?;
    all_zero(data, 16, 20, "item")?;
    all_zero(data, 21, 24, "item")?;
    for offset in [26, 27, 30, 31, 34, 35, 38, 39, 42, 43] {
        if data[offset] != 0 {
            return err(format!("item has a nonzero reserved byte at +0x{offset:x}"));
        }
    }
    Ok(json!([
        read_u16(data, 0),
        data[2],
        data[3],
        read_u16(data, 4),
        read_u16(data, 6),
        read_i16(data, 8),
        data[10] as i8,
        data[11],
        data[12],
        read_u16(data, 14),
        data[20],
        data[24],
        data[25] as i8,
        data[28],
        data[29] as i8,
        data[32],
        data[33] as i8,
        data[36],
        data[37] as i8,
        read_u16(data, 40),
    ]))
}

fn build_item(value: &Value, index: usize) -> Result<Vec<u8>> {
    let item = list(value, 20, &format!("items[{index}]"))?;
    let mut result = vec![0; 44];
    write_u16(&mut result, 0, u16(&item[0], "item name_message")?);
    result[2] = u8(&item[1], "item category")?;
    result[3] = u8(&item[2], "item flags")?;
    write_u16(&mut result, 4, u16(&item[3], "item equip_mask")?);
    write_u16(&mut result, 6, u16(&item[4], "item icon")?);
    write_i16(&mut result, 8, s16(&item[5], "item primary_bonus")?);
    result[10] = s8(&item[6], "item secondary_bonus")? as u8;
    result[11] = u8(&item[7], "item secondary_flags")?;
    result[12] = u8(&item[8], "item use_type")?;
    write_u16(&mut result, 14, u16(&item[9], "item description_message")?);
    result[20] = u8(&item[10], "item element")?;
    for effect in 0..4 {
        let offset = 24 + effect * 4;
        result[offset] = u8(&item[11 + effect * 2], "item effect kind")?;
        result[offset + 1] = s8(&item[12 + effect * 2], "item effect amount")? as u8;
    }
    write_u16(&mut result, 40, u16(&item[19], "item unleash_ability")?);
    Ok(result)
}

fn export_ability(data: &[u8]) -> Result<Value> {
    all_zero(data, 6, 8, "ability")?;
    all_zero(data, 14, 16, "ability")?;
    Ok(json!({
        "element": data[0], "target": data[1], "damage_class": data[2], "effect": data[3],
        "animation": read_u16(data, 4), "range": data[8], "category": data[9],
        "power": read_u16(data, 10), "chance": read_u16(data, 12),
    }))
}

fn build_ability(value: &Value, index: usize) -> Result<Vec<u8>> {
    let ability = object(value, &format!("abilities[{index}]"))?;
    exact_keys(
        ability,
        &[
            "element",
            "target",
            "damage_class",
            "effect",
            "animation",
            "range",
            "category",
            "power",
            "chance",
        ],
        &format!("abilities[{index}]"),
    )?;
    let mut result = vec![0; 16];
    for (offset, key) in [
        (0, "element"),
        (1, "target"),
        (2, "damage_class"),
        (3, "effect"),
        (8, "range"),
        (9, "category"),
    ] {
        result[offset] = u8(field(ability, key)?, &format!("ability {key}"))?;
    }
    write_u16(
        &mut result,
        4,
        u16(field(ability, "animation")?, "ability animation")?,
    );
    write_u16(
        &mut result,
        10,
        u16(field(ability, "power")?, "ability power")?,
    );
    write_u16(
        &mut result,
        12,
        u16(field(ability, "chance")?, "ability chance")?,
    );
    Ok(result)
}

fn export_combatant(data: &[u8]) -> Result<Value> {
    if data[14] != 0 {
        return err("combatant name terminator is nonzero");
    }
    all_zero(data, 30, 40, "combatant")?;
    Ok(json!({
        "name_slot": name_slot(&data[0..14])?, "level": data[15], "hp": read_u16(data, 16),
        "pp": read_u16(data, 18), "attack": read_u16(data, 20), "defense": read_u16(data, 22),
        "agility": read_u16(data, 24), "luck": data[26], "turns": data[27],
        "hp_regeneration": read_u16(data, 28),
        "initial_abilities": [read_u16(data, 40), read_u16(data, 42), read_u16(data, 44), read_u16(data, 46)],
        "initial_ability_counts": data[48..52].to_vec(), "elemental_profile": data[52],
        "behavior": data[53], "flags": read_u16(data, 54),
        "secondary_abilities": [read_u16(data, 56), read_u16(data, 58), read_u16(data, 60), read_u16(data, 62)],
        "secondary_ability_counts": data[64..68].to_vec(),
        "battle_traits": [read_u16(data, 68), read_u16(data, 70), read_u16(data, 72), read_u16(data, 74)],
        "experience_reward": read_u16(data, 76), "sprite": read_u16(data, 78),
        "reward_tier": read_u16(data, 80), "coin_reward": read_u16(data, 82),
    }))
}

fn build_combatant(value: &Value, index: usize) -> Result<Vec<u8>> {
    let item = object(value, &format!("combatants[{index}]"))?;
    exact_keys(
        item,
        &[
            "name_slot",
            "level",
            "hp",
            "pp",
            "attack",
            "defense",
            "agility",
            "luck",
            "turns",
            "hp_regeneration",
            "initial_abilities",
            "initial_ability_counts",
            "elemental_profile",
            "behavior",
            "flags",
            "secondary_abilities",
            "secondary_ability_counts",
            "battle_traits",
            "experience_reward",
            "sprite",
            "reward_tier",
            "coin_reward",
        ],
        &format!("combatants[{index}]"),
    )?;
    let mut result = vec![0; 84];
    result[0..14].copy_from_slice(&build_name_slot(field(item, "name_slot")?)?);
    result[15] = u8(field(item, "level")?, "combatant level")?;
    for (offset, key) in [
        (16, "hp"),
        (18, "pp"),
        (20, "attack"),
        (22, "defense"),
        (24, "agility"),
        (28, "hp_regeneration"),
        (54, "flags"),
        (76, "experience_reward"),
        (78, "sprite"),
        (80, "reward_tier"),
        (82, "coin_reward"),
    ] {
        write_u16(
            &mut result,
            offset,
            u16(field(item, key)?, &format!("combatant {key}"))?,
        );
    }
    result[26] = u8(field(item, "luck")?, "combatant luck")?;
    result[27] = u8(field(item, "turns")?, "combatant turns")?;
    for (field_name, offset, label) in [
        ("initial_abilities", 40, "combatant initial ability"),
        ("secondary_abilities", 56, "combatant secondary ability"),
        ("battle_traits", 68, "combatant battle trait"),
    ] {
        for (slot, entry) in list(field(item, field_name)?, 4, field_name)?
            .iter()
            .enumerate()
        {
            write_u16(&mut result, offset + slot * 2, u16(entry, label)?);
        }
    }
    for (field_name, offset, label) in [
        (
            "initial_ability_counts",
            48,
            "combatant initial ability count",
        ),
        (
            "secondary_ability_counts",
            64,
            "combatant secondary ability count",
        ),
    ] {
        for (slot, entry) in list(field(item, field_name)?, 4, field_name)?
            .iter()
            .enumerate()
        {
            result[offset + slot] = u8(entry, label)?;
        }
    }
    result[52] = u8(
        field(item, "elemental_profile")?,
        "combatant elemental_profile",
    )?;
    result[53] = u8(field(item, "behavior")?, "combatant behavior")?;
    Ok(result)
}

fn export_hero_growth(data: &[u8]) -> Result<Value> {
    all_zero(data, 0, 80, "hero growth")?;
    if data[151] != 0 {
        return err("hero growth class padding is nonzero");
    }
    all_zero(data, 178, 180, "hero growth")?;
    let checkpoint = |offset: usize| -> Vec<Value> {
        (0..6)
            .map(|index| json!(read_u16(data, offset + index * 2)))
            .collect()
    };
    Ok(json!({
        "hp": checkpoint(80), "pp": checkpoint(92), "attack": checkpoint(104),
        "defense": checkpoint(116), "agility": checkpoint(128), "luck": data[140..146].to_vec(),
        "elemental_levels": data[146..150].to_vec(), "class_id": data[150],
        "initial_abilities": (0..13).map(|index| json!(read_u16(data, 152 + index * 2))).collect::<Vec<_>>(),
    }))
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

fn export_class(data: &[u8]) -> Result<Value> {
    all_zero(data, 14, 16, "class")?;
    for offset in (16..80).step_by(4) {
        all_zero(data, offset + 2, offset + 4, "class ability")?;
    }
    Ok(json!({
        "family": read_i32(data, 0), "djinn_requirements": data[4..8].to_vec(),
        "stat_multipliers": data[8..14].to_vec(),
        "abilities": (0..16).map(|index| json!({"id": data[16 + index * 4], "level": data[17 + index * 4]})).collect::<Vec<_>>(),
        "traits": data[80..84].to_vec(),
    }))
}

fn build_class(value: &Value, index: usize) -> Result<Vec<u8>> {
    let item = object(value, &format!("classes[{index}]"))?;
    exact_keys(
        item,
        &[
            "family",
            "djinn_requirements",
            "stat_multipliers",
            "abilities",
            "traits",
        ],
        &format!("classes[{index}]"),
    )?;
    let mut result = vec![0; 84];
    write_i32(&mut result, 0, s32(field(item, "family")?, "class family")?);
    for (slot, entry) in list(
        field(item, "djinn_requirements")?,
        4,
        "class djinn_requirements",
    )?
    .iter()
    .enumerate()
    {
        result[4 + slot] = u8(entry, "class djinn requirement")?;
    }
    for (slot, entry) in list(
        field(item, "stat_multipliers")?,
        6,
        "class stat_multipliers",
    )?
    .iter()
    .enumerate()
    {
        result[8 + slot] = u8(entry, "class stat multiplier")?;
    }
    for (slot, raw) in list(field(item, "abilities")?, 16, "class abilities")?
        .iter()
        .enumerate()
    {
        let ability = object(raw, "class ability")?;
        exact_keys(ability, &["id", "level"], "class ability")?;
        result[16 + slot * 4] = u8(field(ability, "id")?, "class ability id")?;
        result[17 + slot * 4] = u8(field(ability, "level")?, "class ability level")?;
    }
    for (slot, entry) in list(field(item, "traits")?, 4, "class traits")?
        .iter()
        .enumerate()
    {
        result[80 + slot] = u8(entry, "class trait")?;
    }
    Ok(result)
}

fn export_elemental_profile(data: &[u8]) -> Value {
    json!({
        "element": read_i32(data, 0), "levels": data[4..8].to_vec(),
        "stats": (0..4).map(|index| json!({
            "power": read_u16(data, 8 + index * 4),
            "resistance": read_u16(data, 10 + index * 4),
        })).collect::<Vec<_>>(),
    })
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
        let stat = object(raw, "elemental profile stat")?;
        exact_keys(stat, &["power", "resistance"], "elemental profile stat")?;
        write_u16(
            &mut result,
            8 + slot * 4,
            u16(field(stat, "power")?, "elemental profile power")?,
        );
        write_u16(
            &mut result,
            10 + slot * 4,
            u16(field(stat, "resistance")?, "elemental profile resistance")?,
        );
    }
    Ok(result)
}

fn at<'a>(rom: &'a [u8], address: usize, size: usize) -> &'a [u8] {
    &rom[address - ROM_BASE..address - ROM_BASE + size]
}

fn records(rom: &[u8], address: usize, count: usize, size: usize) -> Vec<&[u8]> {
    (0..count)
        .map(|index| at(rom, address + index * size, size))
        .collect()
}

pub fn export_document(rom: &[u8]) -> Result<Value> {
    if rom.len() < END - ROM_BASE {
        return err("ROM is too small for resource 5");
    }
    let alignment = at(rom, ALIGNMENT, END - ALIGNMENT);
    all_zero(alignment, 0, alignment.len(), "resource 5 alignment")?;
    let experience = at(rom, 0x0807_a830, 8 * 99 * 4);
    let inventory = at(rom, 0x0807_b490, 512);
    let party = at(rom, 0x0807_b690, 6 * 4);
    let level_experience = (0..8)
        .map(|hero| {
            (0..99)
                .map(|level| json!(read_u32(experience, (hero * 99 + level) * 4)))
                .collect::<Vec<_>>()
        })
        .collect::<Vec<_>>();
    let inventory_counter_slots = inventory
        .iter()
        .map(|value| {
            if *value == 0 {
                Value::Null
            } else {
                json!(value - 1)
            }
        })
        .collect::<Vec<_>>();
    let item_values = records(rom, 0x0807_b6a8, 324, 44)
        .into_iter()
        .map(export_item)
        .collect::<Result<Vec<_>>>()?;
    let ability_values = records(rom, 0x0807_ee58, 519, 16)
        .into_iter()
        .map(export_ability)
        .collect::<Result<Vec<_>>>()?;
    let combatant_values = records(rom, 0x0808_0ec8, 165, 84)
        .into_iter()
        .map(export_combatant)
        .collect::<Result<Vec<_>>>()?;
    let hero_values = records(rom, 0x0808_44ec, 8, 180)
        .into_iter()
        .map(export_hero_growth)
        .collect::<Result<Vec<_>>>()?;
    let summon_values = records(rom, 0x0808_4a9c, 16, 8)
        .into_iter()
        .map(|data| json!({"ability_id": read_u32(data, 0), "djinn_cost": data[4..8].to_vec()}))
        .collect::<Vec<_>>();
    let class_values = records(rom, 0x0808_4b1c, 203, 84)
        .into_iter()
        .map(export_class)
        .collect::<Result<Vec<_>>>()?;
    let matrix_data = at(rom, 0x0808_8db8, 128);
    let matrix = (0..8)
        .map(|row| {
            (0..4)
                .map(|column| json!(read_i32(matrix_data, (row * 4 + column) * 4)))
                .collect::<Vec<_>>()
        })
        .collect::<Vec<_>>();
    let profile_values = records(rom, 0x0808_8e38, 44, 24)
        .into_iter()
        .map(|data| Ok(export_elemental_profile(data)))
        .collect::<Result<Vec<_>>>()?;
    let scale_values = records(rom, 0x0808_9258, 5, 4)
        .into_iter()
        .map(|data| json!({"input": read_i16(data, 0), "output": read_i16(data, 2)}))
        .collect::<Vec<_>>();
    let djinn_values = records(rom, 0x0808_926c, 80, 12)
        .into_iter()
        .map(|data| {
            all_zero(data, 2, 4, "Djinn")?;
            all_zero(data, 10, 12, "Djinn")?;
            Ok(json!({
                "name_message": read_u16(data, 0),
                "stat_bonuses": data[4..10].iter().map(|value| json!(*value as i8)).collect::<Vec<_>>(),
            }))
        })
        .collect::<Result<Vec<_>>>()?;
    Ok(json!({
        "format": 1,
        "kind": "golden-sun-gameplay-databases",
        "address": "0x0807a828",
        "size": "0x0000f7d8",
        "alignment_bytes": alignment.len(),
        "progression_groups": at(rom, 0x0807_a828, 8).to_vec(),
        "level_experience": level_experience,
        "inventory_counter_slots": inventory_counter_slots,
        "party_order": (0..6).map(|index| json!(read_i32(party, index * 4))).collect::<Vec<_>>(),
        "items": item_values,
        "abilities": ability_values,
        "combatants": combatant_values,
        "hero_growth": hero_values,
        "summon_order": at(rom, 0x0808_4a8c, 16).to_vec(),
        "summons": summon_values,
        "classes": class_values,
        "class_family_matrix": matrix,
        "elemental_profiles": profile_values,
        "signed_scale_curve": scale_values,
        "djinn": djinn_values,
    }))
}

pub fn build_resource_5(value: &Value) -> Result<Vec<u8>> {
    let source = parse_document(value)?;
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
            .map(|(index, value)| build_item(value, index))
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
        let summon = object(raw, &format!("summons[{index}]"))?;
        exact_keys(
            summon,
            &["ability_id", "djinn_cost"],
            &format!("summons[{index}]"),
        )?;
        let mut bytes = vec![0; 8];
        write_u32(
            &mut bytes,
            0,
            u32(field(summon, "ability_id")?, "summon ability_id")?,
        );
        for (slot, entry) in list(field(summon, "djinn_cost")?, 4, "summon djinn_cost")?
            .iter()
            .enumerate()
        {
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
        let point = object(raw, &format!("signed_scale_curve[{index}]"))?;
        exact_keys(
            point,
            &["input", "output"],
            &format!("signed_scale_curve[{index}]"),
        )?;
        let mut bytes = vec![0; 4];
        write_i16(&mut bytes, 0, s16(field(point, "input")?, "scale input")?);
        write_i16(&mut bytes, 2, s16(field(point, "output")?, "scale output")?);
        result.extend(bytes);
    }
    for (index, raw) in list(field(source, "djinn")?, 80, "djinn")?
        .iter()
        .enumerate()
    {
        let djinni = object(raw, &format!("djinn[{index}]"))?;
        exact_keys(
            djinni,
            &["name_message", "stat_bonuses"],
            &format!("djinn[{index}]"),
        )?;
        let mut bytes = vec![0; 12];
        write_u16(
            &mut bytes,
            0,
            u16(field(djinni, "name_message")?, "Djinn name_message")?,
        );
        for (slot, entry) in list(field(djinni, "stat_bonuses")?, 6, "Djinn stat_bonuses")?
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
        return err(format!("resource 5 rebuilt 0x{:x} bytes", result.len()));
    }
    Ok(result)
}

fn read_json(path: &Path) -> Result<Value> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))
}

fn write(path: &Path, bytes: &[u8]) -> Result<()> {
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    fs::write(path, bytes).map_err(|error| format!("{}: {error}", path.display()))
}

pub fn export_resource_5(rom_path: &Path, output_path: &Path) -> Result<usize> {
    let rom = fs::read(rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let source = export_document(&rom)?;
    let rebuilt = build_resource_5(&source)?;
    let original = &rom[ADDRESS - ROM_BASE..END - ROM_BASE];
    if rebuilt != original {
        return err("exported resource 5 does not round-trip");
    }
    write(
        output_path,
        format!("{}\n", canonical_json(&source)).as_bytes(),
    )?;
    Ok(rebuilt.len())
}

pub fn verify_resource_5(rom_path: &Path, source_path: &Path) -> Result<usize> {
    let rom = fs::read(rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let source = read_json(source_path)?;
    let built = build_resource_5(&source)?;
    if rom.len() < END - ROM_BASE || built != rom[ADDRESS - ROM_BASE..END - ROM_BASE] {
        return err("resource 5 differs from ROM");
    }
    Ok(built.len())
}

pub fn self_test() -> Result<()> {
    let name = vec![0x20; 14];
    if name_slot(&name)? != "space" || build_name_slot(&json!("space"))? != name {
        return err("name-slot self-test failed");
    }
    let item = vec![0; 44];
    if build_item(&export_item(&item)?, 0)? != item {
        return err("item self-test failed");
    }
    let ability = vec![0; 16];
    if build_ability(&export_ability(&ability)?, 0)? != ability {
        return err("ability self-test failed");
    }
    let root = Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .unwrap();
    let rom = root.join("baserom.gba");
    let source = root.join("assets/data/resource_5_database.json");
    if rom.is_file() && source.is_file() {
        verify_resource_5(&rom, &source)?;
    }
    Ok(())
}

fn positional(args: &[String]) -> Vec<String> {
    args.iter()
        .enumerate()
        .filter(|(index, value)| {
            !value.starts_with('-')
                && !(*index > 0 && matches!(args[*index - 1].as_str(), "-o" | "--output"))
        })
        .map(|(_, value)| value.clone())
        .collect()
}

fn output(args: &[String]) -> Option<String> {
    args.iter()
        .enumerate()
        .filter(|(_, value)| matches!(value.as_str(), "-o" | "--output"))
        .filter_map(|(index, _)| args.get(index + 1).cloned())
        .next_back()
}

pub fn run(mut args: Vec<String>) -> Result<()> {
    if args.iter().any(|value| value == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        if args.len() == 1 {
            return Ok(());
        }
        args.retain(|value| value != "--self-test");
    }
    let words = positional(&args);
    if words.is_empty()
        || args
            .iter()
            .any(|value| matches!(value.as_str(), "-h" | "--help"))
    {
        println!("usage: resource_5.ts {{export ROM -o SOURCE|build SOURCE -o FILE|verify ROM SOURCE|--self-test}}");
        return Ok(());
    }
    let target = output(&args);
    match words.as_slice() {
        [command, input] if command == "export" => {
            let target = target.ok_or_else(|| "export requires ROM and -o SOURCE".to_string())?;
            let bytes = export_resource_5(Path::new(input), Path::new(&target))?;
            println!("identical=true bytes={bytes} items=324 abilities=519 combatants=165 classes=203 djinn=80");
        }
        [command, input] if command == "build" => {
            let target = target.ok_or_else(|| "build requires SOURCE and -o FILE".to_string())?;
            let source = read_json(Path::new(input))?;
            let bytes = build_resource_5(&source)?;
            write(Path::new(&target), &bytes)?;
            println!("bytes={}", bytes.len());
        }
        [command, rom, source] if command == "verify" => {
            let bytes = verify_resource_5(Path::new(rom), Path::new(source))?;
            println!("identical=true bytes={bytes} address=0x{ADDRESS:x}");
        }
        _ => println!("usage: resource_5.ts {{export ROM -o SOURCE|build SOURCE -o FILE|verify ROM SOURCE|--self-test}}"),
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn primitive_records_round_trip() {
        let item = vec![0; 44];
        assert_eq!(build_item(&export_item(&item).unwrap(), 0).unwrap(), item);
        let ability = vec![0; 16];
        assert_eq!(
            build_ability(&export_ability(&ability).unwrap(), 0).unwrap(),
            ability
        );
    }

    #[test]
    fn strict_document_header() {
        let mut value = json!({
            "format": 1, "kind": "golden-sun-gameplay-databases", "address": "0x0807a828",
            "size": "0x0000f7d8", "alignment_bytes": END - ALIGNMENT,
            "progression_groups": [0,0,0,0,0,0,0,0],
            "level_experience": (0..8).map(|_| (0..99).map(|_| json!(0)).collect::<Vec<_>>()).collect::<Vec<_>>(),
            "inventory_counter_slots": vec![Value::Null; 512], "party_order": vec![json!(0); 6],
            "items": vec![json!([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]); 324], "abilities": vec![json!({"element":0,"target":0,"damage_class":0,"effect":0,"animation":0,"range":0,"category":0,"power":0,"chance":0}); 519],
            "combatants": vec![json!({"name_slot":"zero","level":0,"hp":0,"pp":0,"attack":0,"defense":0,"agility":0,"luck":0,"turns":0,"hp_regeneration":0,"initial_abilities":[0,0,0,0],"initial_ability_counts":[0,0,0,0],"elemental_profile":0,"behavior":0,"flags":0,"secondary_abilities":[0,0,0,0],"secondary_ability_counts":[0,0,0,0],"battle_traits":[0,0,0,0],"experience_reward":0,"sprite":0,"reward_tier":0,"coin_reward":0}); 165],
            "hero_growth": vec![json!({"hp":[0,0,0,0,0,0],"pp":[0,0,0,0,0,0],"attack":[0,0,0,0,0,0],"defense":[0,0,0,0,0,0],"agility":[0,0,0,0,0,0],"luck":[0,0,0,0,0,0],"elemental_levels":[0,0,0,0],"class_id":0,"initial_abilities":[0,0,0,0,0,0,0,0,0,0,0,0,0]}); 8],
            "summon_order": vec![json!(0); 16], "summons": vec![json!({"ability_id":0,"djinn_cost":[0,0,0,0]}); 16],
            "classes": vec![json!({"family":0,"djinn_requirements":[0,0,0,0],"stat_multipliers":[0,0,0,0,0,0],"abilities":vec![json!({"id":0,"level":0});16],"traits":[0,0,0,0]});203],
            "class_family_matrix": vec![vec![json!(0);4];8],
            "elemental_profiles": vec![json!({"element":0,"levels":[0,0,0,0],"stats":vec![json!({"power":0,"resistance":0});4]});44],
            "signed_scale_curve": vec![json!({"input":0,"output":0});5],
            "djinn": vec![json!({"name_message":0,"stat_bonuses":[0,0,0,0,0,0]});80]
        });
        assert!(parse_document(&value).is_ok());
        value
            .as_object_mut()
            .unwrap()
            .insert("extra".into(), json!(1));
        assert!(parse_document(&value).is_err());
    }
}
