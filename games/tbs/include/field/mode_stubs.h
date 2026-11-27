#ifndef ALCHEMY_FIELD_MODE_STUBS_H
#define ALCHEMY_FIELD_MODE_STUBS_H

/* The field mode module's stub calls: the commands scripts are written in
   (actors, messages, camera, destinations), field Psynergy, weather and the
   field's effects. */

Sint32 fdModeRunStub(Sint32 entrance);
void   gmMainRoutineRunStub(Sint32 start);
void   fdScriptWaitStub(Sint32 ticks);
void   fdScriptStartStub(void);
void   fdScriptEndStub(void);
void   fdScriptAppendActorPlacementsStub(struct ACTOR_PLACE *table);
void   fdAssignViewFlagsStub(struct ACTOR_PLACE *entry);
void   fdScriptSetCuesStub(const struct CUE *cues);
void   fdScriptAddPartyMemberStub(Sint32 unit, Sint32 announce);
void   fdScriptAddPartyMemberPairStub(Sint32 first, Sint32 second);
void   fdPartyRemoveMemberRestoredStub(Sint32 unit);
Sint32 fdPartyInventoryGiveItemStub(Sint32 item, Sint32 unused);
Sint32 fdScriptAskYesNoWithActorAnimationStub(
	Sint32 actorId, Sint32 forceBottom);
void		  fdPartyLearnAbilityStub(Sint32 unit, Sint32 ability);
struct ACTOR *fdActorGetByIdStub(Sint32 actorId);
void		  fdActorReleaseByIdStub(Sint32 index);
void		  fdActorSetSpeedParamsStub(
			 Uint32 actorId, Sint32 speedLimit, Sint32 acceleration);
void fdActorSetMovementTypeByIdStub(Uint32 actorId, Sint32 action);
void fdActorWaitForScriptEndByIdStub(Uint32 actorId);
void fdActorStopStub(Uint32 actorId);
void fdActorSetMovementTypeAndWaitByIdStub(Uint32 actorId, Sint32 action);
void fdActorSetDestinationByIdStub(Uint32 actorId, Sint32 x, Sint32 z);
void fdActorMoveToAndWaitByIdStub(Uint32 actorId, Sint32 x, Sint32 z);
void fdActorWalkToByIdStub(Uint32 actorId, Sint32 x, Sint32 z);
void fdActorWalkToAndWaitByIdStub(Uint32 actorId, Sint32 x, Sint32 z);
void fdActorCenterOnColumnStub(Uint32 actorId, Sint32 priority, Sint32 zOffset);
void fdActorSetDestinationOffsetByIdStub(
	Uint32 actorId, Sint32 xOffset, Sint32 zOffset);
void fdActorWaitForMoveByIdStub(Uint32 actorId);
void fdActorSetHorizontalPosWithTerrainStub(Uint32 actorId, Sint32 x, Sint32 z);
void fdActorSetAnimationByIdStub(Uint32 actorId, Sint32 animation);
void fdActorSetAnimationSpeedByIdStub(Uint32 actorId, Sint32 steps);
void fdActorSetAnimationAndWaitStub(Uint32 actorId, Sint32 animation);
void fdActorWaitForAnimationChangeStub(Uint32 actorId);
void fdActorJumpStub(Uint32 actorId, Sint32 speed, Sint32 ticks);
void fdActorStartRepeatedMotionStub(Uint32 actorId, Sint32 count);
void fdActorRunRepeatedMotionStub(Uint32 actorId, Sint32 count);
void fdActorJumpDownLedgeStub(Sint32 actorId, Sint32 pose, Sint32 dustColor);
void fdActorFaceActorByIdStub(Sint32 firstId, Sint32 secondId, Sint32 ticks);
void fdActorTurnToFaceEachOtherStub(
	Sint32 firstId, Sint32 secondId, Sint32 ticks);
void fdActorConfigureSpriteColorStub(Sint32 actorId, Sint32 color);
void fdActorSetSpriteColorStub(struct ACTOR *actor, Sint32 color);
void fdActorWatchAndStartScriptStub(
	Uint32 actorId, Sint32 targetId, const void *script);
void		   fdScriptSetMessageIdStub(Sint32 msg);
struct WINDOW *fdMessageShowAtActorStub(Sint32 speaker, Sint32 unusedFlags);
void		   fdScriptShowMessageAndWaitStub(Sint32 actorId, Sint32 flags);
void		   fdScriptShowMessageThenWaitStub(
			  Sint32 actorId, Sint32 flags, Sint32 ticks);
