#ifndef ALCHEMY_ENEMY_IDS_H
#define ALCHEMY_ENEMY_IDS_H

/// @file
/// Enemy ids, named from the English release.

/// The first enemy id: enemy id `ENEMY_ID_FIRST + n` is the enemy in
/// row n of enemies.c (Enemy_GetDefinition); the ids below it are the
/// party's characters. A formation lists its members by row.
#define ENEMY_ID_FIRST 8

/// The enemies the code tests by id, each named by its row of
/// enemies.c: an enemy id is the row plus ENEMY_ID_FIRST. Where several
/// rows share a name and each is fought in one place (its formation in
/// formations.tsv, chosen from a scripted or mimic zone of encounter.tsv), each
/// takes that place; the others take `_2` onwards in row order.
enum EnemyId {
  ENEMY_SLIME = 18,
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
  ENEMY_AZART = 132,
  ENEMY_AZART_2 = 133,
  ENEMY_SATRAGE = 134,
  ENEMY_SATRAGE_2 = 135,
  ENEMY_NAVAMPA = 136,
  ENEMY_NAVAMPA_2 = 137,
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
  /// BattleUnit_Set puts them on the party's side.
  ENEMY_VENUS_DJINNI = 158,
  ENEMY_VENUS_DJINNI_2 = 159,
  ENEMY_VENUS_DJINNI_3 = 160,
  ENEMY_MERCURY_DJINNI = 161,
  ENEMY_MERCURY_DJINNI_2 = 162,
  ENEMY_MERCURY_DJINNI_3 = 163,
  ENEMY_MERCURY_DJINNI_4 = 164,
  ENEMY_MARS_DJINNI = 165,
  ENEMY_MARS_DJINNI_2 = 166,
  ENEMY_MARS_DJINNI_3 = 167,
  ENEMY_JUPITER_DJINNI = 168,
  ENEMY_JUPITER_DJINNI_2 = 169,
  ENEMY_JUPITER_DJINNI_3 = 170,
  ENEMY_JUPITER_DJINNI_4 = 171
};

#endif
