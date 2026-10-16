// FarCall_ObjectTable: one entry per 8-byte stub, in table order.
FAR_CALL(Field_RunSceneFar, Field_RunScene, s32, (s32 entrance))
FAR_CALL(Game_ResetForNewGameFar, Game_ResetForNewGame, void, (s32 mode))
FAR_CALL(Battle_WaitMode0Far, Battle_WaitMode0, void, (s32 frames))
FAR_CALL(Battle_ResetFar, Battle_Reset, void, (void))
FAR_CALL(BattleFx_FinishActionFar, BattleFx_FinishAction, void, (void))
FAR_SLOT(Event_RunObjectHookAndWait)
FAR_CALL(Event_CallWithLastActiveObjectIdFar, Event_CallWithLastActiveObjectId,
         void, (struct ScenePlacement * table))
FAR_CALL(Scene_AssignViewFlagsFar, Scene_AssignViewFlags, void,
         (struct ScenePlacement * entry))
FAR_CALL(Event_SetSceneEventsFar, Event_SetSceneEvents, void,
         (const struct SceneEvent *events))
FAR_CALL(Event_PrepareObjectAndApplyValueFar, Event_PrepareObjectAndApplyValue,
         void, (s32 event_id, s32 value))
FAR_CALL(Event_PrepareTwoObjectsAndApplyFar, Event_PrepareTwoObjectsAndApply,
         void, (s32 first, s32 second))
FAR_CALL(Party_RemoveOwnerRestoredFar, Party_RemoveOwnerRestored, void,
         (s32 owner))
FAR_CALL(PartyInventory_GiveItemFar, PartyInventory_GiveItem, s32,
         (s32 item, s32 unused))
FAR_SLOT(Inventory_TryAddAndReturnOwner)
FAR_CALL(Event_AskYesNoWithActorAnimationFar, Event_AskYesNoWithActorAnimation,
         s32, (s32 actor, s32 force_bottom))
FAR_CALL(Party_LearnAbilityFar, Party_LearnAbility, void,
         (s32 unit, s32 ability))
FAR_CALL(Object_GetByIdFar, Object_GetById, struct FieldActor *, (s32 objectId))
FAR_CALL(ObjectTable_DestroyByIdFar, ObjectTable_DestroyById, void, (s32 index))
FAR_CALL(ObjectMotion_SetSpeedParametersFar, ObjectMotion_SetSpeedParameters,
         void, (u32 objectId, s32 speedLimit, s32 acceleration))
FAR_CALL(ObjectMotion_EnableActionAndSetCallbackFar,
         ObjectMotion_EnableActionAndSetCallback, void,
         (u32 objectId, s32 action))
FAR_CALL(Object_RefreshSelectorByIdFar, Object_RefreshSelectorById, void,
         (u32 objectId))
FAR_CALL(ObjectMotion_EnableActionAndResetMotionFar,
         ObjectMotion_EnableActionAndResetMotion, void, (u32 objectId))
FAR_CALL(Object_SetActionCallbackAndRefreshByIdFar,
         Object_SetActionCallbackAndRefreshById, void,
         (u32 objectId, s32 action))
FAR_CALL(ObjectMotion_ResetAndSetPositionFar, ObjectMotion_ResetAndSetPosition,
         void, (u32 objectId, s32 x, s32 z))
FAR_CALL(ObjectMotion_SetPositionAndCommitFar,
         ObjectMotion_SetPositionAndCommit, void, (u32 objectId, s32 x, s32 z))
FAR_CALL(ObjectMotion_ResetAndSetPositionInMode2Far,
         ObjectMotion_ResetAndSetPositionInMode2, void,
         (u32 objectId, s32 x, s32 z))
FAR_CALL(ObjectMotion_SetPositionAndResetFar, ObjectMotion_SetPositionAndReset,
         void, (u32 objectId, s32 x, s32 z))
FAR_CALL(ObjectMotion_CenterOnColumnFar, ObjectMotion_CenterOnColumn, void,
         (u32 objectId, s32 action, s32 z_offset))
