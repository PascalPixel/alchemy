#!/usr/bin/env bun
// Tool role: both; imported by tools/build_assets.ts; invoked by package.json.

const ROM_BASE = 0x08000000;
const ADDRESS = 0x0807a828;
const END = 0x0808a000;
const ALIGNMENT = 0x0808962c;

const LAYOUT = {
  progression_groups: [0x0807a828, 8],
  experience: [0x0807a830, 8 * 99 * 4],
  inventory_slots: [0x0807b490, 512],
  party_order: [0x0807b690, 6 * 4],
  items: [0x0807b6a8, 324 * 44],
  abilities: [0x0807ee58, 519 * 16],
  combatants: [0x08080ec8, 165 * 84],
  hero_growth: [0x080844ec, 8 * 180],
  summon_order: [0x08084a8c, 16],
  summons: [0x08084a9c, 16 * 8],
  classes: [0x08084b1c, 203 * 84],
  class_family_matrix: [0x08088db8, 8 * 4 * 4],
  elemental_profiles: [0x08088e38, 44 * 24],
  signed_scale_curve: [0x08089258, 5 * 4],
  djinn: [0x0808926c, 80 * 12],
} as const;

type Json = Record<string, any>;

function integer(value: unknown, minimum: number, maximum: number, label: string): number {
  if (!Number.isSafeInteger(value) || Number(value) < minimum || Number(value) > maximum) {
    throw new Error(`${label} is outside ${minimum}..${maximum}`);
  }
  return Number(value);
}

function list(value: unknown, size: number, label: string): any[] {
  if (!Array.isArray(value) || value.length !== size) throw new Error(`${label} must contain ${size} entries`);
  return value;
}

function object(value: unknown, label: string): Json {
  if (typeof value !== "object" || value === null || Array.isArray(value)) throw new Error(`${label} must be an object`);
  return value as Json;
}

function exactKeys(value: Json, keys: string[], label: string): void {
  const actual = Object.keys(value).sort().join(",");
  const expected = [...keys].sort().join(",");
  if (actual !== expected) throw new Error(`${label} fields differ: ${actual}`);
}

function s8(value: unknown, label: string): number { return integer(value, -0x80, 0x7f, label); }
function u8(value: unknown, label: string): number { return integer(value, 0, 0xff, label); }
function s16(value: unknown, label: string): number { return integer(value, -0x8000, 0x7fff, label); }
function u16(value: unknown, label: string): number { return integer(value, 0, 0xffff, label); }
function s32(value: unknown, label: string): number { return integer(value, -0x80000000, 0x7fffffff, label); }
function u32(value: unknown, label: string): number { return integer(value, 0, 0xffffffff, label); }

function signedByte(data: Buffer, offset: number): number { return data.readInt8(offset); }

function allZero(data: Buffer, start: number, end: number, label: string): void {
  for (let offset = start; offset < end; offset++) {
    if (data[offset] !== 0) throw new Error(`${label} has a nonzero reserved byte at +0x${offset.toString(16)}`);
  }
}

function nameSlot(data: Buffer): string {
  if (data.equals(Buffer.alloc(14, 0x20))) return "space";
  if (data.equals(Buffer.alloc(14))) return "zero";
  const sentinel = Buffer.alloc(14);
  sentinel[0] = 0xff;
  if (data.equals(sentinel)) return "sentinel";
  throw new Error("combatant name slot has an unsupported fill");
}

function buildNameSlot(value: unknown): Buffer {
  if (value === "space") return Buffer.alloc(14, 0x20);
  if (value === "zero") return Buffer.alloc(14);
  if (value === "sentinel") {
    const result = Buffer.alloc(14);
    result[0] = 0xff;
    return result;
  }
  throw new Error("combatant name_slot must be space, zero, or sentinel");
}

