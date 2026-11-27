// The shop's tables: the quantity bar's and the price digits' tile offsets, the Game Ticket costs,
// the gift items, every shop's stock, the forge's recipes, the sanctum's tunes and the inns'
// prices. The linker places them after the shop's pictures, apart from its code.

#include "shop_internal.h"
#include "item_ids.h"
#include "sounds.h"

// clang-format off
// Where each of the quantity bar's 30 segments starts in its tile buffer: a byte column of a 4-bit tile, so four segments to a tile and the next tile 32 bytes on, eight tiles in all.
const Uint16 ShopQuantityBarSegmentOffsets[30] = {
    0, 1, 2, 3,
    32, 33, 34, 35,
    64, 65, 66, 67,
    96, 97, 98, 99,
    128, 129, 130, 131,
    160, 161, 162, 163,
    192, 193, 194, 195,
    224, 225,
};

// Where each of a price's five digits starts in its tile buffer, ones first.
const Uint16 ShopPriceDigitTileOffsets[5] = {
    192, 144, 128, 80, 64,
};

// The shop credit each Game Ticket costs, by how many have been given: five at each price: 300, then 500 doubling to 8000.
const Sint16 ShopGameTicketCreditCosts[30] = {
    300, 300, 300, 300, 300,
    500, 500, 500, 500, 500,
    1000, 1000, 1000, 1000, 1000,
    2000, 2000, 2000, 2000, 2000,
    4000, 4000, 4000, 4000, 4000,
    8000, 8000, 8000, 8000, 8000,
};

// Unused: a list of gift items, ended by 0, that nothing reads: The Broken Seal's, with the Fire Brand where it has the Masamune.
const Sint16 ShopUnusedItemIds[21] = {
    ITEM_LONG_SWORD, ITEM_BROAD_SWORD, ITEM_CLAYMORE, ITEM_GREAT_SWORD, ITEM_SHAMSHIR,
    ITEM_SILVER_BLADE, ITEM_FIRE_BRAND, ITEM_ARCTIC_BLADE, ITEM_GAIA_BLADE,
    ITEM_SOL_BLADE, ITEM_MURAMASA, ITEM_SHORT_SWORD, ITEM_MINT, ITEM_DEMON_AXE,
    ITEM_ASURAS_ARMOR, ITEM_COTTON_SHIRT, ITEM_TRAVEL_VEST, ITEM_HERB, ITEM_NUT,
    ITEM_MIST_POTION, 0,
};

