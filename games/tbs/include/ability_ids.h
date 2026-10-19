#ifndef ALCHEMY_ABILITY_IDS_H
#define ALCHEMY_ABILITY_IDS_H

/// Ability numbers. Each ability's name is message 819 plus its number in the
/// game's text (Frost is message 843, Mind Read 960).
enum AbilityId {
  ABILITY_GROWTH = 0x0c,
  ABILITY_FROST = 0x18,
  ABILITY_DOUSE = 0x21,
  ABILITY_WHIRLWIND = 0x4e,
  ABILITY_PLY = 0x5d,
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
  ABILITY_DROWN = 0xdb,
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
  /// An enemy that splits itself in two ("attempts to divide").
  ABILITY_SPEED_SURGE = 0x1f7
};

#endif