Sint32 fdScriptAskYesNoAndShowResponseStub(Sint32 actorId, Sint32 flags);
void   fdActorFaceDirectionByIdStub(Sint32 actorId, Sint32 angle, Sint32 ticks);
void   fdScriptShowTwoMessagesAndWaitStub(Sint32 first, Sint32 firstX,
	  Sint32 firstY, Sint32 firstPortraitX, Sint32 firstPortraitY, Sint32 second,
	  Sint32 secondX, Sint32 secondY, Sint32 secondPortraitX,
	  Sint32 secondPortraitY);
void   fdMessageSetVoiceFromActorStub(Sint32 actorId);
void   fdActorSetSpritePriorityStub(Sint32 actorId, Sint32 priority);
void   fdActorShowEmoteByIdStub(Sint32 actorId, Sint32 emote, Sint32 ticks);
void   fdEffectSetAttachedStub(Sint32 actorId, Sint32 flags);
void   fdActorCopyGraphicsStub(Uint32 targetId, Uint32 srcId);
void   fdCameraFollowActorStub(Sint32 actorId, Sint32 keepPos);
void   fdCameraSetSpeedParamsStub(Sint32 speedLimit, Sint32 acceleration);
void   fdCameraMoveWithinMapBoundsStub(
	  Sint32 requestedX, Sint32 requestedY, Sint32 requestedZ, Sint32 glide);
void		  fdCameraWaitForArrivalStub(void);
void		  fdCameraMoveToActorByIdStub(Sint32 actorId, Sint32 glide);
struct ACTOR *fdCameraGetActorStub(void);
void		  fdMapGridSetCameraBoundsStub(
			 Sint32 minX, Sint32 minY, Sint32 maxX, Sint32 maxY);
void   fdScriptSetDestinationStub(Sint32 sceneId, Sint32 entrance);
void   fdScriptSetExitRequestStub(Uint16 request);
void   fdRequestBattleFromEncounterTableStub(Sint32 zone, Sint32 slot);
void   fdRequestDjinnBattleStub(Sint32 actorAndHop, Sint32 djinni);
void   fdScriptSetAfterBattleDestinationStub(Sint32 sceneId, Sint32 entrance);
void   fdScriptSetDefeatDestinationStub(Sint32 sceneId, Sint32 entrance);
void   fdScriptLeaveForStub(Uint16 sceneId, Uint16 entrance);
Sint32 fdActorGetScreenPosStub(Sint32 actorId, Sint32 *pos);
Sint32 fdActorGetCueMetatileAheadOfCurrentStub(void);
Sint32 fdPartyGetCharacterSpriteStub(Sint32 character);
Sint32 fdTryJumpForwardStub(void);
Sint32 fdClimbDownStub(void);
Sint32 fdClimbUpStub(void);
void   fdLeaderStopMovingStub(void);
void   fdWeatherStartRainStub(void);
void   fdWeatherStartLightningStub(void);
void   fdWeatherStopLightningStub(void);
void   fdWeatherLastLightningStub(void);
void   fdWeatherResumeLightningStub(void);
void   fdWeatherStartLightningFlashStub(Sint32 from, Sint32 to);
void   fdEffectInitSparklesStub(void);
void   fdScreenTransitionStartHeldWindowStub(Sint32 mode);
void   fdPaletteFadeSetTargetStub(Sint32 mode, Sint32 half);
void   fdPaletteFadeSetSourceStub(Sint32 mode, Sint32 half);
void   fdPaletteFadeStartInterpolationStub(Sint32 ticks);
void   fdScriptStartScreenTransitionStub(void);
void   fdScriptFinishScreenTransitionStub(void);
void   fdScriptWaitTransitionTicksStub(void);
/* Starts a wave in each axis of the background's scroll, changed line by line
   in the horizontal blank; Lamakan Desert starts it everywhere but its cave. */
void   scLineScrollInitWaveStub(Sint32 mode, Sint32 frequencyX, Sint32 stepX,
	  Sint32 ampX, Sint32 frequencyY, Sint32 stepY, Sint32 ampY);
