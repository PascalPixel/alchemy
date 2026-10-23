#ifndef ALCHEMY_BATTLE_SUMMON_H
#define ALCHEMY_BATTLE_SUMMON_H

/// @file
/// Summons and the battle sprite definitions of the enemy classes.

#include "static_assert.h"

#include "types.h"
#include "edition.h"
#include "farcall.h"

/// An enemy class's battle sprite: its animation, size, attack effect,
/// palette and defeat-sound kind.
struct BattleSpriteDefinition {
  u16 animation; ///< the class's battle sprite
  /// The byte at 2.
  /// bits: 7..5 spriteBits5 | 4..1 spriteBits1 | 0 spriteBit0
  ///@{
  u8 spriteBit0 : 1;  ///< 1 for a small enemy, which takes one sprite slot
  u8 spriteBits1 : 4; ///< the attack-effect kind; 0 reads as 1
  u8 spriteBits5 : 3; ///< the attack-hop height
  ///@}
  /// The byte at 3.
  /// bits: 7..5 layoutKind | 4..1 layoutBits1 | 0 heavyImpact
  ///@{
  u8 heavyImpact : 1; ///< 1 for a class whose beam blow lands heavily
  u8 layoutBits1 : 4; ///< the sprite's palette
  u8 layoutKind : 3;  ///< the defeat-sound kind; above 4 reads as none
  ///@}
  /// The height an effect is drawn at over the class, in pixels; 0 for its
  /// size's default.
  u8 effectHeight;
  u8 reserved[3]; ///< 0 in every row
};

STATIC_ASSERT(sizeof(struct BattleSpriteDefinition) == 8);
/// The enemy classes' battle sprites, one row per class 0 to 171 (sprites.tsv),
/// read by the BattleClass_ getters; row 0 stands in for a class out of range.
extern struct BattleSpriteDefinition gSummon_EntryTable[];

/// A summon: its name's message and the Djinn of each element it needs.
struct SummonDefinition {
  /// The summon's ability id: its name is message MsgAbilityName plus it, and
  /// its low 14 bits pick the icon.
  u16 nameMessageId;
  u8 reserved[2];      ///< 0 in every row; read by no code
  u8 djinnRequired[4]; ///< standby Djinn needed, by element
};

/// The party's Djinn recovery table as summons read it: the mask of summons
/// the party can call, bit n for summon n.
struct BattleSummonState {
  u32 availableMask; ///< nonzero offers the Summon command
};

#endif
