#ifndef ALCHEMY_FIELD_STUBS_H
#define ALCHEMY_FIELD_STUBS_H

// The field module's stub calls: actors and their sprites, movement, and the map and world map.

void          fdActorSystemConfigureStub(Sint32 mode);
// Places a sprite for the scrolling camera and queues it, with its shadow, in a draw slot by depth
// (fdRenderApplyProjectedPlacement). `pos` is in 16.16 pixels; an affine matrix is built when the
// sprite is mirrored, scaled or rotated.
void          fdRenderApplyProjectedPlacementStub(struct SPR                 *sprite,
                                                  const struct SPR_PLACEMENT *pos,
                                                  const struct SPR_SCALE     *scale,
                                                  Sint32                      facing);
Sint32        fdSpriteStartAnimationStub(struct SPR *sprite, Sint32 animation);
struct SPR   *fdSpriteCreateStub(Sint32 id);
void          fdSpriteDestroyStub(struct SPR *sprite);
Sint32        fdSpriteAddAnimationStub(struct SPR *sprite, Sint32 id);
void          fdSpriteRemoveAnimationStub(struct SPR *sprite, Sint32 layer);
void          fdSpriteAnimationClearStub(struct SPR_ANIMATION *record);
void          fdSpriteAnimationStartStub(struct SPR_ANIMATION *layer, Sint32 animation);
void          fdActorSystemInitStub(Sint32 controlMode);
// Switches every sprite of an actor, one or a group of four, to `animation`.
void          fdActorSelectAnimationStub(void *actor, Sint32 animation);
// Sets the timer step of every sprite of an actor (16 is normal speed).
void          fdActorSetAnimationSpeedStub(struct ACTOR *actor, Sint32 step);
void          fdActorStartScriptStub(struct ACTOR *actor, const Sint32 *script);
void          fdActorStartPlayerControlStub(struct ACTOR *actor);
void          fdActorStartWorldMapControlStub(struct ACTOR *actor);
void          fdActorStartMoveByKeysStub(struct ACTOR *actor);
void          fdActorStartKeyCheckStub(struct ACTOR *actor);
struct ACTOR *fdActorCreateStub(Sint32 encodedSprite, Sint32 x, Sint32 y, Sint32 z);
void          fdActorReleaseStub(struct ACTOR *actor);
void          fdActorStartCameraStub(struct ACTOR *actor, Sint32 watchedActorHandle);
void          fdActorSetPosAndResetMotionStub(struct ACTOR *actor, Sint32 x, Sint32 y, Sint32 z);
void          fdSpriteDissolveStepStub(Uint8 *sprite, Uint32 step);
void          fdSpriteDissolveRunStub(Uint8 *sprite);
void          fdSpriteDissolveRunManyStub(Uint8 **sprites, Sint32 count);
Sint32        fdMapGridLoadMapStub(Sint32 index);
UNREAD_RESULT fdWorldMapInitPerspectiveStub(void);
void          fdMapGridCenterOnFollowedPointStub(void);
void          fdWorldMapUpdateCurrentTileBlockStub(void);
void          fdWorldMapLoadGraphicsStub(Sint32 x, Sint32 z);
void          fdActorStartIdleStub(struct ACTOR *actor);
void          fdActorWaitForScriptEndStub(void *rawActor);
void          fdActorSetMoveTargetStub(struct ACTOR *actor, Sint32 x, Sint32 y, Sint32 z);
void          fdActorWaitForMoveStub(void *actor);
void          fdWorldMapLoadDefaultMetatilesAndUpdateBlockStub(void);
void          fdMapGridPlayMapEditStub(Uint16 *steps, Sint32 destX, Uint32 destY);
void          fdMapGridCopyMetatileTilesIndicesRectStub(
			 Sint32 srcX, Sint32 srcY, Uint32 destX, Uint32 destY, Uint32 width, Uint32 height);
void   fdTilesetAnimationResumeStub(Uint32 channelIndex);
void   fdTilesetAnimationPauseStub(Uint32 channelIndex);
void   fdBlendScriptResumeStub(void);
void   fdBlendScriptSuspendStub(void);
Sint32 fdMapGridGetTerrainHeightStub(Sint32 layer, Sint32 x, Sint32 y);
Sint32 fdMapGridGetMetatileBehaviorAtStub(Sint32 layer, Sint32 x, Sint32 y);
void   fdMapGridCopyMetatileTilesMetatilesRectStub(
	  Sint32 srcX, Sint32 srcY, Sint32 width, Sint32 height, Sint32 destX, Sint32 destY);
void fdMapGridCopyMetatileAttributeRectStub(
	Sint32 srcX, Sint32 srcY, Sint32 width, Sint32 height, Sint32 destX, Sint32 destY);
void fdMapGridCopyMetatileIndexRectStub(
	Sint32 srcX, Sint32 srcY, Sint32 width, Sint32 height, Sint32 destX, Sint32 destY);
void   fdWorldMapSetGridTileStub(Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY);
Sint32 fdMapGridCheckCollisionStub(struct ACTOR *actor, struct FIELD_POS *pos);
void   fdActorSetSpriteFlagsStub(struct ACTOR *actor, Uint32 flags);
void   fdActorSetBlendModeStub(struct ACTOR *actor, Uint32 mode);
void   fdMapGridSetScreenShakeStub(Sint32 shakeX, Sint32 shakeY, Sint32 shakeDecay);
void   fdMapGridWaitForShakeToSettleStub(void);
void   fdActorSystemSuspendProjectionAndShowSpritesStub(void);
void   fdActorSystemResumeProjectionAndHideLayersStub(void);
Sint32 fdMapGridIsWallAtStub(struct WORLD_POS *pos);
void   fdActorRegisterSpriteMetadataStub(struct ACTOR *actor, Sint32 animationId);
void   fdWorldMapStartStillImageStub(void);
void   fdWorldMapLoadAreaGraphicsStub(void);
void   fdActorSetSpriteColorModeStub(struct ACTOR *actor, Uint32 colorMode);
void   fdSpriteSetAnimationColorModesStub(void *sprite, Uint32 colorMode);
Sint32 fdActorScriptCheckOverlapStub(struct ACTOR *actor, Sint32 *values);
void   fdMapGridSetShownLayersStub(Uint32 layerFlags);
Sint32 fdSpriteAnimationSumCommandLengthsStub(Sint32 id, Uint32 animation, Sint32 timedCount);
Sint32 fdWorldMapGetBattleEnvironmentStub(struct WORLD_POS *pos, Sint32 *terrainKind);
Sint32 fdSpritePictureCacheLoadStub(Uint32 slot, Uint32 *buf, Sint32 number, Uint32 variant);
void   fdMapGridSetMetatileBehaviorAtStub(Uint32 layer, Sint32 x, Sint32 y, Uint32 behavior);
void   fdSpriteSetAnimationSpeedStub(struct SPR *sprite, Sint32 step);
void   fdSpriteColorRemapPixelsStub(Uint8 *bytes, Sint32 byteCount);
void   fdTilesetAnimationSuspendAllStub(void);
void   fdTilesetAnimationResumeAllStub(void);
struct ACTOR *fdActorScriptFindOverlappingActorStub(struct ACTOR *actor, Sint32 *values);
Sint32        fdMapGridGetTerrainKindAtStub(Sint32 layer, Sint32 x, Sint32 y);

#endif