FAR_CALL(ObjectMotion_OffsetPositionAndResetMotionFar,
         ObjectMotion_OffsetPositionAndResetMotion, void,
         (u32 objectId, s32 x_offset, s32 z_offset))
FAR_CALL(ObjectMotion_CommitCurrentPositionAndActivateFar,
         ObjectMotion_CommitCurrentPositionAndActivate, void, (u32 objectId))
FAR_CALL(ObjectMotion_SetHorizontalPositionWithTerrainFar,
         ObjectMotion_SetHorizontalPositionWithTerrain, void,
         (u32 objectId, s32 x, s32 z))
FAR_SLOT(ObjectMotion_SetPositionWithTerrain)
FAR_CALL(Object_SetModeByIdFar, Object_SetModeById, void,
         (u32 objectId, s32 action))
FAR_CALL(Object_SetActionByIdFar, Object_SetActionById, void,
         (u32 objectId, s32 action))
FAR_CALL(Motion_SetModeAndWaitAnimationFar, Motion_SetModeAndWaitAnimation,
         void, (u32 objectId, s32 action))
FAR_CALL(ObjectMotion_WaitForAnimationChangeFar,
         ObjectMotion_WaitForAnimationChange, void, (u32 objectId))
FAR_SLOT(ObjectMotion_NoOp)
FAR_CALL(ObjectMotion_LaunchFar, ObjectMotion_Launch, void,
         (u32 objectId, s32 speed, s32 event_id))
FAR_CALL(ObjectMotion_SetVariantCallbackFar, ObjectMotion_SetVariantCallback,
         void, (u32 objectId, s32 variant))
FAR_CALL(Motion_SetVarCbAndRefreshFar, Motion_SetVarCbAndRefresh, void,
         (u32 objectId, s32 variant))
FAR_CALL(Actor_JumpDownLedgeFar, Actor_JumpDownLedge, void,
         (s32 sequence_arg, s32 mode_or_frame, s32 optional_action))
FAR_CALL(ObjectMotion_SetAngleTowardFar, ObjectMotion_SetAngleToward, void,
         (s32 firstId, s32 second_id, s32 wait))
FAR_CALL(Object_LinkPairFar, Object_LinkPair, void,
         (s32 firstId, s32 second_id, s32 wait))
FAR_CALL(ObjectGroup_ConfigureChildValueFar, ObjectGroup_ConfigureChildValue,
         void, (s32 objectId, s32 value))
FAR_CALL(ObjectGroup_SetChildValueFar, ObjectGroup_SetChildValue, void,
         (struct FieldActor * actor, s32 value))
FAR_CALL(Object_SetTargetAndCallbackFar, Object_SetTargetAndCallback, void,
         (u32 objectId, s32 targetId, const void *callback))
FAR_CALL(Event_SetMessageIdFar, Event_SetMessageId, void, (s32 value))
FAR_CALL(UiText_OpenMessageAtObjectFar, UiText_OpenMessageAtObject,
         struct UiWindow *, (s32 arg, s32 unused))
FAR_CALL(BattleEv_RunWaitFar, BattleEv_RunWait, void, (s32 action, s32 flag))
FAR_CALL(BattlePres_RunActionThenWaitIfModeZeroFar,
         BattlePres_RunActionThenWaitIfModeZero, void,
         (s32 first, s32 second, s32 value))
FAR_CALL(BattleEventRuntime_ProcessActionFar, BattleEventRuntime_ProcessAction,
         s32, (s32 objectId, s32 actionId))
FAR_SLOT(BattleEventRuntime_EmptyCallback)
FAR_SLOT(Event_ShowSpeakerMessage)
FAR_SLOT(ObjectTable_CallRefreshHook)
FAR_SLOT(Event_ShowMessageAtPosition)
FAR_CALL(ObjectMotion_ArmCallbackFar, ObjectMotion_ArmCallback, void,
         (s32 objectId, s32 angle, s32 wait))