// The shops' stock, one row per shop: up to 24 items, then up to 8 artifacts (each list ending with 0 when shorter), then the shop's type, which an item shop, SHOP_TYPE_ITEMS, leaves at 0. Each row is named by the town whose script opens it.
const Sint16 ShopStockTable[SHOP_STOCK_ROWS][SHOP_STOCK_COLUMNS] = {
    // 0: sells nothing, and stands in for a row out of range
    {0},
    // 1: Daila's weapon shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_LONG_SWORD, ITEM_SHORT_SWORD, ITEM_MACE, ITEM_WOODEN_STICK,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    // 2: Daila's armor shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_COTTON_SHIRT, ITEM_TRAVEL_VEST, ITEM_ONE_PIECE_DRESS, ITEM_WOODEN_SHIELD, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    // 3: Daila's item shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE,
    },
    // 4: Madra's weapon shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_LONG_SWORD, ITEM_SHORT_SWORD, ITEM_BATTLE_AXE, ITEM_MACE,
        [SHOP_STOCK_ARTIFACTS] = ITEM_MAGIC_ROD,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    // 5: Madra's armor shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_PADDED_GLOVES, ITEM_LEATHER_ARMLET, ITEM_LEATHER_CAP, ITEM_CIRCLET, ITEM_LEATHER_BOOTS,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    // 6: Madra's item shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE,
    },
    // 7: Alhafra's weapon shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_LONG_SWORD, ITEM_BROAD_SWORD, ITEM_HUNTERS_SWORD, ITEM_BATTLE_AXE, ITEM_HEAVY_MACE,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    // 8: Alhafra's armor shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_LEATHER_ARMOR, ITEM_TRAVEL_VEST, ITEM_TRAVEL_ROBE, ITEM_WOODEN_SHIELD, ITEM_LEATHER_GLOVES, ITEM_LEATHER_ARMLET, ITEM_OPEN_HELM, ITEM_LEATHER_CAP, ITEM_CIRCLET, ITEM_LEATHER_BOOTS,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    // 9: Alhafra's item shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
    },
    // 10: Garoh's weapon shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_BROAD_SWORD, ITEM_HUNTERS_SWORD, ITEM_BATTLE_AXE, ITEM_HEAVY_MACE,
        [SHOP_STOCK_ARTIFACTS] = ITEM_WITCHS_WAND,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    // 11: Garoh's armor shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_LEATHER_ARMOR, ITEM_ADEPTS_CLOTHES, ITEM_TRAVEL_ROBE, ITEM_BRONZE_SHIELD, ITEM_LEATHER_GLOVES, ITEM_LEATHER_ARMLET, ITEM_OPEN_HELM, ITEM_WOODEN_CAP, ITEM_CIRCLET,
        [SHOP_STOCK_ARTIFACTS] = ITEM_SAFETY_BOOTS,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    // 12: Garoh's item shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
    },
    // 13: Mikasalla's general store
    {
        [SHOP_STOCK_ITEMS] = ITEM_BROAD_AXE, ITEM_PSYNERGY_ARMOR, ITEM_ARMLET, ITEM_BRONZE_HELM, ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_BLESSED_ANKH, ITEM_VIAL,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_GENERAL_STORE,
    },
    // 14: Naribwe's weapon shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_BROAD_SWORD, ITEM_BATTLE_RAPIER, ITEM_BROAD_AXE, ITEM_BATTLE_MACE,
        [SHOP_STOCK_ARTIFACTS] = ITEM_PSYNERGY_ROD,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    // 15: Naribwe's armor shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_ADEPTS_CLOTHES, ITEM_SILK_ROBE, ITEM_IRON_SHIELD, ITEM_ARMLET, ITEM_BRONZE_HELM, ITEM_WOODEN_CAP, ITEM_SILVER_CIRCLET,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    // 16: Naribwe's item shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
    },
    // 17: Kibombo's weapon shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_CLAYMORE, ITEM_BATTLE_RAPIER, ITEM_BROAD_AXE, ITEM_BATTLE_MACE,
        [SHOP_STOCK_ARTIFACTS] = ITEM_FROST_WAND,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    // 18: Kibombo's armor shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_CHAIN_MAIL, ITEM_ADEPTS_CLOTHES, ITEM_SILK_ROBE, ITEM_IRON_SHIELD, ITEM_GAUNTLETS, ITEM_ARMLET, ITEM_IRON_HELM, ITEM_SILVER_CIRCLET,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    // 19: Kibombo's item shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
    },
    // 20: Yallam's weapon shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_CLAYMORE, ITEM_BATTLE_RAPIER, ITEM_BROAD_AXE, ITEM_BATTLE_MACE,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    // 21: Yallam's armor shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_MAIL_CAP,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    // 22: Yallam's item shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
    },
    // 23: the Apojii Islands' general store
    {
        [SHOP_STOCK_ITEMS] = ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM, ITEM_GUARDIAN_CIRCLET, ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_DRAGON_AXE, ITEM_ANGELIC_ANKH, ITEM_WATER_OF_LIFE,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_GENERAL_STORE,
    },
    // 24: Izumo's general store
    {
        [SHOP_STOCK_ITEMS] = ITEM_WAR_MACE, ITEM_SILVER_VEST, ITEM_PLATINUM_CIRCLET, ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_SHAMSHIR, ITEM_NINJA_BLADE, ITEM_WAR_GLOVES, ITEM_VIAL, ITEM_PSY_CRYSTAL,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_GENERAL_STORE,
    },
    // 25: Champa's general store
    {
        [SHOP_STOCK_ITEMS] = ITEM_MASTER_RAPIER, ITEM_GREAT_AXE, ITEM_PLATE_MAIL, ITEM_SILVER_ARMLET, ITEM_SILVER_HELM, ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_DEMONIC_STAFF, ITEM_BLESSED_ROBE, ITEM_POTION,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_GENERAL_STORE,
    },
    // 26: Contigo's weapon shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_GREAT_SWORD, ITEM_MASTER_RAPIER, ITEM_GREAT_AXE, ITEM_WAR_MACE,
        [SHOP_STOCK_ARTIFACTS] = ITEM_SWIFT_SWORD, ITEM_RIGHTEOUS_MACE,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_WEAPONS,
    },
    // 27: Contigo's armor shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_STEEL_ARMOR, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_KNIGHTS_HELM, ITEM_PLATINUM_CIRCLET,
        [SHOP_STOCK_ARTIFACTS] = ITEM_MIRRORED_SHIELD, ITEM_JEWELED_CROWN,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_ARMOR,
    },
    // 28: Contigo's item shop
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_VIAL, ITEM_WATER_OF_LIFE, ITEM_MIST_POTION,
    },
    // 29: the Shaman Village's general store
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_SILVER_BLADE, ITEM_CRYSTAL_ROD, ITEM_MAGICAL_CASSOCK,
        [SHOP_STOCK_ARTIFACTS + 4] = ITEM_VIAL, ITEM_POTION, ITEM_PSY_CRYSTAL,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_GENERAL_STORE,
    },
    // 30: Loho's general store
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_VIAL, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_SPIRIT_ARMLET,
        [SHOP_STOCK_ARTIFACTS + 4] = ITEM_POTION, ITEM_MIST_POTION, ITEM_PSY_CRYSTAL,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_GENERAL_STORE,
    },
    // 31: Prox's general store
    {
        [SHOP_STOCK_ITEMS] = ITEM_HERB, ITEM_NUT, ITEM_VIAL, ITEM_MIST_POTION, ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_SACRED_FEATHER,
        [SHOP_STOCK_ARTIFACTS] = ITEM_MYSTERIOUS_ROBE,
        [SHOP_STOCK_ARTIFACTS + 4] = ITEM_POTION, ITEM_PSY_CRYSTAL, ITEM_WATER_OF_LIFE,
        [SHOP_STOCK_TYPE] = SHOP_TYPE_GENERAL_STORE,
    },
};

