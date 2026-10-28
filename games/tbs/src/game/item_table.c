/// @file
/// The 324 items, by item id. Each name is the English release's.

#include "game.h"
#include "item_ids.h"
#include "ability_ids.h"

// clang-format off
const struct ItemDefinition gItem_DefinitionTable[] = {
    // ???
    {
        .element = ELEMENT_NONE,
    },
    // Long Sword
    {
        .price = 200,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 1,
        .primaryBonus = 14,
        .element = ELEMENT_NONE,
    },
    // Broad Sword
    {
        .price = 1000,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 2,
        .primaryBonus = 40,
        .element = ELEMENT_NONE,
    },
    // Claymore
    {
        .price = 4000,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 3,
        .primaryBonus = 70,
        .element = ELEMENT_NONE,
    },
    // Great Sword
    {
        .price = 7000,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 4,
        .primaryBonus = 90,
        .element = ELEMENT_NONE,
    },
    // Shamshir
    {
        .price = 10000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 5,
        .primaryBonus = 99,
        .unleashChance = 7,
        .unleashAbility = 227,
        .element = ELEMENT_NONE,
    },
    // Silver Blade
    {
        .price = 12000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 6,
        .primaryBonus = 108,
        .unleashChance = 7,
        .unleashAbility = 216,
        .element = ELEMENT_NONE,
    },
    // Masamune
    {
        .price = 13400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 7,
        .primaryBonus = 115,
        .element = ELEMENT_NONE,
    },
    // Arctic Blade
    {
        .price = 2600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 8,
        .primaryBonus = 55,
        .unleashChance = 7,
        .unleashAbility = 217,
        .element = ELEMENT_NONE,
    },
    // Gaia Blade
    {
        .price = 17000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 9,
        .primaryBonus = 135,
        .unleashChance = 7,
        .unleashAbility = 237,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_VENUS_POWER, .amount = 20},
            {.kind = EQUIP_EFFECT_ADD_VENUS_RESIST, .amount = 20},
        },
    },
    // Sol Blade
    {
        .price = 19000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 10,
        .primaryBonus = 138,
        .element = ELEMENT_NONE,
    },
    // Muramasa
    {
        .price = 13600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 11,
        .primaryBonus = 126,
        .unleashChance = 7,
        .unleashAbility = 226,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Machete
    {
        .price = 30,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_CANNOT_UNEQUIP,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET),
        .icon = 152,
        .primaryBonus = 6,
        .element = ELEMENT_NONE,
    },
    // Short Sword
    {
        .price = 120,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 12,
        .primaryBonus = 8,
        .element = ELEMENT_NONE,
    },
    // Hunter's Sword
    {
        .price = 520,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 13,
        .primaryBonus = 28,
        .element = ELEMENT_NONE,
    },
    // Battle Rapier
    {
        .price = 2900,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 14,
        .primaryBonus = 58,
        .element = ELEMENT_NONE,
    },
    // Master Rapier
    {
        .price = 6800,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 15,
        .primaryBonus = 86,
        .element = ELEMENT_NONE,
    },
    // Ninja Blade
    {
        .price = 8800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 16,
        .primaryBonus = 94,
        .unleashChance = 7,
        .unleashAbility = 232,
        .element = ELEMENT_NONE,
    },
    // Swift Sword
    {
        .price = 9400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 17,
        .primaryBonus = 104,
        .unleashChance = 7,
        .unleashAbility = 235,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_JUPITER_POWER, .amount = 10}},
    },
    // Elven Rapier
    {
        .price = 2200,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 18,
        .primaryBonus = 44,
        .unleashChance = 7,
        .unleashAbility = 228,
        .element = ELEMENT_JUPITER,
    },
    // Assassin Blade
    {
        .price = 7800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 19,
        .primaryBonus = 90,
        .unleashChance = 7,
        .unleashAbility = 213,
        .element = ELEMENT_NONE,
    },
    // Mystery Blade
    {
        .price = 6400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 20,
        .primaryBonus = 84,
        .unleashChance = 7,
        .unleashAbility = 215,
        .element = ELEMENT_NONE,
    },
    // Kikuichimonji
    {
        .price = 13400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 21,
        .primaryBonus = 128,
        .unleashChance = 7,
        .unleashAbility = 236,
        .element = ELEMENT_NONE,
    },
    // Kusanagi
    {
        .price = 19000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 22,
        .primaryBonus = 135,
        .element = ELEMENT_NONE,
    },
    // Bandit's Sword
    {
        .price = 700,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA),
        .icon = 169,
        .primaryBonus = 12,
        .unleashChance = 7,
        .unleashAbility = 234,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Battle Axe
    {
        .price = 280,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 23,
        .primaryBonus = 24,
        .element = ELEMENT_NONE,
    },
    // Broad Axe
    {
        .price = 1400,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 24,
        .primaryBonus = 50,
        .element = ELEMENT_NONE,
    },
    // Great Axe
    {
        .price = 5200,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 25,
        .primaryBonus = 80,
        .element = ELEMENT_NONE,
    },
    // Dragon Axe
    {
        .price = 10300,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 26,
        .primaryBonus = 100,
        .unleashChance = 7,
        .unleashAbility = ABILITY_HEAT_MIRAGE,
        .element = ELEMENT_NONE,
    },
    // Giant Axe
    {
        .price = 14000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 27,
        .primaryBonus = 114,
        .unleashChance = 7,
        .unleashAbility = 223,
        .element = ELEMENT_NONE,
    },
    // Vulcan Axe
    {
        .price = 4600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 28,
        .primaryBonus = 76,
        .unleashChance = 7,
        .unleashAbility = 225,
        .element = ELEMENT_MARS,
    },
    // Burning Axe
    {
        .price = 7500,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 29,
        .primaryBonus = 84,
        .unleashChance = 7,
        .unleashAbility = 222,
        .element = ELEMENT_MARS,
    },
    // Demon Axe
    {
        .price = 16000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 30,
        .primaryBonus = 132,
        .unleashChance = 7,
        .unleashAbility = 211,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Mace
    {
        .price = 80,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX),
        .icon = 31,
        .primaryBonus = 6,
        .element = ELEMENT_NONE,
    },
    // Heavy Mace
    {
        .price = 500,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX),
        .icon = 32,
        .primaryBonus = 26,
        .element = ELEMENT_NONE,
    },
    // Battle Mace
    {
        .price = 2600,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX),
        .icon = 33,
        .primaryBonus = 56,
        .element = ELEMENT_NONE,
    },
    // War Mace
    {
        .price = 6200,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX),
        .icon = 34,
        .primaryBonus = 84,
        .element = ELEMENT_NONE,
    },
    // Righteous Mace
    {
        .price = 8400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX),
        .icon = 35,
        .primaryBonus = 112,
        .unleashChance = 7,
        .unleashAbility = 230,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP_REGEN, .amount = 3}},
    },
    // Grievous Mace
    {
        .price = 7000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX),
        .icon = 36,
        .primaryBonus = 88,
        .unleashChance = 7,
        .unleashAbility = 210,
    },
    // Blessed Mace
    {
        .price = 14500,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX),
        .icon = 37,
        .primaryBonus = 126,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP_REGEN, .amount = 2}},
        .abilityId = 94,
    },
    // Wicked Mace
    {
        .price = 13800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX),
        .icon = 38,
        .primaryBonus = 130,
        .unleashChance = 7,
        .unleashAbility = 212,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Wooden Stick
    {
        .price = 40,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 39,
        .primaryBonus = 4,
        .element = ELEMENT_NONE,
    },
    // Magic Rod
    {
        .price = 380,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 40,
        .primaryBonus = 16,
        .unleashChance = 7,
        .unleashAbility = 231,
        .element = ELEMENT_NONE,
    },
    // Witch's Wand
    {
        .price = 860,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 41,
        .primaryBonus = 32,
        .unleashChance = 7,
        .unleashAbility = 229,
        .element = ELEMENT_NONE,
    },
    // Blessed Ankh
    {
        .price = 1600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 42,
        .primaryBonus = 46,
        .unleashChance = 7,
        .unleashAbility = 233,
        .element = ELEMENT_NONE,
    },
    // Psynergy Rod
    {
        .price = 3800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 43,
        .primaryBonus = 64,
        .unleashChance = 7,
        .unleashAbility = 221,
        .element = ELEMENT_NONE,
    },
    // Frost Wand
    {
        .price = 5400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 44,
        .primaryBonus = 76,
        .unleashChance = 7,
        .unleashAbility = 218,
        .element = ELEMENT_MERCURY,
    },
    // Angelic Ankh
    {
        .price = 6400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 45,
        .primaryBonus = 83,
        .unleashChance = 7,
        .unleashAbility = 220,
        .element = ELEMENT_NONE,
    },
    // Demonic Staff
    {
        .price = 10000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 46,
        .primaryBonus = 92,
        .unleashChance = 7,
        .unleashAbility = 214,
        .element = ELEMENT_NONE,
    },
    // Crystal Rod
    {
        .price = 13400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 47,
        .primaryBonus = 106,
        .unleashChance = 7,
        .unleashAbility = ABILITY_DROWN,
        .element = ELEMENT_NONE,
    },
    // Zodiac Wand
    {
        .price = 11400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 185,
        .primaryBonus = 102,
        .unleashChance = 7,
        .unleashAbility = 238,
        .element = ELEMENT_NONE,
    },
    // Shaman's Rod
    {
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 153,
        .primaryBonus = 10,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Leather Armor
    {
        .price = 240,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 48,
        .secondaryBonus = 12,
        .element = ELEMENT_NONE,
    },
    // Psynergy Armor
    {
        .price = 1000,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 49,
        .secondaryBonus = 21,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_PP, .amount = 20}},
    },
    // Chain Mail
    {
        .price = 2000,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 50,
        .secondaryBonus = 25,
        .element = ELEMENT_NONE,
    },
    // Armored Shell
    {
        .price = 3600,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 51,
        .secondaryBonus = 30,
        .element = ELEMENT_NONE,
    },
    // Plate Mail
    {
        .price = 4400,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 52,
        .secondaryBonus = 33,
        .element = ELEMENT_NONE,
    },
    // Steel Armor
    {
        .price = 4900,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 53,
        .secondaryBonus = 36,
        .element = ELEMENT_NONE,
    },
    // Spirit Armor
    {
        .price = 4000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 54,
        .secondaryBonus = 32,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_VENUS_RESIST, .amount = 15},
            {.kind = EQUIP_EFFECT_ADD_MERCURY_RESIST, .amount = 15},
            {.kind = EQUIP_EFFECT_ADD_MARS_RESIST, .amount = 15},
            {.kind = EQUIP_EFFECT_ADD_JUPITER_RESIST, .amount = 15},
        },
    },
    // Dragon Scales
    {
        .price = 17000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 55,
        .secondaryBonus = 44,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_MERCURY_RESIST, .amount = 30},
            {.kind = EQUIP_EFFECT_ADD_MARS_RESIST, .amount = 30},
        },
    },
    // Demon Mail
    {
        .price = 17000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 56,
        .secondaryBonus = 50,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_JUPITER_RESIST, .amount = -10}},
    },
    // Asura's Armor
    {
        .price = 15000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 57,
        .primaryBonus = 5,
        .secondaryBonus = 42,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP_REGEN, .amount = 8}},
    },
    // Spiked Armor
    {
        .price = 14000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 180,
        .primaryBonus = 10,
        .secondaryBonus = 34,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_UNLEASH_CHANCE, .amount = 6}},
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Cotton Shirt
    {
        .price = 20,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 58,
        .secondaryBonus = 3,
        .element = ELEMENT_NONE,
    },
    // Travel Vest
    {
        .price = 50,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 59,
        .secondaryBonus = 7,
        .element = ELEMENT_NONE,
    },
    // Fur Coat
    {
        .price = 400,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 60,
        .secondaryBonus = 16,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_MERCURY_RESIST, .amount = 20}},
    },
    // Adept's Clothes
    {
        .price = 850,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 61,
        .secondaryBonus = 18,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_PP, .amount = 8}},
    },
    // Elven Shirt
    {
        .price = 1700,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 62,
        .secondaryBonus = 22,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_SCALE_AGILITY, .amount = 15}},
    },
    // Silver Vest
    {
        .price = 3200,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 63,
        .secondaryBonus = 28,
        .element = ELEMENT_NONE,
    },
    // Water Jacket
    {
        .price = 3000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 64,
        .secondaryBonus = 30,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_MERCURY_RESIST, .amount = 30},
            {.kind = EQUIP_EFFECT_ADD_MARS_RESIST, .amount = 20},
        },
    },
    // Storm Gear
    {
        .price = 9800,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 65,
        .secondaryBonus = 42,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_MERCURY_RESIST, .amount = 30},
            {.kind = EQUIP_EFFECT_ADD_MARS_RESIST, .amount = 30},
            {.kind = EQUIP_EFFECT_ADD_JUPITER_RESIST, .amount = 30},
        },
    },
    // Kimono
    {
        .price = 2800,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 66,
        .secondaryBonus = 25,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_MARS_RESIST, .amount = 10},
            {.kind = EQUIP_EFFECT_ADD_AGILITY, .amount = 10},
        },
    },
    // Ninja Garb
    {
        .price = 6900,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 67,
        .secondaryBonus = 36,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_AGILITY, .amount = 30},
            {.kind = EQUIP_EFFECT_ADD_JUPITER_RESIST, .amount = 10},
        },
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // One-Piece Dress
    {
        .price = 25,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 68,
        .secondaryBonus = 4,
        .element = ELEMENT_NONE,
    },
    // Travel Robe
    {
        .price = 200,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 69,
        .secondaryBonus = 10,
        .element = ELEMENT_NONE,
    },
    // Silk Robe
    {
        .price = 1400,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 70,
        .secondaryBonus = 20,
        .element = ELEMENT_NONE,
    },
    // China Dress
    {
        .price = 1600,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 71,
        .secondaryBonus = 19,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_HEARTRENDER,
    },
    // Jerkin
    {
        .price = 2400,
        .type = ITEM_TYPE_ARMOR,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 72,
        .secondaryBonus = 26,
        .element = ELEMENT_NONE,
    },
    // Cocktail Dress
    {
        .price = 4000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 73,
        .secondaryBonus = 29,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_PP, .amount = 15}},
    },
    // Blessed Robe
    {
        .price = 7000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 74,
        .secondaryBonus = 36,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP_REGEN, .amount = 5}},
    },
    // Magical Cassock
    {
        .price = 9000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 75,
        .secondaryBonus = 39,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_PP_REGEN, .amount = 2}},
    },
    // Mysterious Robe
    {
        .price = 11000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 76,
        .secondaryBonus = 40,
        .element = ELEMENT_NONE,
    },
    // Feathered Robe
    {
        .price = 14000,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 77,
        .secondaryBonus = 45,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_JUPITER_POWER, .amount = 20},
            {.kind = EQUIP_EFFECT_ADD_JUPITER_RESIST, .amount = 30},
            {.kind = EQUIP_EFFECT_ADD_AGILITY, .amount = 30},
        },
    },
    // Oracle's Robe
    {
        .price = 13500,
        .type = ITEM_TYPE_ARMOR,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 78,
        .secondaryBonus = 43,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_MERCURY_RESIST, .amount = 40},
            {.kind = EQUIP_EFFECT_ADD_HP_REGEN, .amount = 10},
        },
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Wooden Shield
    {
        .price = 40,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 79,
        .secondaryBonus = 6,
        .element = ELEMENT_NONE,
    },
    // Bronze Shield
    {
        .price = 500,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 80,
        .secondaryBonus = 14,
        .element = ELEMENT_NONE,
    },
    // Iron Shield
    {
        .price = 1200,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 81,
        .secondaryBonus = 20,
        .element = ELEMENT_NONE,
    },
    // Knight's Shield
    {
        .price = 3000,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 82,
        .secondaryBonus = 28,
        .element = ELEMENT_NONE,
    },
    // Mirrored Shield
    {
        .price = 5200,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 83,
        .secondaryBonus = 39,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 116,
    },
    // Dragon Shield
    {
        .price = 2400,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 84,
        .secondaryBonus = 26,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_MARS_RESIST, .amount = 30}},
    },
    // Earth Shield
    {
        .price = 4100,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 166,
        .secondaryBonus = 31,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_VENUS_RESIST, .amount = 20}},
        .abilityId = 91,
    },
    // ?
    {0},
    // ?
    {0},
    // Padded Gloves
    {
        .price = 10,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 85,
        .secondaryBonus = 2,
        .element = ELEMENT_NONE,
    },
    // Leather Gloves
    {
        .price = 220,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 86,
        .secondaryBonus = 10,
        .element = ELEMENT_NONE,
    },
    // Gauntlets
    {
        .price = 1600,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 87,
        .secondaryBonus = 23,
        .element = ELEMENT_NONE,
    },
    // Vambrace
    {
        .price = 1800,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 88,
        .primaryBonus = 5,
        .secondaryBonus = 27,
        .element = ELEMENT_NONE,
    },
    // War Gloves
    {
        .price = 4000,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 89,
        .primaryBonus = 10,
        .secondaryBonus = 32,
        .element = ELEMENT_NONE,
    },
    // Spirit Gloves
    {
        .price = 5200,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 90,
        .secondaryBonus = 34,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_VENUS_POWER, .amount = 5},
            {.kind = EQUIP_EFFECT_ADD_MERCURY_POWER, .amount = 5},
            {.kind = EQUIP_EFFECT_ADD_MARS_POWER, .amount = 5},
            {.kind = EQUIP_EFFECT_ADD_JUPITER_POWER, .amount = 5},
        },
    },
    // Battle Gloves
    {
        .price = 2100,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 168,
        .primaryBonus = 8,
        .secondaryBonus = 26,
        .element = ELEMENT_NONE,
    },
    // Aura Gloves
    {
        .price = 6500,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 178,
        .secondaryBonus = 36,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 110,
    },
    // ?
    {0},
    // Leather Armlet
    {
        .price = 180,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 91,
        .secondaryBonus = 7,
        .element = ELEMENT_NONE,
    },
    // Armlet
    {
        .price = 900,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 92,
        .secondaryBonus = 17,
        .element = ELEMENT_NONE,
    },
    // Heavy Armlet
    {
        .price = 2000,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 93,
        .secondaryBonus = 25,
        .element = ELEMENT_NONE,
    },
    // Silver Armlet
    {
        .price = 4000,
        .type = ITEM_TYPE_SHIELD,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 94,
        .secondaryBonus = 30,
        .element = ELEMENT_NONE,
    },
    // Spirit Armlet
    {
        .price = 9000,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 95,
        .secondaryBonus = 38,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_VENUS_POWER, .amount = 10},
            {.kind = EQUIP_EFFECT_ADD_MERCURY_POWER, .amount = 10},
        },
        .abilityId = 100,
    },
    // Virtuous Armlet
    {
        .price = 7000,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 96,
        .secondaryBonus = 35,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_MARS_POWER, .amount = 10},
            {.kind = EQUIP_EFFECT_ADD_JUPITER_POWER, .amount = 10},
        },
        .abilityId = ABILITY_PLY,
    },
    // Guardian Armlet
    {
        .price = 2600,
        .type = ITEM_TYPE_SHIELD,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 167,
        .secondaryBonus = 27,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 106,
    },
    // ?
    {0},
    // ?
    {0},
    // Open Helm
    {
        .price = 180,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 97,
        .secondaryBonus = 9,
        .element = ELEMENT_NONE,
    },
    // Bronze Helm
    {
        .price = 600,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 98,
        .secondaryBonus = 14,
        .element = ELEMENT_NONE,
    },
    // Iron Helm
    {
        .price = 1600,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 99,
        .secondaryBonus = 20,
        .element = ELEMENT_NONE,
    },
    // Steel Helm
    {
        .price = 3100,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 100,
        .secondaryBonus = 27,
        .element = ELEMENT_NONE,
    },
    // Silver Helm
    {
        .price = 3900,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 101,
        .secondaryBonus = 30,
        .element = ELEMENT_NONE,
    },
    // Knight's Helm
    {
        .price = 4600,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 102,
        .secondaryBonus = 33,
        .element = ELEMENT_NONE,
    },
    // Warrior's Helm
    {
        .price = 10000,
        .type = ITEM_TYPE_HELM,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 103,
        .secondaryBonus = 35,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_VENUS_POWER, .amount = 10},
            {.kind = EQUIP_EFFECT_ADD_UNLEASH_CHANCE, .amount = 10},
        },
    },
    // Adept's Helm
    {
        .price = 3700,
        .type = ITEM_TYPE_HELM,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX),
        .icon = 104,
        .secondaryBonus = 29,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_SCALE_PP, .amount = 12}},
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Leather Cap
    {
        .price = 30,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 105,
        .secondaryBonus = 3,
        .element = ELEMENT_NONE,
    },
    // Wooden Cap
    {
        .price = 400,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 106,
        .secondaryBonus = 10,
        .element = ELEMENT_NONE,
    },
    // Mail Cap
    {
        .price = 2000,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 107,
        .secondaryBonus = 23,
        .element = ELEMENT_NONE,
    },
    // Jeweled Crown
    {
        .price = 4000,
        .type = ITEM_TYPE_HELM,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 108,
        .secondaryBonus = 35,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_LUCK, .amount = 5}},
    },
    // Ninja Hood
    {
        .price = 2800,
        .type = ITEM_TYPE_HELM,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 109,
        .secondaryBonus = 28,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_AGILITY, .amount = 20}},
    },
    // Lucky Cap
    {
        .price = 5200,
        .type = ITEM_TYPE_HELM,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 110,
        .secondaryBonus = 33,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = EQUIP_EFFECT_ADD_UNLEASH_CHANCE, .amount = 8},
            {.kind = EQUIP_EFFECT_ADD_PP_REGEN, .amount = 2},
        },
    },
    // Thunder Crown
    {
        .price = 7500,
        .type = ITEM_TYPE_HELM,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 111,
        .secondaryBonus = 40,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_PP_REGEN, .amount = 4}},
    },
    // Prophet's Hat
    {
        .price = 4600,
        .type = ITEM_TYPE_HELM,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 179,
        .secondaryBonus = 30,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 123,
    },
    // Lure Cap
    {
        .price = 3000,
        .type = ITEM_TYPE_HELM,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 189,
        .secondaryBonus = 20,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_LURE}},
    },
    // ?
    {0},
    // Circlet
    {
        .price = 120,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 112,
        .secondaryBonus = 6,
        .element = ELEMENT_NONE,
    },
    // Silver Circlet
    {
        .price = 1300,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 113,
        .secondaryBonus = 16,
        .element = ELEMENT_NONE,
    },
    // Guardian Circlet
    {
        .price = 3400,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 114,
        .secondaryBonus = 25,
        .element = ELEMENT_NONE,
    },
    // Platinum Circlet
    {
        .price = 4200,
        .type = ITEM_TYPE_HELM,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 115,
        .secondaryBonus = 29,
        .element = ELEMENT_NONE,
    },
    // Mythril Circlet
    {
        .price = 7000,
        .type = ITEM_TYPE_HELM,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 116,
        .secondaryBonus = 34,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_PP_REGEN, .amount = 3}},
    },
    // Glittering Tiara
    {
        .price = 3600,
        .type = ITEM_TYPE_HELM,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 165,
        .secondaryBonus = 27,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 116,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Herb
    {
        .price = 10,
        .flags = ITEM_STACKABLE,
        .icon = 117,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 250,
    },
    // Nut
    {
        .price = 200,
        .flags = ITEM_STACKABLE,
        .icon = 132,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 251,
    },
    // Vial
    {
        .price = 500,
        .flags = ITEM_STACKABLE,
        .icon = 133,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 252,
    },
    // Potion
    {
        .price = 1000,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 134,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 253,
    },
    // Hermes' Water
    {
        .price = 2000,
        .flags = ITEM_RARE,
        .icon = 135,
        .useType = ITEM_USE_EMPTIES,
        .element = ELEMENT_NONE,
        .abilityId = 254,
    },
    // Empty Bottle
    {
        .price = 1000,
        .flags = ITEM_RARE,
        .icon = 136,
        .element = ELEMENT_NONE,
    },
    // Psy Crystal
    {
        .price = 1500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 137,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 255,
    },
    // Antidote
    {
        .price = 20,
        .flags = ITEM_STACKABLE,
        .icon = 118,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 256,
    },
    // Elixir
    {
        .price = 30,
        .flags = ITEM_STACKABLE,
        .icon = 138,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 257,
    },
    // Water of Life
    {
        .price = 3000,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 139,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 258,
    },
    // ?
    {0},
    // Power Bread
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 140,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_POWER_BREAD,
    },
    // Cookie
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 141,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_COOKIE,
    },
    // Apple
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 142,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_APPLE,
    },
    // Hard Nut
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 143,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_HARD_NUT,
    },
    // Mint
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 144,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_MINT,
    },
    // Lucky Pepper
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 145,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_LUCKY_PEPPER,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Orb of Force
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 124,
        .useType = ITEM_USE_TEACHES,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_FORCE,
    },
    // Douse Drop
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 125,
        .useType = ITEM_USE_TEACHES,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_DOUSE,
    },
    // Frost Jewel
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 126,
        .useType = ITEM_USE_TEACHES,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_FROST,
    },
    // Lifting Gem
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 127,
        .useType = ITEM_USE_TEACHES,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_LIFT,
    },
    // Halt Gem
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 128,
        .useType = ITEM_USE_TEACHES,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_HALT,
    },
    // Cloak Ball
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 129,
        .useType = ITEM_USE_TEACHES,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_CLOAK,
    },
    // Carry Stone
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 130,
        .useType = ITEM_USE_TEACHES,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_CARRY,
    },
    // Catch Beads
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 131,
        .useType = ITEM_USE_TEACHES,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_CATCH,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Venus Star
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 119,
        .element = ELEMENT_NONE,
    },
    // Mercury Star
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 120,
        .element = ELEMENT_NONE,
    },
    // Mars Star
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 121,
        .element = ELEMENT_NONE,
    },
    // Jupiter Star
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 122,
        .element = ELEMENT_NONE,
    },
    // Mythril Bag
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 148,
        .element = ELEMENT_NONE,
    },
    // Small Jewel
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 123,
        .element = ELEMENT_NONE,
    },
    // Smoke Bomb
    {
        .price = 30,
        .flags = ITEM_STACKABLE,
        .icon = 146,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 267,
    },
    // Sleep Bomb
    {
        .price = 60,
        .flags = ITEM_STACKABLE,
        .icon = 147,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 268,
    },
    // Game Ticket
    {
        .price = 50,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 149,
        .element = ELEMENT_NONE,
    },
    // Lucky Medal
    {
        .price = 100,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 150,
        .element = ELEMENT_NONE,
    },
    // Dragon's Eye
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 151,
        .element = ELEMENT_NONE,
    },
    // Bone
    {
        .icon = 154,
        .element = ELEMENT_NONE,
    },
    // Anchor Charm
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 155,
        .element = ELEMENT_NONE,
    },
    // Corn
    {
        .price = 12,
        .icon = 173,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 270,
    },
    // Cell Key
    {
        .price = 30,
        .flags = ITEM_RARE,
        .icon = 174,
        .element = ELEMENT_NONE,
    },
    // Boat Ticket
    {
        .price = 1500,
        .flags = ITEM_INDISPENSABLE,
        .icon = 175,
        .element = ELEMENT_NONE,
    },
    // Sacred Feather
    {
        .price = 70,
        .flags = ITEM_STACKABLE,
        .icon = 176,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = ABILITY_AVOID,
    },
    // Mystic Draught
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 177,
        .element = ELEMENT_NONE,
    },
    // Oil Drop
    {
        .price = 30,
        .flags = ITEM_STACKABLE,
        .icon = 181,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 47,
    },
    // Weasel's Claw
    {
        .price = 40,
        .flags = ITEM_STACKABLE,
        .icon = 182,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 77,
    },
    // Bramble Seed
    {
        .price = 50,
        .flags = ITEM_STACKABLE,
        .icon = 183,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 17,
    },
    // Crystal Powder
    {
        .price = 60,
        .flags = ITEM_STACKABLE,
        .icon = 184,
        .useType = ITEM_USE_CONSUMED,
        .element = ELEMENT_NONE,
        .abilityId = 31,
    },
    // Black Orb
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 187,
        .element = ELEMENT_NONE,
    },
    // Red Key
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 190,
        .element = ELEMENT_NONE,
    },
    // Blue Key
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 191,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Mythril Shirt
    {
        .price = 2900,
        .type = ITEM_TYPE_SHIRT,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 156,
        .secondaryBonus = 10,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 5}},
    },
    // Silk Shirt
    {
        .price = 1800,
        .type = ITEM_TYPE_SHIRT,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA),
        .icon = 157,
        .secondaryBonus = 6,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_LUCK, .amount = 1}},
    },
    // Running Shirt
    {
        .price = 400,
        .type = ITEM_TYPE_SHIRT,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 158,
        .secondaryBonus = 1,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_AGILITY, .amount = 15}},
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Hyper Boots
    {
        .price = 2400,
        .type = ITEM_TYPE_BOOTS,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 159,
        .secondaryBonus = 4,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_UNLEASH_CHANCE, .amount = 12}},
    },
    // Quick Boots
    {
        .price = 2100,
        .type = ITEM_TYPE_BOOTS,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 160,
        .secondaryBonus = 3,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_AGILITY, .amount = 20}},
    },
    // Fur Boots
    {
        .price = 1200,
        .type = ITEM_TYPE_BOOTS,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 161,
        .secondaryBonus = 2,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_MERCURY_RESIST, .amount = 15}},
    },
    // Turtle Boots
    {
        .price = 600,
        .type = ITEM_TYPE_BOOTS,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 188,
        .secondaryBonus = 3,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_SCALE_AGILITY, .amount = 5}},
    },
    // ?
    {0},
    // ?
    {0},
    // Adept Ring
    {
        .price = 3100,
        .type = ITEM_TYPE_RING,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 162,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 269,
    },
    // War Ring
    {
        .price = 2600,
        .type = ITEM_TYPE_RING,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 163,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 102,
    },
    // Sleep Ring
    {
        .price = 1400,
        .type = ITEM_TYPE_RING,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 164,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 120,
    },
    // Healing Ring
    {
        .price = 800,
        .type = ITEM_TYPE_RING,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 170,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 90,
    },
    // Unicorn Ring
    {
        .price = 1100,
        .type = ITEM_TYPE_RING,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 171,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 99,
    },
    // Fairy Ring
    {
        .price = 2900,
        .type = ITEM_TYPE_RING,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 172,
        .useType = ITEM_USE_MAY_BREAK,
        .element = ELEMENT_NONE,
        .abilityId = 100,
    },
    // Cleric's Ring
    {
        .price = 6400,
        .type = ITEM_TYPE_RING,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 186,
        .element = ELEMENT_NONE,
        .effects = {{.kind = EQUIP_EFFECT_CURSE_REMOVABLE}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
    // (no name message)
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAbility = ABILITY_MOVE,
        .effects = {{.kind = EQUIP_EFFECT_ADD_HP, .amount = 10}},
    },
};
// clang-format on