function parseDocument(value: unknown): Json {
  const source = object(value, "resource 5 source");
  exactKeys(source, [
    "format", "kind", "address", "size", "alignment_bytes", "progression_groups", "level_experience",
    "inventory_counter_slots", "party_order", "items", "abilities", "combatants", "hero_growth",
    "summon_order", "summons", "classes", "class_family_matrix", "elemental_profiles",
    "signed_scale_curve", "djinn",
  ], "resource 5 source");
  if (source.format !== 1 || source.kind !== "golden-sun-gameplay-databases" ||
      source.address !== "0x0807a828" || source.size !== "0x0000f7d8" ||
      source.alignment_bytes !== END - ALIGNMENT) throw new Error("resource 5 header differs");
  list(source.progression_groups, 8, "progression_groups");
  list(source.level_experience, 8, "level_experience").forEach((row, index) => list(row, 99, `level_experience[${index}]`));
  list(source.inventory_counter_slots, 512, "inventory_counter_slots");
  list(source.party_order, 6, "party_order");
  list(source.items, 324, "items");
  list(source.abilities, 519, "abilities");
  list(source.combatants, 165, "combatants");
  list(source.hero_growth, 8, "hero_growth");
  list(source.summon_order, 16, "summon_order");
  list(source.summons, 16, "summons");
  list(source.classes, 203, "classes");
  list(source.class_family_matrix, 8, "class_family_matrix").forEach((row, index) => list(row, 4, `class_family_matrix[${index}]`));
  list(source.elemental_profiles, 44, "elemental_profiles");
  list(source.signed_scale_curve, 5, "signed_scale_curve");
  list(source.djinn, 80, "djinn");
  return source;
}

function exportItem(data: Buffer): Json {
  allZero(data, 5, 6, "item");
  allZero(data, 7, 8, "item");
  allZero(data, 13, 14, "item");
  allZero(data, 16, 20, "item");
  allZero(data, 21, 24, "item");
  for (const offset of [26, 27, 30, 31, 34, 35, 38, 39, 42, 43]) {
    if (data[offset] !== 0) throw new Error(`item has a nonzero reserved byte at +0x${offset.toString(16)}`);
  }
  return {
    name_message: data.readUInt16LE(0),
    category: data[2],
    flags: data[3],
    equip_mask: data.readUInt16LE(4),
    icon: data.readUInt16LE(6),
    primary_bonus: data.readInt16LE(8),
    secondary_bonus: signedByte(data, 10),
    secondary_flags: data[11],
    use_type: data[12],
    description_message: data.readUInt16LE(14),
    element: data[20],
    effects: [24, 28, 32, 36].map((offset) => ({ kind: data[offset], amount: signedByte(data, offset + 1) })),
    unleash_ability: data.readUInt16LE(40),
  };
}

function buildItem(value: unknown, index: number): Buffer {
  const item = object(value, `items[${index}]`);
  exactKeys(item, ["name_message", "category", "flags", "equip_mask", "icon", "primary_bonus", "secondary_bonus",
    "secondary_flags", "use_type", "description_message", "element", "effects", "unleash_ability"], `items[${index}]`);
  const result = Buffer.alloc(44);
  result.writeUInt16LE(u16(item.name_message, "item name_message"), 0);
  result[2] = u8(item.category, "item category");
  result[3] = u8(item.flags, "item flags");
  result.writeUInt16LE(u16(item.equip_mask, "item equip_mask"), 4);
  result.writeUInt16LE(u16(item.icon, "item icon"), 6);
  result.writeInt16LE(s16(item.primary_bonus, "item primary_bonus"), 8);
  result.writeInt8(s8(item.secondary_bonus, "item secondary_bonus"), 10);
  result[11] = u8(item.secondary_flags, "item secondary_flags");
  result[12] = u8(item.use_type, "item use_type");
  result.writeUInt16LE(u16(item.description_message, "item description_message"), 14);
  result[20] = u8(item.element, "item element");
  list(item.effects, 4, "item effects").forEach((raw, effectIndex) => {
    const effect = object(raw, "item effect");
    exactKeys(effect, ["kind", "amount"], "item effect");
    const offset = 24 + effectIndex * 4;
    result[offset] = u8(effect.kind, "item effect kind");
    result.writeInt8(s8(effect.amount, "item effect amount"), offset + 1);
  });
  result.writeUInt16LE(u16(item.unleash_ability, "item unleash_ability"), 40);
  return result;
}

