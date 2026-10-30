#ifndef ALCHEMY_ABILITY_IDS_H
#define ALCHEMY_ABILITY_IDS_H

/// @file
/// Ability numbers, named from the English release.

/// Ability numbers. Each ability's name is the message MsgAbilityName plus its
/// number, as ability_table.c's last column gives it.
enum AbilityId {
  ABILITY_ATTACK = 1,
  ABILITY_GROWTH = 12,
  ABILITY_FROST = 24,
  ABILITY_DOUSE = 33,
  ABILITY_WHIRLWIND = 78,
  ABILITY_PLY = 93,
  ABILITY_PSY_DRAIN = 126, ///< Psynergy that costs no PP
  ABILITY_MOVE = 140,
  ABILITY_MIND_READ = 141,
  ABILITY_FORCE = 142,
  ABILITY_LIFT = 143,
  ABILITY_REVEAL = 144,
  ABILITY_HALT = 145,
  ABILITY_CLOAK = 146,
  ABILITY_CARRY = 147,
  ABILITY_CATCH = 148,
  ABILITY_RETREAT = 149,
  ABILITY_AVOID = 150,
  /// Rolls its hit as it is planned.
  ABILITY_ANNIHILATION = 178,
  ABILITY_DROWN = 219,
  /// The first of the enemy abilities BattleActionRun_Build announces in
  /// their own words; the others run from ABILITY_SONIC_WAVE to
  /// ABILITY_SEARCH_FOR_HELP.
  ABILITY_HEAT_MIRAGE = 224,
  /// The stat items' abilities, each raising one base stat for good (the item
  /// menu's use).
  ABILITY_POWER_BREAD = 260,
  ABILITY_COOKIE = 261,
  ABILITY_APPLE = 262,
  ABILITY_HARD_NUT = 263,
  ABILITY_MINT = 264,
  ABILITY_LUCKY_PEPPER = 265,
  /// The first Djinni's ability: Djinni n of element e has ability
  /// ABILITY_DJINNI_FIRST + e * DJINN_PER_ELEMENT + n, djinn_table.c's ability
  /// column, whose name is the Djinni's.
  ABILITY_DJINNI_FIRST = 300,
  /// The summons: four to an element, in the order of their Djinn costs, each
  /// element's six ids apart.
  ABILITY_VENUS = 380,
  ABILITY_RAMSES = 381,
  ABILITY_CYBELE = 382,
  ABILITY_JUDGMENT = 383,
  ABILITY_MERCURY = 386,
  ABILITY_NEREID = 387,
  ABILITY_NEPTUNE = 388,
  ABILITY_BOREAS = 389,
  ABILITY_MARS = 392,
  ABILITY_KIRIN = 393,
  ABILITY_TIAMAT = 394,
  ABILITY_METEOR = 395,
  ABILITY_JUPITER = 398,
  ABILITY_ATALANTA = 399,
  ABILITY_PROCNE = 400,
  ABILITY_THOR = 401,
  /// Power 5: the second Bat row's (enemy_table.c).
  ABILITY_SONIC_WAVE = 435,
  /// Power 45 against the Bat's 5: the Rabid Bat's, the Clay Gargoyle's,
  /// the Ant Lion's and the Roach's.
  /// ★ Named from the game: "Sonic Wave" (MsgDjinnName)
  ABILITY_SONIC_WAVE_STRONG = 436,
  ABILITY_SHRIEK = 437,
  ABILITY_BANSHEE_HOWL = 438,
  ABILITY_CRAZY_VOICE = 439,
  ABILITY_WAR_CRY = 440,
  ABILITY_WICKED_HOWL = 441,
  ABILITY_WING_BEAT = 442,
  ABILITY_WING_FLUTTER = 443,
  ABILITY_WING_STROKE = 444,
  ABILITY_ROTTEN_BLOOD = 472,
  ABILITY_HEARTRENDER = 488,
  ABILITY_DECOMPOSE = 492,
  ABILITY_WORMS = 494,
  ABILITY_BERSERK = 495,
  ABILITY_CONTAIN = 499,
  ABILITY_THREATEN = 500,
  ABILITY_TREMBLE = 501,
  /// An enemy that splits itself in two. Its message: "attempts to divide".
  ABILITY_SPEED_SURGE = 503,
  ABILITY_ALLY_SEARCH = 504,
  ABILITY_SEARCH_FOR_HELP = 508,
  /// What an enemy with nothing it can use commands.
  ABILITY_CANT_USE = 509,
  /// The last row of gAbilityStaging.
  ABILITY_SEVERE_BLOW = 517,
  /// The last row of ability_table.c and gAbilityBattleEffects.
  ABILITY_THRASH = 518
};

#endif
