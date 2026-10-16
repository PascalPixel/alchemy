// FarCall_BattleTable: one entry per 8-byte stub, in table order.
FAR_CALL(DebugBattle_RunMenuFar, DebugBattle_RunMenu, void, (void))
FAR_SLOT(BattlePres_RunMosaicFadeOut)
FAR_CALL(BattleActor_SpawnObjectsForListFar, BattleActor_SpawnObjectsForList,
         void, (s16 *list, s32 refresh))
FAR_SLOT(Graphics_FadeAlphaOut)
FAR_SLOT(Graphics_FadeAlphaIn)
FAR_CALL(BattlePres_SetupTransitionSceneFar, BattlePres_SetupTransitionScene,
         void, (s32 x, s32 depth, s32 y, s32 zoom))
FAR_CALL(BattlePres_SetupTransitionAtPairMidpointFar,
         BattlePres_SetupTransitionAtPairMidpoint, void,
         (s32 a, s32 b, s32 zoom))
FAR_CALL(BattlePresentation_ConfigurePaletteFadeFar,
         BattlePresentation_ConfigurePaletteFade, void,
         (s32 mode, s32 value, s32 fade))
FAR_CALL(BattleBackground_LoadFar, BattleBackground_Load, void,
         (s32 mode, s32 resource, s32 level))
FAR_CALL(BattlePresentation_SetPaletteLevelFar,
         BattlePresentation_SetPaletteLevel, void, (s32 unused, s32 level))
FAR_SLOT(Summon_GetEntryByte4)
FAR_CALL(Summon_IsEntrySecondaryFlaggedFar, Summon_IsEntrySecondaryFlagged, s32,
         (s32 index))
FAR_SLOT(Battle_GetSpriteFlagHighBits)
FAR_SLOT(Summon_IsEntryFlagged)
FAR_CALL(Battle_GetObjectTableValueFar, Battle_GetObjectTableValue, s32,
         (s32 id))
FAR_CALL(BattleMotion_ApproachTargetFar, BattleMotion_ApproachTarget, void,
         (s32 actorId, s32 targetId, s32 travel_divisor,
          s32 initial_velocity_y))
FAR_SLOT(BattleMotion_InitializeObject)
FAR_CALL(BattleMotion_ApplyVariantMotionFar, BattleMotion_ApplyVariantMotion,
         void, (s32 id, s32 variant))
FAR_CALL(BattleParty_ListActorIdsFar, BattleParty_ListActorIds, s32,
         (s32 groups, u16 *dst))
FAR_CALL(GetBattleObjectSlotFar, BattleObjGet, struct BattleObj *,
         (s32 objectId))
FAR_CALL(Battle_RunEncounterFar, Battle_RunEncounter, s32, (s32 enemyGroup))
FAR_CALL(BattleMotion_ProjectPositionFar, BattleMotion_ProjectPosition, s32,
         (s32 id, s32 *projected))
FAR_CALL(BattleMotion_ProjectScaledPositionFar,
         BattleMotion_ProjectScaledPosition, s32, (s32 id, s32 *projected))
FAR_CALL(BattleMotion_ProjectConditionalPositionFar,
         BattleMotion_ProjectConditionalPosition, s32, (s32 id, s32 *projected))
FAR_SLOT(BattleParty_ListLivingUnits)
FAR_CALL(BattleParty_PrepareActiveOwnersFar, BattleParty_PrepareActiveOwners,
         s32, (u16 *owners))
FAR_CALL(Camera_ConfigureSceneFar, Camera_ConfigureScene, void, (s32 pos))
FAR_CALL(GetMotionRecordFar, GetMotionRecord, void *,
         (struct MotionObject * object, s32 record_index))
FAR_CALL(BattlePres_SetActorModesFar, BattlePres_SetActorModes, void,
         (u16 *actors, s32 mode))
FAR_CALL(BattleEventRuntime_BeginPhaseFar, BattleEventRuntime_BeginPhase, void,
         (s32 cue))
FAR_CALL(Graphics_ScaleRgb555ClampedFar, Graphics_ScaleRgb555Clamped, s32,
         (u16 *source, u16 *destination, s32 scale, s32 count))
FAR_CALL(BattlePlacement_UpdateTimedEntriesFar,
         BattlePlacement_UpdateTimedEntries, s32, (void))
FAR_CALL(BattleMotion_GetSlotPaletteFar, BattleMotion_GetSlotPalette, s32,
         (s32 id))
FAR_CALL(BattlePlacement_UpdateTimedEntriesTwentyTimesFar,
         BattlePlacement_UpdateTimedEntriesTwentyTimes, s32, (void))
FAR_CALL(DebugParty_LoadPresetFar, DebugParty_LoadPreset, s32, (s32 preset))
FAR_CALL(BattleActor_CommitPlacementFar, BattleObjPut, void, (void))
FAR_SLOT(ReleaseBattleObjectRecords)
FAR_CALL(BattlePresentation_WaitForPromptAtFar,
         BattlePresentation_WaitForPromptAt, s32, (s32 x, s32 y))
FAR_CALL(BattlePlacement_CountValidEntriesFar,
         BattlePlacement_CountValidEntries, s32, (u32 unitId, u8 *counts))