FAR_SLOT(Object_ResetTargetAndSetMode1)
FAR_SLOT(ObjectTable_RunIfActive)
FAR_CALL(Battle_ShowPairedUnitWorkAndWaitFar, Battle_ShowPairedUnitWorkAndWait,
         void,
         (s32 first, s32 first_x, s32 first_y, s32 first_arg, s32 first_extra,
          s32 second, s32 second_x, s32 second_y, s32 second_arg,
          s32 second_extra))
FAR_CALL(Ui_SetMessageVoiceFromActorFar, Ui_SetMessageVoiceFromActor, void,
         (s32 arg0))
FAR_CALL(ObjectMotion_SetActionVariantFar, ObjectMotion_SetActionVariant, void,
         (s32 objectId, s32 priority))
FAR_CALL(BattleFx_SpawnLinkedFar, BattleFx_SpawnLinked, void,
         (s32 id, s32 mode, s32 wait))
FAR_CALL(BattleFx_ConfigureLinkedObjectFar, BattleFx_ConfigureLinkedObject,
         void, (s32 id, s32 flags))
FAR_CALL(ObjectVisual_CopyAttributesFar, ObjectVisual_CopyAttributes, void,
         (u32 targetId, u32 sourceId))
FAR_CALL(Object_AttachWorkTargetToObjectFar, Object_AttachWorkTargetToObject,
         void, (s32 id, s32 flag))
FAR_CALL(ObjectTable_AllocateAndSetObjectSpeedFar,
         ObjectTable_AllocateAndSetObjectSpeed, void, (s32 first, s32 second))
FAR_CALL(Motion_CamBoundsFar, Motion_CamBounds, void,
         (s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter))
FAR_CALL(BattleFx_CommitObjectPositionAndWaitFar,
         BattleFx_CommitObjectPositionAndWait, void, (void))
FAR_CALL(Object_PlaceCurrentWithinCameraBoundsFar,
         Object_PlaceCurrentWithinCameraBounds, void, (s32 arg0, s32 arg1))
FAR_CALL(Camera_GetEventObjectFar, Camera_GetEventObject,
         struct FieldActor *, (void))
FAR_CALL(Map_SetViewBoundsFar, Map_SetViewBounds, void,
         (s32 minX, s32 minY, s32 maxX, s32 maxY))
FAR_CALL(Event_SetDestinationFar, Event_SetDestination, void,
         (s32 first, s32 second))
FAR_SLOT(Event_SetSavedDestination)
FAR_CALL(Event_SetExitRequestFar, Event_SetExitRequest, void, (u16 value))
FAR_CALL(BattleFx_SetWeightedResultFar, BattleFx_SetWeightedResult, void,
         (s32 zone_no, s32 slot_no))
FAR_CALL(BattleFx_SetPhaseRequestFar, BattleFx_SetPhaseRequest, void,
         (s32 flag, s32 no))
FAR_CALL(Event_SetNextDestinationFar, Event_SetNextDestination, void,
         (s32 first, s32 second))
FAR_CALL(Event_SetDefeatDestinationFar, Event_SetDefeatDestination, void,
         (s32 first, s32 second))
FAR_CALL(Event_RequestDestinationFar, Event_RequestDestination, void,
         (u16 first, u16 second))
FAR_CALL(Object_GetScreenPositionFar, Object_GetScreenPosition, s32,
         (s32 objectId, s32 *position))
FAR_CALL(Object_GetTriggerTileAheadOfCurrentFar,
         Object_GetTriggerTileAheadOfCurrent, s32, (void))
FAR_CALL(Party_RemapCharacterIdByFlagsFar, Party_RemapCharacterIdByFlags, s32,
         (s32 arg0))
FAR_SLOT(BattleFx_SteerPushedTarget)
FAR_SLOT(FieldFx_StartCloak)
FAR_CALL(Field_TryJumpForwardFar, Field_TryJumpForward, s32, (void))
FAR_CALL(Field_ClimbDownFar, Field_ClimbDown, s32, (void))
FAR_CALL(Field_ClimbUpFar, Field_ClimbUp, s32, (void))
FAR_CALL(Leader_StopMovingFar, Leader_StopMoving, void, (void))
FAR_CALL(FieldFx_StartGroundParticlesFar, FieldFx_StartGroundParticles, void,
         (void))
