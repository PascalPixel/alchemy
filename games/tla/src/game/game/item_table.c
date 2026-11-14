/// @file
/// The 516 items, by item id. Each name is the English release's
/// (MsgItemName plus the item id); the 55 rows past the last name have none.
// Part of game.c.

#include "../game_internal.h"
#include "game/character_ids.h"

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
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 1,
        .primaryBonus = 14,
        .element = ELEMENT_NONE,
    },
    // Broad Sword
    {
        .price = 1000,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 2,
        .primaryBonus = 40,
        .element = ELEMENT_NONE,
    },
    // Claymore
    {
        .price = 4000,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 3,
        .primaryBonus = 70,
        .element = ELEMENT_NONE,
    },
    // Great Sword
    {
        .price = 7000,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 4,
        .primaryBonus = 90,
        .element = ELEMENT_NONE,
    },
    // Shamshir
    {
        .price = 10000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 5,
        .primaryBonus = 99,
        .unleashChance = 7,
        .unleashAction = 227,
        .element = ELEMENT_NONE,
    },
    // Silver Blade
    {
        .price = 12000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 6,
        .primaryBonus = 108,
        .unleashChance = 7,
        .unleashAction = 216,
        .element = ELEMENT_NONE,
    },
    // Fire Brand
    {
        .price = 23400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 7,
        .primaryBonus = 176,
        .unleashChance = 7,
        .unleashAction = 558,
        .element = ELEMENT_MARS,
    },
    // Arctic Blade
    {
        .price = 2600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 8,
        .primaryBonus = 55,
        .unleashChance = 7,
        .unleashAction = 217,
        .element = ELEMENT_NONE,
    },
    // Gaia Blade
    {
        .price = 17000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 9,
        .primaryBonus = 135,
        .unleashChance = 7,
        .unleashAction = 237,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_VENUS_POWER, .amount = 20},
            {.kind = ITEM_EFFECT_ADD_VENUS_RESIST, .amount = 20},
        },
    },
    // Sol Blade
    {
        .price = 31200,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(FELIX),
        .icon = 10,
        .primaryBonus = 200,
        .unleashChance = 7,
        .unleashAction = 531,
        .element = ELEMENT_NONE,
    },
    // Muramasa
    {
        .price = 13600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 11,
        .primaryBonus = 126,
        .unleashChance = 7,
        .unleashAction = 226,
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
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 12,
        .primaryBonus = 8,
        .element = ELEMENT_NONE,
    },
    // Hunter's Sword
    {
        .price = 520,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 13,
        .primaryBonus = 28,
        .element = ELEMENT_NONE,
    },
    // Battle Rapier
    {
        .price = 2900,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 14,
        .primaryBonus = 58,
        .element = ELEMENT_NONE,
    },
    // Master Rapier
    {
        .price = 6800,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 15,
        .primaryBonus = 86,
        .element = ELEMENT_NONE,
    },
    // Ninja Blade
    {
        .price = 8800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 16,
        .primaryBonus = 94,
        .unleashChance = 7,
        .unleashAction = 232,
        .element = ELEMENT_NONE,
    },
    // Swift Sword
    {
        .price = 9400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 17,
        .primaryBonus = 104,
        .unleashChance = 7,
        .unleashAction = 235,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_JUPITER_POWER, .amount = 10}},
    },
    // Elven Rapier
    {
        .price = 2200,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 18,
        .primaryBonus = 44,
        .unleashChance = 7,
        .unleashAction = 228,
        .element = ELEMENT_JUPITER,
    },
    // Assassin Blade
    {
        .price = 7800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 19,
        .primaryBonus = 90,
        .unleashChance = 7,
        .unleashAction = 213,
        .element = ELEMENT_NONE,
    },
    // Mystery Blade
    {
        .price = 6400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 20,
        .primaryBonus = 84,
        .unleashChance = 7,
        .unleashAction = 215,
        .element = ELEMENT_NONE,
    },
    // Kikuichimonji
    {
        .price = 13400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 21,
        .primaryBonus = 128,
        .unleashChance = 7,
        .unleashAction = 236,
        .element = ELEMENT_NONE,
    },
    // Masamune
    {
        .price = 16400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 22,
        .primaryBonus = 161,
        .unleashChance = 8,
        .unleashAction = 544,
        .element = ELEMENT_NONE,
    },
    // Bandit's Sword
    {
        .price = 700,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 169,
        .primaryBonus = 12,
        .unleashChance = 7,
        .unleashAction = 234,
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
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 23,
        .primaryBonus = 24,
        .element = ELEMENT_NONE,
    },
    // Broad Axe
    {
        .price = 1400,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 24,
        .primaryBonus = 50,
        .element = ELEMENT_NONE,
    },
    // Great Axe
    {
        .price = 5200,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 25,
        .primaryBonus = 80,
        .element = ELEMENT_NONE,
    },
    // Dragon Axe
    {
        .price = 10300,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 26,
        .primaryBonus = 100,
        .unleashChance = 7,
        .unleashAction = 224,
        .element = ELEMENT_NONE,
    },
    // Giant Axe
    {
        .price = 14000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 27,
        .primaryBonus = 114,
        .unleashChance = 7,
        .unleashAction = 223,
        .element = ELEMENT_NONE,
    },
    // Vulcan Axe
    {
        .price = 4600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 28,
        .primaryBonus = 76,
        .unleashChance = 7,
        .unleashAction = 225,
        .element = ELEMENT_MARS,
    },
    // Burning Axe
    {
        .price = 7500,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 29,
        .primaryBonus = 84,
        .unleashChance = 7,
        .unleashAction = 222,
        .element = ELEMENT_MARS,
    },
    // Demon Axe
    {
        .price = 16000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 30,
        .primaryBonus = 132,
        .unleashChance = 7,
        .unleashAction = 211,
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
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 31,
        .primaryBonus = 6,
        .element = ELEMENT_NONE,
    },
    // Heavy Mace
    {
        .price = 500,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 32,
        .primaryBonus = 26,
        .element = ELEMENT_NONE,
    },
    // Battle Mace
    {
        .price = 2600,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 33,
        .primaryBonus = 56,
        .element = ELEMENT_NONE,
    },
    // War Mace
    {
        .price = 6200,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 34,
        .primaryBonus = 84,
        .element = ELEMENT_NONE,
    },
    // Righteous Mace
    {
        .price = 8400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 35,
        .primaryBonus = 112,
        .unleashChance = 7,
        .unleashAction = 230,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP_REGEN, .amount = 3}},
    },
    // Grievous Mace
    {
        .price = 7000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 36,
        .primaryBonus = 88,
        .unleashChance = 7,
        .unleashAction = 210,
    },
    // Blessed Mace
    {
        .price = 14500,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 37,
        .primaryBonus = 126,
        .useType = 2,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP_REGEN, .amount = 2}},
        .actionId = 94,
    },
    // Wicked Mace
    {
        .price = 13800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 38,
        .primaryBonus = 130,
        .unleashChance = 7,
        .unleashAction = 212,
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
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 39,
        .primaryBonus = 4,
        .element = ELEMENT_NONE,
    },
    // Magic Rod
    {
        .price = 380,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 40,
        .primaryBonus = 16,
        .unleashChance = 7,
        .unleashAction = 231,
        .element = ELEMENT_NONE,
    },
    // Witch's Wand
    {
        .price = 860,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 41,
        .primaryBonus = 32,
        .unleashChance = 7,
        .unleashAction = 229,
        .element = ELEMENT_NONE,
    },
    // Blessed Ankh
    {
        .price = 1600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 42,
        .primaryBonus = 46,
        .unleashChance = 7,
        .unleashAction = 233,
        .element = ELEMENT_NONE,
    },
    // Psynergy Rod
    {
        .price = 3800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 43,
        .primaryBonus = 64,
        .unleashChance = 7,
        .unleashAction = 221,
        .element = ELEMENT_NONE,
    },
    // Frost Wand
    {
        .price = 5400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 44,
        .primaryBonus = 76,
        .unleashChance = 7,
        .unleashAction = 218,
        .element = ELEMENT_MERCURY,
    },
    // Angelic Ankh
    {
        .price = 6400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 45,
        .primaryBonus = 83,
        .unleashChance = 7,
        .unleashAction = 220,
        .element = ELEMENT_NONE,
    },
    // Demonic Staff
    {
        .price = 10000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 46,
        .primaryBonus = 92,
        .unleashChance = 7,
        .unleashAction = 214,
        .element = ELEMENT_NONE,
    },
    // Crystal Rod
    {
        .price = 13400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 47,
        .primaryBonus = 106,
        .unleashChance = 7,
        .unleashAction = 219,
        .element = ELEMENT_NONE,
    },
    // Zodiac Wand
    {
        .price = 11400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 185,
        .primaryBonus = 102,
        .unleashChance = 7,
        .unleashAction = 238,
        .element = ELEMENT_NONE,
    },
    // Shaman's Rod
    {
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
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
        .type = 2,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 48,
        .secondaryBonus = 12,
        .element = ELEMENT_NONE,
    },
    // Psynergy Armor
    {
        .price = 1000,
        .type = 2,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 49,
        .secondaryBonus = 21,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP, .amount = 20}},
    },
    // Chain Mail
    {
        .price = 2000,
        .type = 2,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 50,
        .secondaryBonus = 25,
        .element = ELEMENT_NONE,
    },
    // Armored Shell
    {
        .price = 3600,
        .type = 2,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 51,
        .secondaryBonus = 30,
        .element = ELEMENT_NONE,
    },
    // Plate Mail
    {
        .price = 4400,
        .type = 2,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 52,
        .secondaryBonus = 33,
        .element = ELEMENT_NONE,
    },
    // Steel Armor
    {
        .price = 4900,
        .type = 2,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 53,
        .secondaryBonus = 36,
        .element = ELEMENT_NONE,
    },
    // Spirit Armor
    {
        .price = 4000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 54,
        .secondaryBonus = 32,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_VENUS_RESIST, .amount = 15},
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 15},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 15},
            {.kind = ITEM_EFFECT_ADD_JUPITER_RESIST, .amount = 15},
        },
    },
    // Dragon Scales
    {
        .price = 17000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 55,
        .secondaryBonus = 44,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 30},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 30},
        },
    },
    // Demon Mail
    {
        .price = 17000,
        .type = 2,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 56,
        .secondaryBonus = 50,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_JUPITER_RESIST, .amount = -10}},
    },
    // Asura's Armor
    {
        .price = 15000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 57,
        .primaryBonus = 5,
        .secondaryBonus = 42,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP_REGEN, .amount = 8}},
    },
    // Spiked Armor
    {
        .price = 14000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 180,
        .primaryBonus = 10,
        .secondaryBonus = 34,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 6}},
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
        .type = 2,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 58,
        .secondaryBonus = 3,
        .element = ELEMENT_NONE,
    },
    // Travel Vest
    {
        .price = 50,
        .type = 2,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 59,
        .secondaryBonus = 7,
        .element = ELEMENT_NONE,
    },
    // Fur Coat
    {
        .price = 400,
        .type = 2,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 60,
        .secondaryBonus = 16,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 20}},
    },
    // Adept's Clothes
    {
        .price = 850,
        .type = 2,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 61,
        .secondaryBonus = 18,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP, .amount = 8}},
    },
    // Elven Shirt
    {
        .price = 1700,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 62,
        .secondaryBonus = 22,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_SCALE_AGILITY, .amount = 15}},
    },
    // Silver Vest
    {
        .price = 3200,
        .type = 2,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 63,
        .secondaryBonus = 28,
        .element = ELEMENT_NONE,
    },
    // Water Jacket
    {
        .price = 3000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 64,
        .secondaryBonus = 30,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 30},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 20},
        },
    },
    // Storm Gear
    {
        .price = 9800,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 65,
        .secondaryBonus = 42,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 30},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 30},
            {.kind = ITEM_EFFECT_ADD_JUPITER_RESIST, .amount = 30},
        },
    },
    // Kimono
    {
        .price = 2800,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 66,
        .secondaryBonus = 25,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 10},
            {.kind = ITEM_EFFECT_ADD_AGILITY, .amount = 10},
        },
    },
    // Ninja Garb
    {
        .price = 6900,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 67,
        .secondaryBonus = 36,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_AGILITY, .amount = 30},
            {.kind = ITEM_EFFECT_ADD_JUPITER_RESIST, .amount = 10},
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
        .type = 2,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 68,
        .secondaryBonus = 4,
        .element = ELEMENT_NONE,
    },
    // Travel Robe
    {
        .price = 200,
        .type = 2,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 69,
        .secondaryBonus = 10,
        .element = ELEMENT_NONE,
    },
    // Silk Robe
    {
        .price = 1400,
        .type = 2,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 70,
        .secondaryBonus = 20,
        .element = ELEMENT_NONE,
    },
    // China Dress
    {
        .price = 1600,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 71,
        .secondaryBonus = 19,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 488,
    },
    // Jerkin
    {
        .price = 2400,
        .type = 2,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 72,
        .secondaryBonus = 26,
        .element = ELEMENT_NONE,
    },
    // Cocktail Dress
    {
        .price = 4000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 73,
        .secondaryBonus = 29,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP, .amount = 15}},
    },
    // Blessed Robe
    {
        .price = 7000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 74,
        .secondaryBonus = 36,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP_REGEN, .amount = 5}},
    },
    // Magical Cassock
    {
        .price = 9000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 75,
        .secondaryBonus = 39,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP_REGEN, .amount = 2}},
    },
    // Mysterious Robe
    {
        .price = -29036,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 76,
        .secondaryBonus = 48,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_HP_REGEN, .amount = 20},
            {.kind = ITEM_EFFECT_ADD_PP_REGEN, .amount = 10},
        },
    },
    // Feathered Robe
    {
        .price = 14000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 77,
        .secondaryBonus = 45,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_JUPITER_POWER, .amount = 20},
            {.kind = ITEM_EFFECT_ADD_JUPITER_RESIST, .amount = 30},
            {.kind = ITEM_EFFECT_ADD_AGILITY, .amount = 30},
        },
    },
    // Oracle's Robe
    {
        .price = 13500,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 78,
        .secondaryBonus = 43,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 40},
            {.kind = ITEM_EFFECT_ADD_HP_REGEN, .amount = 10},
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
        .type = 3,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 79,
        .secondaryBonus = 6,
        .element = ELEMENT_NONE,
    },
    // Bronze Shield
    {
        .price = 500,
        .type = 3,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 80,
        .secondaryBonus = 14,
        .element = ELEMENT_NONE,
    },
    // Iron Shield
    {
        .price = 1200,
        .type = 3,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 81,
        .secondaryBonus = 20,
        .element = ELEMENT_NONE,
    },
    // Knight's Shield
    {
        .price = 3000,
        .type = 3,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 82,
        .secondaryBonus = 28,
        .element = ELEMENT_NONE,
    },
    // Mirrored Shield
    {
        .price = 5200,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 83,
        .secondaryBonus = 39,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 116,
    },
    // Dragon Shield
    {
        .price = 2400,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 84,
        .secondaryBonus = 26,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 30}},
    },
    // Earth Shield
    {
        .price = 4100,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 166,
        .secondaryBonus = 31,
        .useType = 2,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_VENUS_RESIST, .amount = 20}},
        .actionId = 91,
    },
    // ?
    {0},
    // ?
    {0},
    // Padded Gloves
    {
        .price = 10,
        .type = 3,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 85,
        .secondaryBonus = 2,
        .element = ELEMENT_NONE,
    },
    // Leather Gloves
    {
        .price = 220,
        .type = 3,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 86,
        .secondaryBonus = 10,
        .element = ELEMENT_NONE,
    },
    // Gauntlets
    {
        .price = 1600,
        .type = 3,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 87,
        .secondaryBonus = 23,
        .element = ELEMENT_NONE,
    },
    // Vambrace
    {
        .price = 1800,
        .type = 3,
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
        .type = 3,
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
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 90,
        .secondaryBonus = 34,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_VENUS_POWER, .amount = 5},
            {.kind = ITEM_EFFECT_ADD_MERCURY_POWER, .amount = 5},
            {.kind = ITEM_EFFECT_ADD_MARS_POWER, .amount = 5},
            {.kind = ITEM_EFFECT_ADD_JUPITER_POWER, .amount = 5},
        },
    },
    // Battle Gloves
    {
        .price = 2100,
        .type = 3,
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
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 178,
        .secondaryBonus = 36,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 110,
    },
    // ?
    {0},
    // Leather Armlet
    {
        .price = 180,
        .type = 3,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 91,
        .secondaryBonus = 7,
        .element = ELEMENT_NONE,
    },
    // Armlet
    {
        .price = 900,
        .type = 3,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 92,
        .secondaryBonus = 17,
        .element = ELEMENT_NONE,
    },
    // Heavy Armlet
    {
        .price = 2000,
        .type = 3,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 93,
        .secondaryBonus = 25,
        .element = ELEMENT_NONE,
    },
    // Silver Armlet
    {
        .price = 4000,
        .type = 3,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 94,
        .secondaryBonus = 30,
        .element = ELEMENT_NONE,
    },
    // Spirit Armlet
    {
        .price = 9000,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 95,
        .secondaryBonus = 38,
        .useType = 2,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_VENUS_POWER, .amount = 10},
            {.kind = ITEM_EFFECT_ADD_MERCURY_POWER, .amount = 10},
        },
        .actionId = 100,
    },
    // Virtuous Armlet
    {
        .price = 7000,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 96,
        .secondaryBonus = 35,
        .useType = 2,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MARS_POWER, .amount = 10},
            {.kind = ITEM_EFFECT_ADD_JUPITER_POWER, .amount = 10},
        },
        .actionId = 93,
    },
    // Guardian Armlet
    {
        .price = 2600,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 167,
        .secondaryBonus = 27,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 106,
    },
    // ?
    {0},
    // ?
    {0},
    // Open Helm
    {
        .price = 180,
        .type = 4,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 97,
        .secondaryBonus = 9,
        .element = ELEMENT_NONE,
    },
    // Bronze Helm
    {
        .price = 600,
        .type = 4,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 98,
        .secondaryBonus = 14,
        .element = ELEMENT_NONE,
    },
    // Iron Helm
    {
        .price = 1600,
        .type = 4,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 99,
        .secondaryBonus = 20,
        .element = ELEMENT_NONE,
    },
    // Steel Helm
    {
        .price = 3100,
        .type = 4,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 100,
        .secondaryBonus = 27,
        .element = ELEMENT_NONE,
    },
    // Silver Helm
    {
        .price = 3900,
        .type = 4,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 101,
        .secondaryBonus = 30,
        .element = ELEMENT_NONE,
    },
    // Knight's Helm
    {
        .price = 4600,
        .type = 4,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 102,
        .secondaryBonus = 33,
        .element = ELEMENT_NONE,
    },
    // Warrior's Helm
    {
        .price = 10000,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 103,
        .secondaryBonus = 35,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_VENUS_POWER, .amount = 10},
            {.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 10},
        },
    },
    // Adept's Helm
    {
        .price = 3700,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 104,
        .secondaryBonus = 29,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_SCALE_PP, .amount = 12}},
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
        .type = 4,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 105,
        .secondaryBonus = 3,
        .element = ELEMENT_NONE,
    },
    // Wooden Cap
    {
        .price = 400,
        .type = 4,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 106,
        .secondaryBonus = 10,
        .element = ELEMENT_NONE,
    },
    // Mail Cap
    {
        .price = 2000,
        .type = 4,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 107,
        .secondaryBonus = 23,
        .element = ELEMENT_NONE,
    },
    // Jeweled Crown
    {
        .price = 4000,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 108,
        .secondaryBonus = 35,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_LUCK, .amount = 5}},
    },
    // Ninja Hood
    {
        .price = 2800,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 109,
        .secondaryBonus = 28,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_AGILITY, .amount = 20}},
    },
    // Lucky Cap
    {
        .price = 5200,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 110,
        .secondaryBonus = 33,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 8},
            {.kind = ITEM_EFFECT_ADD_PP_REGEN, .amount = 2},
        },
    },
    // Thunder Crown
    {
        .price = 7500,
        .type = 4,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 111,
        .secondaryBonus = 40,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP_REGEN, .amount = 4}},
    },
    // Prophet's Hat
    {
        .price = 4600,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 179,
        .secondaryBonus = 30,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 123,
    },
    // Lure Cap
    {
        .price = 3000,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 189,
        .secondaryBonus = 20,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_LURE}},
    },
    // ?
    {0},
    // Circlet
    {
        .price = 120,
        .type = 4,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 112,
        .secondaryBonus = 6,
        .element = ELEMENT_NONE,
    },
    // Silver Circlet
    {
        .price = 1300,
        .type = 4,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 113,
        .secondaryBonus = 16,
        .element = ELEMENT_NONE,
    },
    // Guardian Circlet
    {
        .price = 3400,
        .type = 4,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 114,
        .secondaryBonus = 25,
        .element = ELEMENT_NONE,
    },
    // Platinum Circlet
    {
        .price = 4200,
        .type = 4,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 115,
        .secondaryBonus = 29,
        .element = ELEMENT_NONE,
    },
    // Mythril Circlet
    {
        .price = 7000,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 116,
        .secondaryBonus = 34,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP_REGEN, .amount = 3}},
    },
    // Glittering Tiara
    {
        .price = 3600,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 165,
        .secondaryBonus = 27,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 116,
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
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 250,
    },
    // Nut
    {
        .price = 200,
        .flags = ITEM_STACKABLE,
        .icon = 132,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 251,
    },
    // Vial
    {
        .price = 500,
        .flags = ITEM_STACKABLE,
        .icon = 133,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 252,
    },
    // Potion
    {
        .price = 1000,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 134,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 253,
    },
    // Hermes' Water
    {
        .price = 2000,
        .flags = ITEM_RARE,
        .icon = 135,
        .useType = 4,
        .element = ELEMENT_NONE,
        .actionId = 254,
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
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 255,
    },
    // Antidote
    {
        .price = 20,
        .flags = ITEM_STACKABLE,
        .icon = 118,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 256,
    },
    // Elixir
    {
        .price = 30,
        .flags = ITEM_STACKABLE,
        .icon = 138,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 257,
    },
    // Water of Life
    {
        .price = 3000,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 139,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 258,
    },
    // Mist Potion
    {
        .price = 9000,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 360,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 259,
    },
    // Power Bread
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 140,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 260,
    },
    // Cookie
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 141,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 261,
    },
    // Apple
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 142,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 262,
    },
    // Hard Nut
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 143,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 263,
    },
    // Mint
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 144,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 264,
    },
    // Lucky Pepper
    {
        .price = 500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 145,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 265,
    },
    // ?
    {0},
    // Lash Pebble
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 339,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 133,
    },
    // Pound Cube
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 340,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 134,
    },
    // Orb of Force
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 124,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 142,
    },
    // Douse Drop
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 125,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 33,
    },
    // Frost Jewel
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 126,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 24,
    },
    // Lifting Gem
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 127,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 143,
    },
    // Halt Gem
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 128,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 145,
    },
    // Cloak Ball
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 129,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 146,
    },
    // Carry Stone
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 130,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 147,
    },
    // Catch Beads
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 131,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 148,
    },
    // Tremor Bit
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 345,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 135,
    },
    // Scoop Gem
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 349,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 136,
    },
    // Cyclone Chip
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 343,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 137,
    },
    // ?
    {0},
    // ?
    {0},
    // Burst Brooch
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 346,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 151,
    },
    // Grindstone
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(FELIX),
        .icon = 347,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 152,
    },
    // Hover Jade
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 342,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 153,
    },
    // ?
    {0},
    // Teleport Lapis
    {
        .type = ITEM_TYPE_PSYNERGY,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 351,
        .useType = 3,
        .element = ELEMENT_NONE,
        .actionId = 156,
    },
    // ?
    {0},
    // ?
    {0},
    // Venus Star
    {
        .type = 11,
        .flags = ITEM_CANNOT_UNEQUIP | ITEM_INDISPENSABLE | 0x20,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(FELIX),
        .icon = 119,
        .element = ELEMENT_NONE,
    },
    // Mercury Star
    {
        .type = 11,
        .flags = ITEM_CANNOT_UNEQUIP | ITEM_INDISPENSABLE | 0x20,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(FELIX),
        .icon = 120,
        .element = ELEMENT_NONE,
    },
    // Mythril Bag
    {
        .type = 11,
        .flags = ITEM_CANNOT_UNEQUIP | ITEM_INDISPENSABLE | 0x20,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(FELIX),
        .icon = 148,
        .element = ELEMENT_NONE,
    },
    // Mythril Bag
    {
        .type = 11,
        .flags = ITEM_CANNOT_UNEQUIP | ITEM_INDISPENSABLE | 0x20,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(FELIX),
        .icon = 148,
        .element = ELEMENT_NONE,
    },
    // Mythril Bag
    {
        .type = 11,
        .flags = ITEM_CANNOT_UNEQUIP | ITEM_INDISPENSABLE | 0x20,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(FELIX),
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
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 267,
    },
    // Sleep Bomb
    {
        .price = 60,
        .flags = ITEM_STACKABLE,
        .icon = 147,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 268,
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
        .flags = ITEM_STACKABLE,
        .icon = 173,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 270,
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
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 150,
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
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 47,
    },
    // Weasel's Claw
    {
        .price = 40,
        .flags = ITEM_STACKABLE,
        .icon = 182,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 77,
    },
    // Bramble Seed
    {
        .price = 50,
        .flags = ITEM_STACKABLE,
        .icon = 183,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 17,
    },
    // Crystal Powder
    {
        .price = 60,
        .flags = ITEM_STACKABLE,
        .icon = 184,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 31,
    },
    // Black Crystal
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
    // Mythril Bag
    {
        .type = 11,
        .flags = ITEM_CANNOT_UNEQUIP | ITEM_INDISPENSABLE | 0x20,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(FELIX),
        .icon = 148,
        .element = ELEMENT_NONE,
    },
    // Jupiter Star
    {
        .type = 11,
        .flags = ITEM_CANNOT_UNEQUIP | ITEM_INDISPENSABLE | 0x20,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(FELIX),
        .icon = 122,
        .element = ELEMENT_NONE,
    },
    // Mars Star
    {
        .type = 11,
        .flags = ITEM_CANNOT_UNEQUIP | ITEM_INDISPENSABLE | 0x20,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(FELIX),
        .icon = 121,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // ?
    {0},
    // Mythril Shirt
    {
        .price = 2900,
        .type = 9,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 156,
        .secondaryBonus = 10,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 5}},
    },
    // Silk Shirt
    {
        .price = 1800,
        .type = 9,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 157,
        .secondaryBonus = 6,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_LUCK, .amount = 1}},
    },
    // Running Shirt
    {
        .price = 400,
        .type = 9,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 158,
        .secondaryBonus = 1,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_AGILITY, .amount = 15}},
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
        .type = 5,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 159,
        .secondaryBonus = 4,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 12}},
    },
    // Quick Boots
    {
        .price = 2100,
        .type = 5,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 160,
        .secondaryBonus = 3,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_AGILITY, .amount = 20}},
    },
    // Fur Boots
    {
        .price = 1200,
        .type = 5,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 161,
        .secondaryBonus = 2,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 15}},
    },
    // Turtle Boots
    {
        .price = 600,
        .type = 5,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 188,
        .secondaryBonus = 3,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_SCALE_AGILITY, .amount = 5}},
    },
    // ?
    {0},
    // ?
    {0},
    // Adept Ring
    {
        .price = 3100,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 162,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 269,
    },
    // War Ring
    {
        .price = 2600,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 163,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 102,
    },
    // Sleep Ring
    {
        .price = 1400,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 164,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 120,
    },
    // Healing Ring
    {
        .price = 800,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 170,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 90,
    },
    // Unicorn Ring
    {
        .price = 1100,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 171,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 99,
    },
    // Fairy Ring
    {
        .price = 2900,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 172,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 100,
    },
    // Cleric's Ring
    {
        .price = 6400,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 186,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_25}},
    },
    // ?
    {0},
    // ?
    {0},
    // ?
    {0},
    // Huge Sword
    {
        .price = 14000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 192,
        .primaryBonus = 155,
        .unleashChance = 7,
        .unleashAction = 524,
    },
    // Mythril Blade
    {
        .price = 14600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 193,
        .primaryBonus = 160,
        .unleashChance = 7,
        .unleashAction = 542,
        .element = ELEMENT_NONE,
    },
    // Levatine
    {
        .price = 19700,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 194,
        .primaryBonus = 173,
        .unleashChance = 7,
        .unleashAction = 560,
        .element = ELEMENT_NONE,
    },
    // Darksword
    {
        .price = 24200,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 195,
        .primaryBonus = 210,
        .unleashChance = 7,
        .unleashAction = 530,
        .element = ELEMENT_NONE,
    },
    // Excalibur
    {
        .price = 29200,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 196,
        .primaryBonus = 180,
        .unleashChance = 7,
        .unleashAction = 579,
        .element = ELEMENT_NONE,
    },
    // Robber's Blade
    {
        .price = 7600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 197,
        .primaryBonus = 101,
        .unleashChance = 7,
        .unleashAction = 555,
        .element = ELEMENT_NONE,
    },
    // Soul Brand
    {
        .price = 11600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 198,
        .primaryBonus = 141,
        .unleashChance = 7,
        .unleashAction = 559,
        .element = ELEMENT_NONE,
    },
    // Storm Brand
    {
        .price = 3900,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 199,
        .primaryBonus = 60,
        .unleashChance = 7,
        .unleashAction = 535,
        .element = ELEMENT_NONE,
    },
    // Hestia Blade
    {
        .price = 12900,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 200,
        .primaryBonus = 145,
        .unleashChance = 7,
        .unleashAction = 556,
        .element = ELEMENT_NONE,
    },
    // Lightning Sword
    {
        .price = 11400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 201,
        .primaryBonus = 134,
        .unleashChance = 7,
        .unleashAction = 566,
        .element = ELEMENT_NONE,
    },
    // Rune Blade
    {
        .price = 15000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 202,
        .primaryBonus = 162,
        .unleashChance = 7,
        .unleashAction = 576,
        .element = ELEMENT_NONE,
    },
    // Cloud Brand
    {
        .price = 11800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 203,
        .primaryBonus = 130,
        .unleashChance = 8,
        .unleashAction = 575,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // Sylph Rapier
    {
        .price = 10100,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 204,
        .primaryBonus = 124,
        .unleashChance = 7,
        .unleashAction = 568,
        .element = ELEMENT_JUPITER,
    },
    // Burning Sword
    {
        .price = 14600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 205,
        .primaryBonus = 157,
        .unleashChance = 7,
        .unleashAction = 553,
        .element = ELEMENT_MARS,
    },
    // Pirate's Sword
    {
        .price = 2000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 206,
        .primaryBonus = 34,
        .unleashChance = 7,
        .unleashAction = 536,
        .element = ELEMENT_NONE,
    },
    // Corsair's Edge
    {
        .price = 6300,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 207,
        .primaryBonus = 90,
        .unleashChance = 7,
        .unleashAction = 569,
        .element = ELEMENT_NONE,
    },
    // Pirate's Sabre
    {
        .price = 10600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 208,
        .primaryBonus = 136,
        .unleashChance = 7,
        .unleashAction = 557,
        .element = ELEMENT_NONE,
    },
    // Hypnos' Sword
    {
        .price = 2900,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 209,
        .primaryBonus = 49,
        .unleashChance = 7,
        .unleashAction = 565,
        .element = ELEMENT_NONE,
    },
    // Mist Sabre
    {
        .price = 8600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 210,
        .primaryBonus = 110,
        .unleashChance = 7,
        .unleashAction = 538,
        .element = ELEMENT_NONE,
    },
    // Phaeton's Blade
    {
        .price = 13600,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 211,
        .primaryBonus = 151,
        .unleashChance = 7,
        .unleashAction = 574,
        .element = ELEMENT_NONE,
    },
    // Tisiphone Edge
    {
        .price = 23000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 212,
        .primaryBonus = 178,
        .unleashChance = 7,
        .unleashAction = 529,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // Apollo's Axe
    {
        .price = 15200,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 213,
        .primaryBonus = 158,
        .unleashChance = 7,
        .unleashAction = 554,
        .element = ELEMENT_MARS,
    },
    // Gaia's Axe
    {
        .price = 16400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 214,
        .primaryBonus = 163,
        .unleashChance = 7,
        .unleashAction = 526,
    },
    // Stellar Axe
    {
        .price = 19700,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 215,
        .primaryBonus = 171,
        .unleashChance = 7,
        .unleashAction = 577,
        .element = ELEMENT_NONE,
    },
    // Captain's Axe
    {
        .price = 6800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 216,
        .primaryBonus = 95,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 106,
    },
    // Viking Axe
    {
        .price = 11000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 217,
        .primaryBonus = 137,
        .unleashChance = 7,
        .unleashAction = 572,
        .element = ELEMENT_NONE,
    },
    // Disk Axe
    {
        .price = 4700,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 218,
        .primaryBonus = 76,
        .unleashChance = 7,
        .unleashAction = 549,
        .element = ELEMENT_NONE,
    },
    // Themis' Axe
    {
        .price = 1900,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 219,
        .primaryBonus = 30,
        .unleashChance = 7,
        .unleashAction = 520,
        .element = ELEMENT_NONE,
    },
    // Mighty Axe
    {
        .price = 11900,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 220,
        .primaryBonus = 142,
        .unleashChance = 7,
        .unleashAction = 551,
        .element = ELEMENT_NONE,
    },
    // Tartarus Axe
    {
        .price = 10800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 221,
        .primaryBonus = 127,
        .unleashChance = 7,
        .unleashAction = 523,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // Comet Mace
    {
        .price = 8000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 222,
        .primaryBonus = 105,
        .unleashChance = 7,
        .unleashAction = 539,
        .element = ELEMENT_NONE,
    },
    // Tungsten Mace
    {
        .price = 14700,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 223,
        .primaryBonus = 159,
        .unleashChance = 7,
        .unleashAction = 525,
        .element = ELEMENT_NONE,
    },
    // Demon Mace
    {
        .price = 9200,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 224,
        .primaryBonus = 115,
        .unleashChance = 7,
        .unleashAction = 522,
        .element = ELEMENT_NONE,
    },
    // Hagbone Mace
    {
        .price = 8400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 225,
        .primaryBonus = 108,
        .unleashChance = 7,
        .unleashAction = 527,
        .element = ELEMENT_NONE,
    },
    // Blow Mace
    {
        .price = 2800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 226,
        .primaryBonus = 45,
        .unleashChance = 7,
        .unleashAction = 550,
        .element = ELEMENT_NONE,
    },
    // Rising Mace
    {
        .price = 13900,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 227,
        .primaryBonus = 152,
        .unleashChance = 7,
        .unleashAction = 571,
        .element = ELEMENT_NONE,
    },
    // Thanatos Mace
    {
        .price = 10400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 228,
        .primaryBonus = 125,
        .unleashChance = 7,
        .unleashAction = 528,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // Cloud Wand
    {
        .price = 7000,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 229,
        .primaryBonus = 98,
        .unleashChance = 7,
        .unleashAction = 537,
        .element = ELEMENT_NONE,
    },
    // Salamander Rod
    {
        .price = 14300,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 230,
        .primaryBonus = 156,
        .unleashChance = 7,
        .unleashAction = 552,
        .element = ELEMENT_NONE,
    },
    // Nebula Wand
    {
        .price = 15200,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 231,
        .primaryBonus = 165,
        .unleashChance = 7,
        .unleashAction = 543,
        .element = ELEMENT_NONE,
    },
    // Dracomace
    {
        .price = 10700,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 232,
        .primaryBonus = 128,
        .unleashChance = 7,
        .unleashAction = 567,
        .element = ELEMENT_NONE,
    },
    // Glower Staff
    {
        .price = 10100,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 233,
        .primaryBonus = 126,
        .unleashChance = 7,
        .unleashAction = 540,
        .element = ELEMENT_NONE,
    },
    // Goblin's Rod
    {
        .price = 9800,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 234,
        .primaryBonus = 134,
        .unleashChance = 7,
        .unleashAction = 541,
        .element = ELEMENT_NONE,
    },
    // Meditation Rod
    {
        .price = 13400,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 235,
        .primaryBonus = 150,
        .unleashChance = 7,
        .unleashAction = 570,
        .element = ELEMENT_NONE,
    },
    // Fireman's Pole
    {
        .price = 8200,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 236,
        .primaryBonus = 130,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 111,
    },
    // Atropos' Rod
    {
        .price = 18900,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 237,
        .primaryBonus = 169,
        .unleashChance = 8,
        .unleashAction = 561,
        .element = ELEMENT_NONE,
    },
    // Lachesis' Rule
    {
        .price = 21700,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 238,
        .primaryBonus = 177,
        .unleashChance = 8,
        .unleashAction = 578,
        .element = ELEMENT_NONE,
    },
    // Clotho's Distaff
    {
        .price = 17200,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 239,
        .primaryBonus = 168,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 95,
    },
    // Staff of Anubis
    {
        .price = 5500,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 240,
        .primaryBonus = 83,
        .unleashChance = 7,
        .unleashAction = 521,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // Trident
    {
        .type = 7,
        .flags = ITEM_INDISPENSABLE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 241,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 20}},
        .actionId = 573,
    },
    // ?
    {0},
    // Planet Armor
    {
        .price = 4800,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 242,
        .secondaryBonus = 36,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_VENUS_POWER, .amount = 10},
            {.kind = ITEM_EFFECT_ADD_MERCURY_POWER, .amount = 10},
            {.kind = ITEM_EFFECT_ADD_MARS_POWER, .amount = 10},
            {.kind = ITEM_EFFECT_ADD_JUPITER_POWER, .amount = 10},
        },
    },
    // Dragon Mail
    {
        .price = 9700,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 243,
        .secondaryBonus = 44,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 15},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 15},
        },
    },
    // Chronos Mail
    {
        .price = 13100,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 244,
        .secondaryBonus = 47,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 20}},
    },
    // Stealth Armor
    {
        .price = 14000,
        .type = 2,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 245,
        .secondaryBonus = 48,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_VENUS_RESIST, .amount = -10}},
    },
    // Xylion Armor
    {
        .price = 22500,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 246,
        .primaryBonus = 12,
        .secondaryBonus = 50,
        .element = ELEMENT_NONE,
    },
    // Ixion Mail
    {
        .price = 1300,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 247,
        .secondaryBonus = 26,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 20},
            {.kind = ITEM_EFFECT_ADD_JUPITER_RESIST, .amount = 20},
        },
    },
    // Phantasmal Mail
    {
        .price = 5800,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 248,
        .secondaryBonus = 38,
        .useType = 2,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_LUCK, .amount = 7}},
        .actionId = 116,
    },
    // Erebus Armor
    {
        .price = 9000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 249,
        .secondaryBonus = 43,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP_REGEN, .amount = 6}},
    },
    // Valkyrie Mail
    {
        .price = 25500,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 250,
        .secondaryBonus = 53,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 20}},
    },
    // ?
    {0},
    // Faery Vest
    {
        .price = 6900,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 251,
        .secondaryBonus = 38,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 94,
    },
    // Mythril Clothes
    {
        .price = 14900,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 252,
        .secondaryBonus = 49,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 15}},
    },
    // Full Metal Vest
    {
        .price = 1100,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 253,
        .secondaryBonus = 21,
        .element = ELEMENT_NONE,
    },
    // Wild Coat
    {
        .price = 4000,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 254,
        .secondaryBonus = 37,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_AGILITY, .amount = 40}},
    },
    // Floral Dress
    {
        .price = 6600,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 255,
        .secondaryBonus = 38,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 120,
    },
    // Festival Coat
    {
        .price = 2800,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 256,
        .secondaryBonus = 28,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_LUCK, .amount = 10}},
    },
    // Erinyes Tunic
    {
        .price = 10400,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 257,
        .primaryBonus = 5,
        .secondaryBonus = 45,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 10}},
    },
    // Triton's Ward
    {
        .price = 16200,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 258,
        .secondaryBonus = 47,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MERCURY_POWER, .amount = 30},
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 70},
        },
    },
    // ?
    {0},
    // Dragon Robe
    {
        .price = 8900,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 259,
        .secondaryBonus = 42,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 18},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 18},
        },
    },
    // Ardagh Robe
    {
        .price = 9900,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 260,
        .secondaryBonus = 44,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MARS_POWER, .amount = 20},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 40},
        },
    },
    // Muni Robe
    {
        .price = 6300,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 261,
        .secondaryBonus = 39,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP_REGEN, .amount = 10}},
    },
    // Aeolian Cassock
    {
        .price = 11400,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 262,
        .secondaryBonus = 46,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_JUPITER_POWER, .amount = 15},
            {.kind = ITEM_EFFECT_ADD_JUPITER_RESIST, .amount = 50},
        },
    },
    // Iris Robe
    {
        .price = 16600,
        .type = 2,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 263,
        .secondaryBonus = 47,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_LUCK, .amount = 15},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 30},
            {.kind = ITEM_EFFECT_ADD_PP_REGEN, .amount = 12},
        },
    },
    // ?
    {0},
    // Luna Shield
    {
        .price = 3900,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 264,
        .secondaryBonus = 33,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_VENUS_RESIST, .amount = 30}},
    },
    // Dragon Shield
    {
        .price = 7400,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 265,
        .secondaryBonus = 42,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 15},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 15},
        },
    },
    // Flame Shield
    {
        .price = 8600,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 266,
        .secondaryBonus = 44,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 60}},
    },
    // Terra Shield
    {
        .price = 11200,
        .type = 3,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 267,
        .primaryBonus = 5,
        .secondaryBonus = 48,
        .element = ELEMENT_NONE,
    },
    // Cosmos Shield
    {
        .price = 12200,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 268,
        .secondaryBonus = 49,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_VENUS_RESIST, .amount = 20},
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 20},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 20},
            {.kind = ITEM_EFFECT_ADD_JUPITER_RESIST, .amount = 20},
        },
    },
    // Fujin Shield
    {
        .price = 1400,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 269,
        .secondaryBonus = 23,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_JUPITER_RESIST, .amount = 50}},
    },
    // Aegis Shield
    {
        .price = 6700,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 270,
        .secondaryBonus = 41,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 10}},
    },
    // ?
    {0},
    // Aerial Gloves
    {
        .price = 5400,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 271,
        .secondaryBonus = 37,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_AGILITY, .amount = 30},
            {.kind = ITEM_EFFECT_ADD_JUPITER_POWER, .amount = 20},
        },
    },
    // Titan Gloves
    {
        .price = 8100,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 272,
        .secondaryBonus = 43,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 30}},
    },
    // Big Bang Gloves
    {
        .price = 10200,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 273,
        .secondaryBonus = 47,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_MARS_POWER, .amount = 40}},
    },
    // Crafted Gloves
    {
        .price = 4600,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 274,
        .primaryBonus = 5,
        .secondaryBonus = 35,
        .element = ELEMENT_NONE,
    },
    // Riot Gloves
    {
        .price = 10400,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 275,
        .primaryBonus = 15,
        .secondaryBonus = 45,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 20}},
    },
    // Spirit Gloves
    {
        .price = 7200,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 276,
        .secondaryBonus = 40,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP, .amount = 20}},
    },
    // ?
    {0},
    // Clear Bracelet
    {
        .price = 3500,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 277,
        .secondaryBonus = 31,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_MERCURY_POWER, .amount = 25}},
    },
    // Mythril Armlet
    {
        .price = 9700,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 278,
        .secondaryBonus = 46,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 10}},
    },
    // Bone Armlet
    {
        .price = 3000,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 279,
        .secondaryBonus = 30,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 122,
    },
    // Jester's Armlet
    {
        .price = 5100,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 280,
        .secondaryBonus = 39,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP, .amount = 30}},
    },
    // Leda's Bracelet
    {
        .price = 5800,
        .type = 3,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 281,
        .secondaryBonus = 38,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_JUPITER_POWER, .amount = 30}},
    },
    // ?
    {0},
    // Dragon Helm
    {
        .price = 8900,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 282,
        .secondaryBonus = 42,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 20},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 20},
        },
    },
    // Mythril Helm
    {
        .price = 11400,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 283,
        .secondaryBonus = 44,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 12}},
    },
    // Fear Helm
    {
        .price = 12800,
        .type = 4,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 284,
        .primaryBonus = 10,
        .secondaryBonus = 48,
        .element = ELEMENT_NONE,
    },
    // Millenium Helm
    {
        .price = 11400,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 285,
        .secondaryBonus = 45,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 20}},
    },
    // Viking Helm
    {
        .price = 4000,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 286,
        .secondaryBonus = 30,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 30}},
    },
    // Gloria Helm
    {
        .price = 13800,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 287,
        .secondaryBonus = 49,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP_REGEN, .amount = 10}},
    },
    // Minerva Helm
    {
        .price = 8700,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 288,
        .secondaryBonus = 43,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP, .amount = 20}},
    },
    // ?
    {0},
    // Floating Hat
    {
        .price = 5700,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 289,
        .secondaryBonus = 34,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_SCALE_LUCK, .amount = 12},
            {.kind = ITEM_EFFECT_ADD_JUPITER_RESIST, .amount = 20},
        },
    },
    // Nurse's Cap
    {
        .price = 1200,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 290,
        .secondaryBonus = 18,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 90,
    },
    // Thorn Crown
    {
        .price = 3300,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 291,
        .primaryBonus = 8,
        .secondaryBonus = 28,
        .element = ELEMENT_NONE,
    },
    // Otafuku Mask
    {
        .price = 3900,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 292,
        .secondaryBonus = 31,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 424,
    },
    // Hiotoko Mask
    {
        .price = 6600,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 293,
        .secondaryBonus = 33,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 586,
    },
    // Crown of Glory
    {
        .price = 7400,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 294,
        .secondaryBonus = 40,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP_REGEN, .amount = 8}},
    },
    // Alastor's Hood
    {
        .price = 11800,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 295,
        .secondaryBonus = 47,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 124,
    },
    // ?
    {0},
    // Pure Circlet
    {
        .price = 3700,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 296,
        .secondaryBonus = 29,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_MERCURY_POWER, .amount = 20}},
    },
    // Astral Circlet
    {
        .price = 4600,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 297,
        .secondaryBonus = 32,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP, .amount = 15}},
    },
    // Psychic Circlet
    {
        .price = 8800,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 298,
        .secondaryBonus = 39,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_PP_REGEN, .amount = 12}},
    },
    // Demon Circlet
    {
        .price = 15800,
        .type = 4,
        .flags = ITEM_CURSED | ITEM_CANNOT_UNEQUIP | ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 299,
        .secondaryBonus = 50,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 15}},
    },
    // Clarity Circlet
    {
        .price = 1500,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 300,
        .secondaryBonus = 21,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_JUPITER_POWER, .amount = 15}},
    },
    // Brilliant Circlet
    {
        .price = 6100,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 301,
        .secondaryBonus = 36,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_LUCK, .amount = 10}},
    },
    // Berserker Band
    {
        .price = 13700,
        .type = 4,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 302,
        .primaryBonus = 15,
        .secondaryBonus = 46,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // Divine Camisole
    {
        .price = 2700,
        .type = 9,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 303,
        .secondaryBonus = 10,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_LUCK, .amount = 5}},
    },
    // Herbed Shirt
    {
        .price = 1900,
        .type = 9,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 304,
        .secondaryBonus = 7,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 250,
    },
    // Golden Shirt
    {
        .price = 2400,
        .type = 9,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 305,
        .secondaryBonus = 12,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    // Casual Shirt
    {
        .price = 50,
        .type = 9,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 306,
        .secondaryBonus = 3,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP_REGEN, .amount = 5}},
    },
    // Leather Boots
    {
        .price = 270,
        .type = 5,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 357,
        .secondaryBonus = 6,
        .element = ELEMENT_NONE,
    },
    // Dragon Boots
    {
        .price = 4200,
        .type = 5,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 307,
        .secondaryBonus = 13,
        .element = ELEMENT_NONE,
        .effects = {
            {.kind = ITEM_EFFECT_ADD_VENUS_RESIST, .amount = 10},
            {.kind = ITEM_EFFECT_ADD_MERCURY_RESIST, .amount = 10},
            {.kind = ITEM_EFFECT_ADD_MARS_RESIST, .amount = 10},
        },
    },
    // Safety Boots
    {
        .price = 700,
        .type = 5,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 308,
        .secondaryBonus = 12,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_SCALE_AGILITY, .amount = 7}},
    },
    // Knight's Greave
    {
        .price = 2700,
        .type = 5,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 309,
        .secondaryBonus = 8,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 5}},
    },
    // Silver Greave
    {
        .price = 3800,
        .type = 5,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 310,
        .secondaryBonus = 11,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_LUCK, .amount = 4}},
    },
    // Ninja Sandals
    {
        .price = 2000,
        .type = 5,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 311,
        .secondaryBonus = 5,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_UNLEASH_CHANCE, .amount = 15}},
    },
    // Golden Boots
    {
        .price = 4400,
        .type = 5,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 362,
        .secondaryBonus = 15,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_AGILITY, .amount = 30}},
    },
    // Spirit Ring
    {
        .price = 3600,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 312,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 97,
    },
    // Stardust Ring
    {
        .price = 2500,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 313,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 121,
    },
    // Aroma Ring
    {
        .price = 2300,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 314,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 88,
    },
    // Rainbow Ring
    {
        .price = 900,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 315,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 116,
    },
    // Soul Ring
    {
        .price = 1800,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 316,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 101,
    },
    // Guardian Ring
    {
        .price = 1700,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 317,
        .secondaryBonus = 4,
        .element = ELEMENT_NONE,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 20}},
    },
    // Golden Ring
    {
        .price = 4000,
        .type = 8,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 363,
        .useType = 2,
        .element = ELEMENT_NONE,
        .actionId = 111,
    },
    // ?
    {0},
    // Rusty Sword
    {
        .price = 320,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 318,
        .primaryBonus = 9,
        .element = ELEMENT_NONE,
    },
    // Rusty Sword
    {
        .price = 370,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 318,
        .primaryBonus = 10,
        .element = ELEMENT_NONE,
    },
    // Rusty Sword
    {
        .price = 520,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(PIERS),
        .icon = 318,
        .primaryBonus = 13,
        .element = ELEMENT_NONE,
    },
    // Rusty Sword
    {
        .price = 580,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 318,
        .primaryBonus = 14,
        .element = ELEMENT_NONE,
    },
    // Rusty Axe
    {
        .price = 340,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 319,
        .primaryBonus = 10,
        .element = ELEMENT_NONE,
    },
    // Rusty Axe
    {
        .price = 550,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(PIERS),
        .icon = 319,
        .primaryBonus = 14,
        .element = ELEMENT_NONE,
    },
    // Rusty Mace
    {
        .price = 460,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 320,
        .primaryBonus = 12,
        .element = ELEMENT_NONE,
    },
    // Rusty Mace
    {
        .price = 420,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(ISAAC) | EQUIP_CHARACTER(GARET) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(FELIX) | EQUIP_CHARACTER(SHEBA) | EQUIP_CHARACTER(PIERS),
        .icon = 320,
        .primaryBonus = 11,
        .element = ELEMENT_NONE,
    },
    // Rusty Staff
    {
        .price = 530,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 321,
        .primaryBonus = 13,
        .element = ELEMENT_NONE,
    },
    // Rusty Staff
    {
        .price = 500,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 321,
        .primaryBonus = 12,
        .element = ELEMENT_NONE,
    },
    // Rusty Staff
    {
        .price = 480,
        .type = ITEM_TYPE_WEAPON,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_CHARACTER(IVAN) | EQUIP_CHARACTER(MIA) | EQUIP_CHARACTER(JENNA) | EQUIP_CHARACTER(SHEBA),
        .icon = 321,
        .primaryBonus = 11,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // Tear Stone
    {
        .price = 300,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 322,
        .element = ELEMENT_NONE,
    },
    // Star Dust
    {
        .price = 400,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 323,
        .element = ELEMENT_NONE,
    },
    // Sylph Feather
    {
        .price = 700,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 324,
        .element = ELEMENT_NONE,
    },
    // Dragon Skin
    {
        .price = 1200,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 325,
        .element = ELEMENT_NONE,
    },
    // Salamander Tail
    {
        .price = 1400,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 326,
        .element = ELEMENT_NONE,
    },
    // Golem Core
    {
        .price = 1500,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 327,
        .element = ELEMENT_NONE,
    },
    // Mythril Silver
    {
        .price = 2000,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 328,
        .element = ELEMENT_NONE,
    },
    // Dark Matter
    {
        .price = 3000,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 329,
        .element = ELEMENT_NONE,
    },
    // Orihalcon
    {
        .price = 4000,
        .flags = ITEM_RARE | ITEM_STACKABLE,
        .icon = 330,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // Right Prong
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 331,
        .element = ELEMENT_NONE,
    },
    // Left Prong
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 332,
        .element = ELEMENT_NONE,
    },
    // Center Prong
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 333,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // Mysterious Card
    {
        .price = 333,
        .type = 10,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 334,
        .element = ELEMENT_NONE,
    },
    // Trainer's Whip
    {
        .price = 666,
        .type = 10,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 335,
        .element = ELEMENT_NONE,
    },
    // Tomegathericon
    {
        .price = 999,
        .type = 10,
        .flags = ITEM_RARE,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .icon = 338,
        .element = ELEMENT_NONE,
    },
    // ?
    {0},
    // ?
    {
        .price = 10,
        .type = 8,
        .equipMask = EQUIP_ALL_CHARACTERS,
        .element = ELEMENT_NONE,
    },
    // Healing Fungus
    {
        .price = 300,
        .flags = ITEM_RARE | 0x20,
        .icon = 336,
        .element = ELEMENT_NONE,
    },
    // Laughing Fungus
    {
        .price = 700,
        .flags = 0x20,
        .icon = 337,
        .element = ELEMENT_NONE,
    },
    // Signal Whistle
    {
        .flags = 0x20,
        .useType = 1,
        .element = ELEMENT_NONE,
        .actionId = 667,
    },
    // Dancing Idol
    {
        .price = 400,
        .flags = ITEM_RARE | 0x20,
        .icon = 352,
        .element = ELEMENT_NONE,
    },
    // Pretty Stone
    {
        .price = 300,
        .flags = ITEM_RARE | 0x20,
        .icon = 353,
        .element = ELEMENT_NONE,
    },
    // Red Cloth
    {
        .price = 120,
        .flags = ITEM_RARE | 0x20,
        .icon = 354,
        .element = ELEMENT_NONE,
    },
    // Milk
    {
        .price = 90,
        .flags = ITEM_RARE | 0x20,
        .icon = 355,
        .element = ELEMENT_NONE,
    },
    // Li'l Turtle
    {
        .price = 500,
        .flags = ITEM_RARE | 0x20,
        .icon = 356,
        .element = ELEMENT_NONE,
    },
    // Aquarius Stone
    {
        .flags = ITEM_INDISPENSABLE,
        .icon = 358,
        .element = ELEMENT_NONE,
    },
    // Large Bread
    {
        .price = 8,
        .icon = 359,
        .element = ELEMENT_NONE,
    },
    // Sea God's Tear
    {
        .price = 300,
        .flags = ITEM_RARE,
        .icon = 361,
        .element = ELEMENT_NONE,
    },
    // Ruin Key
    {
        .price = 230,
        .flags = ITEM_RARE,
        .icon = 364,
        .element = ELEMENT_NONE,
    },
    // Magma Ball
    {
        .price = 800,
        .flags = ITEM_RARE,
        .icon = 365,
        .element = ELEMENT_NONE,
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
    {
        .price = 100,
        .type = ITEM_TYPE_WEAPON,
        .equipMask = EQUIP_CHARACTER(ISAAC),
        .primaryBonus = 11,
        .secondaryBonus = 12,
        .unleashChance = 64,
        .unleashAction = 140,
        .effects = {{.kind = ITEM_EFFECT_ADD_HP, .amount = 10}},
    },
};
// clang-format on
