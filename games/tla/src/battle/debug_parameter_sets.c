/// @file
/// The debug parameter sets: runs of party members, each run ended by
/// DEBUG_PARAMETER_SET_END, that DebugParameterSet_Load gives the party when
/// the man hidden below the Battle lobby is given the run's number. From set
/// 70 on, an even set brings the odd one after it too.

#include "ability_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/debug_parameter_set.h"
#include "game/character_ids.h"

// clang-format off
struct DebugPartyEntry gDebug_ParameterSets[] ALIGNED(4) = {
    // Set 0
    {
        .owner = FELIX,
        .level = 90,
        .djinnCounts = {9},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 1
    {
        .owner = FELIX,
        .level = 90,
        .djinnCounts = {0, 3, 3, 3},
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_MYSTERIOUS_CARD},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 90,
        .djinnCounts = {3, 0, 3, 3},
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_TRAINERS_WHIP},
    },
    {
        .owner = JENNA,
        .level = 90,
        .djinnCounts = {3, 3, 0, 3},
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_TOMEGATHERICON},
    },
    {
        .owner = SHEBA,
        .level = 90,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_ATROPOS_ROD, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 2
    {
        .owner = JENNA,
        .level = 50,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 50,
        .djinnCounts = {9},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 50,
        .djinnCounts = {0, 9},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 50,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 3
    {
        .owner = FELIX,
        .level = 90,
        .djinnCounts = {9},
        .items = {ITEM_GAIA_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 90,
        .djinnCounts = {0, 9},
        .items = {ITEM_EXCALIBUR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
    },
    {
        .owner = JENNA,
        .level = 90,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_TISIPHONE_EDGE, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
    },
    {
        .owner = SHEBA,
        .level = 90,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_LACHESIS_RULE, ITEM_COCKTAIL_DRESS, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 4
    {
        .owner = FELIX,
        .level = 90,
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_TRAINERS_WHIP},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 90,
        .djinnCounts = {1, 0, 1, 1},
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_TRAINERS_WHIP},
    },
    {
        .owner = JENNA,
        .level = 90,
        .djinnCounts = {2, 2, 0, 2},
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_TRAINERS_WHIP},
    },
    {
        .owner = SHEBA,
        .level = 90,
        .djinnCounts = {3, 3, 3},
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_TRAINERS_WHIP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 5
    {
        .owner = JENNA,
        .level = 5,
        .items = {ITEM_SHORT_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = FELIX,
        .level = 5,
        .djinnCounts = {1},
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 5,
        .items = {ITEM_MACE, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 6
    {
        .owner = JENNA,
        .level = 6,
        .items = {ITEM_SHORT_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = FELIX,
        .level = 6,
        .djinnCounts = {1},
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 6,
        .djinnCounts = {0, 1},
        .items = {ITEM_MACE, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 7
    {
        .owner = JENNA,
        .level = 7,
        .djinnCounts = {0, 1},
        .items = {ITEM_SHORT_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = FELIX,
        .level = 7,
        .djinnCounts = {1},
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 7,
        .djinnCounts = {0, 0, 0, 1},
        .items = {ITEM_MACE, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 8
    {
        .owner = JENNA,
        .level = 8,
        .djinnCounts = {0, 1},
        .items = {ITEM_SHORT_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = FELIX,
        .level = 8,
        .djinnCounts = {1},
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 8,
        .djinnCounts = {0, 0, 0, 1},
        .items = {ITEM_MACE, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 9
    {
        .owner = JENNA,
        .level = 9,
        .djinnCounts = {0, 1, 1},
        .items = {ITEM_SHORT_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = FELIX,
        .level = 9,
        .djinnCounts = {1},
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 9,
        .djinnCounts = {0, 0, 0, 1},
        .items = {ITEM_MACE, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 10
    {
        .owner = JENNA,
        .level = 10,
        .djinnCounts = {0, 1, 1},
        .items = {ITEM_SHORT_SWORD, ITEM_TRAVEL_VEST, ITEM_LEATHER_ARMLET, ITEM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 10,
        .djinnCounts = {1},
        .items = {ITEM_BATTLE_AXE, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 10,
        .djinnCounts = {0, 0, 0, 1},
        .items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_VEST, ITEM_LEATHER_ARMLET, ITEM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 11
    {
        .owner = JENNA,
        .level = 11,
        .djinnCounts = {0, 1, 1},
        .items = {ITEM_SHORT_SWORD, ITEM_TRAVEL_VEST, ITEM_LEATHER_ARMLET, ITEM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 11,
        .djinnCounts = {1},
        .items = {ITEM_BATTLE_AXE, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 11,
        .djinnCounts = {0, 0, 0, 1},
        .items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_VEST, ITEM_LEATHER_ARMLET, ITEM_CIRCLET, ITEM_LEATHER_BOOTS, ITEM_GUARDIAN_RING},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 12
    {
        .owner = JENNA,
        .level = 12,
        .djinnCounts = {0, 1, 1},
        .items = {ITEM_HUNTERS_SWORD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 12,
        .djinnCounts = {2},
        .items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 12,
        .djinnCounts = {0, 0, 0, 2},
        .items = {ITEM_HEAVY_MACE, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 13
    {
        .owner = JENNA,
        .level = 13,
        .djinnCounts = {0, 1, 1},
        .items = {ITEM_HUNTERS_SWORD, ITEM_ADEPTS_CLOTHES, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 13,
        .djinnCounts = {2},
        .items = {ITEM_BROAD_SWORD, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 13,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_WITCHS_WAND, ITEM_ADEPTS_CLOTHES, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 14
    {
        .owner = JENNA,
        .level = 14,
        .djinnCounts = {0, 1, 1},
        .items = {ITEM_HUNTERS_SWORD, ITEM_ADEPTS_CLOTHES, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 14,
        .djinnCounts = {2},
        .items = {ITEM_BROAD_SWORD, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 14,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_WITCHS_WAND, ITEM_ADEPTS_CLOTHES, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 15
    {
        .owner = JENNA,
        .level = 15,
        .djinnCounts = {0, 1, 2},
        .items = {ITEM_BLESSED_ANKH, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 15,
        .djinnCounts = {2},
        .items = {ITEM_BROAD_AXE, ITEM_PSYNERGY_ARMOR, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 15,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_WITCHS_WAND, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP, ITEM_SAFETY_BOOTS, ITEM_GUARDIAN_RING},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 16
    {
        .owner = JENNA,
        .level = 16,
        .djinnCounts = {0, 1, 2},
        .items = {ITEM_BLESSED_ANKH, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 16,
        .djinnCounts = {2},
        .items = {ITEM_BROAD_AXE, ITEM_PSYNERGY_ARMOR, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 16,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_WITCHS_WAND, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP, ITEM_SAFETY_BOOTS, ITEM_GUARDIAN_RING},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 17
    {
        .owner = JENNA,
        .level = 17,
        .djinnCounts = {0, 1, 2},
        .items = {ITEM_BLESSED_ANKH, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 17,
        .djinnCounts = {2},
        .items = {ITEM_BROAD_AXE, ITEM_PSYNERGY_ARMOR, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 17,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_WITCHS_WAND, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP, ITEM_SAFETY_BOOTS, ITEM_GUARDIAN_RING},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 18
    {
        .owner = JENNA,
        .level = 18,
        .djinnCounts = {0, 2, 1},
        .items = {ITEM_BATTLE_RAPIER, ITEM_SILK_ROBE, ITEM_ARMLET, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 18,
        .djinnCounts = {2, 0, 1},
        .items = {ITEM_BATTLE_RAPIER, ITEM_PSYNERGY_ARMOR, ITEM_IRON_SHIELD, ITEM_BRONZE_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 18,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_PSYNERGY_ROD, ITEM_SILK_ROBE, ITEM_ARMLET, ITEM_SILVER_CIRCLET, ITEM_SAFETY_BOOTS, ITEM_GUARDIAN_RING},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 19
    {
        .owner = JENNA,
        .level = 19,
        .djinnCounts = {0, 1, 2},
        .items = {ITEM_FROST_WAND, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 19,
        .djinnCounts = {2, 0, 1},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 19,
        .djinnCounts = {0, 4},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 19,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_PSYNERGY_ROD, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 20
    {
        .owner = JENNA,
        .level = 20,
        .djinnCounts = {0, 1, 2},
        .items = {ITEM_FROST_WAND, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 20,
        .djinnCounts = {2, 0, 1},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 20,
        .djinnCounts = {0, 4},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 20,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_PSYNERGY_ROD, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 21
    {
        .owner = JENNA,
        .level = 21,
        .djinnCounts = {0, 1, 2},
        .items = {ITEM_FROST_WAND, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 21,
        .djinnCounts = {2, 0, 1},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 21,
        .djinnCounts = {0, 4},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 21,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_PSYNERGY_ROD, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 22
    {
        .owner = JENNA,
        .level = 22,
        .djinnCounts = {0, 1, 2},
        .items = {ITEM_FROST_WAND, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 22,
        .djinnCounts = {2, 0, 1},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 22,
        .djinnCounts = {0, 4},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 22,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_PSYNERGY_ROD, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 23
    {
        .owner = JENNA,
        .level = 23,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_FROST_WAND, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_MAIL_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 23,
        .djinnCounts = {4},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_MAIL_CAP, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 23,
        .djinnCounts = {0, 5},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_MAIL_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 23,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_PSYNERGY_ROD, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_MAIL_CAP, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 24
    {
        .owner = JENNA,
        .level = 24,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_ANGELIC_ANKH, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 24,
        .djinnCounts = {5},
        .items = {ITEM_DRAGON_AXE, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 24,
        .djinnCounts = {0, 5},
        .items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 24,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_FROST_WAND, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 25
    {
        .owner = JENNA,
        .level = 25,
        .djinnCounts = {0, 1, 4},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_HEAVY_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 25,
        .djinnCounts = {5},
        .items = {ITEM_SHAMSHIR, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 25,
        .djinnCounts = {0, 5},
        .items = {ITEM_DRAGON_AXE, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 25,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_WAR_MACE, ITEM_SILVER_VEST, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 26
    {
        .owner = JENNA,
        .level = 26,
        .djinnCounts = {0, 1, 4},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_HEAVY_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 26,
        .djinnCounts = {5},
        .items = {ITEM_SHAMSHIR, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 26,
        .djinnCounts = {0, 5},
        .items = {ITEM_DRAGON_AXE, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 26,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_WAR_MACE, ITEM_SILVER_VEST, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 27
    {
        .owner = JENNA,
        .level = 27,
        .djinnCounts = {0, 0, 5},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 27,
        .djinnCounts = {5},
        .items = {ITEM_SHAMSHIR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 27,
        .djinnCounts = {0, 6},
        .items = {ITEM_DRAGON_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 27,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_WAR_MACE, ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 28
    {
        .owner = JENNA,
        .level = 28,
        .djinnCounts = {0, 0, 5},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 28,
        .djinnCounts = {5},
        .items = {ITEM_SHAMSHIR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 28,
        .djinnCounts = {0, 6},
        .items = {ITEM_DRAGON_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 28,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_WAR_MACE, ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 29
    {
        .owner = JENNA,
        .level = 29,
        .djinnCounts = {0, 0, 5},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 29,
        .djinnCounts = {5},
        .items = {ITEM_SHAMSHIR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS, ITEM_TRIDENT},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 29,
        .djinnCounts = {0, 6},
        .items = {ITEM_DRAGON_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 29,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_WAR_MACE, ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 30
    {
        .owner = JENNA,
        .level = 30,
        .djinnCounts = {0, 0, 5},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 30,
        .djinnCounts = {5},
        .items = {ITEM_SHAMSHIR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS, ITEM_TRIDENT},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 30,
        .djinnCounts = {0, 6},
        .items = {ITEM_DRAGON_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 30,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_WAR_MACE, ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 31
    {
        .owner = JENNA,
        .level = 31,
        .djinnCounts = {0, 0, 6},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 31,
        .djinnCounts = {6},
        .items = {ITEM_SWIFT_SWORD, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 31,
        .djinnCounts = {0, 7},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 31,
        .djinnCounts = {0, 0, 0, 6},
        .items = {ITEM_RIGHTEOUS_MACE, ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 32
    {
        .owner = JENNA,
        .level = 32,
        .djinnCounts = {0, 0, 7},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 32,
        .djinnCounts = {7},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 32,
        .djinnCounts = {0, 7},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 32,
        .djinnCounts = {0, 0, 0, 7},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 33
    {
        .owner = ISAAC,
        .level = 1,
        .items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 1,
        .items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = IVAN,
        .level = 1,
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = MIA,
        .level = 1,
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 34
    {
        .owner = ISAAC,
        .level = 1,
        .items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 1,
        .items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = IVAN,
        .level = 1,
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = MIA,
        .level = 1,
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 35
    {
        .owner = ISAAC,
        .level = 1,
        .items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 1,
        .items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = IVAN,
        .level = 1,
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = MIA,
        .level = 1,
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 36
    {
        .owner = ISAAC,
        .level = 1,
        .items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 1,
        .items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = IVAN,
        .level = 1,
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = MIA,
        .level = 1,
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 37
    {
        .owner = JENNA,
        .level = 26,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 26,
        .djinnCounts = {4},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 26,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_PSYNERGY_ROD, ITEM_CHINA_DRESS, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 38
    {
        .owner = FELIX,
        .level = 27,
        .djinnCounts = {5},
        .items = {ITEM_DRAGON_AXE, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = JENNA,
        .level = 27,
        .djinnCounts = {0, 0, 5},
        .items = {ITEM_PSYNERGY_ROD, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 27,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_ANGELIC_ANKH, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = PIERS,
        .level = 27,
        .djinnCounts = {0, 5},
        .items = {ITEM_SHAMSHIR, ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 39
    {
        .owner = JENNA,
        .level = 32,
        .djinnCounts = {0, 0, 7},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 32,
        .djinnCounts = {7},
        .items = {ITEM_BLESSED_MACE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 32,
        .djinnCounts = {0, 7},
        .items = {ITEM_BLESSED_MACE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 32,
        .djinnCounts = {0, 0, 0, 7},
        .items = {ITEM_BLESSED_MACE, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 40
    {
        .owner = JENNA,
        .level = 5,
    },
    {
        .owner = FELIX,
        .level = 5,
        .djinnCounts = {1},
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 5,
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 41
    {
        .owner = JENNA,
        .level = 29,
        .djinnCounts = {0, 0, 5},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 29,
        .djinnCounts = {5},
        .items = {ITEM_SHAMSHIR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS, ITEM_TRIDENT},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 29,
        .djinnCounts = {0, 6},
        .items = {ITEM_CLOUD_BRAND, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 29,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_WAR_MACE, ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 42
    {
        .owner = JENNA,
        .level = 20,
        .djinnCounts = {0, 1, 2},
        .items = {ITEM_FROST_WAND, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 20,
        .djinnCounts = {2, 0, 1},
        .items = {ITEM_BATTLE_RAPIER, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 20,
        .djinnCounts = {0, 4},
        .items = {ITEM_BATTLE_RAPIER, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 20,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_PSYNERGY_ROD, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 43
    {
        .owner = JENNA,
        .level = 29,
        .djinnCounts = {4, 0, 0, 5},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 29,
        .djinnCounts = {0, 0, 5, 4},
        .items = {ITEM_TISIPHONE_EDGE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS, ITEM_TRIDENT},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 29,
        .djinnCounts = {7, 1},
        .items = {ITEM_CLOUD_BRAND, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 29,
        .djinnCounts = {7, 0, 0, 1},
        .items = {ITEM_TUNGSTEN_MACE, ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 44
    {
        .owner = ISAAC,
        .level = 1,
        .items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 1,
        .items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = IVAN,
        .level = 1,
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = MIA,
        .level = 1,
        .items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 45
    {
        .owner = JENNA,
        .level = 5,
        .items = {ITEM_SHORT_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
    },
    {
        .owner = FELIX,
        .level = 5,
        .djinnCounts = {1},
        .items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 5,
        .items = {ITEM_MACE, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 46
    {
        .owner = JENNA,
        .level = 9,
        .djinnCounts = {0, 1, 1},
        .items = {ITEM_SHORT_SWORD, ITEM_TRAVEL_VEST, ITEM_LEATHER_ARMLET, ITEM_CIRCLET},
    },
    {
        .owner = FELIX,
        .level = 9,
        .djinnCounts = {1},
        .items = {ITEM_BATTLE_AXE, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 9,
        .djinnCounts = {0, 0, 0, 1},
        .items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_VEST, ITEM_LEATHER_ARMLET, ITEM_CIRCLET},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 47
    {
        .owner = JENNA,
        .level = 12,
        .djinnCounts = {0, 1, 1},
        .items = {ITEM_HUNTERS_SWORD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 12,
        .djinnCounts = {2},
        .items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 12,
        .djinnCounts = {0, 0, 0, 2},
        .items = {ITEM_HEAVY_MACE, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 48
    {
        .owner = JENNA,
        .level = 14,
        .djinnCounts = {0, 1, 1},
        .items = {ITEM_HUNTERS_SWORD, ITEM_ADEPTS_CLOTHES, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 14,
        .djinnCounts = {2},
        .items = {ITEM_BROAD_SWORD, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 14,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_WITCHS_WAND, ITEM_ADEPTS_CLOTHES, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 49
    {
        .owner = JENNA,
        .level = 15,
        .djinnCounts = {0, 1, 2},
        .items = {ITEM_BLESSED_ANKH, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 15,
        .djinnCounts = {2},
        .items = {ITEM_BROAD_AXE, ITEM_PSYNERGY_ARMOR, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 15,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_WITCHS_WAND, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 50
    {
        .owner = JENNA,
        .level = 17,
        .djinnCounts = {0, 2, 1},
        .items = {ITEM_BATTLE_RAPIER, ITEM_SILK_ROBE, ITEM_ARMLET, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 17,
        .djinnCounts = {2, 0, 1},
        .items = {ITEM_BATTLE_RAPIER, ITEM_PSYNERGY_ARMOR, ITEM_IRON_SHIELD, ITEM_BRONZE_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = SHEBA,
        .level = 17,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_PSYNERGY_ROD, ITEM_SILK_ROBE, ITEM_ARMLET, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 51
    {
        .owner = JENNA,
        .level = 19,
        .djinnCounts = {0, 1, 2},
        .items = {ITEM_FROST_WAND, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 19,
        .djinnCounts = {2, 0, 1},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 19,
        .djinnCounts = {0, 4},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_IRON_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 19,
        .djinnCounts = {0, 0, 0, 3},
        .items = {ITEM_PSYNERGY_ROD, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 52
    {
        .owner = JENNA,
        .level = 23,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_FROST_WAND, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_MAIL_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 23,
        .djinnCounts = {4},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_MAIL_CAP, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 23,
        .djinnCounts = {0, 5},
        .items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_GAUNTLETS, ITEM_MAIL_CAP, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 23,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_PSYNERGY_ROD, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_MAIL_CAP, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 53
    {
        .owner = JENNA,
        .level = 24,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_ANGELIC_ANKH, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 24,
        .djinnCounts = {5},
        .items = {ITEM_DRAGON_AXE, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 24,
        .djinnCounts = {0, 5},
        .items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 24,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_FROST_WAND, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 54
    {
        .owner = JENNA,
        .level = 25,
        .djinnCounts = {0, 1, 4},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_HEAVY_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 25,
        .djinnCounts = {5},
        .items = {ITEM_SHAMSHIR, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 25,
        .djinnCounts = {0, 5},
        .items = {ITEM_DRAGON_AXE, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 25,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_WAR_MACE, ITEM_SILVER_VEST, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 55
    {
        .owner = JENNA,
        .level = 27,
        .djinnCounts = {0, 0, 5},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 27,
        .djinnCounts = {5},
        .items = {ITEM_SHAMSHIR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 27,
        .djinnCounts = {0, 6},
        .items = {ITEM_DRAGON_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 27,
        .djinnCounts = {0, 0, 0, 5},
        .items = {ITEM_WAR_MACE, ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 56
    {
        .owner = JENNA,
        .level = 31,
        .djinnCounts = {0, 0, 6},
        .items = {ITEM_NINJA_BLADE, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 31,
        .djinnCounts = {6},
        .items = {ITEM_SWIFT_SWORD, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 31,
        .djinnCounts = {0, 7},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 31,
        .djinnCounts = {0, 0, 0, 6},
        .items = {ITEM_RIGHTEOUS_MACE, ITEM_BLESSED_ROBE, ITEM_WAR_GLOVES, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 57
    {
        .owner = JENNA,
        .level = 32,
        .djinnCounts = {0, 0, 7},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 32,
        .djinnCounts = {7},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 32,
        .djinnCounts = {0, 7},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 32,
        .djinnCounts = {0, 0, 0, 7},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 58
    {
        .owner = JENNA,
        .level = 33,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 33,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 33,
        .djinnCounts = {0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 33,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 59
    {
        .owner = JENNA,
        .level = 35,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 35,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 35,
        .djinnCounts = {0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 35,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 60
    {
        .owner = FELIX,
        .level = 40,
        .djinnCounts = {9},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 40,
        .djinnCounts = {0, 9},
        .items = {ITEM_BURNING_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
    },
    {
        .owner = JENNA,
        .level = 40,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_WITCHS_WAND, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
    },
    {
        .owner = SHEBA,
        .level = 40,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_FROST_WAND, ITEM_COCKTAIL_DRESS, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 61
    {
        .owner = FELIX,
        .level = 30,
        .djinnCounts = {4},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 30,
        .djinnCounts = {0, 4},
        .items = {ITEM_BURNING_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
    },
    {
        .owner = JENNA,
        .level = 30,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
    },
    {
        .owner = SHEBA,
        .level = 30,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_FROST_WAND, ITEM_COCKTAIL_DRESS, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 62
    {
        .owner = FELIX,
        .level = 20,
        .djinnCounts = {4},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 20,
        .djinnCounts = {0, 4},
        .items = {ITEM_BURNING_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
    },
    {
        .owner = JENNA,
        .level = 20,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
    },
    {
        .owner = SHEBA,
        .level = 20,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_FROST_WAND, ITEM_COCKTAIL_DRESS, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 63
    {
        .owner = FELIX,
        .level = 15,
        .djinnCounts = {4},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 15,
        .djinnCounts = {0, 4},
        .items = {ITEM_BURNING_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
    },
    {
        .owner = JENNA,
        .level = 15,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
    },
    {
        .owner = SHEBA,
        .level = 15,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_FROST_WAND, ITEM_COCKTAIL_DRESS, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 64
    {
        .owner = FELIX,
        .level = 10,
        .djinnCounts = {4},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 10,
        .djinnCounts = {0, 4},
        .items = {ITEM_BURNING_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
    },
    {
        .owner = JENNA,
        .level = 10,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
    },
    {
        .owner = SHEBA,
        .level = 10,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_FROST_WAND, ITEM_COCKTAIL_DRESS, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 65
    {
        .owner = FELIX,
        .level = 40,
        .djinnCounts = {9},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = JENNA,
        .level = 40,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_WITCHS_WAND, ITEM_ELVEN_SHIRT, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
    },
    {
        .owner = SHEBA,
        .level = 40,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_WITCHS_WAND, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 66
    {
        .owner = FELIX,
        .level = 30,
        .djinnCounts = {4},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = JENNA,
        .level = 30,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_ELVEN_SHIRT, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
    },
    {
        .owner = SHEBA,
        .level = 30,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 67
    {
        .owner = FELIX,
        .level = 20,
        .djinnCounts = {4},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = JENNA,
        .level = 20,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_ELVEN_SHIRT, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
    },
    {
        .owner = SHEBA,
        .level = 20,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 68
    {
        .owner = FELIX,
        .level = 15,
        .djinnCounts = {4},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = JENNA,
        .level = 15,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_ELVEN_SHIRT, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
    },
    {
        .owner = SHEBA,
        .level = 15,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 69
    {
        .owner = FELIX,
        .level = 10,
        .djinnCounts = {4},
        .items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = JENNA,
        .level = 10,
        .djinnCounts = {0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_ELVEN_SHIRT, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
    },
    {
        .owner = SHEBA,
        .level = 10,
        .djinnCounts = {0, 0, 0, 4},
        .items = {ITEM_WITCHS_WAND, ITEM_WATER_JACKET, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
        .actions = {ABILITY_MIND_READ},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 70
    {
        .owner = JENNA,
        .level = 33,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 33,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 33,
        .djinnCounts = {0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 33,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 71
    {
        .owner = IVAN,
        .level = 33,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 33,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 33,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 33,
        .djinnCounts = {0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 72
    {
        .owner = JENNA,
        .level = 34,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 34,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 34,
        .djinnCounts = {0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 34,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 73
    {
        .owner = IVAN,
        .level = 34,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 34,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 34,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 34,
        .djinnCounts = {0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 74
    {
        .owner = JENNA,
        .level = 35,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 35,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 35,
        .djinnCounts = {0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 35,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 75
    {
        .owner = IVAN,
        .level = 35,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 35,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 35,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 35,
        .djinnCounts = {0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 76
    {
        .owner = JENNA,
        .level = 36,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 36,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 36,
        .djinnCounts = {0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 36,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 77
    {
        .owner = IVAN,
        .level = 36,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 36,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 36,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 36,
        .djinnCounts = {0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 78
    {
        .owner = JENNA,
        .level = 37,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 37,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 37,
        .djinnCounts = {0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 37,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 79
    {
        .owner = IVAN,
        .level = 37,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 37,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 37,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 37,
        .djinnCounts = {0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 80
    {
        .owner = JENNA,
        .level = 38,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 38,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 38,
        .djinnCounts = {0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 38,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 81
    {
        .owner = IVAN,
        .level = 38,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 38,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 38,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 38,
        .djinnCounts = {0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 82
    {
        .owner = JENNA,
        .level = 39,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 39,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 39,
        .djinnCounts = {0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 39,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 83
    {
        .owner = IVAN,
        .level = 39,
        .djinnCounts = {0, 0, 0, 8},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 39,
        .djinnCounts = {8},
        .items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 39,
        .djinnCounts = {0, 0, 8},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 39,
        .djinnCounts = {0, 8},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 84
    {
        .owner = JENNA,
        .level = 40,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_TISIPHONE_EDGE, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 40,
        .djinnCounts = {9},
        .items = {ITEM_SOL_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 40,
        .djinnCounts = {0, 9},
        .items = {ITEM_DARKSWORD, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS, ITEM_CLERICS_RING},
    },
    {
        .owner = SHEBA,
        .level = 40,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 85
    {
        .owner = IVAN,
        .level = 40,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_MASAMUNE, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 40,
        .djinnCounts = {9},
        .items = {ITEM_EXCALIBUR, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 40,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 40,
        .djinnCounts = {0, 9},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 86
    {
        .owner = JENNA,
        .level = 50,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_TISIPHONE_EDGE, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 50,
        .djinnCounts = {9},
        .items = {ITEM_SOL_BLADE, ITEM_EREBUS_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 50,
        .djinnCounts = {0, 9},
        .items = {ITEM_DARKSWORD, ITEM_XYLION_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS, ITEM_CLERICS_RING},
    },
    {
        .owner = SHEBA,
        .level = 50,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 87
    {
        .owner = IVAN,
        .level = 50,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_MASAMUNE, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 50,
        .djinnCounts = {9},
        .items = {ITEM_EXCALIBUR, ITEM_ASURAS_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 50,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_DRAGON_AXE, ITEM_DRAGON_SCALES, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 50,
        .djinnCounts = {0, 9},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 88
    {
        .owner = JENNA,
        .level = 60,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_TISIPHONE_EDGE, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 60,
        .djinnCounts = {9},
        .items = {ITEM_SOL_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 60,
        .djinnCounts = {0, 9},
        .items = {ITEM_DARKSWORD, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS, ITEM_CLERICS_RING},
    },
    {
        .owner = SHEBA,
        .level = 60,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 89
    {
        .owner = IVAN,
        .level = 60,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_MASAMUNE, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 60,
        .djinnCounts = {9},
        .items = {ITEM_EXCALIBUR, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 60,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 60,
        .djinnCounts = {0, 9},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 90
    {
        .owner = JENNA,
        .level = 70,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_TISIPHONE_EDGE, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 70,
        .djinnCounts = {9},
        .items = {ITEM_SOL_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 70,
        .djinnCounts = {0, 9},
        .items = {ITEM_DARKSWORD, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS, ITEM_CLERICS_RING},
    },
    {
        .owner = SHEBA,
        .level = 70,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 91
    {
        .owner = IVAN,
        .level = 70,
        .djinnCounts = {0, 0, 0, 9},
        .items = {ITEM_MASAMUNE, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 70,
        .djinnCounts = {9},
        .items = {ITEM_EXCALIBUR, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 70,
        .djinnCounts = {0, 0, 9},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 70,
        .djinnCounts = {0, 9},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 92
    {
        .owner = JENNA,
        .level = 35,
        .djinnCounts = {0, 0, 6},
        .items = {ITEM_NINJA_BLADE, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = FELIX,
        .level = 35,
        .djinnCounts = {6},
        .items = {ITEM_SOL_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = PIERS,
        .level = 35,
        .djinnCounts = {0, 6},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = SHEBA,
        .level = 35,
        .djinnCounts = {0, 0, 0, 6},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 93
    {
        .owner = IVAN,
        .level = 35,
        .djinnCounts = {0, 0, 0, 6},
        .items = {ITEM_SWIFT_SWORD, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MIND_READ, ABILITY_REVEAL},
    },
    {
        .owner = ISAAC,
        .level = 35,
        .djinnCounts = {6},
        .items = {ITEM_GAIA_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = GARET,
        .level = 35,
        .djinnCounts = {0, 0, 6},
        .items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM, ITEM_LEATHER_BOOTS},
        .actions = {ABILITY_MOVE},
    },
    {
        .owner = MIA,
        .level = 35,
        .djinnCounts = {0, 6},
        .items = {ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_ARMLET, ITEM_JEWELED_CROWN, ITEM_LEATHER_BOOTS},
    },
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
    // Set 94: none, a second end marker
    {
        .owner = DEBUG_PARAMETER_SET_END,
    },
};
// clang-format on
