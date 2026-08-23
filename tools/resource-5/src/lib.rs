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
    exact_keys(item, ITEM_KEYS, &label)?;

    let name = field(item, "name")?
        .as_str()
        .ok_or_else(|| format!("{label} name must be a string"))?;
    if !is_snake_case(name) {
        return err(format!("{label} name must be unique nonempty snake_case"));
    }

    for (effect_index, effect) in list(field(item, "effects")?, 4, "item effects")?
        .iter()
        .enumerate()
    {
        exact_keys(
            object(effect, &format!("{label}.effects[{effect_index}]"))?,
            &["kind", "amount"],
            &format!("{label}.effects[{effect_index}]"),
        )?;
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
    if field(source, "format")?.as_u64() != Some(1)
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
    write_u16(&mut result, 0, u16(field(item, "price")?, "item price")?);
    result[2] = u8(field(item, "type")?, "item type")?;
    result[3] = u8(field(item, "flags")?, "item flags")?;
    write_u16(
        &mut result,
        4,
        u16(field(item, "equip_mask")?, "item equip_mask")?,
    );
    write_u16(&mut result, 6, u16(field(item, "icon")?, "item icon")?);
    write_i16(
        &mut result,
        8,
        s16(field(item, "primary_bonus")?, "item primary_bonus")?,
    );
    result[10] = s8(field(item, "secondary_bonus")?, "item secondary_bonus")? as u8;
    result[11] = u8(field(item, "secondary_flags")?, "item secondary_flags")?;
    result[12] = u8(field(item, "use_type")?, "item use_type")?;
    write_u16(
        &mut result,
        14,
        u16(
            field(item, "description_message")?,
            "item description_message",
        )?,
    );
    result[20] = u8(field(item, "element")?, "item element")?;
    for (effect_index, raw) in list(field(item, "effects")?, 4, "item effects")?
        .iter()
        .enumerate()
    {
        let effect = object(raw, &format!("items[{index}].effects[{effect_index}]"))?;
        let offset = 24 + effect_index * 4;
        result[offset] = u8(field(effect, "kind")?, "item effect kind")?;
        result[offset + 1] = s8(field(effect, "amount")?, "item effect amount")? as u8;
    }
    write_u16(
        &mut result,
        40,
        u16(field(item, "action_id")?, "item action_id")?,
    );
    Ok(result)
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
        return err(format!(
            "gameplay databases rebuilt 0x{:x} bytes",
            result.len()
        ));
    }
    Ok(result)
}

pub fn run(args: Vec<String>) -> Result<()> {
    if args.first().map(String::as_str) != Some("build-stdout") {
        return err("usage: assets 5 build-stdout SOURCE");
    }
    let source = args
        .get(1)
        .ok_or_else(|| "build-stdout requires a source".to_string())?;
    let text = fs::read_to_string(source).map_err(|error| format!("{source}: {error}"))?;
    let value: Value = serde_json::from_str(&text).map_err(|error| format!("{source}: {error}"))?;
    let bytes = build_gameplay_databases(&value)?;
    io::stdout()
        .write_all(&bytes)
        .map_err(|error| error.to_string())
}
