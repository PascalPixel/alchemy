#ifndef ALCHEMY_FAR_CALL_BATTLE_H
#define ALCHEMY_FAR_CALL_BATTLE_H

/// @file
/// The far calls into the battle bank, as the other banks' code calls them.

#include "types.h"

struct TargetPoint;
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);
void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);
void *GetMotionRecordFar(void *object, s32 index);
s32 BattleMotion_ProjectConditionalPositionFar(s32 unit,
                                               struct TargetPoint *point);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void ReleaseBattleObjectRecordsFar(s32 actor);
void BattleObject_PutFar(void);
void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar

void BattlePlayback_StartFar(s32 phase);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void BattleBackground_SetModeFar(s32 mode, s32 background, s32 value);
void Func_08118150(s32 kind);
s32 BattleMotion_GetSlotPaletteFar(s32 id);
s32 BattleParty_PrepareReserveOwnersFar(u16 *owners);
s32 BattleUnit_ListIdsFar(s32, s32);
void BattleSummon_UpdateAvailabilityFar(void);
void BattleCamera_SetCommandViewFar(s32 mode);

#endif
