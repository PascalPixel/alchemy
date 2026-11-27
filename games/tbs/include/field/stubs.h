#ifndef ALCHEMY_FIELD_STUBS_H
#define ALCHEMY_FIELD_STUBS_H

/* @file */
/* The field module's stub calls: actors and their sprites, movement, and the */
/* map and world map. */

void fdActorSystemConfigureStub(s32 mode);
/* Places a sprite for the scrolling camera and queues it, with its shadow, in
 */
/* a draw slot by depth (fdRenderApplyProjectedPlacement). `position` is in */
/* 16.16 pixels; an affine matrix is built when the sprite is mirrored, scaled
 */
/* or rotated. */
void fdRenderApplyProjectedPlacementStub(struct SPR *sprite,
										 const struct SPR_PLACEMENT *position,
										 const struct SPR_SCALE *scale,
										 s32 facing);
s32 fdSprStartAnimationStub(struct SPR *sprite, s32 animation);
struct SPR *fdSprCreateStub(s32 id);
void fdSprDestroyStub(struct SPR *sprite);
s32 fdSprAddAnimationStub(struct SPR *sprite, s32 id);
void fdSprRemoveAnimationStub(struct SPR *sprite, s32 layer);
void fdSprAnimationClearStub(struct SPR_ANIMATION *record);
void fdSprAnimationStartStub(struct SPR_ANIMATION *layer, s32 animation);
void fdActorSystemInitStub(s32 controlMode);
/* Switches every sprite of an actor, one or a group of four, to `animation`. */
void fdActorSelectAnimationStub(void *actor, s32 animation);
/* Sets the timer step of every sprite of an actor (16 is normal speed). */
void fdActorSetAnimationSpeedStub(struct ACTOR *actor, s32 step);
void fdActorStartScriptStub(struct ACTOR *actor, const s32 *script);
void fdActorStartPlayerCtrlStub(struct ACTOR *actor);
void fdActorStartWorldMapCtrlStub(struct ACTOR *actor);
void fdActorStartMoveByKeysStub(struct ACTOR *actor);
void fdActorStartKeyCheckStub(struct ACTOR *actor);
struct ACTOR *fdActorCreateStub(s32 encodedSprite, s32 x, s32 y, s32 z);
void fdActorReleaseStub(struct ACTOR *actor);
void fdActorStartCameraStub(struct ACTOR *actor, s32 watchedActorHandle);
void fdActorSetPosAndResetMotionStub(struct ACTOR *actor, s32 x, s32 y, s32 z);
void fdSprDissolveStepStub(u8 *sprite, u32 step);
void fdSprDissolveRunStub(u8 *sprite);
void fdSprDissolveRunManyStub(u8 **sprites, s32 count);
s32 fdMapGridLoadMapStub(s32 index);
UNREAD_RESULT scWorldMapInitPerspectiveStub(void);
void fdMapGridCenterOnFollowedPointStub(void);
void scWorldMapUpdateCurrentTileBlockStub(void);
void scWorldMapLoadGraphicsStub(s32 x, s32 z);
void fdActorStartIdleStub(struct ACTOR *actor);
void fdActorWaitForScriptEndStub(void *rawActor);
void fdActorSetMoveTargetStub(struct ACTOR *actor, s32 x, s32 y, s32 z);
void fdActorWaitForMoveStub(void *actor);
void scWorldMapLoadDefaultCellsAndUpdateBlockStub(void);
void fdMapGridPlayMapEditStub(u16 *steps, s32 destinationX, u32 destinationY);
void fdMapGridCopyCellTilesIndicesRectStub(s32 sourceX, s32 sourceY, u32 dstX,
										   u32 dstY, u32 width, u32 height);
void fdTilesetAnimResumeStub(u32 channelIndex);
void fdTilesetAnimPauseStub(u32 channelIndex);
void fdBlendScriptResumeStub(void);
void fdBlendScriptSuspendStub(void);
s32 fdMapGridGetTerrainHeightStub(s32 layer, s32 x, s32 y);
s32 fdMapGridGetCellBehaviorAtStub(s32 layer, s32 x, s32 y);
void fdMapGridCopyCellTilesCellsRectStub(s32 sourceX, s32 sourceY, s32 width,
										 s32 height, s32 dstX, s32 dstY);
void fdMapGridCopyCellAttributeRectStub(s32 sourceX, s32 sourceY, s32 width,
										s32 height, s32 dstX, s32 dstY);
void fdMapGridCopyCellIndexRectStub(s32 sourceX, s32 sourceY, s32 width,
									s32 height, s32 dstX, s32 dstY);
void scWorldMapSetGridTileStub(s32 x, s32 y, s32 cellX, s32 cellY);
s32 fdMapGridCheckCollisionStub(struct ACTOR *actor,
								struct FIELD_POS *position);
void fdActorSetSprFlagsStub(struct ACTOR *actor, u32 flags);
void fdActorSetBlendModeStub(struct ACTOR *actor, u32 mode);
void fdMapGridSetScrShakeStub(s32 shakeX, s32 shakeY, s32 shakeDecay);
void fdMapGridWaitForShakeToSettleStub(void);
void fdActorSystemSuspendProjectionAndShowSprsStub(void);
void fdActorSystemResumeProjectionAndHideLayersStub(void);
s32 fdMapGridIsWallAtStub(struct WORLD_POS *position);
void fdActorRegisterSprMetadataStub(struct ACTOR *actor, s32 animationId);
void scWorldMapStartStillImageStub(void);
void scWorldMapLoadAreaGraphicsStub(void);
void fdActorSetSprColModeStub(struct ACTOR *actor, u32 colorMode);
void fdSprSetAnimationColModesStub(void *sprite, u32 colorMode);
s32 fdActorScriptCheckOverlapStub(struct ACTOR *actor, s32 *values);
void fdMapGridSetShownLayersStub(u32 layerFlags);
s32 fdSprAnimationSumCommandLengthsStub(s32 id, u32 animation, s32 timedCount);
s32 scWorldMapGetBattleEnvironmentStub(struct WORLD_POS *position,
									   s32 *terrainKind);
s32 fdSprPictureCacheLoadStub(u32 slot, u32 *buffer, s32 number, u32 variant);
void fdMapGridSetCellBehaviorAtStub(u32 layer, s32 x, s32 y, u32 behavior);
void fdSprSetAnimationSpeedStub(struct SPR *sprite, s32 step);
void fdSprColRemapPixelsStub(u8 *bytes, s32 byteCount);
void fdTilesetAnimSuspendAllStub(void);
void fdTilesetAnimResumeAllStub(void);
struct ACTOR *fdActorScriptFindOverlappingActorStub(struct ACTOR *actor,
													s32 *values);
s32 fdMapGridGetTerrainKindAtStub(s32 layer, s32 x, s32 y);

#endif
