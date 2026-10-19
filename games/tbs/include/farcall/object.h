/// @file
/// Scene events, field Psynergy, objects and effect lifecycles.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/OBJECT.S.

/// Far call to Field_RunScene.
s32 Field_RunSceneFar(s32 entrance);
/// Far call to Game_ResetForNewGame.
void Game_ResetForNewGameFar(s32 mode);
/// Far call to FieldEvent_WaitFrames.
void FieldEvent_WaitFramesFar(s32 frames);
/// Far call to FieldEvent_Begin.
void FieldEvent_BeginFar(void);
/// Far call to FieldEvent_End.
void FieldEvent_EndFar(void);
/// Far call to Event_CallWithLastActiveObjectId.
void Event_CallWithLastActiveObjectIdFar(struct ScenePlacement *table);
/// Far call to Scene_AssignViewFlags.
void Scene_AssignViewFlagsFar(struct ScenePlacement *entry);
/// Far call to Event_SetSceneEvents.
void Event_SetSceneEventsFar(const struct SceneEvent *events);
/// Far call to Event_PrepareObjectAndApplyValue.
void Event_PrepareObjectAndApplyValueFar(s32 event_id, s32 value);
/// Far call to Event_PrepareTwoObjectsAndApply.
void Event_PrepareTwoObjectsAndApplyFar(s32 first, s32 second);
/// Far call to Party_RemoveOwnerRestored.
void Party_RemoveOwnerRestoredFar(s32 owner);
/// Far call to PartyInventory_GiveItem.
s32 PartyInventory_GiveItemFar(s32 item, s32 unused);
/// Far call to Event_AskYesNoWithActorAnimation.
s32 Event_AskYesNoWithActorAnimationFar(s32 actor, s32 force_bottom);
/// Far call to Party_LearnAbility.
void Party_LearnAbilityFar(s32 unit, s32 ability);
/// Far call to Object_GetById.
struct FieldActor *Object_GetByIdFar(s32 objectId);
/// Far call to ObjectTable_DestroyById.
void ObjectTable_DestroyByIdFar(s32 index);
/// Far call to ObjectMotion_SetSpeedParameters.
void ObjectMotion_SetSpeedParametersFar(u32 objectId, s32 speedLimit,
                                        s32 acceleration);
/// Far call to ObjectMotion_EnableActionAndSetCallback.
void ObjectMotion_EnableActionAndSetCallbackFar(u32 objectId, s32 action);
/// Far call to Object_RefreshSelectorById.
void Object_RefreshSelectorByIdFar(u32 objectId);
/// Far call to ObjectMotion_EnableActionAndResetMotion.
void ObjectMotion_EnableActionAndResetMotionFar(u32 objectId);
/// Far call to Object_SetActionCallbackAndRefreshById.
void Object_SetActionCallbackAndRefreshByIdFar(u32 objectId, s32 action);
/// Far call to ObjectMotion_ResetAndSetPosition.
void ObjectMotion_ResetAndSetPositionFar(u32 objectId, s32 x, s32 z);
/// Far call to ObjectMotion_SetPositionAndCommit.
void ObjectMotion_SetPositionAndCommitFar(u32 objectId, s32 x, s32 z);
/// Far call to ObjectMotion_ResetAndSetPositionInMode2.
void ObjectMotion_ResetAndSetPositionInMode2Far(u32 objectId, s32 x, s32 z);
/// Far call to ObjectMotion_SetPositionAndReset.
void ObjectMotion_SetPositionAndResetFar(u32 objectId, s32 x, s32 z);
/// Far call to ObjectMotion_CenterOnColumn.
void ObjectMotion_CenterOnColumnFar(u32 objectId, s32 action, s32 z_offset);
/// Far call to ObjectMotion_OffsetPositionAndResetMotion.
void ObjectMotion_OffsetPositionAndResetMotionFar(u32 objectId, s32 x_offset,
                                                  s32 z_offset);
