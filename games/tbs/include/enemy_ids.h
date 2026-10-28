#ifndef ALCHEMY_ENEMY_IDS_H
#define ALCHEMY_ENEMY_IDS_H

/// @file
/// Enemy ids, named from the English release.

/// The first enemy id: enemy id `ENEMY_ID_FIRST + n` is the enemy in
/// row n of enemy_table.c (Enemy_GetDefinition); the ids below it are the
/// party's characters. A formation lists its members by row.
#define ENEMY_ID_FIRST 8

/// The enemies the code tests by id, each named by its row of
/// enemy_table.c: an enemy id is the row plus ENEMY_ID_FIRST. Where several
/// rows share a name, each takes what tells its battle apart: the one place
/// it is fought (its formation in formations.c, chosen from a zone of
/// encounter_zones.c), the prize a Colosso finalist carries, or the Djinni a
/// Djinni battle is with; a row fought in several places takes `_2` onwards in
/// row order.
enum EnemyId {
  /// Level 2: fought only in Sol Sanctum (formations 11 to 14 and 16).
  /// ★ Named from its area name: Sol Sanctum
  ENEMY_SLIME_SOL_SANCTUM = 18,
  /// Level 3: the Slime of the world map's first encounter region, around
  /// Vale and Vault (formations 23 and 27), and of Goma Cave (formation 32).
  /// Fought in both, it has no one place to be named for.
  ENEMY_SLIME_2 = 19,
  ENEMY_OOZE = 20,
  ENEMY_SLIME_BEAST = 21,
  ENEMY_BANDIT = 29,
  ENEMY_THIEF = 30,
  ENEMY_TROLL = 47,
  ENEMY_CAVE_TROLL = 48,
  ENEMY_BRUTAL_TROLL = 49,
  ENEMY_RECLUSE = 52,
  ENEMY_SIREN = 68,
  ENEMY_SUCCUBUS = 69,
  ENEMY_NIGHTMARE = 70,
  ENEMY_KILLER_APE = 78,
  ENEMY_ORC_CAPTAIN = 84,
  ENEMY_ORC_LORD = 85,
  ENEMY_MANTICORE = 89,
  ENEMY_KOBOLD = 92,
  ENEMY_HOBGOBLIN = 94,
  ENEMY_GRYPHON = 98,
  ENEMY_CEREBUS = 105,
  ENEMY_CHIMERA = 110,
  ENEMY_KRAKEN = 121,
  ENEMY_MYSTERY_MAN = 122,
  /// Level 18.
  /// ★ Named from what the player sees: Saturos, fought alone at Mercury
  /// Lighthouse (its render)
  ENEMY_SATUROS_MERCURY_LIGHTHOUSE = 123,
  /// Level 38.
  /// ★ Named from what the player sees: Saturos, fought beside Menardi at
  /// Venus Lighthouse's aerie (its render)
  ENEMY_SATUROS_VENUS_LIGHTHOUSE = 124,
  ENEMY_MYSTERY_WOMAN = 125,
  ENEMY_MENARDI = 126,
  ENEMY_LIZARD_FIGHTER = 128,
  ENEMY_LIZARD_KING = 129,
  ENEMY_LIVING_STATUE = 130,
  ENEMY_HYDROS_STATUE = 131,
  /// The Colosso finalists, two rows each: whoever reaches a match's arena
  /// first takes its better prize, and the battle (ENCOUNTER_ZONE_COLOSSO,
  /// one formation a row) is with the opponent carrying what he took.
  /// Azart having reached the arena first: defense 20, against 2 without the
  /// shield.
  /// ★ Named from the game: "Iron Shield" (MsgItemName)
  ENEMY_AZART_IRON_SHIELD = 132,
  /// Azart having reached the arena second.
  /// ★ Named from the game: "Padded Gloves" (MsgItemName)
  ENEMY_AZART_PADDED_GLOVES = 133,
  /// Satrage having reached the arena first: defense 28, against 6 without
  /// the mail.
  /// ★ Named from the game: "Chain Mail" (MsgItemName)
  ENEMY_SATRAGE_CHAIN_MAIL = 134,
  /// Satrage having reached the arena second.
  /// ★ Named from the game: "Cotton Shirt" (MsgItemName)
  ENEMY_SATRAGE_COTTON_SHIRT = 135,
  /// Navampa having reached the arena first: attack 274, against 232 without
  /// the Claymore.
  /// ★ Named from the game: "Claymore" (MsgItemName)
  ENEMY_NAVAMPA_CLAYMORE = 136,
  /// Navampa having reached the arena second.
  /// ★ Named from the game: "Hunter's Sword" (MsgItemName)
  ENEMY_NAVAMPA_HUNTERS_SWORD = 137,
  ENEMY_ANT_LION = 138,
  ENEMY_TOADONPA = 141,
  ENEMY_TORNADO_LIZARD = 144,
  ENEMY_STORM_LIZARD = 145,
  ENEMY_TEMPEST_LIZARD = 146,
  ENEMY_TRET = 147,
  ENEMY_FUSION_DRAGON = 148,
  ENEMY_DEADBEARD = 149,
  /// ★ Named from what the player sees: the Mimic that leaps out of the second
  /// chest of Mercury Lighthouse's entrance (FIND_MIMIC, slot 0) (its render)
  ENEMY_MIMIC_MERCURY_LIGHTHOUSE = 150,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// in Fuchin Falls Cave's entrance (FIND_MIMIC, slot 1) (its render)
  ENEMY_MIMIC_FUCHIN_FALLS_CAVE = 151,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// by Altin Peak's pool (FIND_MIMIC, slot 2) (its render)
  ENEMY_MIMIC_ALTIN_PEAK = 152,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// in the Vault cave passage's pool room (FIND_MIMIC, slot 3) (its render)
  ENEMY_MIMIC_VAULT_CAVE = 153,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// on Crossbone Isle's first floor (FIND_MIMIC, slot 4) (its render)
  ENEMY_MIMIC_CROSSBONE_ISLE = 154,
  /// ★ Named from what the player sees: the Mimic that leaps out of the chest
  /// in Altmiller Cave (FIND_MIMIC, slot 5) (its render)
  ENEMY_MIMIC_ALTMILLER_CAVE = 155,
  /// ★ Named from what the player sees: the Mimic that leaps out of a chest in
  /// Suhalla Desert (FIND_MIMIC, slot 6) (its render)
  ENEMY_MIMIC_SUHALLA_DESERT = 156,
  /// ★ Named from what the player sees: the Mimic that leaps out of the second
  /// chest on Crossbone Isle's pirate ship (FIND_MIMIC, slot 7) (its render)
  ENEMY_MIMIC_PIRATE_SHIP = 157,
  /// The Djinn the party fights, each element's in order of level;
  /// BattleUnit_Set puts them on the party's side. A Djinni's battle is a slot
  /// of its element's zone (gEncounter_AreaEntryTable), which Djinn who join
  /// without a fight share; each row is named for the one Djinni whose scene
  /// starts its battle (Field_RequestDjinnBattle).
  /// Level 12, in Mogall Forest.
  /// ★ Named from the game: "Quartz" (MsgDjinnName)
  ENEMY_VENUS_DJINNI_QUARTZ = 158,
  /// Level 15, on the world map.
  /// ★ Named from the game: "Vine" (MsgDjinnName)
  ENEMY_VENUS_DJINNI_VINE = 159,
  /// Unused: level 23, the battle of Bane's slot, but Bane joins on Crossbone
  /// Isle without a fight (Djinn_JoinParty), so no battle reaches this row.
  /// ★ Named from the game: "Bane" (MsgDjinnName)
  ENEMY_VENUS_DJINNI_UNUSED_BANE = 160,
  /// Level 10, in Mercury Lighthouse.
  /// ★ Named from the game: "Sleet" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_SLEET = 161,
  /// Level 13, on Altin Peak.
  /// ★ Named from the game: "Spritz" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_SPRITZ = 162,
  /// Level 18, on the world map.
  /// ★ Named from the game: "Hail" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_HAIL = 163,
  /// Level 24, at Suhalla Gate.
  /// ★ Named from the game: "Dew" (MsgDjinnName)
  ENEMY_MERCURY_DJINNI_DEW = 164,
  /// Level 5, in Goma Cave.
  /// ★ Named from the game: "Forge" (MsgDjinnName)
  ENEMY_MARS_DJINNI_FORGE = 165,
  /// Level 13, on the world map.
  /// ★ Named from the game: "Corona" (MsgDjinnName)
  ENEMY_MARS_DJINNI_CORONA = 166,
  /// Level 22, in the Suhalla Desert.
  /// ★ Named from the game: "Flash" (MsgDjinnName)
  ENEMY_MARS_DJINNI_FLASH = 167,
  /// Level 8, in Tret Tree.
  /// ★ Named from the game: "Breeze" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_BREEZE = 168,
  /// Level 11, in Fuchin Falls Cave.
  /// ★ Named from the game: "Zephyr" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_ZEPHYR = 169,
  /// Level 18, in Altmiller Cave.
  /// ★ Named from the game: "Squall" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_SQUALL = 170,
  /// Level 25, in Babi Lighthouse.
  /// ★ Named from the game: "Luff" (MsgDjinnName)
  ENEMY_JUPITER_DJINNI_LUFF = 171
};

#endif
