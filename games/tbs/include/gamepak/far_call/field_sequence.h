#ifndef ALCHEMY_FAR_CALL_FIELD_SEQUENCE_H
#define ALCHEMY_FAR_CALL_FIELD_SEQUENCE_H

/// @file
/// The field sequence bank's far calls: the commands events are written in
/// (actors, messages, camera, destinations), field Psynergy, weather and the
/// field's effects.

s32 FieldSequence_RunFar(s32 entrance);
void MainRoutine_RunFar(s32 mode);
void Event_WaitFar(s32 frames);
void Event_StartFar(void);
void Event_EndFar(void);
void Event_AppendActorPlacementsFar(struct ActorPlacement *table);
void Scene_AssignViewFlagsFar(struct ActorPlacement *entry);
void Event_SetTriggersFar(const struct Trigger *triggers);
void Event_AddPartyMemberFar(s32 unit, s32 announce);
void Event_AddPartyMemberPairFar(s32 first, s32 second);
void Party_RemoveMemberRestoredFar(s32 unit);
s32 PartyInventory_GiveItemFar(s32 item, s32 unused);
s32 Event_AskYesNoWithActorAnimationFar(s32 actorId, s32 forceBottom);
void Party_LearnAbilityFar(s32 unit, s32 ability);
struct Actor *Actor_GetByIdFar(s32 actorId);
void Actor_ReleaseByIdFar(s32 index);
void Actor_SetSpeedParametersFar(u32 actorId, s32 speedLimit, s32 acceleration);
void Actor_SetMovementTypeByIdFar(u32 actorId, s32 action);
void Actor_WaitForScriptEndByIdFar(u32 actorId);
void Actor_StopFar(u32 actorId);
void Actor_SetMovementTypeAndWaitByIdFar(u32 actorId, s32 action);
void Actor_SetDestinationByIdFar(u32 actorId, s32 x, s32 z);
void Actor_MoveToAndWaitByIdFar(u32 actorId, s32 x, s32 z);
void Actor_WalkToByIdFar(u32 actorId, s32 x, s32 z);
void Actor_WalkToAndWaitByIdFar(u32 actorId, s32 x, s32 z);
void Actor_CenterOnColumnFar(u32 actorId, s32 priority, s32 zOffset);
void Actor_SetDestinationOffsetByIdFar(u32 actorId, s32 xOffset, s32 zOffset);
void Actor_WaitForMoveByIdFar(u32 actorId);
void Actor_SetHorizontalPositionWithTerrainFar(u32 actorId, s32 x, s32 z);
void Actor_SetAnimationByIdFar(u32 actorId, s32 animation);
void Actor_SetAnimationSpeedByIdFar(u32 actorId, s32 steps);
void Actor_SetAnimationAndWaitFar(u32 actorId, s32 animation);
void Actor_WaitForAnimationChangeFar(u32 actorId);
void Actor_JumpFar(u32 actorId, s32 speed, s32 frames);
void Actor_StartRepeatedMotionFar(u32 actorId, s32 count);
void Actor_RunRepeatedMotionFar(u32 actorId, s32 count);
void Actor_JumpDownLedgeFar(s32 actorId, s32 pose, s32 param);
void Actor_FaceActorByIdFar(s32 firstId, s32 secondId, s32 frames);
void Actor_TurnToFaceEachOtherFar(s32 firstId, s32 secondId, s32 frames);
void Actor_ConfigureSpriteColorFar(s32 actorId, s32 param);
void Actor_SetSpriteColorFar(struct Actor *actor, s32 param);
void Actor_WatchAndStartScriptFar(u32 actorId, s32 targetId,
                                  const void *script);