/// Far call to ObjectMotion_CommitCurrentPositionAndActivate.
void ObjectMotion_CommitCurrentPositionAndActivateFar(u32 objectId);
/// Far call to ObjectMotion_SetHorizontalPositionWithTerrain.
void ObjectMotion_SetHorizontalPositionWithTerrainFar(u32 objectId, s32 x,
                                                      s32 z);
/// Far call to Object_SetModeById.
void Object_SetModeByIdFar(u32 objectId, s32 action);
/// Far call to Object_SetActionById.
void Object_SetActionByIdFar(u32 objectId, s32 action);
/// Far call to Motion_SetModeAndWaitAnimation.
void Motion_SetModeAndWaitAnimationFar(u32 objectId, s32 action);
/// Far call to ObjectMotion_WaitForAnimationChange.
void ObjectMotion_WaitForAnimationChangeFar(u32 objectId);
/// Far call to ObjectMotion_Launch.
void ObjectMotion_LaunchFar(u32 objectId, s32 speed, s32 event_id);
/// Far call to ObjectMotion_SetVariantCallback.
void ObjectMotion_SetVariantCallbackFar(u32 objectId, s32 variant);
/// Far call to Motion_SetVarCbAndRefresh.
void Motion_SetVarCbAndRefreshFar(u32 objectId, s32 variant);
/// Far call to Actor_JumpDownLedge.
void Actor_JumpDownLedgeFar(s32 sequence_arg, s32 mode_or_frame,
                            s32 optional_action);
/// Far call to ObjectMotion_SetAngleToward.
void ObjectMotion_SetAngleTowardFar(s32 firstId, s32 second_id, s32 wait);
/// Far call to Object_LinkPair.
void Object_LinkPairFar(s32 firstId, s32 second_id, s32 wait);
/// Far call to ObjectGroup_ConfigureChildValue.
void ObjectGroup_ConfigureChildValueFar(s32 objectId, s32 value);
/// Far call to ObjectGroup_SetChildValue.
void ObjectGroup_SetChildValueFar(struct FieldActor *actor, s32 value);
/// Far call to Object_SetTargetAndCallback.
void Object_SetTargetAndCallbackFar(u32 objectId, s32 targetId,
                                    const void *callback);
/// Far call to Event_SetMessageId.
void Event_SetMessageIdFar(s32 value);
/// Far call to UiText_OpenMessageAtObject.
struct UiWindow *UiText_OpenMessageAtObjectFar(s32 arg, s32 unused);
/// Far call to FieldEvent_ShowMessageAndWait.
void FieldEvent_ShowMessageAndWaitFar(s32 action, s32 flag);
/// Far call to BattlePres_RunActionThenWaitIfModeZero.
void BattlePres_RunActionThenWaitIfModeZeroFar(s32 first, s32 second,
                                               s32 value);
/// Far call to FieldEvent_AskYesNoAndShowResponse.
s32 FieldEvent_AskYesNoAndShowResponseFar(s32 objectId, s32 actionId);
/// Far call to ObjectMotion_ArmCallback.
void ObjectMotion_ArmCallbackFar(s32 objectId, s32 angle, s32 wait);
/// Far call to Battle_ShowPairedUnitWorkAndWait.
void Battle_ShowPairedUnitWorkAndWaitFar(s32 first, s32 first_x, s32 first_y,
                                         s32 first_arg, s32 first_extra,
                                         s32 second, s32 second_x, s32 second_y,
                                         s32 second_arg, s32 second_extra);
/// Far call to Ui_SetMessageVoiceFromActor.
void Ui_SetMessageVoiceFromActorFar(s32 arg0);
/// Far call to ObjectMotion_SetActionVariant.
void ObjectMotion_SetActionVariantFar(s32 objectId, s32 priority);
/// Far call to BattleFx_SpawnLinked.
void BattleFx_SpawnLinkedFar(s32 id, s32 mode, s32 wait);
/// Far call to BattleFx_ConfigureLinkedObject.
void BattleFx_ConfigureLinkedObjectFar(s32 id, s32 flags);
/// Far call to ObjectVisual_CopyAttributes.
void ObjectVisual_CopyAttributesFar(u32 targetId, u32 sourceId);
/// Far call to Camera_FollowActor.
void Camera_FollowActorFar(s32 id, s32 flag);
/// Far call to Camera_SetSpeedParameters.
void Camera_SetSpeedParametersFar(s32 first, s32 second);
/// Far call to Camera_MoveWithinMapBounds.
void Camera_MoveWithinMapBoundsFar(s32 requested_x, s32 requested_y,
                                   s32 requested_z, s32 use_setter);
