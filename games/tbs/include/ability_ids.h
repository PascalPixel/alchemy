#ifndef ALCHEMY_ABILITY_IDS_H
#define ALCHEMY_ABILITY_IDS_H

/// @file
/// Ability numbers, named from the English release.

/// Ability numbers. Each ability's name is the message MsgAbilityName plus its
/// number, as actions.tsv's last column gives it.
enum AbilityId {
  ABILITY_ATTACK = 0x01,
  ABILITY_GROWTH = 0x0c,
  ABILITY_FROST = 0x18,
  ABILITY_DOUSE = 0x21,
  ABILITY_WHIRLWIND = 0x4e,
  ABILITY_PLY = 0x5d,
  ABILITY_PSY_DRAIN = 0x7e, ///< Psynergy that costs no PP
  ABILITY_MOVE = 0x8c,
  ABILITY_MIND_READ = 0x8d,
  ABILITY_FORCE = 0x8e,
  ABILITY_LIFT = 0x8f,
  ABILITY_REVEAL = 0x90,
  ABILITY_HALT = 0x91,
  ABILITY_CLOAK = 0x92,
  ABILITY_CARRY = 0x93,
  ABILITY_CATCH = 0x94,
  ABILITY_RETREAT = 0x95,
  ABILITY_AVOID = 0x96,
  /// Rolls its hit as it is planned.
  ABILITY_ANNIHILATION = 0xb2,
  ABILITY_DROWN = 0xdb,
  /// The first of the enemy abilities BattleTurn_Build announces in
  /// their own words; the others run from ABILITY_SONIC_WAVE to
  /// ABILITY_SEARCH_FOR_HELP.
  ABILITY_HEAT_MIRAGE = 0xe0,
  /// The stat items' abilities, each raising one base stat for good (the item
  /// menu's use).
  ABILITY_POWER_BREAD = 0x104,
  ABILITY_COOKIE = 0x105,
  ABILITY_APPLE = 0x106,
  ABILITY_HARD_NUT = 0x107,
  ABILITY_MINT = 0x108,
  ABILITY_LUCKY_PEPPER = 0x109,
  /// The first Djinni's ability: Djinni n of element e has ability
  /// ABILITY_DJINNI_FIRST + e * DJINN_PER_ELEMENT + n, djinn.tsv's ability
  /// column, whose name is the Djinni's.
  ABILITY_DJINNI_FIRST = 0x12c,
  /// The summons: four to an element, in the order of their Djinn costs, each
  /// element's six ids apart.
  ABILITY_VENUS = 0x17c,
  ABILITY_RAMSES = 0x17d,
  ABILITY_CYBELE = 0x17e,
  ABILITY_JUDGMENT = 0x17f,
  ABILITY_MERCURY = 0x182,
  ABILITY_NEREID = 0x183,
  ABILITY_NEPTUNE = 0x184,
  ABILITY_BOREAS = 0x185,
  ABILITY_MARS = 0x188,
  ABILITY_KIRIN = 0x189,
  ABILITY_TIAMAT = 0x18a,
  ABILITY_METEOR = 0x18b,
  ABILITY_JUPITER = 0x18e,
  ABILITY_ATALANTA = 0x18f,
  ABILITY_PROCNE = 0x190,
  ABILITY_THOR = 0x191,
  ABILITY_SONIC_WAVE = 0x1b3,
  /// Sonic Wave's second row.
  ABILITY_SONIC_WAVE_2 = 0x1b4,
  ABILITY_SHRIEK = 0x1b5,
  ABILITY_BANSHEE_HOWL = 0x1b6,
  ABILITY_CRAZY_VOICE = 0x1b7,
  ABILITY_WAR_CRY = 0x1b8,
  ABILITY_WICKED_HOWL = 0x1b9,
  ABILITY_WING_BEAT = 0x1ba,
  ABILITY_WING_FLUTTER = 0x1bb,
  ABILITY_WING_STROKE = 0x1bc,
  ABILITY_ROTTEN_BLOOD = 0x1d8,
  ABILITY_HEARTRENDER = 0x1e8,
  ABILITY_DECOMPOSE = 0x1ec,
  ABILITY_WORMS = 0x1ee,
  ABILITY_BERSERK = 0x1ef,
  ABILITY_CONTAIN = 0x1f3,
  ABILITY_THREATEN = 0x1f4,
  ABILITY_TREMBLE = 0x1f5,
  /// An enemy that splits itself in two. Its message: "attempts to divide".
  ABILITY_SPEED_SURGE = 0x1f7,
  ABILITY_ALLY_SEARCH = 0x1f8,
  ABILITY_SEARCH_FOR_HELP = 0x1fc,
  /// What an enemy with nothing it can use commands.
  ABILITY_CANT_USE = 0x1fd,
  /// The last row of actstat.tsv.
  ABILITY_SEVERE_BLOW = 0x205,
  /// The last row of actions.tsv and actflags.tsv.
  ABILITY_THRASH = 0x206
};

#endif
