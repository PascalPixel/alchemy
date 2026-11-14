#ifndef ALCHEMY_FAR_CALL_FIELD_H
#define ALCHEMY_FAR_CALL_FIELD_H

/// @file
/// Actors, movement, map drawing and map routines.

void ActorSystem_ConfigureFar(s32 mode);
/// Places a sprite for the scrolling camera and queues it, with its shadow, in
/// a draw slot by depth (Render_ApplyProjectedPlacement). `position` is in
/// 16.16 pixels; an affine matrix is built when the sprite is mirrored, scaled
/// or rotated.
void Actor_ApplyProjectedPlacementFar(struct Sprite *sprite,
                                      const struct SpritePlacement *position,
                                      const struct SpriteScale *scale,
                                      s32 facing);
s32 Sprite_StartAnimationFar(struct Sprite *state, s32 flags);
struct Sprite *Sprite_CreateFar(s32 id);
void Sprite_DestroyFar(struct Sprite *sprite);
s32 Sprite_AddAnimationFar(struct Sprite *state, s32 id);
void Sprite_RemoveAnimationFar(struct Sprite *state, s32 handle);
void SpriteAnimation_ClearFar(struct SpriteAnimation *layer);
void SpriteAnimation_StartFar(struct SpriteAnimation *work, s32 no);
void ActorSystem_InitializeFar(s32 mode);
/// Switches every sprite of an actor, one or a group of four, to animation
/// `no`.
void Actor_SelectAnimationFar(void *raw, s32 no);
/// Sets the timer step of every sprite of an actor (16 is normal speed).
void Actor_SetAnimationSpeedFar(struct Actor *actor, s32 val);
void Actor_StartScriptFar(struct Actor *actor, const s32 *script);
void Actor_StartPlayerControlFar(struct Actor *actor);
void Actor_StartWorldMapControlFar(struct Actor *actor);
void Actor_StartMoveByKeysFar(struct Actor *actor);
void Actor_StartKeyCheckFar(struct Actor *actor);
struct Actor *Actor_CreateFar(s32 id, s32 x, s32 y, s32 z);
void Actor_ReleaseFar(struct Actor *actor);
void Actor_StartCameraFar(struct Actor *actor, s32 arg);
void Actor_SetPositionAndResetMotionFar(struct Actor *actor, s32 x, s32 y,
                                        s32 z);
void SpriteDissolve_StepFar(u8 *obj, u32 pos);
void SpriteDissolve_RunFar(u8 *obj);
void SpriteDissolve_RunManyFar(u8 **table, s32 cnt);
s32 MapGrid_LoadMapFar(s32 index);
UNREAD_RESULT WorldMap_InitializePerspectiveFar(void);
void MapGrid_CenterOnFollowedPointFar(void);
void WorldMap_UpdateCurrentTileBlockFar(void);
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
void Actor_StartIdleFar(struct Actor *actor);
void Actor_WaitForScriptEndFar(void *raw);
void Actor_SetMoveTargetFar(struct Actor *actor, s32 x, s32 y, s32 z);
void Actor_WaitForMoveFar(void *actor);
void WorldMap_LoadDefaultCellsAndUpdateBlockFar(void);
void MapGrid_PlayMapEditFar(u16 *command, s32 destination_x, u32 destination_y);
void MapGrid_CopyCellPictureIndicesRectFar(s32 sourceX, s32 sourceY, u32 dst_x,
                                           u32 dst_y, u32 width, u32 height);
void TilesetAnim_ResumeFar(u32 no);
void TilesetAnim_PauseFar(u32 no);
void BlendScript_ResumeFar(void);
void BlendScript_SuspendFar(void);
s32 MapGrid_GetTerrainHeightFar(s32 layer, s32 x, s32 y);
s32 MapGrid_GetCellBehaviorAtFar(s32 layer, s32 x, s32 y);
void MapGrid_CopyCellPictureCellsRectFar(s32 sourceX, s32 sourceY, s32 width,
                                         s32 height, s32 dst_x, s32 dst_y);
void MapGrid_CopyCellAttributeRectFar(s32 sourceX, s32 sourceY, s32 width,
                                      s32 height, s32 dst_x, s32 dst_y);
void MapGrid_CopyCellIndexRectFar(s32 sourceX, s32 sourceY, s32 width,
                                  s32 height, s32 dst_x, s32 dst_y);
void WorldMap_SetWindowCellTileFar(s32 x, s32 y, s32 cellX, s32 cellY);
s32 MapGrid_CheckCollisionFar(struct Actor *actor,
                              struct FieldPosition *position);
void Actor_SetSpriteFlagsFar(struct Actor *actor, u32 flags);
void Actor_SetBlendModeFar(struct Actor *actor, u32 mode);
void MapGrid_SetScreenShakeFar(s32 shakeX, s32 shakeY, s32 shakeDecay);
void MapGrid_WaitForShakeToSettleFar(void);
void ActorSystem_SuspendProjectionAndShowSpritesFar(void);
void ActorSystem_ResumeProjectionAndHideLayersFar(void);
s32 MapGrid_IsWallAtFar(struct WorldPosition *position);
void Actor_RegisterSpriteMetadataFar(struct Actor *actor, s32 no);
void WorldMap_StartStillPictureFar(void);
void WorldMap_LoadAreaGraphicsFar(void);
void Actor_SetSpriteColorModeFar(struct Actor *actor, u32 val);
void Sprite_SetAnimationColorModesFar(void *raw_parent, u32 value);
s32 ActorScript_CheckOverlapFar(struct Actor *actor, s32 *values);
void MapGrid_SetShownLayersFar(u32 v);
s32 SpriteAnimation_SumCommandLengthsFar(s32 id, u32 no, s32 cnt);
s32 WorldMap_GetBattleEnvironmentFar(struct WorldPosition *position,
                                     s32 *terrain_kind);
s32 SpritePictureCache_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);
void MapGrid_SetCellBehaviorAtFar(u32 layer, s32 x, s32 y, u32 behavior);
void Sprite_SetAnimationSpeedFar(struct Sprite *group, s32 val);
void SpriteColor_RemapPixelsFar(u8 *buf, s32 cnt);
void TilesetAnim_SuspendAllFar(void);
void TilesetAnim_ResumeAllFar(void);
struct Actor *ActorScript_FindOverlappingEntryFar(struct Actor *actor,
                                                  s32 *values);
s32 MapGrid_GetCellAttributeLowNibbleFar(s32 index, s32 x, s32 y);

#endif