FAR_CALL(FieldFx_StartLightningFar, FieldFx_StartLightning, void, (void))
FAR_CALL(FieldFx_StopLightningFar, FieldFx_StopLightning, void, (void))
FAR_CALL(FieldFx_LastLightningFar, FieldFx_LastLightning, void, (void))
FAR_CALL(FieldFx_ResumeLightningFar, FieldFx_ResumeLightning, void, (void))
FAR_CALL(BattleFx_StartBufferBlendFar, BattleFx_StartBufferBlend, void,
         (s32 from, s32 to))
FAR_SLOT(FieldMotes_Start)
FAR_CALL(FieldEffect_InitSparklesFar, FieldEffect_InitSparkles, void, (void))
FAR_CALL(DisplayTransition_InitializeBattleEffectStateFar,
         DisplayTransition_InitializeBattleEffectState, void, (s32 mode))
FAR_SLOT(DisplayTransition_InitializeState)
FAR_SLOT(DisplayTransition_Start)
FAR_SLOT(DisplayTransition_Finish)
FAR_SLOT(DisplayState_ClearFlags)
FAR_SLOT(BattleEffect_InitializeBuffers)
FAR_CALL(BattleFx_ApplyColorToTargetBufferFar,
         BattleFx_ApplyColorToTargetBuffer, void, (s32 mode, s32 half))
FAR_CALL(BattleFx_ApplyColorToSourceBufferFar,
         BattleFx_ApplyColorToSourceBuffer, void, (s32 mode, s32 half))
FAR_SLOT(BattleFx_InterpolateBuffers)
FAR_CALL(BattleFx_StartBufferInterpolationFar,
         BattleFx_StartBufferInterpolation, void, (s32 frames))
FAR_SLOT(BattlePalette_StopBlendAndReleaseBuffer)
FAR_SLOT(BattleFx_SetPrimaryBufferValue)
FAR_CALL(Event_StartScreenTransitionFar, Event_StartScreenTransition, void,
         (void))
FAR_CALL(Event_FinishScreenTransitionFar, Event_FinishScreenTransition, void,
         (void))
FAR_CALL(Event_WaitTransitionFramesFar, Event_WaitTransitionFrames, void,
         (void))
FAR_CALL(DisplayScroll_InitializeHBlankDmaFar,
         DisplayScroll_InitializeHBlankDma, void,
         (s32 mode, s32 freqX, s32 stepX, s32 ampX, s32 freqY, s32 stepY,
          s32 ampY))
FAR_CALL(DisplayScroll_DisarmHBlankDmaFar, DisplayScroll_DisarmHBlankDma, void,
         (void))
FAR_CALL(BattleFx_PlayCueAndStartEmitterOnTargetFar,
         BattleFx_PlayCueAndStartEmitterOnTarget, s32,
         (s32 effect, s32 target, s32 mode))
FAR_CALL(EffectRuntime_RunRisingObjectSequenceFar,
         EffectRuntime_RunRisingObjectSequence, void, (void *object, s32 flags))
FAR_CALL(BattleFx_StartEffectObject22Far, BattleFx_StartEffectObject22, void,
         (s32 value, s32 flags))
FAR_SLOT(Marker_GetActor)
FAR_SLOT(Marker_OpenChest)
FAR_SLOT(Marker_CloseChest)
FAR_SLOT(Marker_ShowOpened)
FAR_CALL(Marker_SetActorPositionFar, Marker_SetActorPosition,
         void, (s32 id, s32 x, s32 y))
FAR_SLOT(Marker_Hide)
FAR_CALL(Marker_MoveLeaderAwayFar, Marker_MoveLeaderAway, void,
         (void))
FAR_CALL(Battle_SetObjectFlag5bWhenMode3Far, Battle_SetObjectFlag5bWhenMode3,
         void, (void))
FAR_CALL(Battle_ClearObjectFlag5bWhenMode3Far,
         Battle_ClearObjectFlag5bWhenMode3, void, (void))
