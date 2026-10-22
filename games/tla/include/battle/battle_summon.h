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
  /// bits: 7..5 spriteBits5 | 4..1 spriteBits1 | 0 spriteBit0
  ///@{
  u8 spriteBit0 : 1;  ///< 1 for a small enemy (BattleClass_IsSmall)
  u8 spriteBits1 : 4; ///< the attack-effect kind; 0 reads as 1
  u8 spriteBits5 : 3; ///< the attack-hop height
  ///@}
  /// The byte at 3.
  /// bits: 7..5 layoutKind | 4..1 layoutBits1 | 0 layoutBit0
  ///@{
  u8 layoutBit0 : 1;  ///< read by Summon_IsEntrySecondaryFlagged only
  u8 layoutBits1 : 4; ///< the sprite's palette
  u8 layoutKind : 3;  ///< the defeat-sound kind; above 4 reads as none
  ///@}
  u8 unk04[4]; ///< [0] is the effect height (BattleClass_GetEffectHeight)
};

STATIC_ASSERT(sizeof(struct BattleSpriteDefinition) == 8);
extern struct BattleSpriteDefinition gSummon_EntryTable[];

/// A summon: its name and the Djinn of each element it needs.
struct SummonDefinition {
  u16 nameMessageId;
  u8 unk02[2];         ///< not named by TLA code
  u8 djinnRequired[4]; ///< standby Djinn needed, by element
};

struct BattleSummonState {
  u32 availableMask;
};

const struct SummonDefinition *SummonDefinition_Get(s32 summon_id);
s32 Enemy_GetFreeSlot(void);
s32 Enemy_CheckRoom(s32 classId);

const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id);
s32 BattleFormation_GetRandomMember(s32 recordId);
s32 BattleEnemy_ClaimNameLetter(s32 classId, s32 n);
s32 BattleEnemy_ReleaseNameLetter(s32 actorId);
s32 BattleEnemy_LetterFirstOfClass(s32 classId);
s32 Enemy_Update(void);

#endif
