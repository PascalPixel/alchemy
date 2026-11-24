#ifndef ALCHEMY_FAR_CALL_BATTLE_H
#define ALCHEMY_FAR_CALL_BATTLE_H

/// @file
/// The battle bank's far calls: running a battle, its camera and background,
/// and how its units move.

void DebugBattle_RunMenuFar(void);
void BattleObject_SpawnObjectsForListFar(s16 *unitIds, s32 refreshPoses);
void BattleCamera_SetViewFar(s32 x, s32 depth, s32 y, s32 zoom);
void BattleCamera_SetViewAtPairMidpointFar(s32 firstUnitId, s32 secondUnitId,
                                           s32 zoom);
void BattleBackground_SetModeFar(s32 mode, s32 unusedResource, s32 fade);
void BattleBackground_LoadFar(s32 mode, s32 resourceId, s32 level);
void BattleBackground_SetPaletteLevelFar(s32 unused, s32 level);
s32 BattleSprite_HasHeavyImpactFar(s32 index);
s32 BattleUnit_GetEffectHeightFar(s32 unitId);
void BattleMotion_ApproachTargetFar(s32 unitId, s32 targetId, s32 frames,
                                    s32 initialVelocityY);
void BattleMotion_ApplyKnockbackFar(s32 unitId, s32 variant);
s32 BattleUnit_ListIdsFar(s32 sideMask, u16 *unitIds);
/// Returns a unit's battle object, or null for a unit with no place
/// (BattleObject_Get); the battle effects find the sprites they move through
/// it.
struct BattleObject *BattleObject_GetFar(s32 unitId);
s32 Battle_RunFar(s32 enemyGroup);
s32 BattleMotion_ProjectPositionFar(s32 unitId, s32 *projected);
s32 BattleMotion_ProjectScaledPositionFar(s32 unitId, s32 *projected);
s32 BattleMotion_ProjectCursorPointFar(s32 unitId, s32 *projected);
s32 BattleParty_PrepareMembersFar(u16 *unitIds);
void BattleCamera_SetCommandViewFar(s32 verticalOffset);
void *Actor_GetSpriteFar(struct Actor *actor, s32 recordIndex);
void BattleHighlight_SetModesFar(u16 *selectedUnits, s32 mode);
void BattlePlayback_StartFar(s32 cue);
s32 Graphics_ScaleRgb555ClampedFar(u16 *source, u16 *destination, s32 scale,
                                   s32 count);
s32 DjinnRecovery_CountDownFar(void);
s32 BattleObject_GetPaletteFar(s32 id);
s32 DjinnRecovery_CountDownTwentyFar(void);
s32 DebugParameterSet_LoadFar(s32 set);
void BattleObject_PutFar(void);
UNREAD_RESULT BattleMessage_WaitForAdvanceAtFar(s32 x, s32 y);
s32 BattleDjinn_CountStandbyFar(u32 unitId, u8 *counts);

#endif
