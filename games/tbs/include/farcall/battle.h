// Battle state, unit motion and presentation services.
// Far-call interfaces; their ordered jump stubs live in
// games/tbs/src/system/far_call/BATTLE.S.
void DebugBattle_RunMenuFar(void);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 refresh);
void BattlePres_SetupTransitionSceneFar(s32 x, s32 depth, s32 y, s32 zoom);
void BattlePres_SetupTransitionAtPairMidpointFar(s32 a, s32 b, s32 zoom);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 value, s32 fade);
void BattleBackground_LoadFar(s32 mode, s32 resource, s32 level);
void BattlePresentation_SetPaletteLevelFar(s32 unused, s32 level);
s32 Summon_IsEntrySecondaryFlaggedFar(s32 index);
s32 Battle_GetObjectTableValueFar(s32 id);
void BattleMotion_ApproachTargetFar(s32 actorId, s32 targetId,
                                    s32 travel_divisor, s32 initial_velocity_y);
void BattleMotion_ApplyVariantMotionFar(s32 id, s32 variant);
s32 BattleParty_ListActorIdsFar(s32 groups, u16 *dst);
struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
s32 Battle_RunEncounterFar(s32 enemyGroup);
s32 BattleMotion_ProjectPositionFar(s32 id, s32 *projected);
s32 BattleMotion_ProjectScaledPositionFar(s32 id, s32 *projected);
s32 BattleMotion_ProjectConditionalPositionFar(s32 id, s32 *projected);
s32 BattleParty_PrepareActiveOwnersFar(u16 *owners);
void Camera_ConfigureSceneFar(s32 pos);
void *GetMotionRecordFar(struct MotionObject *object, s32 record_index);
void BattlePres_SetActorModesFar(u16 *actors, s32 mode);
void BattleEventRuntime_BeginPhaseFar(s32 cue);
s32 Graphics_ScaleRgb555ClampedFar(u16 *source, u16 *destination, s32 scale,
                                   s32 count);
s32 DjinnRecovery_TickFar(void);
s32 BattleMotion_GetSlotPaletteFar(s32 id);
s32 DjinnRecovery_TickTwentyFar(void);
s32 DebugParty_LoadPresetFar(s32 preset);
void BattleActor_CommitPlacementFar(void);
s32 BattlePresentation_WaitForPromptAtFar(s32 x, s32 y);
s32 BattleParty_CountStandbyDjinnFar(u32 unitId, u8 *counts);