// What the forge makes of each material, ended by an item of -1: a rare material may become any of its results, by weight out of 100, and a rusted weapon always becomes its one result.
const FORGE_RECIPE ForgeRecipes[] = {
    {
        .item = ITEM_TEAR_STONE,
        .condition = FORGE_CONDITION_PROCESSED,
        .results = {ITEM_CLOUD_WAND, ITEM_CLEAR_BRACELET, ITEM_PURE_CIRCLET, ITEM_SPIRIT_RING},
        .weights = {25, 30, 25, 20},
    },
    {
        .item = ITEM_STAR_DUST,
        .condition = FORGE_CONDITION_PROCESSED,
        .results = {ITEM_COMET_MACE, ITEM_PLANET_ARMOR, ITEM_LUNA_SHIELD, ITEM_ASTRAL_CIRCLET, ITEM_STARDUST_RING},
        .weights = {20, 20, 20, 20, 20},
    },
    {
        .item = ITEM_SYLPH_FEATHER,
        .condition = FORGE_CONDITION_PROCESSED,
        .results = {ITEM_SYLPH_RAPIER, ITEM_FAERY_VEST, ITEM_AERIAL_GLOVES, ITEM_FLOATING_HAT},
        .weights = {25, 25, 25, 25},
    },
    {
        .item = ITEM_DRAGON_SKIN,
        .condition = FORGE_CONDITION_PROCESSED,
        .results = {ITEM_DRAGON_MAIL, ITEM_DRAGON_ROBE, ITEM_DRAGON_SHIELD_FORGED, ITEM_DRAGON_HELM, ITEM_DRAGON_BOOTS},
        .weights = {20, 15, 25, 25, 15},
    },
    {
        .item = ITEM_SALAMANDER_TAIL,
        .condition = FORGE_CONDITION_PROCESSED,
        .results = {ITEM_SALAMANDER_ROD, ITEM_APOLLOS_AXE, ITEM_BURNING_SWORD, ITEM_ARDAGH_ROBE, ITEM_FLAME_SHIELD},
        .weights = {20, 15, 15, 25, 25},
    },
    {
        .item = ITEM_GOLEM_CORE,
        .condition = FORGE_CONDITION_PROCESSED,
        .results = {ITEM_HUGE_SWORD, ITEM_GAIAS_AXE, ITEM_TUNGSTEN_MACE, ITEM_CHRONOS_MAIL, ITEM_TITAN_GLOVES},
        .weights = {15, 15, 15, 30, 25},
    },
    {
        .item = ITEM_MYTHRIL_SILVER,
        .condition = FORGE_CONDITION_PROCESSED,
        .results = {ITEM_MYTHRIL_BLADE, ITEM_MYTHRIL_CLOTHES, ITEM_MYTHRIL_ARMLET, ITEM_MYTHRIL_HELM, ITEM_LEVATINE, ITEM_PSYCHIC_CIRCLET},
        .weights = {20, 20, 20, 15, 10, 15},
    },
    {
        .item = ITEM_DARK_MATTER,
        .condition = FORGE_CONDITION_PROCESSED,
        .results = {ITEM_DARKSWORD, ITEM_STEALTH_ARMOR, ITEM_TERRA_SHIELD, ITEM_FEAR_HELM, ITEM_DEMON_CIRCLET},
        .weights = {10, 30, 25, 20, 15},
    },
    {
        .item = ITEM_ORIHALCON,
        .condition = FORGE_CONDITION_PROCESSED,
        .results = {ITEM_EXCALIBUR, ITEM_STELLAR_AXE, ITEM_NEBULA_WAND, ITEM_XYLION_ARMOR, ITEM_COSMOS_SHIELD, ITEM_BIG_BANG_GLOVES, ITEM_MILLENIUM_HELM},
        .weights = {5, 10, 15, 20, 15, 15, 20},
    },
    {
        .item = ITEM_RUSTY_SWORD,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_CORSAIRS_EDGE},
        .weights = {100},
    },
    {
        .item = ITEM_RUSTY_SWORD_ROBBERS_BLADE,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_ROBBERS_BLADE},
        .weights = {100},
    },
    {
        .item = ITEM_RUSTY_SWORD_PIRATES_SABRE,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_PIRATES_SABRE},
        .weights = {100},
    },
    {
        .item = ITEM_RUSTY_SWORD_SOUL_BRAND,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_SOUL_BRAND},
        .weights = {100},
    },
    {
        .item = ITEM_RUSTY_AXE,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_CAPTAINS_AXE},
        .weights = {100},
    },
    {
        .item = ITEM_RUSTY_AXE_VIKING_AXE,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_VIKING_AXE},
        .weights = {100},
    },
    {
        .item = ITEM_RUSTY_MACE,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_DEMON_MACE},
        .weights = {100},
    },
    {
        .item = ITEM_RUSTY_MACE_HAGBONE_MACE,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_HAGBONE_MACE},
        .weights = {100},
    },
    {
        .item = ITEM_RUSTY_STAFF,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_DRACOMACE},
        .weights = {100},
    },
    {
        .item = ITEM_RUSTY_STAFF_GLOWER_STAFF,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_GLOWER_STAFF},
        .weights = {100},
    },
    {
        .item = ITEM_RUSTY_STAFF_GOBLINS_ROD,
        .condition = FORGE_CONDITION_RUSTED,
        .results = {ITEM_GOBLINS_ROD},
        .weights = {100},
    },
    {.item = -1},
};

// The tune each service's burst plays.
const Sint8 SanctumServiceTunes[4] = {
    [SANCTUM_SERVICE_REVIVE] = MUS_RECOVERING_AFTER_COMBAT,
    [SANCTUM_SERVICE_CURE_POISON] = MUS_POISON_CURED,
    [SANCTUM_SERVICE_REPEL_EVIL] = MUS_EXORCISED,
    [SANCTUM_SERVICE_REMOVE_CURSE] = MUS_CURSE_LIFTED,
};

// The coins a night costs each living guest, by inn: Daila, Madra, Alhafra, Garoh, Mikasalla, Naribwe, Kibombo, Yallam, the Apojii Islands, Izumo, Champa, the Shaman Village, Contigo, Loho and Prox, as their scripts open them. Unused: inn 15 costs nothing and no script opens it.
const Sint8 InnPricesPerGuest[16] = {
    2, 4, 7, 8, 8, 10, 12, 16, 14, 15, 18, 20, 20, 22, 25, 0,
};
// clang-format on