function exportAbility(data: Buffer): Json {
  allZero(data, 6, 8, "ability");
  allZero(data, 14, 16, "ability");
  return {
    element: data[0], target: data[1], damage_class: data[2], effect: data[3], animation: data.readUInt16LE(4),
    range: data[8], category: data[9], power: data.readUInt16LE(10), chance: data.readUInt16LE(12),
  };
}

function buildAbility(value: unknown, index: number): Buffer {
  const ability = object(value, `abilities[${index}]`);
  exactKeys(ability, ["element", "target", "damage_class", "effect", "animation", "range", "category", "power", "chance"], `abilities[${index}]`);
  const result = Buffer.alloc(16);
  for (const [offset, key] of [[0, "element"], [1, "target"], [2, "damage_class"], [3, "effect"], [8, "range"], [9, "category"]] as const) {
    result[offset] = u8(ability[key], `ability ${key}`);
  }
  result.writeUInt16LE(u16(ability.animation, "ability animation"), 4);
  result.writeUInt16LE(u16(ability.power, "ability power"), 10);
  result.writeUInt16LE(u16(ability.chance, "ability chance"), 12);
  return result;
}

function exportCombatant(data: Buffer): Json {
  if (data[14] !== 0) throw new Error("combatant name terminator is nonzero");
  allZero(data, 30, 40, "combatant");
  return {
    name_slot: nameSlot(data.subarray(0, 14)),
    level: data[15], hp: data.readUInt16LE(16), pp: data.readUInt16LE(18), attack: data.readUInt16LE(20),
    defense: data.readUInt16LE(22), agility: data.readUInt16LE(24), luck: data[26], turns: data[27],
    hp_regeneration: data.readUInt16LE(28),
    initial_abilities: [40, 42, 44, 46].map((offset) => data.readUInt16LE(offset)),
    initial_ability_counts: [...data.subarray(48, 52)],
    elemental_profile: data[52], behavior: data[53], flags: data.readUInt16LE(54),
    secondary_abilities: [56, 58, 60, 62].map((offset) => data.readUInt16LE(offset)),
    secondary_ability_counts: [...data.subarray(64, 68)],
    battle_traits: [68, 70, 72, 74].map((offset) => data.readUInt16LE(offset)),
    experience_reward: data.readUInt16LE(76), sprite: data.readUInt16LE(78), reward_tier: data.readUInt16LE(80),
    coin_reward: data.readUInt16LE(82),
  };
}

function buildCombatant(value: unknown, index: number): Buffer {
  const item = object(value, `combatants[${index}]`);
  exactKeys(item, ["name_slot", "level", "hp", "pp", "attack", "defense", "agility", "luck", "turns",
    "hp_regeneration", "initial_abilities", "initial_ability_counts", "elemental_profile", "behavior", "flags",
    "secondary_abilities", "secondary_ability_counts", "battle_traits", "experience_reward", "sprite", "reward_tier",
    "coin_reward"], `combatants[${index}]`);
  const result = Buffer.alloc(84);
  buildNameSlot(item.name_slot).copy(result, 0);
  result[15] = u8(item.level, "combatant level");
  for (const [offset, key] of [[16, "hp"], [18, "pp"], [20, "attack"], [22, "defense"], [24, "agility"],
    [28, "hp_regeneration"], [54, "flags"], [76, "experience_reward"], [78, "sprite"], [80, "reward_tier"],
    [82, "coin_reward"]] as const) result.writeUInt16LE(u16(item[key], `combatant ${key}`), offset);
  result[26] = u8(item.luck, "combatant luck");
  result[27] = u8(item.turns, "combatant turns");
  list(item.initial_abilities, 4, "combatant initial_abilities").forEach((entry, slot) => result.writeUInt16LE(u16(entry, "combatant initial ability"), 40 + slot * 2));
  list(item.initial_ability_counts, 4, "combatant initial_ability_counts").forEach((entry, slot) => result[48 + slot] = u8(entry, "combatant initial ability count"));
  result[52] = u8(item.elemental_profile, "combatant elemental_profile");
  result[53] = u8(item.behavior, "combatant behavior");
  list(item.secondary_abilities, 4, "combatant secondary_abilities").forEach((entry, slot) => result.writeUInt16LE(u16(entry, "combatant secondary ability"), 56 + slot * 2));
  list(item.secondary_ability_counts, 4, "combatant secondary_ability_counts").forEach((entry, slot) => result[64 + slot] = u8(entry, "combatant secondary ability count"));
  list(item.battle_traits, 4, "combatant battle_traits").forEach((entry, slot) => result.writeUInt16LE(u16(entry, "combatant battle trait"), 68 + slot * 2));
  return result;
}

