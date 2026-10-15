#ifndef ALCHEMY_ABILITY_IDS_H
#define ALCHEMY_ABILITY_IDS_H

// Ability numbers. Each ability's name is message 819 plus its number in the
// game's text (Frost is message 843, Mind Read 960).
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
  ABILITY_AVOID = 0x96
};

#endif