void   fdLineScrollStopStub(void);
Sint32 fdChestOpenAndShowItemStub(Sint32 actorId, Sint32 target, Sint32 itemId);
void   fdItemDisplayShowPictureStub(void *object, Sint32 flags);
void   fdItemDisplayShowItemStub(Sint32 item, Sint32 flags);
void   fdTreasureSpotSetActorPosStub(Sint32 id, Sint32 x, Sint32 z);
void   fdTreasureSpotMoveLeaderAwayStub(void);
void   fdWorldMapFreezeStub(void);
void   fdWorldMapUnfreezeStub(void);
void  *fdActorLookupStub(Uint32 actorIndex);
void   fdEffectStartGroundMaskStub(void *object);
void   fdPsynergyRunHandlerStub(Sint32 kind);
void   fdPsynergyLoadResourcesStub(Sint32 abilityId, Sint32 mode);
void   fdPsynergySetupActorPairStub(Sint32 userId, Sint32 targetId);
void   fdPsynergyStopCasterUpdateStub(void);
void   fdPsynergyEndStub(void);
void   fdPsynergyStartCastStub(void);
void   fdEndBusyAndRevealStub(void);
void   fdPsynergyStartMindReadWaveStub(Sint32 x, Sint32 y);
void   fdPsynergyStopMindReadWaveStub(void);
void   fdPsynergyRestoreMindReadPaletteStub(void);
Sint32 fdHasPsynergyCueStub(Sint32 group);
Uint32 fdHasItemCueStub(Uint16 item);
struct SPR *fdSpriteShareVRAMCacheEntryStub(
	struct SPR *srcSprite, struct SPR *sharedSprite);
Sint16 fdBattleBackgroundGetSelectedStub(void);
void   fdPartyAddPpStub(Sint32 amount, Sint32 unused);
Sint32 fdYesNoAskStub(Sint32 atBottom);
void   gmPlayMusicStub(void);
void   fdDjinnJoinPartyStub(Sint32 slot, Sint32 element, Sint32 djinn);
void   fdParticleUpdateStub(FIELD_PARTICLE *effect);
Uint32 fdParticleHasPendingTargetStub(FIELD_PARTICLE *effect);
void   fdParticleSetPosStub(FIELD_PARTICLE *effect, Sint32 x, Sint32 z);
void   fdParticleSetAnimationStub(FIELD_PARTICLE *effect, Sint32 mode);
void   fdParticleSetCallbackStub(
	  FIELD_PARTICLE *effect, EFFECT_CALLBACK callback);
void fdParticleInitStub(
	FIELD_PARTICLE *effect, Sint32 kind, Sint32 x, Sint32 z);
void   fdEffectClearOwnedSlotStub(FIELD_PARTICLE *effect);
void   fdCameraWorldToScreenStub(VECTOR3 *pos);
Sint32 fdSpeakerGetVoiceStub(Uint32 spriteId);
void   fdPartyClearUnusableShortcutsStub(void);
void   fdEffectInitSlotsStub(void);
void   fdEffectReleaseSlotsStub(void);
void   fdActorLaunchFromLeaderStub(
	  Uint32 actorId, Sint32 xOffset, Sint32 zOffset, Sint32 action);
void fdActorWalkByByIdStub(Uint32 actorId, Sint32 xOffset, Sint32 zOffset);
void fdActorWalkByAndWaitByIdStub(
	Uint32 actorId, Sint32 xOffset, Sint32 zOffset);
void fdActorWatchAndSetScriptStub(Uint32 actorId, Uint32 watchedActorId);
void fdPsynergyHaltActorStub(Sint32 actorId);
/* Captures up to 32 actors into ActorSnapshotWork, for fdActorRestore
   (fdActorSnapshot); the save menu calls it. */
void   fdActorSnapshotStub(void);
void   fdActorRestoreStub(void);
void   fdWorldMapStartZoomStub(Sint32 zoom, Sint32 ticks);
void   fdScriptWaitForZoomTransitionStub(void);
Sint32 fdAreaGetNameIndexStub(Sint32 sceneId, Sint32 entrance);
void   fdActorSetMovementTypeStub(struct ACTOR *actor, Sint32 action);
void   fdQueueMusicStub(Sint32 soundId);
void   fdPlayQueuedMusicStub(void);
Sint32 fdPartyGetCharacterSpriteForChildhoodStub(
	Sint32 character, Sint32 childhood);
#if FIELD_MENU_STOPS_REVEAL_SPARKLES
void fdEffectReleaseRevealSparklesStub(void);
#endif

#endif
