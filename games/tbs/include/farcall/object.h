// Scene events, field Psynergy, objects and effect lifecycles.
// Far-call interfaces; their ordered jump stubs live in
// games/tbs/src/system/far_call/OBJECT.S.
s32 Field_RunSceneFar(s32 entrance);
void Game_ResetForNewGameFar(s32 mode);
void FieldEvent_WaitFramesFar(s32 frames);
void FieldEvent_BeginFar(void);
void FieldEvent_EndFar(void);
void Event_CallWithLastActiveObjectIdFar(struct ScenePlacement *table);
void Scene_AssignViewFlagsFar(struct ScenePlacement *entry);
void Event_SetSceneEventsFar(const struct SceneEvent *events);
void Event_PrepareObjectAndApplyValueFar(s32 event_id, s32 value);
void Event_PrepareTwoObjectsAndApplyFar(s32 first, s32 second);
void Party_RemoveOwnerRestoredFar(s32 owner);
s32 PartyInventory_GiveItemFar(s32 item, s32 unused);
s32 Event_AskYesNoWithActorAnimationFar(s32 actor, s32 force_bottom);
void Party_LearnAbilityFar(s32 unit, s32 ability);
struct FieldActor *Object_GetByIdFar(s32 objectId);
void ObjectTable_DestroyByIdFar(s32 index);
void ObjectMotion_SetSpeedParametersFar(u32 objectId, s32 speedLimit,
                                        s32 acceleration);
void ObjectMotion_EnableActionAndSetCallbackFar(u32 objectId, s32 action);
void Object_RefreshSelectorByIdFar(u32 objectId);
void ObjectMotion_EnableActionAndResetMotionFar(u32 objectId);
void Object_SetActionCallbackAndRefreshByIdFar(u32 objectId, s32 action);
void ObjectMotion_ResetAndSetPositionFar(u32 objectId, s32 x, s32 z);
void ObjectMotion_SetPositionAndCommitFar(u32 objectId, s32 x, s32 z);
void ObjectMotion_ResetAndSetPositionInMode2Far(u32 objectId, s32 x, s32 z);
void ObjectMotion_SetPositionAndResetFar(u32 objectId, s32 x, s32 z);
void ObjectMotion_CenterOnColumnFar(u32 objectId, s32 action, s32 z_offset);
void ObjectMotion_OffsetPositionAndResetMotionFar(u32 objectId, s32 x_offset,
                                                  s32 z_offset);
void ObjectMotion_CommitCurrentPositionAndActivateFar(u32 objectId);
void ObjectMotion_SetHorizontalPositionWithTerrainFar(u32 objectId, s32 x,
                                                      s32 z);
void Object_SetModeByIdFar(u32 objectId, s32 action);
void Object_SetActionByIdFar(u32 objectId, s32 action);
void Motion_SetModeAndWaitAnimationFar(u32 objectId, s32 action);
void ObjectMotion_WaitForAnimationChangeFar(u32 objectId);
void ObjectMotion_LaunchFar(u32 objectId, s32 speed, s32 event_id);
void ObjectMotion_SetVariantCallbackFar(u32 objectId, s32 variant);
void Motion_SetVarCbAndRefreshFar(u32 objectId, s32 variant);
void Actor_JumpDownLedgeFar(s32 sequence_arg, s32 mode_or_frame,
                            s32 optional_action);
void ObjectMotion_SetAngleTowardFar(s32 firstId, s32 second_id, s32 wait);
void Object_LinkPairFar(s32 firstId, s32 second_id, s32 wait);
void ObjectGroup_ConfigureChildValueFar(s32 objectId, s32 value);
void ObjectGroup_SetChildValueFar(struct FieldActor *actor, s32 value);
void Object_SetTargetAndCallbackFar(u32 objectId, s32 targetId,
                                    const void *callback);