/// Far call to Camera_WaitForArrival.
void Camera_WaitForArrivalFar(void);
/// Far call to Object_PlaceCurrentWithinCameraBounds.
void Object_PlaceCurrentWithinCameraBoundsFar(s32 arg0, s32 arg1);
/// Far call to Camera_GetEventObject.
struct FieldActor *Camera_GetEventObjectFar(void);
/// Far call to Map_SetViewBounds.
void Map_SetViewBoundsFar(s32 minX, s32 minY, s32 maxX, s32 maxY);
/// Far call to Event_SetDestination.
void Event_SetDestinationFar(s32 first, s32 second);
/// Far call to Event_SetExitRequest.
void Event_SetExitRequestFar(u16 value);
/// Far call to Field_RequestBattleFromEncounterTable.
void Field_RequestBattleFromEncounterTableFar(s32 zone_no, s32 slot_no);
/// Far call to Field_RequestDjinnBattle.
void Field_RequestDjinnBattleFar(s32 flag, s32 no);
/// Far call to Event_SetNextDestination.
void Event_SetNextDestinationFar(s32 first, s32 second);
/// Far call to Event_SetDefeatDestination.
void Event_SetDefeatDestinationFar(s32 first, s32 second);
/// Far call to Event_RequestDestination.
void Event_RequestDestinationFar(u16 first, u16 second);
/// Far call to Object_GetScreenPosition.
s32 Object_GetScreenPositionFar(s32 objectId, s32 *position);
/// Far call to Object_GetTriggerTileAheadOfCurrent.
s32 Object_GetTriggerTileAheadOfCurrentFar(void);
/// Far call to Party_RemapCharacterIdByFlags.
s32 Party_RemapCharacterIdByFlagsFar(s32 arg0);
/// Far call to Field_TryJumpForward.
s32 Field_TryJumpForwardFar(void);
/// Far call to Field_ClimbDown.
s32 Field_ClimbDownFar(void);
/// Far call to Field_ClimbUp.
s32 Field_ClimbUpFar(void);
/// Far call to Leader_StopMoving.
void Leader_StopMovingFar(void);
/// Far call to FieldFx_StartGroundParticles.
void FieldFx_StartGroundParticlesFar(void);
/// Far call to FieldFx_StartLightning.
void FieldFx_StartLightningFar(void);
/// Far call to FieldFx_StopLightning.
void FieldFx_StopLightningFar(void);
/// Far call to FieldFx_LastLightning.
void FieldFx_LastLightningFar(void);
/// Far call to FieldFx_ResumeLightning.
void FieldFx_ResumeLightningFar(void);
/// Far call to BattleFx_StartBufferBlend.
void BattleFx_StartBufferBlendFar(s32 from, s32 to);
/// Far call to FieldEffect_InitSparkles.
void FieldEffect_InitSparklesFar(void);
/// Far call to DisplayTransition_InitializeBattleEffectState.
void DisplayTransition_InitializeBattleEffectStateFar(s32 mode);
/// Far call to BattleFx_ApplyColorToTargetBuffer.
void BattleFx_ApplyColorToTargetBufferFar(s32 mode, s32 half);
/// Far call to BattleFx_ApplyColorToSourceBuffer.
void BattleFx_ApplyColorToSourceBufferFar(s32 mode, s32 half);
/// Far call to BattleFx_StartBufferInterpolation.
void BattleFx_StartBufferInterpolationFar(s32 frames);
/// Far call to Event_StartScreenTransition.
void Event_StartScreenTransitionFar(void);
/// Far call to Event_FinishScreenTransition.
void Event_FinishScreenTransitionFar(void);
/// Far call to Event_WaitTransitionFrames.
void Event_WaitTransitionFramesFar(void);
/// Far call to DisplayScroll_InitializeHBlankDma.
void DisplayScroll_InitializeHBlankDmaFar(s32 mode, s32 freqX, s32 stepX,
                                          s32 ampX, s32 freqY, s32 stepY,
                                          s32 ampY);
