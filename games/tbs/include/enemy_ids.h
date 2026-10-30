#ifndef ALCHEMY_ENEMY_IDS_H
#define ALCHEMY_ENEMY_IDS_H

/// @file
/// Enemy ids, named from the English release.

/// The first enemy id: enemy id `ENEMY_ID_FIRST + n` is the enemy in
/// row n of enemy_table.c (Enemy_GetDefinition); the ids below it are the
/// party's characters. A formation lists its members by row.
#define ENEMY_ID_FIRST 8

/// Enemy id `id`'s row of enemy_table.c, as a formation lists its members.
#define ENEMY_ROW(id) ((id) - ENEMY_ID_FIRST)

/// Enemy ids, one for each row of enemy_table.c in order: an enemy id is the
/// row plus ENEMY_ID_FIRST, and a row the code doesn't name yet keeps its row
/// number as its placeholder. Where several rows share a name, each takes what
/// tells its battle apart: the one place it is fought (its formation in
/// formations.c, chosen from a zone of encounter_zones.c), the prize a Colosso
/// finalist carries, or the Djinni a Djinni battle is with; a row fought in
/// several places takes `_2` onwards in row order.
enum EnemyId {
  ENEMY_000 = ENEMY_ID_FIRST,
  ENEMY_001,
  ENEMY_002,
  ENEMY_003,
  ENEMY_004,
  ENEMY_005,
  ENEMY_006,
  ENEMY_007,
  ENEMY_008,
  ENEMY_009,
  /// Level 2: fought only in Sol Sanctum (formations 11 to 14 and 16).
  /// ★ Named from its area name: Sol Sanctum
  ENEMY_SLIME_SOL_SANCTUM,
  /// Level 3: the Slime of the world map's first encounter region, around
  /// Vale and Vault (formations 23 and 27), and of Goma Cave (formation 32).
  /// Fought in both, it has no one place to be named for.
  ENEMY_SLIME_2,
  ENEMY_OOZE,
  ENEMY_SLIME_BEAST,
  ENEMY_014,
  ENEMY_015,
  ENEMY_016,
  ENEMY_017,
  ENEMY_018,
  ENEMY_019,
  ENEMY_020,
  ENEMY_BANDIT,
  ENEMY_THIEF,
  ENEMY_023,
  ENEMY_024,
  ENEMY_025,
  ENEMY_026,
  ENEMY_027,
  ENEMY_028,
  ENEMY_029,
  ENEMY_030,
  ENEMY_031,
  ENEMY_032,
  ENEMY_033,
  ENEMY_034,
  ENEMY_035,
  ENEMY_036,
  ENEMY_037,
  ENEMY_038,
  ENEMY_TROLL,
  ENEMY_CAVE_TROLL,
  ENEMY_BRUTAL_TROLL,
  ENEMY_042,
  ENEMY_043,
  ENEMY_RECLUSE,
  ENEMY_045,
  ENEMY_046,
  ENEMY_047,
  ENEMY_048,
  ENEMY_049,
  ENEMY_050,
  ENEMY_051,
  ENEMY_052,
  ENEMY_053,
  ENEMY_054,
  ENEMY_055,
  ENEMY_056,
  ENEMY_057,
  ENEMY_058,
  ENEMY_059,
  ENEMY_SIREN,
  ENEMY_SUCCUBUS,
  ENEMY_NIGHTMARE,
  ENEMY_063,
  ENEMY_064,
  ENEMY_065,
  ENEMY_066,
  ENEMY_067,
  ENEMY_068,
  ENEMY_069,
  ENEMY_KILLER_APE,
  ENEMY_071,
  ENEMY_072,
  ENEMY_073,
  ENEMY_074,
  ENEMY_075,
  ENEMY_ORC_CAPTAIN,
  ENEMY_ORC_LORD,
  ENEMY_078,
  ENEMY_079,
  ENEMY_080,
  ENEMY_MANTICORE,
  ENEMY_082,
  ENEMY_083,
  ENEMY_KOBOLD,
  ENEMY_085,
  ENEMY_HOBGOBLIN,
  ENEMY_087,
  ENEMY_088,
  ENEMY_089,
  ENEMY_GRYPHON,
  ENEMY_091,
  ENEMY_092,
  ENEMY_093,
  ENEMY_094,
  ENEMY_095,
  ENEMY_096,
  ENEMY_CEREBUS,
  ENEMY_098,
  ENEMY_099,
  ENEMY_100,
  ENEMY_101,
  ENEMY_CHIMERA,
  ENEMY_103,
  ENEMY_104,
  ENEMY_105,
  ENEMY_106,
  ENEMY_107,
  ENEMY_108,
  ENEMY_109,
  ENEMY_110,
  ENEMY_111,
  ENEMY_112,
  ENEMY_KRAKEN,
  ENEMY_MYSTERY_MAN,
  /// Level 18.
  /// ★ Named from what the player sees: Saturos, fought alone at Mercury
  /// Lighthouse (its render)
  ENEMY_SATUROS_MERCURY_LIGHTHOUSE,
  /// Level 38.
  /// ★ Named from what the player sees: Saturos, fought beside Menardi at
  /// Venus Lighthouse's aerie (its render)
  ENEMY_SATUROS_VENUS_LIGHTHOUSE,
  ENEMY_MYSTERY_WOMAN,
  ENEMY_MENARDI,
  ENEMY_119,
  ENEMY_LIZARD_FIGHTER,
  ENEMY_LIZARD_KING,
  ENEMY_LIVING_STATUE,
  ENEMY_HYDROS_STATUE,
  /// The Colosso finalists, two rows each: whoever reaches a match's arena
  /// first takes its better prize, and the battle (ENCOUNTER_ZONE_COLOSSO,
  /// one formation a row) is with the opponent carrying what he took.
  /// Azart having reached the arena first: defense 20, against 2 without the
  /// shield.
  /// ★ Named from the game: "Iron Shield" (MsgItemName)
  ENEMY_AZART_IRON_SHIELD,
  /// Azart having reached the arena second.
  /// ★ Named from the game: "Padded Gloves" (MsgItemName)
  ENEMY_AZART_PADDED_GLOVES,
  /// Satrage having reached the arena first: defense 28, against 6 without
  /// the mail.
  /// ★ Named from the game: "Chain Mail" (MsgItemName)
  ENEMY_SATRAGE_CHAIN_MAIL,
  /// Satrage having reached the arena second.
  /// ★ Named from the game: "Cotton Shirt" (MsgItemName)
  ENEMY_SATRAGE_COTTON_SHIRT,
  /// Navampa having reached the arena first: attack 274, against 232 without
  /// the Claymore.
  /// ★ Named from the game: "Claymore" (MsgItemName)
  ENEMY_NAVAMPA_CLAYMORE,
  /// Navampa having reached the arena second.
  /// ★ Named from the game: "Hunter's Sword" (MsgItemName)
  ENEMY_NAVAMPA_HUNTERS_SWORD,
  ENEMY_ANT_LION,
  ENEMY_131,
  ENEMY_132,
  ENEMY_TOADONPA,
  ENEMY_134,
  ENEMY_135,
  ENEMY_TORNADO_LIZARD,
  ENEMY_STORM_LIZARD,
  ENEMY_TEMPEST_LIZARD,
  ENEMY_TRET,
  ENEMY_FUSION_DRAGON,
  ENEMY_DEADBEARD,
  /// ★ Named from what the player sees: the Mimic that leaps out of the second
  /// chest of Mercury Lighthouse's entrance (FIND_MIMIC, slot 0) (its render)
  ENEMY_MIMIC_MERCURY_LIGHTHOUSE,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// in Fuchin Falls Cave's entrance (FIND_MIMIC, slot 1) (its render)
  ENEMY_MIMIC_FUCHIN_FALLS_CAVE,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// by Altin Peak's pool (FIND_MIMIC, slot 2) (its render)
  ENEMY_MIMIC_ALTIN_PEAK,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// in the Vault cave passage's pool room (FIND_MIMIC, slot 3) (its render)
  ENEMY_MIMIC_VAULT_CAVE,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// on Crossbone Isle's first floor (FIND_MIMIC, slot 4) (its render)
  ENEMY_MIMIC_CROSSBONE_ISLE,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// in Altmiller Cave (FIND_MIMIC, slot 5) (its render)
  ENEMY_MIMIC_ALTMILLER_CAVE,
  /// ★ Named from what the player sees: the Mimic that leaps out of a chest in
  /// Suhalla Desert (FIND_MIMIC, slot 6) (its render)
  ENEMY_MIMIC_SUHALLA_DESERT,
  /// ★ Named from what the player sees: the Mimic that leaps out of the second
  /// chest on Crossbone Isle's pirate ship (FIND_MIMIC, slot 7) (its render)
  ENEMY_MIMIC_PIRATE_SHIP,
  /// The Djinn the party fights, each element's in order of level;
  /// BattleUnit_Set puts them on the party's side. A Djinni's battle is a slot
  /// of its element's zone (gEncounter_AreaEntryTable), which Djinn who join
  /// without a fight share; each row is named for the one Djinni whose scene
  /// starts its battle (Field_RequestDjinnBattle).
  /// Level 12, in Mogall Forest.
  /// ★ Named from the game: "Quartz" (MsgDjinnName)
  ENEMY_VENUS_DJINNI_QUARTZ,
  /// Level 15, on the world map.
  /// ★ Named from the game: "Vine" (MsgDjinnName)
  ENEMY_VENUS_DJINNI_VINE,
  /// Unused: level 23, the battle of Bane's slot, but Bane joins on Crossbone
  /// Isle without a fight (Djinn_JoinParty), so no battle reaches this row.
  /// ★ Named from the game: "Bane" (MsgDjinnName)
  ENEMY_VENUS_DJINNI_UNUSED_BANE,
  /// Level 10, in Mercury Lighthouse.
  /// ★ Named from the game: "Sleet" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_SLEET,
  /// Level 13, on Altin Peak.
  /// ★ Named from the game: "Spritz" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_SPRITZ,
  /// Level 18, on the world map.
  /// ★ Named from the game: "Hail" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_HAIL,
  /// Level 24, at Suhalla Gate.
  /// ★ Named from the game: "Dew" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_DEW,
  /// Level 5, in Goma Cave.
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  ENEMY_MARS_DJINNI_FORGE,
  /// Level 13, on the world map.
  /// ★ Named from the game: "Corona" (MsgDjinnName)
  ENEMY_MARS_DJINNI_CORONA,
  /// Level 22, in the Suhalla Desert.
  /// ★ Named from the game: "Flash" (MsgDjinnName)
  ENEMY_MARS_DJINNI_FLASH,
  /// Level 8, in Tret Tree.
  /// ★ Named from the game: "Breeze" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_BREEZE,
  /// Level 11, in Fuchin Falls Cave.
  /// ★ Named from the game: "Zephyr" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_ZEPHYR,
  /// Level 18, in Altmiller Cave.
  /// ★ Named from the game: "Squall" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_SQUALL,
  /// Level 25, in Babi Lighthouse.
  /// ★ Named from the game: "Luff" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_LUFF,
  ENEMY_164
};

#endif