function exportHeroGrowth(data: Buffer): Json {
  allZero(data, 0, 80, "hero growth");
  if (data[151] !== 0) throw new Error("hero growth class padding is nonzero");
  allZero(data, 178, 180, "hero growth");
  const checkpoint = (offset: number): number[] => Array.from({ length: 6 }, (_, index) => data.readUInt16LE(offset + index * 2));
  return {
    hp: checkpoint(80), pp: checkpoint(92), attack: checkpoint(104), defense: checkpoint(116), agility: checkpoint(128),
    luck: [...data.subarray(140, 146)], elemental_levels: [...data.subarray(146, 150)], class_id: data[150],
    initial_abilities: Array.from({ length: 13 }, (_, index) => data.readUInt16LE(152 + index * 2)),
  };
}

function buildHeroGrowth(value: unknown, index: number): Buffer {
  const growth = object(value, `hero_growth[${index}]`);
  exactKeys(growth, ["hp", "pp", "attack", "defense", "agility", "luck", "elemental_levels", "class_id", "initial_abilities"], `hero_growth[${index}]`);
  const result = Buffer.alloc(180);
  for (const [offset, key] of [[80, "hp"], [92, "pp"], [104, "attack"], [116, "defense"], [128, "agility"]] as const) {
    list(growth[key], 6, `hero growth ${key}`).forEach((entry, slot) => result.writeUInt16LE(u16(entry, `hero growth ${key}`), offset + slot * 2));
  }
  list(growth.luck, 6, "hero growth luck").forEach((entry, slot) => result[140 + slot] = u8(entry, "hero growth luck"));
  list(growth.elemental_levels, 4, "hero growth elemental_levels").forEach((entry, slot) => result[146 + slot] = u8(entry, "hero growth elemental level"));
  result[150] = u8(growth.class_id, "hero growth class_id");
  list(growth.initial_abilities, 13, "hero growth initial_abilities").forEach((entry, slot) => result.writeUInt16LE(u16(entry, "hero growth initial ability"), 152 + slot * 2));
  return result;
}

function exportClass(data: Buffer): Json {
  allZero(data, 14, 16, "class");
  for (let offset = 16; offset < 80; offset += 4) allZero(data, offset + 2, offset + 4, "class ability");
  return {
    family: data.readInt32LE(0), djinn_requirements: [...data.subarray(4, 8)], stat_multipliers: [...data.subarray(8, 14)],
    abilities: Array.from({ length: 16 }, (_, index) => ({ id: data[16 + index * 4], level: data[17 + index * 4] })),
    traits: [...data.subarray(80, 84)],
  };
}