FAR_SLOT(ObjectTable_ResetForObject)
FAR_SLOT(Event_SpawnObjectTable)
FAR_SLOT(ObjectTable_DestroyAtIndex)
FAR_CALL(ObjectTable_GetFar, ObjectTable_Get, void *, (u32 index))
FAR_CALL(BattleFx_StartFadeOverlayFar, BattleFx_StartFadeOverlay, void,
         (void *object))
FAR_SLOT(FadeOverlay_Suspend)
FAR_SLOT(FadeOverlay_Resume)
FAR_CALL(Field_DispatchTypeHandlerFar, Field_DispatchTypeHandler, void,
         (s32 kind))
FAR_CALL(BattleFx_LoadActionEffectResourcesFar,
         BattleFx_LoadActionEffectResources, void, (s32 abilityId, s32 mode))
FAR_CALL(BattleFx_SetupObjectPairFar, BattleFx_SetupObjectPair, void,
         (s32 user_id, s32 targetId))
FAR_CALL(EffectRuntime_StopCurrentObjectFar, EffectRuntime_StopCurrentObject,
         void, (void))
FAR_CALL(BattleEffect_CleanupSceneObjectsFar, BattleEffect_CleanupSceneObjects,
         void, (void))
FAR_CALL(EventObject_InitializeFar, EventObject_Initialize, void, (void))
FAR_SLOT(BattleEffect_InitializeSharedScene)
FAR_SLOT(BattleFx_PrepareBufferInterpolation)
FAR_CALL(Battle_ResetEffectCounterFar, Battle_ResetEffectCounter, void, (void))
FAR_CALL(RunSceneTransitionEffectFar, RunSceneTransitionEffect, void,
         (s32 x, s32 y))
FAR_CALL(BattleFx_FinishSceneAndReleaseHeapBlockFar,
         BattleFx_FinishSceneAndReleaseHeapBlock, void, (void))
FAR_SLOT(Ui_FillBank15PaletteGrey)
FAR_CALL(Ui_SetBank15PaletteAndClearRenderModeFar,
         Ui_SetBank15PaletteAndClearRenderMode, void, (void))
FAR_CALL(BattleFx_HasMatchingEvent5Far, BattleFx_HasMatchingEvent5, s32,
         (s32 group))
FAR_CALL(BattleFx_HasTriggerFar, BattleFx_HasTrigger, u32, (u16 item))
FAR_CALL(Object_ReplaceResourceEntryFar, Object_ReplaceResourceEntry, void *,
         (void *src, void *alt))
FAR_CALL(Event_GetSpecialValueFar, Event_GetSpecialValue, s16, (void))
FAR_SLOT(BattleFx_StartItemBreak)
FAR_SLOT(BattleFx_SnapScaleToFull)
FAR_SLOT(UpdateRisingParticleBurst)
FAR_CALL(BattleParty_ApplyDrainFar, BattleParty_ApplyDrain, void,
         (s32 amount, s32 unused))
FAR_SLOT(BattleParty_ApplyHealthDelta)
FAR_SLOT(ResourceTable_CountFreeBlocks)
FAR_CALL(Menu_AskYesNoFar, Menu_AskYesNo, s32, (s32 value))
FAR_SLOT(BattleFx_GetWeightedResult)
FAR_SLOT(BattleFx_GetPhaseResult)
FAR_CALL(Audio_PlayCueFromEventWorkFar, Audio_PlayCueFromEventWork, void,
         (void))
FAR_CALL(Djinn_JoinPartyFar, Djinn_JoinParty, void,
         (s32 slot, s32 element, s32 djinn))
FAR_CALL(EffectSlot_UpdateFar, EffectSlot_Update, void,
         (struct EffectSlot * effect))
FAR_CALL(EffectSlot_HasPendingTargetFar, EffectSlot_HasPendingTarget, u32,
         (struct EffectSlot * effect))
FAR_CALL(EffectSlot_SetPositionFar, EffectSlot_SetPosition, void,
         (struct EffectSlot * effect, s32 x, s32 z))
