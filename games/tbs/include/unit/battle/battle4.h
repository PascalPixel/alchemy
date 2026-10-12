#ifndef ALCHEMY_UNIT_BATTLE_BATTLE4_H
#define ALCHEMY_UNIT_BATTLE_BATTLE4_H

#include "types.h"

struct BattleActionRecord;

void Palette_UpdatePulseBrightness(void);
void Camera_ConfigureScene(s32 pos);
void BattleUnit_ResetGuardLevels(void);
s32 BattleEscape_CheckSuccess(void);
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries);
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries);
void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
void Palette_CopyBanksWithBrightnessOffset(s32 ofs);
s32 BattlePresentation_BuildActions(struct BattleActionRecord *acts);
s32 Battle_GetTaggedSlotValue(s32 slot);
s32 Battle_FindTaggedSlotByValue(u32 id);
void Battle_ReservedNoOp9B2C(void);

#endif