function buildClass(value: unknown, index: number): Buffer {
  const item = object(value, `classes[${index}]`);
  exactKeys(item, ["family", "djinn_requirements", "stat_multipliers", "abilities", "traits"], `classes[${index}]`);
  const result = Buffer.alloc(84);
  result.writeInt32LE(s32(item.family, "class family"), 0);
  list(item.djinn_requirements, 4, "class djinn_requirements").forEach((entry, slot) => result[4 + slot] = u8(entry, "class djinn requirement"));
  list(item.stat_multipliers, 6, "class stat_multipliers").forEach((entry, slot) => result[8 + slot] = u8(entry, "class stat multiplier"));
  list(item.abilities, 16, "class abilities").forEach((raw, slot) => {
    const ability = object(raw, "class ability");
    exactKeys(ability, ["id", "level"], "class ability");
    result[16 + slot * 4] = u8(ability.id, "class ability id");
    result[17 + slot * 4] = u8(ability.level, "class ability level");
  });
  list(item.traits, 4, "class traits").forEach((entry, slot) => result[80 + slot] = u8(entry, "class trait"));
  return result;
}

function exportElementalProfile(data: Buffer): Json {
  return {
    element: data.readInt32LE(0), levels: [...data.subarray(4, 8)],
    stats: Array.from({ length: 4 }, (_, index) => ({ power: data.readUInt16LE(8 + index * 4), resistance: data.readUInt16LE(10 + index * 4) })),
  };
}

function buildElementalProfile(value: unknown, index: number): Buffer {
  const item = object(value, `elemental_profiles[${index}]`);
  exactKeys(item, ["element", "levels", "stats"], `elemental_profiles[${index}]`);
  const result = Buffer.alloc(24);
  result.writeInt32LE(s32(item.element, "elemental profile element"), 0);
  list(item.levels, 4, "elemental profile levels").forEach((entry, slot) => result[4 + slot] = u8(entry, "elemental profile level"));
  list(item.stats, 4, "elemental profile stats").forEach((raw, slot) => {
    const stat = object(raw, "elemental profile stat");
    exactKeys(stat, ["power", "resistance"], "elemental profile stat");
    result.writeUInt16LE(u16(stat.power, "elemental profile power"), 8 + slot * 4);
    result.writeUInt16LE(u16(stat.resistance, "elemental profile resistance"), 10 + slot * 4);
  });
  return result;
}

function exportDocument(rom: Buffer): Json {
  if (rom.length < END - ROM_BASE) throw new Error("ROM is too small for resource 5");
  const at = (address: number, size: number): Buffer => rom.subarray(address - ROM_BASE, address - ROM_BASE + size);
  const records = (address: number, count: number, size: number): Buffer[] => Array.from({ length: count }, (_, index) => at(address + index * size, size));
  const alignment = at(ALIGNMENT, END - ALIGNMENT);
  allZero(alignment, 0, alignment.length, "resource 5 alignment");
  const experience = at(LAYOUT.experience[0], LAYOUT.experience[1]);
  const inventory = at(LAYOUT.inventory_slots[0], LAYOUT.inventory_slots[1]);
  const party = at(LAYOUT.party_order[0], LAYOUT.party_order[1]);
  return {
    format: 1,
    kind: "golden-sun-gameplay-databases",
    address: "0x0807a828",
    size: "0x0000f7d8",
    alignment_bytes: alignment.length,
    progression_groups: [...at(LAYOUT.progression_groups[0], LAYOUT.progression_groups[1])],
    level_experience: Array.from({ length: 8 }, (_, hero) => Array.from({ length: 99 }, (_, level) => experience.readUInt32LE((hero * 99 + level) * 4))),
    inventory_counter_slots: [...inventory].map((value) => value === 0 ? null : value - 1),
    party_order: Array.from({ length: 6 }, (_, index) => party.readInt32LE(index * 4)),
    items: records(LAYOUT.items[0], 324, 44).map(exportItem),
    abilities: records(LAYOUT.abilities[0], 519, 16).map(exportAbility),
    combatants: records(LAYOUT.combatants[0], 165, 84).map(exportCombatant),
    hero_growth: records(LAYOUT.hero_growth[0], 8, 180).map(exportHeroGrowth),
    summon_order: [...at(LAYOUT.summon_order[0], 16)],
    summons: records(LAYOUT.summons[0], 16, 8).map((data) => ({ ability_id: data.readUInt32LE(0), djinn_cost: [...data.subarray(4, 8)] })),
    classes: records(LAYOUT.classes[0], 203, 84).map(exportClass),
    class_family_matrix: Array.from({ length: 8 }, (_, row) => Array.from({ length: 4 }, (_, column) => at(LAYOUT.class_family_matrix[0], 128).readInt32LE((row * 4 + column) * 4))),
    elemental_profiles: records(LAYOUT.elemental_profiles[0], 44, 24).map(exportElementalProfile),
    signed_scale_curve: records(LAYOUT.signed_scale_curve[0], 5, 4).map((data) => ({ input: data.readInt16LE(0), output: data.readInt16LE(2) })),
    djinn: records(LAYOUT.djinn[0], 80, 12).map((data) => {
      allZero(data, 2, 4, "Djinn");
      allZero(data, 10, 12, "Djinn");
      return { name_message: data.readUInt16LE(0), stat_bonuses: Array.from({ length: 6 }, (_, index) => signedByte(data, 4 + index)) };
    }),
  };
}

