#ifndef ALCHEMY_ITEM_IDS_H
#define ALCHEMY_ITEM_IDS_H

/// @file
/// Item numbers, named from the English release.

/// Item ids, one for each row of gItem_DefinitionTable (item_table.c) in
/// order, named from the English release: an item's name is the message
/// MsgItemName + its id. A row the code doesn't name yet keeps its number as
/// its placeholder. The rows the text names "?" are blank, and nothing reaches
/// them but the debug menus' item browser (debug/level.c, items 0 to 269)
/// and gShop_GiftItemIds, which nothing reads.
enum ItemId {
  ITEM_000,
  /// ★ Named from the game: "Long Sword" (MsgItemName)
  ITEM_LONG_SWORD,
  /// ★ Named from the game: "Broad Sword" (MsgItemName)
  ITEM_BROAD_SWORD,
  /// ★ Named from the game: "Claymore" (MsgItemName)
  ITEM_CLAYMORE,
  /// ★ Named from the game: "Great Sword" (MsgItemName)
  ITEM_GREAT_SWORD,
  /// ★ Named from the game: "Shamshir" (MsgItemName)
  ITEM_SHAMSHIR,
  /// ★ Named from the game: "Silver Blade" (MsgItemName)
  ITEM_SILVER_BLADE,
  /// ★ Named from the game: "Masamune" (MsgItemName)
  ITEM_MASAMUNE,
  /// ★ Named from the game: "Arctic Blade" (MsgItemName)
  ITEM_ARCTIC_BLADE,
  /// ★ Named from the game: "Gaia Blade" (MsgItemName)
  ITEM_GAIA_BLADE,
  /// ★ Named from the game: "Sol Blade" (MsgItemName)
  ITEM_SOL_BLADE,
  /// ★ Named from the game: "Muramasa" (MsgItemName)
  ITEM_MURAMASA,
  ITEM_012,
  ITEM_013,
  ITEM_014,
  /// ★ Named from the game: "Machete" (MsgItemName)
  ITEM_MACHETE,
  /// ★ Named from the game: "Short Sword" (MsgItemName)
  ITEM_SHORT_SWORD,
  /// ★ Named from the game: "Hunter's Sword" (MsgItemName)
  ITEM_HUNTERS_SWORD,
  /// ★ Named from the game: "Battle Rapier" (MsgItemName)
  ITEM_BATTLE_RAPIER,
  /// ★ Named from the game: "Master Rapier" (MsgItemName)
  ITEM_MASTER_RAPIER,
  /// ★ Named from the game: "Ninja Blade" (MsgItemName)
  ITEM_NINJA_BLADE,
  /// ★ Named from the game: "Swift Sword" (MsgItemName)
  ITEM_SWIFT_SWORD,
  /// ★ Named from the game: "Elven Rapier" (MsgItemName)
  ITEM_ELVEN_RAPIER,
  /// ★ Named from the game: "Assassin Blade" (MsgItemName)
  ITEM_ASSASSIN_BLADE,
  /// ★ Named from the game: "Mystery Blade" (MsgItemName)
  ITEM_MYSTERY_BLADE,
  /// ★ Named from the game: "Kikuichimonji" (MsgItemName)
  ITEM_KIKUICHIMONJI,
  /// ★ Named from the game: "Kusanagi" (MsgItemName)
  ITEM_KUSANAGI,
  /// ★ Named from the game: "Bandit's Sword" (MsgItemName)
  ITEM_BANDITS_SWORD,
  ITEM_028,
  ITEM_029,
  ITEM_030,
  /// ★ Named from the game: "Battle Axe" (MsgItemName)
  ITEM_BATTLE_AXE,
  /// ★ Named from the game: "Broad Axe" (MsgItemName)
  ITEM_BROAD_AXE,
  /// ★ Named from the game: "Great Axe" (MsgItemName)
  ITEM_GREAT_AXE,
  /// ★ Named from the game: "Dragon Axe" (MsgItemName)
  ITEM_DRAGON_AXE,
  /// ★ Named from the game: "Giant Axe" (MsgItemName)
  ITEM_GIANT_AXE,
  /// ★ Named from the game: "Vulcan Axe" (MsgItemName)
  ITEM_VULCAN_AXE,
  /// ★ Named from the game: "Burning Axe" (MsgItemName)
  ITEM_BURNING_AXE,
  /// ★ Named from the game: "Demon Axe" (MsgItemName)
  ITEM_DEMON_AXE,
  ITEM_039,
  ITEM_040,
  ITEM_041,
  ITEM_042,
  /// ★ Named from the game: "Mace" (MsgItemName)
  ITEM_MACE,
  /// ★ Named from the game: "Heavy Mace" (MsgItemName)
  ITEM_HEAVY_MACE,
  /// ★ Named from the game: "Battle Mace" (MsgItemName)
  ITEM_BATTLE_MACE,
  /// ★ Named from the game: "War Mace" (MsgItemName)
  ITEM_WAR_MACE,
  /// ★ Named from the game: "Righteous Mace" (MsgItemName)
  ITEM_RIGHTEOUS_MACE,
  /// ★ Named from the game: "Grievous Mace" (MsgItemName)
  ITEM_GRIEVOUS_MACE,
  /// ★ Named from the game: "Blessed Mace" (MsgItemName)
  ITEM_BLESSED_MACE,
  /// ★ Named from the game: "Wicked Mace" (MsgItemName)
  ITEM_WICKED_MACE,
  ITEM_051,
  ITEM_052,
  ITEM_053,
  ITEM_054,
  /// ★ Named from the game: "Wooden Stick" (MsgItemName)
  ITEM_WOODEN_STICK,
  /// ★ Named from the game: "Magic Rod" (MsgItemName)
  ITEM_MAGIC_ROD,
  /// ★ Named from the game: "Witch's Wand" (MsgItemName)
  ITEM_WITCHS_WAND,
  /// ★ Named from the game: "Blessed Ankh" (MsgItemName)
  ITEM_BLESSED_ANKH,
  /// ★ Named from the game: "Psynergy Rod" (MsgItemName)
  ITEM_PSYNERGY_ROD,
  /// ★ Named from the game: "Frost Wand" (MsgItemName)
  ITEM_FROST_WAND,
  /// ★ Named from the game: "Angelic Ankh" (MsgItemName)
  ITEM_ANGELIC_ANKH,
  /// ★ Named from the game: "Demonic Staff" (MsgItemName)
  ITEM_DEMONIC_STAFF,
  /// ★ Named from the game: "Crystal Rod" (MsgItemName)
  ITEM_CRYSTAL_ROD,
  /// ★ Named from the game: "Zodiac Wand" (MsgItemName)
  ITEM_ZODIAC_WAND,
  /// ★ Named from the game: "Shaman's Rod" (MsgItemName)
  ITEM_SHAMANS_ROD,
  ITEM_066,
  ITEM_067,
  ITEM_068,
  ITEM_069,
  ITEM_070,
  ITEM_071,
  ITEM_072,
  ITEM_073,
  ITEM_074,
  /// ★ Named from the game: "Leather Armor" (MsgItemName)
  ITEM_LEATHER_ARMOR,
  /// ★ Named from the game: "Psynergy Armor" (MsgItemName)
  ITEM_PSYNERGY_ARMOR,
  /// ★ Named from the game: "Chain Mail" (MsgItemName)
  ITEM_CHAIN_MAIL,
  /// ★ Named from the game: "Armored Shell" (MsgItemName)
  ITEM_ARMORED_SHELL,
  /// ★ Named from the game: "Plate Mail" (MsgItemName)
  ITEM_PLATE_MAIL,
  /// ★ Named from the game: "Steel Armor" (MsgItemName)
  ITEM_STEEL_ARMOR,
  /// ★ Named from the game: "Spirit Armor" (MsgItemName)
  ITEM_SPIRIT_ARMOR,
  /// ★ Named from the game: "Dragon Scales" (MsgItemName)
  ITEM_DRAGON_SCALES,
  /// ★ Named from the game: "Demon Mail" (MsgItemName)
  ITEM_DEMON_MAIL,
  /// ★ Named from the game: "Asura's Armor" (MsgItemName)
  ITEM_ASURAS_ARMOR,
  /// ★ Named from the game: "Spiked Armor" (MsgItemName)
  ITEM_SPIKED_ARMOR,
  ITEM_086,
  ITEM_087,
  ITEM_088,
  /// ★ Named from the game: "Cotton Shirt" (MsgItemName)
  ITEM_COTTON_SHIRT,
  /// ★ Named from the game: "Travel Vest" (MsgItemName)
  ITEM_TRAVEL_VEST,
  /// ★ Named from the game: "Fur Coat" (MsgItemName)
  ITEM_FUR_COAT,
  /// ★ Named from the game: "Adept's Clothes" (MsgItemName)
  ITEM_ADEPTS_CLOTHES,
  /// ★ Named from the game: "Elven Shirt" (MsgItemName)
  ITEM_ELVEN_SHIRT,
  /// ★ Named from the game: "Silver Vest" (MsgItemName)
  ITEM_SILVER_VEST,
  /// ★ Named from the game: "Water Jacket" (MsgItemName)
  ITEM_WATER_JACKET,
  /// ★ Named from the game: "Storm Gear" (MsgItemName)
  ITEM_STORM_GEAR,
  /// ★ Named from the game: "Kimono" (MsgItemName)
  ITEM_KIMONO,
  /// ★ Named from the game: "Ninja Garb" (MsgItemName)
  ITEM_NINJA_GARB,
  ITEM_099,
  ITEM_100,
  ITEM_101,
  ITEM_102,
  /// ★ Named from the game: "One-Piece Dress" (MsgItemName)
  ITEM_ONE_PIECE_DRESS,
  /// ★ Named from the game: "Travel Robe" (MsgItemName)
  ITEM_TRAVEL_ROBE,
  /// ★ Named from the game: "Silk Robe" (MsgItemName)
  ITEM_SILK_ROBE,
  /// ★ Named from the game: "China Dress" (MsgItemName)
  ITEM_CHINA_DRESS,
  /// ★ Named from the game: "Jerkin" (MsgItemName)
  ITEM_JERKIN,
  /// ★ Named from the game: "Cocktail Dress" (MsgItemName)
  ITEM_COCKTAIL_DRESS,
  /// ★ Named from the game: "Blessed Robe" (MsgItemName)
  ITEM_BLESSED_ROBE,
  /// ★ Named from the game: "Magical Cassock" (MsgItemName)
  ITEM_MAGICAL_CASSOCK,
  /// ★ Named from the game: "Mysterious Robe" (MsgItemName)
  ITEM_MYSTERIOUS_ROBE,
  /// ★ Named from the game: "Feathered Robe" (MsgItemName)
  ITEM_FEATHERED_ROBE,
  /// ★ Named from the game: "Oracle's Robe" (MsgItemName)
  ITEM_ORACLES_ROBE,
  ITEM_114,
  ITEM_115,
  ITEM_116,
  ITEM_117,
  /// ★ Named from the game: "Wooden Shield" (MsgItemName)
  ITEM_WOODEN_SHIELD,
  /// ★ Named from the game: "Bronze Shield" (MsgItemName)
  ITEM_BRONZE_SHIELD,
  /// ★ Named from the game: "Iron Shield" (MsgItemName)
  ITEM_IRON_SHIELD,
  /// ★ Named from the game: "Knight's Shield" (MsgItemName)
  ITEM_KNIGHTS_SHIELD,
  /// ★ Named from the game: "Mirrored Shield" (MsgItemName)
  ITEM_MIRRORED_SHIELD,
  /// ★ Named from the game: "Dragon Shield" (MsgItemName)
  ITEM_DRAGON_SHIELD,
  /// ★ Named from the game: "Earth Shield" (MsgItemName)
  ITEM_EARTH_SHIELD,
  ITEM_125,
  ITEM_126,
  /// ★ Named from the game: "Padded Gloves" (MsgItemName)
  ITEM_PADDED_GLOVES,
  /// ★ Named from the game: "Leather Gloves" (MsgItemName)
  ITEM_LEATHER_GLOVES,
  /// ★ Named from the game: "Gauntlets" (MsgItemName)
  ITEM_GAUNTLETS,
  /// ★ Named from the game: "Vambrace" (MsgItemName)
  ITEM_VAMBRACE,
  /// ★ Named from the game: "War Gloves" (MsgItemName)
  ITEM_WAR_GLOVES,
  /// ★ Named from the game: "Spirit Gloves" (MsgItemName)
  ITEM_SPIRIT_GLOVES,
  /// ★ Named from the game: "Battle Gloves" (MsgItemName)
  ITEM_BATTLE_GLOVES,
  /// ★ Named from the game: "Aura Gloves" (MsgItemName)
  ITEM_AURA_GLOVES,
  ITEM_135,
  /// ★ Named from the game: "Leather Armlet" (MsgItemName)
  ITEM_LEATHER_ARMLET,
  /// ★ Named from the game: "Armlet" (MsgItemName)
  ITEM_ARMLET,
  /// ★ Named from the game: "Heavy Armlet" (MsgItemName)
  ITEM_HEAVY_ARMLET,
  /// ★ Named from the game: "Silver Armlet" (MsgItemName)
  ITEM_SILVER_ARMLET,
  /// ★ Named from the game: "Spirit Armlet" (MsgItemName)
  ITEM_SPIRIT_ARMLET,
  /// ★ Named from the game: "Virtuous Armlet" (MsgItemName)
  ITEM_VIRTUOUS_ARMLET,
  /// ★ Named from the game: "Guardian Armlet" (MsgItemName)
  ITEM_GUARDIAN_ARMLET,
  ITEM_143,
  ITEM_144,
  /// ★ Named from the game: "Open Helm" (MsgItemName)
  ITEM_OPEN_HELM,
  /// ★ Named from the game: "Bronze Helm" (MsgItemName)
  ITEM_BRONZE_HELM,
  /// ★ Named from the game: "Iron Helm" (MsgItemName)
  ITEM_IRON_HELM,
  /// ★ Named from the game: "Steel Helm" (MsgItemName)
  ITEM_STEEL_HELM,
  /// ★ Named from the game: "Silver Helm" (MsgItemName)
  ITEM_SILVER_HELM,
  /// ★ Named from the game: "Knight's Helm" (MsgItemName)
  ITEM_KNIGHTS_HELM,
  /// ★ Named from the game: "Warrior's Helm" (MsgItemName)
  ITEM_WARRIORS_HELM,
  /// ★ Named from the game: "Adept's Helm" (MsgItemName)
  ITEM_ADEPTS_HELM,
  ITEM_153,
  ITEM_154,
  ITEM_155,
  /// ★ Named from the game: "Leather Cap" (MsgItemName)
  ITEM_LEATHER_CAP,
  /// ★ Named from the game: "Wooden Cap" (MsgItemName)
  ITEM_WOODEN_CAP,
  /// ★ Named from the game: "Mail Cap" (MsgItemName)
  ITEM_MAIL_CAP,
  /// ★ Named from the game: "Jeweled Crown" (MsgItemName)
  ITEM_JEWELED_CROWN,
  /// ★ Named from the game: "Ninja Hood" (MsgItemName)
  ITEM_NINJA_HOOD,
  /// ★ Named from the game: "Lucky Cap" (MsgItemName)
  ITEM_LUCKY_CAP,
  /// ★ Named from the game: "Thunder Crown" (MsgItemName)
  ITEM_THUNDER_CROWN,
  /// ★ Named from the game: "Prophet's Hat" (MsgItemName)
  ITEM_PROPHETS_HAT,
  /// ★ Named from the game: "Lure Cap" (MsgItemName)
  ITEM_LURE_CAP,
  ITEM_165,
  /// ★ Named from the game: "Circlet" (MsgItemName)
  ITEM_CIRCLET,
  /// ★ Named from the game: "Silver Circlet" (MsgItemName)
  ITEM_SILVER_CIRCLET,
  /// ★ Named from the game: "Guardian Circlet" (MsgItemName)
  ITEM_GUARDIAN_CIRCLET,
  /// ★ Named from the game: "Platinum Circlet" (MsgItemName)
  ITEM_PLATINUM_CIRCLET,
  /// ★ Named from the game: "Mythril Circlet" (MsgItemName)
  ITEM_MYTHRIL_CIRCLET,
  /// ★ Named from the game: "Glittering Tiara" (MsgItemName)
  ITEM_GLITTERING_TIARA,
  ITEM_172,
  ITEM_173,
  ITEM_174,
  ITEM_175,
  ITEM_176,
  ITEM_177,
  ITEM_178,
  ITEM_179,
  /// ★ Named from the game: "Herb" (MsgItemName)
  ITEM_HERB,
  /// ★ Named from the game: "Nut" (MsgItemName)
  ITEM_NUT,
  /// ★ Named from the game: "Vial" (MsgItemName)
  ITEM_VIAL,
  /// ★ Named from the game: "Potion" (MsgItemName)
  ITEM_POTION,
  /// ★ Named from the game: "Hermes' Water" (MsgItemName)
  ITEM_HERMES_WATER,
  /// ★ Named from the game: "Empty Bottle" (MsgItemName)
  ITEM_EMPTY_BOTTLE,
  /// ★ Named from the game: "Psy Crystal" (MsgItemName)
  ITEM_PSY_CRYSTAL,
  /// ★ Named from the game: "Antidote" (MsgItemName)
  ITEM_ANTIDOTE,
  /// ★ Named from the game: "Elixir" (MsgItemName)
  ITEM_ELIXIR,
  /// ★ Named from the game: "Water of Life" (MsgItemName)
  ITEM_WATER_OF_LIFE,
  ITEM_190,
  /// ★ Named from the game: "Power Bread" (MsgItemName)
  ITEM_POWER_BREAD,
  /// ★ Named from the game: "Cookie" (MsgItemName)
  ITEM_COOKIE,
  /// ★ Named from the game: "Apple" (MsgItemName)
  ITEM_APPLE,
  /// ★ Named from the game: "Hard Nut" (MsgItemName)
  ITEM_HARD_NUT,
  /// ★ Named from the game: "Mint" (MsgItemName)
  ITEM_MINT,
  /// ★ Named from the game: "Lucky Pepper" (MsgItemName)
  ITEM_LUCKY_PEPPER,
  ITEM_197,
  ITEM_198,
  ITEM_199,
  /// ★ Named from the game: "Orb of Force" (MsgItemName)
  ITEM_ORB_OF_FORCE,
  /// ★ Named from the game: "Douse Drop" (MsgItemName)
  ITEM_DOUSE_DROP,
  /// ★ Named from the game: "Frost Jewel" (MsgItemName)
  ITEM_FROST_JEWEL,
  /// ★ Named from the game: "Lifting Gem" (MsgItemName)
  ITEM_LIFTING_GEM,
  /// ★ Named from the game: "Halt Gem" (MsgItemName)
  ITEM_HALT_GEM,
  /// ★ Named from the game: "Cloak Ball" (MsgItemName)
  ITEM_CLOAK_BALL,
  /// ★ Named from the game: "Carry Stone" (MsgItemName)
  ITEM_CARRY_STONE,
  /// ★ Named from the game: "Catch Beads" (MsgItemName)
  ITEM_CATCH_BEADS,
  ITEM_208,
  ITEM_209,
  ITEM_210,
  ITEM_211,
  ITEM_212,
  ITEM_213,
  ITEM_214,
  ITEM_215,
  ITEM_216,
  ITEM_217,
  ITEM_218,
  ITEM_219,
  /// ★ Named from the game: "Venus Star" (MsgItemName)
  ITEM_VENUS_STAR,
  /// ★ Named from the game: "Mercury Star" (MsgItemName)
  ITEM_MERCURY_STAR,
  /// ★ Named from the game: "Mars Star" (MsgItemName)
  ITEM_MARS_STAR,
  /// ★ Named from the game: "Jupiter Star" (MsgItemName)
  ITEM_JUPITER_STAR,
  /// ★ Named from the game: "Mythril Bag" (MsgItemName)
  ITEM_MYTHRIL_BAG,
  /// ★ Named from the game: "Small Jewel" (MsgItemName)
  ITEM_SMALL_JEWEL,
  /// ★ Named from the game: "Smoke Bomb" (MsgItemName)
  ITEM_SMOKE_BOMB,
  /// ★ Named from the game: "Sleep Bomb" (MsgItemName)
  ITEM_SLEEP_BOMB,
  /// ★ Named from the game: "Game Ticket" (MsgItemName)
  ITEM_GAME_TICKET,
  /// ★ Named from the game: "Lucky Medal" (MsgItemName)
  ITEM_LUCKY_MEDAL,
  /// ★ Named from the game: "Dragon's Eye" (MsgItemName)
  ITEM_DRAGONS_EYE,
  /// ★ Named from the game: "Bone" (MsgItemName)
  ITEM_BONE,
  /// ★ Named from the game: "Anchor Charm" (MsgItemName)
  ITEM_ANCHOR_CHARM,
  /// ★ Named from the game: "Corn" (MsgItemName)
  ITEM_CORN,
  /// ★ Named from the game: "Cell Key" (MsgItemName)
  ITEM_CELL_KEY,
  /// ★ Named from the game: "Boat Ticket" (MsgItemName)
  ITEM_BOAT_TICKET,
  /// ★ Named from the game: "Sacred Feather" (MsgItemName)
  ITEM_SACRED_FEATHER,
  /// ★ Named from the game: "Mystic Draught" (MsgItemName)
  ITEM_MYSTIC_DRAUGHT,
  /// ★ Named from the game: "Oil Drop" (MsgItemName)
  ITEM_OIL_DROP,
  /// ★ Named from the game: "Weasel's Claw" (MsgItemName)
  ITEM_WEASELS_CLAW,
  /// ★ Named from the game: "Bramble Seed" (MsgItemName)
  ITEM_BRAMBLE_SEED,
  /// ★ Named from the game: "Crystal Powder" (MsgItemName)
  ITEM_CRYSTAL_POWDER,
  /// ★ Named from the game: "Black Orb" (MsgItemName)
  ITEM_BLACK_ORB,
  /// ★ Named from the game: "Red Key" (MsgItemName)
  ITEM_RED_KEY,
  /// ★ Named from the game: "Blue Key" (MsgItemName)
  ITEM_BLUE_KEY,
  ITEM_245,
  ITEM_246,
  ITEM_247,
  ITEM_248,
  ITEM_249,
  /// ★ Named from the game: "Mythril Shirt" (MsgItemName)
  ITEM_MYTHRIL_SHIRT,
  /// ★ Named from the game: "Silk Shirt" (MsgItemName)
  ITEM_SILK_SHIRT,
  /// ★ Named from the game: "Running Shirt" (MsgItemName)
  ITEM_RUNNING_SHIRT,
  ITEM_253,
  ITEM_254,
  ITEM_255,
  /// ★ Named from the game: "Hyper Boots" (MsgItemName)
  ITEM_HYPER_BOOTS,
  /// ★ Named from the game: "Quick Boots" (MsgItemName)
  ITEM_QUICK_BOOTS,
  /// ★ Named from the game: "Fur Boots" (MsgItemName)
  ITEM_FUR_BOOTS,
  /// ★ Named from the game: "Turtle Boots" (MsgItemName)
  ITEM_TURTLE_BOOTS,
  ITEM_260,
  ITEM_261,
  /// ★ Named from the game: "Adept Ring" (MsgItemName)
  ITEM_ADEPT_RING,
  /// ★ Named from the game: "War Ring" (MsgItemName)
  ITEM_WAR_RING,
  /// ★ Named from the game: "Sleep Ring" (MsgItemName)
  ITEM_SLEEP_RING,
  /// ★ Named from the game: "Healing Ring" (MsgItemName)
  ITEM_HEALING_RING,
  /// ★ Named from the game: "Unicorn Ring" (MsgItemName)
  ITEM_UNICORN_RING,
  /// ★ Named from the game: "Fairy Ring" (MsgItemName)
  ITEM_FAIRY_RING,
  /// ★ Named from the game: "Cleric's Ring" (MsgItemName)
  ITEM_CLERICS_RING,
  ITEM_269,
  // Unused: rows 270 to 323 are the same weapon for Isaac, priced 100.
  // Nothing gives them and the debug menus' item browser stops at 269; the
  // name each would show, MsgItemName + its id, is an enemy's.
  ITEM_270,
  ITEM_271,
  ITEM_272,
  ITEM_273,
  ITEM_274,
  ITEM_275,
  ITEM_276,
  ITEM_277,
  ITEM_278,
  ITEM_279,
  ITEM_280,
  ITEM_281,
  ITEM_282,
  ITEM_283,
  ITEM_284,
  ITEM_285,
  ITEM_286,
  ITEM_287,
  ITEM_288,
  ITEM_289,
  ITEM_290,
  ITEM_291,
  ITEM_292,
  ITEM_293,
  ITEM_294,
  ITEM_295,
  ITEM_296,
  ITEM_297,
  ITEM_298,
  ITEM_299,
  ITEM_300,
  ITEM_301,
  ITEM_302,
  ITEM_303,
  ITEM_304,
  ITEM_305,
  ITEM_306,
  ITEM_307,
  ITEM_308,
  ITEM_309,
  ITEM_310,
  ITEM_311,
  ITEM_312,
  ITEM_313,
  ITEM_314,
  ITEM_315,
  ITEM_316,
  ITEM_317,
  ITEM_318,
  ITEM_319,
  ITEM_320,
  ITEM_321,
  ITEM_322,
  ITEM_323
};

#endif