void Event_SetMessageIdFar(s32 value);
struct UiWindow *UiText_OpenMessageAtObjectFar(s32 arg, s32 unused);
void FieldEvent_ShowMessageAndWaitFar(s32 action, s32 flag);
void BattlePres_RunActionThenWaitIfModeZeroFar(s32 first, s32 second,
                                               s32 value);
s32 FieldEvent_AskYesNoAndShowResponseFar(s32 objectId, s32 actionId);
void ObjectMotion_ArmCallbackFar(s32 objectId, s32 angle, s32 wait);
void Battle_ShowPairedUnitWorkAndWaitFar(s32 first, s32 first_x, s32 first_y,
                                         s32 first_arg, s32 first_extra,
                                         s32 second, s32 second_x, s32 second_y,
                                         s32 second_arg, s32 second_extra);
void Ui_SetMessageVoiceFromActorFar(s32 arg0);
void ObjectMotion_SetActionVariantFar(s32 objectId, s32 priority);
void BattleFx_SpawnLinkedFar(s32 id, s32 mode, s32 wait);
void BattleFx_ConfigureLinkedObjectFar(s32 id, s32 flags);
void ObjectVisual_CopyAttributesFar(u32 targetId, u32 sourceId);
void Camera_FollowActorFar(s32 id, s32 flag);
void Camera_SetSpeedParametersFar(s32 first, s32 second);
void Camera_MoveWithinMapBoundsFar(s32 requested_x, s32 requested_y,
                                   s32 requested_z, s32 use_setter);
void Camera_WaitForArrivalFar(void);
void Object_PlaceCurrentWithinCameraBoundsFar(s32 arg0, s32 arg1);
struct FieldActor *Camera_GetEventObjectFar(void);
void Map_SetViewBoundsFar(s32 minX, s32 minY, s32 maxX, s32 maxY);
void Event_SetDestinationFar(s32 first, s32 second);
void Event_SetExitRequestFar(u16 value);
void Field_RequestBattleFromEncounterTableFar(s32 zone_no, s32 slot_no);
void Field_RequestDjinnBattleFar(s32 flag, s32 no);
void Event_SetNextDestinationFar(s32 first, s32 second);
void Event_SetDefeatDestinationFar(s32 first, s32 second);
void Event_RequestDestinationFar(u16 first, u16 second);
s32 Object_GetScreenPositionFar(s32 objectId, s32 *position);
s32 Object_GetTriggerTileAheadOfCurrentFar(void);
s32 Party_RemapCharacterIdByFlagsFar(s32 arg0);
s32 Field_TryJumpForwardFar(void);
s32 Field_ClimbDownFar(void);
s32 Field_ClimbUpFar(void);
void Leader_StopMovingFar(void);
void FieldFx_StartGroundParticlesFar(void);
void FieldFx_StartLightningFar(void);
void FieldFx_StopLightningFar(void);
void FieldFx_LastLightningFar(void);
void FieldFx_ResumeLightningFar(void);
void BattleFx_StartBufferBlendFar(s32 from, s32 to);
void FieldEffect_InitSparklesFar(void);
void DisplayTransition_InitializeBattleEffectStateFar(s32 mode);
void BattleFx_ApplyColorToTargetBufferFar(s32 mode, s32 half);
void BattleFx_ApplyColorToSourceBufferFar(s32 mode, s32 half);
void BattleFx_StartBufferInterpolationFar(s32 frames);
void Event_StartScreenTransitionFar(void);
void Event_FinishScreenTransitionFar(void);
void Event_WaitTransitionFramesFar(void);
void DisplayScroll_InitializeHBlankDmaFar(s32 mode, s32 freqX, s32 stepX,
                                          s32 ampX, s32 freqY, s32 stepY,
                                          s32 ampY);
void DisplayScroll_DisarmHBlankDmaFar(void);
s32 BattleFx_PlayCueAndStartEmitterOnTargetFar(s32 effect, s32 target,
                                               s32 mode);