void Event_SetMessageIdFar(s32 message);
struct Window *FieldMessage_ShowAtActorFar(s32 actorId, s32 unused);
void Event_ShowMessageAndWaitFar(s32 actorId, s32 flags);
void Event_ShowMessageThenWaitFar(s32 actorId, s32 flags, s32 frames);
s32 Event_AskYesNoAndShowResponseFar(s32 actorId, s32 flags);
void Actor_FaceDirectionByIdFar(s32 actorId, s32 angle, s32 frames);
void Event_ShowTwoMessagesAndWaitFar(s32 first, s32 firstX, s32 firstY,
                                     s32 firstPortraitX, s32 firstPortraitY,
                                     s32 second, s32 secondX, s32 secondY,
                                     s32 secondPortraitX, s32 secondPortraitY);
void Message_SetVoiceFromActorFar(s32 actorId);
void Actor_SetSpritePriorityFar(s32 actorId, s32 priority);
void Actor_ShowEmoteByIdFar(s32 actorId, s32 emote, s32 frames);
void FieldEffect_SetAttachedFar(s32 actorId, s32 flags);
void Actor_CopyGraphicsFar(u32 targetId, u32 sourceId);
void FieldCamera_FollowActorFar(s32 actorId, s32 keepPosition);
void FieldCamera_SetSpeedParametersFar(s32 speedLimit, s32 acceleration);
void FieldCamera_MoveWithinMapBoundsFar(s32 requestedX, s32 requestedY,
                                        s32 requestedZ, s32 glide);
void FieldCamera_WaitForArrivalFar(void);
void FieldCamera_MoveToActorByIdFar(s32 actorId, s32 glide);
struct Actor *FieldCamera_GetActorFar(void);
void MapGrid_SetCameraBoundsFar(s32 minX, s32 minY, s32 maxX, s32 maxY);
void Event_SetDestinationFar(s32 sceneId, s32 entrance);
void Event_SetExitRequestFar(u16 request);
void Field_RequestBattleFromEncounterTableFar(s32 zone, s32 slot);
void Field_RequestDjinnBattleFar(s32 flag, s32 no);
void Event_SetAfterBattleDestinationFar(s32 sceneId, s32 entrance);
void Event_SetDefeatDestinationFar(s32 sceneId, s32 entrance);
void Event_LeaveForFar(u16 sceneId, u16 entrance);
s32 Actor_GetScreenPositionFar(s32 actorId, s32 *position);
s32 Actor_GetTriggerCellAheadOfCurrentFar(void);
s32 Party_GetCharacterSpriteFar(s32 character);
s32 Field_TryJumpForwardFar(void);
s32 Field_ClimbDownFar(void);
s32 Field_ClimbUpFar(void);
void Leader_StopMovingFar(void);
void Weather_StartRainFar(void);
void Weather_StartLightningFar(void);
void Weather_StopLightningFar(void);
void Weather_LastLightningFar(void);
void Weather_ResumeLightningFar(void);
void Weather_StartLightningFlashFar(s32 from, s32 to);
void FieldEffect_InitializeSparklesFar(void);
void ScreenTransition_StartHeldWindowFar(s32 mode);
void PaletteFade_SetTargetFar(s32 mode, s32 half);
void PaletteFade_SetSourceFar(s32 mode, s32 half);
void PaletteFade_StartInterpolationFar(s32 frames);
void Event_StartScreenTransitionFar(void);
void Event_FinishScreenTransitionFar(void);
void Event_WaitTransitionFramesFar(void);
/// Starts a wave in each axis of the background's scroll, changed line by
/// line in the horizontal blank; Lamakan Desert starts it everywhere but its
/// cave.
void LineScroll_InitWaveFar(s32 mode, s32 freqX, s32 stepX, s32 ampX, s32 freqY,
                            s32 stepY, s32 ampY);
