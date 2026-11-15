#ifndef ALCHEMY_BATTLE_SUMMON_H
#define ALCHEMY_BATTLE_SUMMON_H

#include "static_assert.h"

#include "types.h"
#include "edition.h"

/// An enemy class's battle sprite: its animation, size, attack effect,
/// palette and defeat-sound kind.
struct BattleSpriteDefinition {
  u16 animation; ///< the class's battle sprite
  /// The byte at 2.
  /// bits: 7..5 attackHop | 4..1 attackEffect | 0 isSmall
  ///@{
  u8 isSmall : 1;      ///< 1 for a small enemy, which takes one sprite slot
  u8 attackEffect : 4; ///< the beam its attack plays; 0 reads as 1
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
/// Each enemy's battle sprite and how it attacks and falls, by enemy id.
extern struct BattleSpriteDefinition gBattleSprites[];

/// A summon: its name and the Djinn of each element it needs.
struct SummonDefinition {
  u16 abilityId;
  u8 reserved[2];      ///< 0 in every row; read by no code
  u8 djinnRequired[4]; ///< standby Djinn needed, by element
};

/// Returns a summon's definition, or NULL past the last.
const struct SummonDefinition *SummonDefinition_Get(s32 summon_id);
s32 Enemy_GetFreeUnitId(void);
s32 Enemy_HasSpriteRoom(s32 classId);

const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id);
s32 BattleFormation_GetRandomMember(s32 recordId);
s32 BattleEnemy_ClaimNameLetter(s32 classId, s32 n);
UNREAD_RESULT BattleEnemy_ReleaseNameLetter(s32 unitId);
s32 BattleEnemy_LetterFirstOfKind(s32 classId);
void Enemy_Update(void);

#endif
