#ifndef ALCHEMY_UNIT_GAME_CHARA_H
#define ALCHEMY_UNIT_GAME_CHARA_H

#include "types.h"

struct BtlAct;
struct BtlUnit;
struct CharacterDefinition;
struct ClassDefinition;
struct ItemDefinition;
struct LevelUpResult;
struct SummonDefinition;

void Owner_RefreshAndResetZero(void);
void *Trade_GetOfferState(s32 arg0);
u32 Party_GetAverageLevel(void);
struct BtlUnit *UnitGet(u32 owner);
const u8 *Owner_GetRecord(s32 selector);
void Runtime_CopyBytesDirectional(u8 *first, u8 *second, s32 count, s32 direction);
void UnitCalc(s32 owner);
void GameFlag_RefreshLureCap(void);
u32 Runtime_GetBuildStampTime(void);
void GameState_InitDefaults(void);
void Party_SetFlag32AndRefreshMembers(void);
void Party_ApplyStatePreset(void);
void Owner_RefreshActiveRatios(s32 arg0);
void Owner_RefreshRatiosOnFlag(void);
void Owner_RatioNoOp(void);
void UnitRatio(s32 owner_no);
void Owner_UpdateRatioPair(struct BtlUnit *state, s32 input);
void Owner_UpdateSecondInputAndRatios(struct BtlUnit *unit, s32 ep);
s32 Owner_AdjustFirstValue(s32 owner, s32 delta);
s16 UnitEpAdd(s32 owner, s32 delta);
s32 Party_AdjustSixDigitCounterB(s32 amount);
s32 Party_AdjustCounterCappedAt28(s32 amount);

#endif