export function build_resource_5(value: unknown): Buffer {
  const source = parseDocument(value);
  const chunks: Buffer[] = [];
  chunks.push(Buffer.from(list(source.progression_groups, 8, "progression_groups").map((entry) => u8(entry, "progression group"))));
  const experience = Buffer.alloc(8 * 99 * 4);
  source.level_experience.forEach((row: unknown[], hero: number) => row.forEach((entry, level) => experience.writeUInt32LE(u32(entry, "level experience"), (hero * 99 + level) * 4)));
  chunks.push(experience);
  chunks.push(Buffer.from(source.inventory_counter_slots.map((entry: unknown) => entry === null ? 0 : integer(entry, 0, 0xfe, "inventory counter slot") + 1)));
  const party = Buffer.alloc(24);
  source.party_order.forEach((entry: unknown, index: number) => party.writeInt32LE(s32(entry, "party order"), index * 4));
  chunks.push(party);
  chunks.push(Buffer.concat(source.items.map(buildItem)));
  chunks.push(Buffer.concat(source.abilities.map(buildAbility)));
  chunks.push(Buffer.concat(source.combatants.map(buildCombatant)));
  chunks.push(Buffer.concat(source.hero_growth.map(buildHeroGrowth)));
  chunks.push(Buffer.from(source.summon_order.map((entry: unknown) => u8(entry, "summon order"))));
  chunks.push(Buffer.concat(source.summons.map((raw: unknown, index: number) => {
    const summon = object(raw, `summons[${index}]`);
    exactKeys(summon, ["ability_id", "djinn_cost"], `summons[${index}]`);
    const result = Buffer.alloc(8);
    result.writeUInt32LE(u32(summon.ability_id, "summon ability_id"), 0);
    list(summon.djinn_cost, 4, "summon djinn_cost").forEach((entry, slot) => result[4 + slot] = u8(entry, "summon Djinn cost"));
    return result;
  })));
  chunks.push(Buffer.concat(source.classes.map(buildClass)));
  const matrix = Buffer.alloc(128);
  source.class_family_matrix.forEach((row: unknown[], rowIndex: number) => row.forEach((entry, column) => matrix.writeInt32LE(s32(entry, "class family matrix"), (rowIndex * 4 + column) * 4)));
  chunks.push(matrix);
  chunks.push(Buffer.concat(source.elemental_profiles.map(buildElementalProfile)));
  chunks.push(Buffer.concat(source.signed_scale_curve.map((raw: unknown, index: number) => {
    const point = object(raw, `signed_scale_curve[${index}]`);
    exactKeys(point, ["input", "output"], `signed_scale_curve[${index}]`);
    const result = Buffer.alloc(4);
    result.writeInt16LE(s16(point.input, "scale input"), 0);
    result.writeInt16LE(s16(point.output, "scale output"), 2);
    return result;
  })));
  chunks.push(Buffer.concat(source.djinn.map((raw: unknown, index: number) => {
    const djinni = object(raw, `djinn[${index}]`);
    exactKeys(djinni, ["name_message", "stat_bonuses"], `djinn[${index}]`);
    const result = Buffer.alloc(12);
    result.writeUInt16LE(u16(djinni.name_message, "Djinn name_message"), 0);
    list(djinni.stat_bonuses, 6, "Djinn stat_bonuses").forEach((entry, slot) => result.writeInt8(s8(entry, "Djinn stat bonus"), 4 + slot));
    return result;
  })));
  chunks.push(Buffer.alloc(source.alignment_bytes));
  const result = Buffer.concat(chunks);
  if (result.length !== END - ADDRESS) throw new Error(`resource 5 rebuilt 0x${result.length.toString(16)} bytes`);
  return result;
}