void EffectRuntime_RunRisingObjectSequenceFar(void *object, s32 flags);
void BattleFx_StartEffectObject22Far(s32 value, s32 flags);
void Marker_SetActorPositionFar(s32 id, s32 x, s32 y);
void Marker_MoveLeaderAwayFar(void);
void Battle_SetObjectFlag5bWhenMode3Far(void);
void Battle_ClearObjectFlag5bWhenMode3Far(void);
void *ObjectTable_GetFar(u32 index);
void BattleFx_StartFadeOverlayFar(void *object);
void Field_DispatchTypeHandlerFar(s32 kind);
void BattleFx_LoadActionEffectResourcesFar(s32 abilityId, s32 mode);
void BattleFx_SetupObjectPairFar(s32 user_id, s32 targetId);
void EffectRuntime_StopCurrentObjectFar(void);
void BattleEffect_CleanupSceneObjectsFar(void);
void EventObject_InitializeFar(void);
void Battle_ResetEffectCounterFar(void);
void RunSceneTransitionEffectFar(s32 x, s32 y);
void BattleFx_FinishSceneAndReleaseHeapBlockFar(void);
void Ui_SetBank15PaletteAndClearRenderModeFar(void);
s32 BattleFx_HasMatchingEvent5Far(s32 group);
u32 BattleFx_HasTriggerFar(u16 item);
void *Object_ReplaceResourceEntryFar(void *src, void *alt);
s16 Event_GetSpecialValueFar(void);
void BattleParty_ApplyDrainFar(s32 amount, s32 unused);
s32 Menu_AskYesNoFar(s32 value);
void Audio_PlayCueFromEventWorkFar(void);
void Djinn_JoinPartyFar(s32 slot, s32 element, s32 djinn);
void EffectSlot_UpdateFar(struct EffectSlot *effect);
u32 EffectSlot_HasPendingTargetFar(struct EffectSlot *effect);
void EffectSlot_SetPositionFar(struct EffectSlot *effect, s32 x, s32 z);
void EffectSlot_SetObjectModeFar(struct EffectSlot *effect, s32 mode);
void EffectSlot_SetCallbackFar(struct EffectSlot *effect,
                               EffectCallback callback);
void EffectSlot_InitializeFar(struct EffectSlot *effect, s32 kind, s32 x,
                              s32 z);
void BattleFx_ClearOwnedSlotFar(struct EffectSlot *slot);
void Camera_WorldToScreenFar(struct EffectVector *position);
s32 BattleFx_GetResourceIdFar(u32 id);
void Event_ClearInvalidPackedValuesFar(void);
void BattleFx_InitializeSlotsFar(void);
void BattleFx_ClearActiveSlotsAndScheduleUpdatesFar(void);
void Motion_LaunchFromFocusedObjectFar(u32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ObjectMotion_OffsetPositionAndResetFar(u32 objectId, s32 x_offset,
                                            s32 z_offset);
void ObjectMotion_CommitPositionAndActivateFar(u32 objectId, s32 x_offset,
                                               s32 z_offset);
void Object_LinkObjectAndSetCallbackFar(u32 objectId, u32 linked_object_id);
void BattleEffect_PauseObjectFar(s32 id);
void SaveState_CaptureObjectTableFar(void);
void ObjectTable_RestoreFar(void);
void BattleFx_ScheduleRatioTransitionFar(s32 zoom, s32 frames);
void Event_WaitForZoomTransitionFar(void);
s32 Scene_GetAreaNameIndexFar(s32 scene, s32 entrance);
void ObjectMotion_SetActionCallbackFar(struct ObjectRuntime *object, s32 kind);
void BattleFx_SetQueuedSoundAndPlayFar(s32 sound_id);
void BattleFx_PlayQueuedSoundFar(void);
s32 Party_RemapCharacterIdIfEnabledFar(s32 arg0, s32 arg1);
#if TBS_PATCH_2
void BattleFx_ClearRandomParticlesFar(void);
#endif
