#ifndef ALCHEMY_BATTLE_SPRITES_H
#define ALCHEMY_BATTLE_SPRITES_H

/// @file
/// Summons and the battle sprite definitions of the enemies.

#include "static_assert.h"

#include "types.h"
#include "edition.h"
#include "far_call.h"

/// An enemy's battle sprite: its sprite id, size, attack effect,
/// palette and defeat-sound kind.
struct BattleSpriteDefinition {
  u16 animation; ///< the enemy's battle sprite
  /// The byte at 2.
  /// bits: 7..5 attackHop | 4..1 attackEffect | 0 isSmall
  ///@{
  u8 isSmall : 1;      ///< 1 for a small enemy, which takes one sprite slot
  u8 attackEffect : 4; ///< the attack effect's kind; 0 reads as 1
  u8 attackHop : 3;    ///< the attack-hop height
  ///@}
  /// The byte at 3.
  /// bits: 7..5 defeatSound | 4..1 palette | 0 heavyImpact
  ///@{
  u8 heavyImpact : 1; ///< 1 for an enemy whose beam blow lands heavily
  u8 palette : 4;     ///< the sprite's palette
  u8 defeatSound : 3; ///< the defeat-sound kind; above 4 reads as none
  ///@}
  /// The height a battle effect is drawn at over the enemy, in pixels; 0 for
  /// its size's default.
  u8 effectHeight;
  u8 reserved[3]; ///< 0 in every row
};

STATIC_ASSERT(sizeof(struct BattleSpriteDefinition) == 8);
/// The enemies' battle sprites, one row per enemy id 0 to 171
/// (battle_sprites.c), read by the BattleSprite_ getters; row 0 stands in for
/// an id out of range.
extern const struct BattleSpriteDefinition gBattleSprites[];

/// A summon: its name's message and the Djinn of each element it needs.
struct SummonDefinition {
  /// The summon's ability id: its name is message MsgAbilityName plus it, and
  /// its low 14 bits pick the icon.
  u16 abilityId;
  u8 reserved[2];      ///< 0 in every row; read by no code
  u8 djinnRequired[4]; ///< standby Djinn needed, by element
};

/// The party's Djinn recovery table as summons read it: the mask of summons
/// the party can call, bit n for summon n.
struct BattleSummonState {
  u32 availableMask; ///< nonzero offers the Summon command
};

#endif