/// Far call to DisplayScroll_DisarmHBlankDma.
void DisplayScroll_DisarmHBlankDmaFar(void);
/// Far call to BattleFx_PlayCueAndStartEmitterOnTarget.
s32 BattleFx_PlayCueAndStartEmitterOnTargetFar(s32 effect, s32 target,
                                               s32 mode);
/// Far call to EffectRuntime_RunRisingObjectSequence.
void EffectRuntime_RunRisingObjectSequenceFar(void *object, s32 flags);
/// Far call to BattleFx_StartEffectObject22.
void BattleFx_StartEffectObject22Far(s32 value, s32 flags);
/// Far call to Marker_SetActorPosition.
void Marker_SetActorPositionFar(s32 id, s32 x, s32 y);
/// Far call to Marker_MoveLeaderAway.
void Marker_MoveLeaderAwayFar(void);
/// Far call to Battle_SetObjectFlag5bWhenMode3.
void Battle_SetObjectFlag5bWhenMode3Far(void);
/// Far call to Battle_ClearObjectFlag5bWhenMode3.
void Battle_ClearObjectFlag5bWhenMode3Far(void);
/// Far call to ObjectTable_Get.
void *ObjectTable_GetFar(u32 index);
/// Far call to BattleFx_StartFadeOverlay.
void BattleFx_StartFadeOverlayFar(void *object);
/// Far call to Field_DispatchTypeHandler.
void Field_DispatchTypeHandlerFar(s32 kind);
/// Far call to BattleFx_LoadActionEffectResources.
void BattleFx_LoadActionEffectResourcesFar(s32 abilityId, s32 mode);
/// Far call to BattleFx_SetupObjectPair.
void BattleFx_SetupObjectPairFar(s32 user_id, s32 targetId);
/// Far call to EffectRuntime_StopCurrentObject.
void EffectRuntime_StopCurrentObjectFar(void);
/// Far call to BattleEffect_CleanupSceneObjects.
void BattleEffect_CleanupSceneObjectsFar(void);
/// Far call to EventObject_Initialize.
void EventObject_InitializeFar(void);
/// Far call to Battle_ResetEffectCounter.
void Battle_ResetEffectCounterFar(void);
/// Far call to RunSceneTransitionEffect.
void RunSceneTransitionEffectFar(s32 x, s32 y);
/// Far call to BattleFx_FinishSceneAndReleaseHeapBlock.
void BattleFx_FinishSceneAndReleaseHeapBlockFar(void);
/// Far call to Ui_SetBank15PaletteAndClearRenderMode.
void Ui_SetBank15PaletteAndClearRenderModeFar(void);
/// Far call to BattleFx_HasMatchingEvent5.
s32 BattleFx_HasMatchingEvent5Far(s32 group);
/// Far call to BattleFx_HasTrigger.
u32 BattleFx_HasTriggerFar(u16 item);
struct AnimationObject *
/// Far call to Sprite_ShareVramCache.
Sprite_ShareVramCacheFar(struct AnimationObject *sourceSprite,
                         struct AnimationObject *sharedSprite);
