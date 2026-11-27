#ifndef ALCHEMY_FIELD_SEQUENCE_STUBS_H
#define ALCHEMY_FIELD_SEQUENCE_STUBS_H

/* @file */
/* The field sequence module's stub calls: the commands events are written in */
/* (actors, messages, camera, destinations), field Psynergy, weather and the */
/* field's effects. */

s32 fdSequenceRunStub(s32 entrance);
void gmMainRoutineRunStub(s32 mode);
void fdEventWaitStub(s32 frames);
void fdEventStartStub(void);
void fdEventEndStub(void);
void fdEventAppendActorPlacementsStub(struct ACTOR_PLACE *table);
void scAssignViewFlagsStub(struct ACTOR_PLACE *entry);
void fdEventSetTriggersStub(const struct TRIGGER *triggers);
void fdEventAddPartyMemberStub(s32 unit, s32 announce);
void fdEventAddPartyMemberPairStub(s32 first, s32 second);
void gmPartyRemoveMemberRestoredStub(s32 unit);
s32 gmPartyInventoryGiveItemStub(s32 item, s32 unused);
s32 fdEventAskYesNoWithActorAnimationStub(s32 actorId, s32 forceBottom);
void gmPartyLearnAbilityStub(s32 unit, s32 ability);
struct ACTOR *fdActorGetByIdStub(s32 actorId);
void fdActorReleaseByIdStub(s32 index);
void fdActorSetSpeedParmsStub(u32 actorId, s32 speedLimit, s32 acceleration);
void fdActorSetMovementTypeByIdStub(u32 actorId, s32 action);
void fdActorWaitForScriptEndByIdStub(u32 actorId);
void fdActorStopStub(u32 actorId);
void fdActorSetMovementTypeAndWaitByIdStub(u32 actorId, s32 action);
void fdActorSetDestinationByIdStub(u32 actorId, s32 x, s32 z);
void fdActorMoveToAndWaitByIdStub(u32 actorId, s32 x, s32 z);
void fdActorWalkToByIdStub(u32 actorId, s32 x, s32 z);
void fdActorWalkToAndWaitByIdStub(u32 actorId, s32 x, s32 z);
void fdActorCenterOnColumnStub(u32 actorId, s32 priority, s32 zOffset);
void fdActorSetDestinationOffsetByIdStub(u32 actorId, s32 xOffset, s32 zOffset);
void fdActorWaitForMoveByIdStub(u32 actorId);
void fdActorSetHorizontalPosWithTerrainStub(u32 actorId, s32 x, s32 z);
void fdActorSetAnimationByIdStub(u32 actorId, s32 animation);
void fdActorSetAnimationSpeedByIdStub(u32 actorId, s32 steps);
void fdActorSetAnimationAndWaitStub(u32 actorId, s32 animation);
void fdActorWaitForAnimationChangeStub(u32 actorId);
void fdActorJumpStub(u32 actorId, s32 speed, s32 frames);
void fdActorStartRepeatedMotionStub(u32 actorId, s32 count);
void fdActorRunRepeatedMotionStub(u32 actorId, s32 count);
void fdActorJumpDownLedgeStub(s32 actorId, s32 pose, s32 dustColor);
void fdActorFaceActorByIdStub(s32 firstId, s32 secondId, s32 frames);
void fdActorTurnToFaceEachOtherStub(s32 firstId, s32 secondId, s32 frames);
void fdActorConfigureSprColStub(s32 actorId, s32 color);
void fdActorSetSprColStub(struct ACTOR *actor, s32 color);
void fdActorWatchAndStartScriptStub(u32 actorId, s32 targetId,
									const void *script);
void fdEventSetMessageIdStub(s32 message);
struct WINDOW *mnFieldMessageShowAtActorStub(s32 speaker, s32 unusedFlags);
void fdEventShowMessageAndWaitStub(s32 actorId, s32 flags);
void fdEventShowMessageThenWaitStub(s32 actorId, s32 flags, s32 frames);
s32 fdEventAskYesNoAndShowResponseStub(s32 actorId, s32 flags);
void fdActorFaceDirectionByIdStub(s32 actorId, s32 angle, s32 frames);
void fdEventShowTwoMessagesAndWaitStub(s32 first, s32 firstX, s32 firstY,
									   s32 firstPortraitX, s32 firstPortraitY,
									   s32 second, s32 secondX, s32 secondY,
									   s32 secondPortraitX,
									   s32 secondPortraitY);
void mnMessageSetVoiceFromActorStub(s32 actorId);
void fdActorSetSprPriorityStub(s32 actorId, s32 priority);
void fdActorShowEmoteByIdStub(s32 actorId, s32 emote, s32 frames);
void fdEffectSetAttachedStub(s32 actorId, s32 flags);
void fdActorCopyGraphicsStub(u32 targetId, u32 sourceId);
void fdCameraFollowActorStub(s32 actorId, s32 keepPosition);
void fdCameraSetSpeedParmsStub(s32 speedLimit, s32 acceleration);
void fdCameraMoveWithinMapBoundsStub(s32 requestedX, s32 requestedY,
									 s32 requestedZ, s32 glide);
