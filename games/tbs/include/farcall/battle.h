/// @file
/// Battle state, unit motion and presentation services.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/BATTLE.S.

/// Far call to DebugBattle_RunMenu.
void DebugBattle_RunMenuFar(void);
/// Far call to BattleActor_SpawnObjectsForList.
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 refresh);
/// Far call to BattlePres_SetupTransitionScene.
void BattlePres_SetupTransitionSceneFar(s32 x, s32 depth, s32 y, s32 zoom);
/// Far call to BattlePres_SetupTransitionAtPairMidpoint.
void BattlePres_SetupTransitionAtPairMidpointFar(s32 a, s32 b, s32 zoom);
/// Far call to BattlePresentation_ConfigurePaletteFade.
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 value, s32 fade);
/// Far call to BattleBackground_Load.
void BattleBackground_LoadFar(s32 mode, s32 resource, s32 level);
/// Far call to BattlePresentation_SetPaletteLevel.
void BattlePresentation_SetPaletteLevelFar(s32 unused, s32 level);
/// Far call to Summon_IsEntrySecondaryFlagged.
s32 Summon_IsEntrySecondaryFlaggedFar(s32 index);
/// Far call to BattleUnit_GetEffectHeight.
s32 BattleUnit_GetEffectHeightFar(s32 id);
/// Far call to BattleMotion_ApproachTarget.
void BattleMotion_ApproachTargetFar(s32 actorId, s32 targetId,
                                    s32 travel_divisor, s32 initial_velocity_y);
/// Far call to BattleMotion_ApplyVariantMotion.
void BattleMotion_ApplyVariantMotionFar(s32 id, s32 variant);
/// Far call to BattleParty_ListActorIds.
s32 BattleParty_ListActorIdsFar(s32 groups, u16 *dst);
/// Far call to BattleObjGet.
struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
/// Far call to Battle_RunEncounter.
s32 Battle_RunEncounterFar(s32 enemyGroup);
/// Far call to BattleMotion_ProjectPosition.
s32 BattleMotion_ProjectPositionFar(s32 id, s32 *projected);
/// Far call to BattleMotion_ProjectScaledPosition.
s32 BattleMotion_ProjectScaledPositionFar(s32 id, s32 *projected);
/// Far call to BattleMotion_ProjectConditionalPosition.
s32 BattleMotion_ProjectConditionalPositionFar(s32 id, s32 *projected);
/// Far call to BattleParty_PrepareActiveOwners.
s32 BattleParty_PrepareActiveOwnersFar(u16 *owners);
/// Far call to Camera_ConfigureScene.
void Camera_ConfigureSceneFar(s32 pos);
/// Far call to GetMotionRecord.
void *GetMotionRecordFar(struct MotionObject *object, s32 record_index);
/// Far call to BattlePres_SetActorModes.
void BattlePres_SetActorModesFar(u16 *actors, s32 mode);
/// Far call to BattleEventRuntime_BeginPhase.
void BattleEventRuntime_BeginPhaseFar(s32 cue);
/// Far call to Graphics_ScaleRgb555Clamped.
s32 Graphics_ScaleRgb555ClampedFar(u16 *source, u16 *destination, s32 scale,
                                   s32 count);
/// Far call to DjinnRecovery_Tick.
s32 DjinnRecovery_TickFar(void);
/// Far call to BattleMotion_GetSlotPalette.
s32 BattleMotion_GetSlotPaletteFar(s32 id);
/// Far call to DjinnRecovery_TickTwenty.
s32 DjinnRecovery_TickTwentyFar(void);
/// Far call to DebugParty_LoadPreset.
s32 DebugParty_LoadPresetFar(s32 preset);
/// Far call to BattleObjPut.
void BattleActor_CommitPlacementFar(void);
/// Far call to BattlePresentation_WaitForPromptAt.
s32 BattlePresentation_WaitForPromptAtFar(s32 x, s32 y);
/// Far call to BattleParty_CountStandbyDjinn.
s32 BattleParty_CountStandbyDjinnFar(u32 unitId, u8 *counts);