void LineScroll_StopFar(void);
s32 Chest_OpenAndShowItemFar(s32 actorId, s32 target, s32 itemId);
void ItemDisplay_ShowPictureFar(void *object, s32 flags);
void ItemDisplay_ShowItemFar(s32 item, s32 flags);
void TreasureSpot_SetActorPositionFar(s32 id, s32 x, s32 z);
void TreasureSpot_MoveLeaderAwayFar(void);
void WorldMap_FreezeFar(void);
void WorldMap_UnfreezeFar(void);
void *Actor_LookupFar(u32 actorIndex);
void FieldEffect_StartFadeOverlayFar(void *object);
void FieldPsynergy_RunHandlerFar(s32 kind);
void FieldPsynergy_LoadResourcesFar(s32 abilityId, s32 mode);
void FieldPsynergy_SetupActorPairFar(s32 userId, s32 targetId);
void FieldPsynergy_StopCasterUpdateFar(void);
void FieldPsynergy_EndFar(void);
void FieldPsynergy_StartCastFar(void);
void Field_EndBusyAndRevealFar(void);
void FieldPsynergy_StartMindReadWaveFar(s32 x, s32 y);
void FieldPsynergy_StopMindReadWaveFar(void);
void FieldPsynergy_RestoreMindReadPaletteFar(void);
s32 Scene_HasPsynergyTriggerFar(s32 group);
u32 Scene_HasItemTriggerFar(u16 item);
struct Sprite *Sprite_ShareVramCacheEntryFar(struct Sprite *sourceSprite,
                                             struct Sprite *sharedSprite);
s16 BattleBackground_GetSelectedFar(void);
void Party_AddPpFar(s32 amount, s32 unused);
s32 FieldYesNo_AskFar(s32 atBottom);
void Scene_PlayMusicFar(void);
void Djinn_JoinPartyFar(s32 slot, s32 element, s32 djinn);
void FieldParticle_UpdateFar(struct FieldParticle *effect);
u32 FieldParticle_HasPendingTargetFar(struct FieldParticle *effect);
void FieldParticle_SetPositionFar(struct FieldParticle *effect, s32 x, s32 z);
void FieldParticle_SetAnimationFar(struct FieldParticle *effect, s32 mode);
void FieldParticle_SetCallbackFar(struct FieldParticle *effect,
                                  EffectCallback callback);
void FieldParticle_InitializeFar(struct FieldParticle *effect, s32 kind, s32 x,
                                 s32 z);
void FieldEffect_ClearOwnedSlotFar(struct FieldParticle *effect);
void Camera_WorldToScreenFar(struct Vector3 *position);
s32 Speaker_GetVoiceFar(u32 id);
void Party_ClearUnusableShortcutsFar(void);
void FieldEffect_InitializeSlotsFar(void);
void FieldEffect_ReleaseSlotsFar(void);
void Actor_LaunchFromLeaderFar(u32 actorId, s32 xOffset, s32 zOffset,
                               s32 action);
void Actor_WalkByByIdFar(u32 actorId, s32 xOffset, s32 zOffset);
void Actor_WalkByAndWaitByIdFar(u32 actorId, s32 xOffset, s32 zOffset);
void Actor_WatchAndSetScriptFar(u32 actorId, u32 watchedActorId);
void FieldPsynergy_HaltActorFar(s32 actorId);
/// Captures up to 32 actors into gActorSnapshot, for
/// Actor_Restore (Actor_Snapshot); the save menu calls it.
void Actor_SnapshotFar(void);
void Actor_RestoreFar(void);
void WorldMap_StartZoomFar(s32 zoom, s32 frames);
void Event_WaitForZoomTransitionFar(void);
s32 Area_GetNameIndexFar(s32 sceneId, s32 entrance);
void Actor_SetMovementTypeFar(struct Actor *actor, s32 action);
void Scene_QueueMusicFar(s32 soundId);
void Scene_PlayQueuedMusicFar(void);
s32 Party_GetCharacterSpriteForChildhoodFar(s32 character, s32 childhood);
#if TBS_PATCH_2
void FieldEffect_ReleaseRevealSparklesFar(void);
#endif

#endif