FAR_CALL(EffectSlot_SetObjectModeFar, EffectSlot_SetObjectMode, void,
         (struct EffectSlot * effect, s32 mode))
FAR_CALL(EffectSlot_SetCallbackFar, EffectSlot_SetCallback, void,
         (struct EffectSlot * effect, EffectCallback callback))
FAR_CALL(EffectSlot_InitializeFar, EffectSlot_Initialize, void,
         (struct EffectSlot * effect, s32 kind, s32 x, s32 z))
FAR_CALL(BattleFx_ClearOwnedSlotFar, BattleFx_ClearOwnedSlot, void,
         (struct EffectSlot * slot))
FAR_CALL(Camera_WorldToScreenFar, Camera_WorldToScreen, void,
         (struct EffectVector * position))
FAR_CALL(BattleFx_GetResourceIdFar, BattleFx_GetResourceId, s32, (u32 id))
FAR_CALL(Event_ClearInvalidPackedValuesFar, Event_ClearInvalidPackedValues,
         void, (void))
FAR_CALL(BattleFx_InitializeSlotsFar, BattleFx_InitializeSlots, void, (void))
FAR_CALL(BattleFx_ClearActiveSlotsAndScheduleUpdatesFar,
         BattleFx_ClearActiveSlotsAndScheduleUpdates, void, (void))
FAR_SLOT(BattleEffect_ClearOutOfBoundsObjects)
FAR_SLOT(BattleEffect_ClearAllObjects)
FAR_CALL(Motion_LaunchFromFocusedObjectFar, Motion_LaunchFromFocusedObject,
         void, (u32 arg0, s32 arg1, s32 arg2, s32 arg3))
FAR_CALL(ObjectMotion_OffsetPositionAndResetFar,
         ObjectMotion_OffsetPositionAndReset, void,
         (u32 objectId, s32 x_offset, s32 z_offset))
FAR_CALL(ObjectMotion_CommitPositionAndActivateFar,
         ObjectMotion_CommitPositionAndActivate, void,
         (u32 objectId, s32 x_offset, s32 z_offset))
FAR_CALL(Object_LinkObjectAndSetCallbackFar, Object_LinkObjectAndSetCallback,
         void, (u32 objectId, u32 linked_object_id))
FAR_CALL(BattleEffect_PauseObjectFar, BattleEffect_PauseObject, void, (s32 id))
FAR_SLOT(BattleFx_ResumeObject)
FAR_SLOT(ObjectTable_FindActiveByValue)
FAR_SLOT(ObjectTable_ReadActiveValue)
FAR_CALL(SaveState_CaptureObjectTableFar, ObjectTable_Snapshot, void, (void))
FAR_CALL(ObjectTable_RestoreFar, ObjectTable_Restore, void, (void))
FAR_CALL(BattleFx_ScheduleRatioTransitionFar, BattleFx_ScheduleRatioTransition,
         void, (s32 zoom, s32 frames))
FAR_CALL(Event_WaitForZoomTransitionFar, Event_WaitForZoomTransition, void,
         (void))
FAR_CALL(Scene_GetAreaNameIndexFar, Scene_GetAreaNameIndex, s32,
         (s32 scene, s32 entrance))
FAR_CALL(ObjectMotion_SetActionCallbackFar, ObjectMotion_SetActionCallback,
         void, (struct ObjectRuntime * object, s32 kind))
FAR_CALL(BattleFx_SetQueuedSoundAndPlayFar, BattleFx_SetQueuedSoundAndPlay,
         void, (s32 sound_id))
FAR_CALL(BattleFx_PlayQueuedSoundFar, BattleFx_PlayQueuedSound, void, (void))
FAR_CALL(Party_RemapCharacterIdIfEnabledFar, Party_RemapCharacterIdIfEnabled,
         s32, (s32 arg0, s32 arg1))
#if TBS_PATCH_2
FAR_CALL(BattleFx_ClearRandomParticlesFar, BattleFx_ClearRandomParticles, void,
         (void))
#endif
