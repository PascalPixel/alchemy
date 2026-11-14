#ifndef ALCHEMY_FAR_CALL_BATTLE_H
#define ALCHEMY_FAR_CALL_BATTLE_H

/// @file
/// Battle state, unit motion and presentation services.

void DebugBattle_RunMenuFar(void);
void BattleObject_SpawnObjectsForListFar(s16 *list, s32 refresh);
void BattleCamera_SetViewFar(s32 x, s32 depth, s32 y, s32 zoom);
void BattleCamera_SetViewAtPairMidpointFar(s32 a, s32 b, s32 zoom);
void BattleBackground_SetModeFar(s32 mode, s32 value, s32 fade);
void BattleBackground_LoadFar(s32 mode, s32 resource, s32 level);
void BattleBackground_SetPaletteLevelFar(s32 unused, s32 level);
s32 BattleSprite_HasHeavyImpactFar(s32 index);
s32 BattleUnit_GetEffectHeightFar(s32 id);
void BattleMotion_ApproachTargetFar(s32 unitId, s32 targetId,
                                    s32 travel_divisor, s32 initial_velocity_y);
void BattleMotion_ApplyKnockbackFar(s32 id, s32 variant);
s32 BattleUnit_ListIdsFar(s32 groups, u16 *dst);
/// Returns a unit's battle object, or null for a unit with no place
/// (BattleObject_Get); the battle effects find the sprites they move through
/// it.
struct BattleObject *GetBattleObjectSlotFar(s32 unitId);
s32 Battle_RunFar(s32 enemyGroup);
s32 BattleMotion_ProjectPositionFar(s32 id, s32 *projected);
s32 BattleMotion_ProjectScaledPositionFar(s32 id, s32 *projected);
s32 BattleMotion_ProjectConditionalPositionFar(s32 id, s32 *projected);
s32 BattleParty_PrepareMembersFar(u16 *owners);
void BattleCamera_SetCommandViewFar(s32 pos);
void *GetMotionRecordFar(struct Actor *actor, s32 record_index);
void BattleHighlight_SetModesFar(u16 *actors, s32 mode);
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