void fdCameraWaitForArrivalStub(void);
void fdCameraMoveToActorByIdStub(s32 actorId, s32 glide);
struct ACTOR *fdCameraGetActorStub(void);
void fdMapGridSetCameraBoundsStub(s32 minX, s32 minY, s32 maxX, s32 maxY);
void fdEventSetDestinationStub(s32 sceneId, s32 entrance);
void fdEventSetExitRequestStub(u16 request);
void fdRequestBattleFromEncounterTableStub(s32 zone, s32 slot);
void fdRequestDjinnBattleStub(s32 actorAndHop, s32 djinni);
void fdEventSetAfterBattleDestinationStub(s32 sceneId, s32 entrance);
void fdEventSetDefeatDestinationStub(s32 sceneId, s32 entrance);
void fdEventLeaveForStub(u16 sceneId, u16 entrance);
s32 fdActorGetScrPosStub(s32 actorId, s32 *position);
s32 fdActorGetTriggerCellAheadOfCurrentStub(void);
s32 gmPartyGetCharacterSprStub(s32 character);
s32 fdTryJumpForwardStub(void);
s32 fdClimbDownStub(void);
s32 fdClimbUpStub(void);
void fdLeaderStopMovingStub(void);
void fdWeatherStartRainStub(void);
void fdWeatherStartLightningStub(void);
void fdWeatherStopLightningStub(void);
void fdWeatherLastLightningStub(void);
void fdWeatherResumeLightningStub(void);
void fdWeatherStartLightningFlashStub(s32 from, s32 to);
void fdEffectInitSparklesStub(void);
void fdScrTransitionStartHeldWindowStub(s32 mode);
void fdPaletteFadeSetTargetStub(s32 mode, s32 half);
void fdPaletteFadeSetSourceStub(s32 mode, s32 half);
void fdPaletteFadeStartInterpolationStub(s32 frames);
void fdEventStartScrTransitionStub(void);
void fdEventFinishScrTransitionStub(void);
void fdEventWaitTransitionFramesStub(void);
/* Starts a wave in each axis of the background's scroll, changed line by */
/* line in the horizontal blank; Lamakan Desert starts it everywhere but its */
/* cave. */
void fdLineScrollInitWaveStub(s32 mode, s32 freqX, s32 stepX, s32 ampX,
							  s32 freqY, s32 stepY, s32 ampY);
void fdLineScrollStopStub(void);
s32 fdChestOpenAndShowItemStub(s32 actorId, s32 target, s32 itemId);
void fdItemDispShowPictureStub(void *object, s32 flags);
void fdItemDispShowItemStub(s32 item, s32 flags);
void fdTreasureSpotSetActorPosStub(s32 id, s32 x, s32 z);
void fdTreasureSpotMoveLeaderAwayStub(void);
void scWorldMapFreezeStub(void);
void scWorldMapUnfreezeStub(void);
void *fdActorLookupStub(u32 actorIndex);
void fdEffectStartGroundMaskStub(void *object);
void fdPsynergyRunHandlerStub(s32 kind);
void fdPsynergyLoadResourcesStub(s32 abilityId, s32 mode);
void fdPsynergySetupActorPairStub(s32 userId, s32 targetId);
void fdPsynergyStopCasterUpdateStub(void);
void fdPsynergyEndStub(void);
void fdPsynergyStartCastStub(void);
void fdEndBusyAndRevealStub(void);
void fdPsynergyStartMindReadWaveStub(s32 x, s32 y);
void fdPsynergyStopMindReadWaveStub(void);
void fdPsynergyRestoreMindReadPaletteStub(void);
s32 scHasPsynergyTriggerStub(s32 group);
u32 scHasItemTriggerStub(u16 item);
struct SPR *fdSprShareVramCacheEntryStub(struct SPR *sourceSprite,
										 struct SPR *sharedSprite);
s16 btBackgroundGetSelectedStub(void);
void gmPartyAddPpStub(s32 amount, s32 unused);
s32 fdYesNoAskStub(s32 atBottom);
void scPlayMusicStub(void);
void gmDjinnJoinPartyStub(s32 slot, s32 element, s32 djinn);
void fdParticleUpdateStub(FIELD_PARTICLE *effect);
u32 fdParticleHasPendingTargetStub(FIELD_PARTICLE *effect);
void fdParticleSetPosStub(FIELD_PARTICLE *effect, s32 x, s32 z);
void fdParticleSetAnimationStub(FIELD_PARTICLE *effect, s32 mode);
void fdParticleSetCallbackStub(FIELD_PARTICLE *effect,
							   EFFECT_CALLBACK callback);
void fdParticleInitStub(FIELD_PARTICLE *effect, s32 kind, s32 x, s32 z);
void fdEffectClearOwnedSlotStub(FIELD_PARTICLE *effect);
void fdCameraWorldToScrStub(VECTOR3 *position);
s32 fdSpeakerGetVoiceStub(u32 spriteId);
void gmPartyClearUnusableShortcutsStub(void);
void fdEffectInitSlotsStub(void);
void fdEffectReleaseSlotsStub(void);
void fdActorLaunchFromLeaderStub(u32 actorId, s32 xOffset, s32 zOffset,
								 s32 action);
void fdActorWalkByByIdStub(u32 actorId, s32 xOffset, s32 zOffset);
void fdActorWalkByAndWaitByIdStub(u32 actorId, s32 xOffset, s32 zOffset);
void fdActorWatchAndSetScriptStub(u32 actorId, u32 watchedActorId);
void fdPsynergyHaltActorStub(s32 actorId);
/* Captures up to 32 actors into ActorSnapshotWork, for */
/* fdActorRestore (fdActorSnapshot); the save menu calls it. */
void fdActorSnapshotStub(void);
void fdActorRestoreStub(void);
void scWorldMapStartZoomStub(s32 zoom, s32 frames);
void fdEventWaitForZoomTransitionStub(void);
s32 fdAreaGetNameIndexStub(s32 sceneId, s32 entrance);
void fdActorSetMovementTypeStub(struct ACTOR *actor, s32 action);
void scQueueMusicStub(s32 soundId);
void scPlayQueuedMusicStub(void);
s32 gmPartyGetCharacterSprForChildhoodStub(s32 character, s32 childhood);
#if TBS_PATCH_2
void fdEffectReleaseRevealSparklesStub(void);
#endif

#endif