function positional(args: string[]): string[] {
  const valued = new Set(["-o", "--output"]);
  return args.filter((entry, index) => !entry.startsWith("-") && !valued.has(args[index - 1]));
}

function output(args: string[]): string | undefined {
  const index = Math.max(args.indexOf("-o"), args.indexOf("--output"));
  return index >= 0 ? args[index + 1] : undefined;
}

async function selfTest(): Promise<void> {
  const name = Buffer.alloc(14, 0x20);
  if (nameSlot(name) !== "space" || !buildNameSlot("space").equals(name)) throw new Error("name-slot self-test failed");
  const item = Buffer.alloc(44);
  if (!buildItem(exportItem(item), 0).equals(item)) throw new Error("item self-test failed");
  const ability = Buffer.alloc(16);
  if (!buildAbility(exportAbility(ability), 0).equals(ability)) throw new Error("ability self-test failed");
  console.log("self-test=ok");
}

async function main(args: string[]): Promise<void> {
  if (args.includes("--self-test")) {
    await selfTest();
    if (args.length === 1) return;
    args = args.filter((entry) => entry !== "--self-test");
  }
  const [command, input, sourcePath] = positional(args);
  const target = output(args);
  if (command === "export") {
    if (!input || !target) throw new Error("export requires ROM and -o SOURCE");
    const rom = Buffer.from(await Bun.file(input).arrayBuffer());
    const source = exportDocument(rom);
    const rebuilt = build_resource_5(source);
    const original = rom.subarray(ADDRESS - ROM_BASE, END - ROM_BASE);
    if (!rebuilt.equals(original)) throw new Error("exported resource 5 does not round-trip");
    await Bun.write(target, `${JSON.stringify(source, null, 2)}\n`);
    console.log(`identical=true bytes=${rebuilt.length} items=324 abilities=519 combatants=165 classes=203 djinn=80`);
    return;
  }
  if (command === "build") {
    if (!input || !target) throw new Error("build requires SOURCE and -o FILE");
    const built = build_resource_5(await Bun.file(input).json());
    await Bun.write(target, built);
    console.log(`bytes=${built.length}`);
    return;
  }
  if (command === "verify") {
    if (!input || !sourcePath) throw new Error("verify requires ROM and SOURCE");
    const rom = Buffer.from(await Bun.file(input).arrayBuffer());
    const built = build_resource_5(await Bun.file(sourcePath).json());
    const original = rom.subarray(ADDRESS - ROM_BASE, END - ROM_BASE);
    if (!built.equals(original)) throw new Error("resource 5 differs from ROM");
    console.log(`identical=true bytes=${built.length} address=0x${ADDRESS.toString(16)}`);
    return;
  }
  console.log("usage: resource_5.ts {export ROM -o SOURCE|build SOURCE -o FILE|verify ROM SOURCE|--self-test}");
}

if (import.meta.main) await main(Bun.argv.slice(2));
