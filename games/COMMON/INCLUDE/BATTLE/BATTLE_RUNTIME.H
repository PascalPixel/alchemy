#ifndef ALCHEMY_BATTLE_RUNTIME_H
#define ALCHEMY_BATTLE_RUNTIME_H

#include "BATTLE_EVENT.H"
#include "BATTLE_TYPES.H"

/* Battle-facing views of the polymorphic runtime lookup functions. */
struct BattleUnit *Owner_GetStateFar(s32 unit_id);
void BattleUnit_Recalculate(s32 unit_id);
struct BattleAction *BattleAction_Get(s32 action_id);
struct BattleAction *BattleAction_GetDirect(s32 action_id);
s32 GameFlag_TestFar(s32 flag_id);
void Owner_RecalculateRatiosFar(s32 unit_id);
s32 BattleRandom16Far(void);
s32 BattleFx_IsReviveFar(s32 effect_id);
s32 Battle_ClassifyEntryKind(const struct BattleAction *entry);

#endif