/// Far call to Event_GetSpecialValue.
s16 Event_GetSpecialValueFar(void);
/// Far call to BattleParty_ApplyDrain.
void BattleParty_ApplyDrainFar(s32 amount, s32 unused);
/// Far call to Menu_AskYesNo.
s32 Menu_AskYesNoFar(s32 value);
/// Far call to Audio_PlayCueFromEventWork.
void Audio_PlayCueFromEventWorkFar(void);
/// Far call to Djinn_JoinParty.
void Djinn_JoinPartyFar(s32 slot, s32 element, s32 djinn);
/// Far call to EffectSlot_Update.
void EffectSlot_UpdateFar(struct EffectSlot *effect);
/// Far call to EffectSlot_HasPendingTarget.
u32 EffectSlot_HasPendingTargetFar(struct EffectSlot *effect);
/// Far call to EffectSlot_SetPosition.
void EffectSlot_SetPositionFar(struct EffectSlot *effect, s32 x, s32 z);
/// Far call to EffectSlot_SetObjectMode.
void EffectSlot_SetObjectModeFar(struct EffectSlot *effect, s32 mode);
/// Far call to EffectSlot_SetCallback.
void EffectSlot_SetCallbackFar(struct EffectSlot *effect,
                               EffectCallback callback);
/// Far call to EffectSlot_Initialize.
void EffectSlot_InitializeFar(struct EffectSlot *effect, s32 kind, s32 x,
                              s32 z);
/// Far call to BattleFx_ClearOwnedSlot.
void BattleFx_ClearOwnedSlotFar(struct EffectSlot *slot);
/// Far call to Camera_WorldToScreen.
void Camera_WorldToScreenFar(struct EffectVector *position);
/// Far call to BattleFx_GetResourceId.
s32 BattleFx_GetResourceIdFar(u32 id);
/// Far call to Event_ClearInvalidPackedValues.
void Event_ClearInvalidPackedValuesFar(void);
/// Far call to BattleFx_InitializeSlots.
void BattleFx_InitializeSlotsFar(void);
/// Far call to BattleFx_ClearActiveSlotsAndScheduleUpdates.
void BattleFx_ClearActiveSlotsAndScheduleUpdatesFar(void);
/// Far call to Motion_LaunchFromFocusedObject.
void Motion_LaunchFromFocusedObjectFar(u32 arg0, s32 arg1, s32 arg2, s32 arg3);
/// Far call to ObjectMotion_OffsetPositionAndReset.
void ObjectMotion_OffsetPositionAndResetFar(u32 objectId, s32 x_offset,
                                            s32 z_offset);
/// Far call to ObjectMotion_CommitPositionAndActivate.
void ObjectMotion_CommitPositionAndActivateFar(u32 objectId, s32 x_offset,
                                               s32 z_offset);
/// Far call to Object_LinkObjectAndSetCallback.
void Object_LinkObjectAndSetCallbackFar(u32 objectId, u32 linked_object_id);
/// Far call to BattleEffect_PauseObject.
void BattleEffect_PauseObjectFar(s32 id);
/// Far call to ObjectTable_Snapshot.
void SaveState_CaptureObjectTableFar(void);
/// Far call to ObjectTable_Restore.
void ObjectTable_RestoreFar(void);
/// Far call to BattleFx_ScheduleRatioTransition.
void BattleFx_ScheduleRatioTransitionFar(s32 zoom, s32 frames);
/// Far call to Event_WaitForZoomTransition.
void Event_WaitForZoomTransitionFar(void);
/// Far call to Scene_GetAreaNameIndex.
s32 Scene_GetAreaNameIndexFar(s32 scene, s32 entrance);
/// Far call to ObjectMotion_SetActionCallback.
void ObjectMotion_SetActionCallbackFar(struct ObjectRuntime *object, s32 kind);
/// Far call to BattleFx_SetQueuedSoundAndPlay.
void BattleFx_SetQueuedSoundAndPlayFar(s32 sound_id);
/// Far call to BattleFx_PlayQueuedSound.
void BattleFx_PlayQueuedSoundFar(void);
/// Far call to Party_RemapCharacterIdIfEnabled.
s32 Party_RemapCharacterIdIfEnabledFar(s32 arg0, s32 arg1);
#if TBS_PATCH_2
/// Far call to BattleFx_ClearRandomParticles.
void BattleFx_ClearRandomParticlesFar(void);
#endif
