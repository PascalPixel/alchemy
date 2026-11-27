#ifndef ALCHEMY_FIELD_STUBS_H
#define ALCHEMY_FIELD_STUBS_H

/* The field module's stub calls: actors and their sprites, movement, and the
   map and world map. */

void fdActorSystemConfigureStub(Sint32 mode);
/* Places a sprite for the scrolling camera and queues it, with its shadow, in a
   draw slot by depth (fdRenderApplyProjectedPlacement). `pos` is in 16.16
   pixels; an affine matrix is built when the sprite is mirrored, scaled or
   rotated. */
void		fdRenderApplyProjectedPlacementStub(struct SPR *spr,
		   const struct SPR_PLACEMENT *pos, const struct SPR_SCALE *scale,
		   Sint32 facing);
Sint32		fdSprStartAnimationStub(struct SPR *spr, Sint32 anm);
struct SPR *fdSprCreateStub(Sint32 id);
void		fdSprDestroyStub(struct SPR *spr);
Sint32		fdSprAddAnimationStub(struct SPR *spr, Sint32 id);
void		fdSprRemoveAnimationStub(struct SPR *spr, Sint32 layer);
void		fdSprAnimationClearStub(struct SPR_ANIMATION *record);
void		fdSprAnimationStartStub(struct SPR_ANIMATION *layer, Sint32 anm);
void		fdActorSystemInitStub(Sint32 controlMode);
/* Switches every sprite of an actor, one or a group of four, to `anm`. */
void fdActorSelectAnimationStub(void *actor, Sint32 anm);
/* Sets the timer step of every sprite of an actor (16 is normal speed). */
void		  fdActorSetAnimationSpeedStub(struct ACTOR *actor, Sint32 step);
void		  fdActorStartScriptStub(struct ACTOR *actor, const Sint32 *script);
void		  fdActorStartPlayerCtrlStub(struct ACTOR *actor);
void		  fdActorStartWorldMapCtrlStub(struct ACTOR *actor);
void		  fdActorStartMoveByKeysStub(struct ACTOR *actor);
void		  fdActorStartKeyCheckStub(struct ACTOR *actor);
struct ACTOR *fdActorCreateStub(
	Sint32 encodedSpr, Sint32 x, Sint32 y, Sint32 z);
void fdActorReleaseStub(struct ACTOR *actor);
void fdActorStartCameraStub(struct ACTOR *actor, Sint32 watchedActorHandle);
void fdActorSetPosAndResetMotionStub(
	struct ACTOR *actor, Sint32 x, Sint32 y, Sint32 z);
void		  fdSprDissolveStepStub(Uint8 *spr, Uint32 step);
void		  fdSprDissolveRunStub(Uint8 *spr);
void		  fdSprDissolveRunManyStub(Uint8 **sprs, Sint32 cnt);
Sint32		  fdMapGridLoadMapStub(Sint32 idx);
UNREAD_RESULT fdWorldMapInitPerspectiveStub(void);
void		  fdMapGridCenterOnFollowedPointStub(void);
void		  fdWorldMapUpdateCurrentTileBlockStub(void);
void		  fdWorldMapLoadGraphicsStub(Sint32 x, Sint32 z);
void		  fdActorStartIdleStub(struct ACTOR *actor);
void		  fdActorWaitForScriptEndStub(void *rawActor);
void		  fdActorSetMoveTargetStub(
			 struct ACTOR *actor, Sint32 x, Sint32 y, Sint32 z);
void   fdActorWaitForMoveStub(void *actor);
void   fdWorldMapLoadDefaultMetatilesAndUpdateBlockStub(void);
void   fdMapGridPlayMapEditStub(Uint16 *steps, Sint32 dstX, Uint32 dstY);
void   fdMapGridCopyMetatileTilesIndicesRectStub(Sint32 srcX, Sint32 srcY,
	  Uint32 dstX, Uint32 dstY, Uint32 width, Uint32 height);
void   fdTilesetAnimResumeStub(Uint32 channelIdx);
void   fdTilesetAnimPauseStub(Uint32 channelIdx);
void   fdBlendScriptResumeStub(void);
void   fdBlendScriptSuspendStub(void);
Sint32 fdMapGridGetTerrainHeightStub(Sint32 layer, Sint32 x, Sint32 y);
Sint32 fdMapGridGetMetatileBehaviorAtStub(Sint32 layer, Sint32 x, Sint32 y);
void   fdMapGridCopyMetatileTilesMetatilesRectStub(Sint32 srcX, Sint32 srcY,
	  Sint32 width, Sint32 height, Sint32 dstX, Sint32 dstY);
void   fdMapGridCopyMetatileAttributeRectStub(Sint32 srcX, Sint32 srcY,
	  Sint32 width, Sint32 height, Sint32 dstX, Sint32 dstY);
void fdMapGridCopyMetatileIndexRectStub(Sint32 srcX, Sint32 srcY, Sint32 width,
	Sint32 height, Sint32 dstX, Sint32 dstY);
void fdWorldMapSetGridTileStub(
	Sint32 x, Sint32 y, Sint32 metatileX, Sint32 metatileY);
Sint32 fdMapGridCheckCollisionStub(struct ACTOR *actor, struct FIELD_POS *pos);
void   fdActorSetSprFlagsStub(struct ACTOR *actor, Uint32 flg);
void   fdActorSetBlendModeStub(struct ACTOR *actor, Uint32 mode);
void   fdMapGridSetScreenShakeStub(
	  Sint32 shakeX, Sint32 shakeY, Sint32 shakeDecay);
void   fdMapGridWaitForShakeToSettleStub(void);
void   fdActorSystemSuspendProjectionAndShowSprsStub(void);
void   fdActorSystemResumeProjectionAndHideLayersStub(void);
Sint32 fdMapGridIsWallAtStub(struct WORLD_POS *pos);
void   fdActorRegisterSprMetadataStub(struct ACTOR *actor, Sint32 anmId);
void   fdWorldMapStartStillImageStub(void);
void   fdWorldMapLoadAreaGraphicsStub(void);
void   fdActorSetSprColModeStub(struct ACTOR *actor, Uint32 colMode);
void   fdSprSetAnimationColModesStub(void *spr, Uint32 colMode);
Sint32 fdActorScriptCheckOverlapStub(struct ACTOR *actor, Sint32 *values);
void   fdMapGridSetShownLayersStub(Uint32 layerFlg);
Sint32 fdSprAnimationSumCommandLengthsStub(
	Sint32 id, Uint32 anm, Sint32 timedCnt);
Sint32 fdWorldMapGetBattleEnvironmentStub(
	struct WORLD_POS *pos, Sint32 *terrainKind);
Sint32 fdSprPictureCacheLoadStub(
	Uint32 slot, Uint32 *buf, Sint32 num, Uint32 variant);
void fdMapGridSetMetatileBehaviorAtStub(
	Uint32 layer, Sint32 x, Sint32 y, Uint32 behavior);
void		  fdSprSetAnimationSpeedStub(struct SPR *spr, Sint32 step);
void		  fdSprColRemapPixelsStub(Uint8 *bytes, Sint32 byteCnt);
void		  fdTilesetAnimSuspendAllStub(void);
void		  fdTilesetAnimResumeAllStub(void);
struct ACTOR *fdActorScriptFindOverlappingActorStub(
	struct ACTOR *actor, Sint32 *values);
Sint32 fdMapGridGetTerrainKindAtStub(Sint32 layer, Sint32 x, Sint32 y);

#endif
