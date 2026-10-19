/// @file
/// Field actors, movement, map drawing and scene services.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/FIELD.S.

/// Far call to ObjectSystem_Configure.
void ObjectSystem_ConfigureFar(s32 mode);
/// Far call to Render_ApplyProjectedPlacement.
void Object_ApplyProjectedPlacementFar(struct AnimationObject *sprite,
                                       const struct SpritePlacement *position,
                                       const struct SpriteScale *scale,
                                       s32 facing);
/// Far call to AnimationObjects_SelectAnimation.
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state,
                                        s32 flags);
/// Far call to ResourceObject_Create.
struct AnimationObject *ResourceObject_CreateFar(s32 id);
/// Far call to ResourceObject_Release.
void ResourceObject_ReleaseFar(struct AnimationObject *sprite);
/// Far call to ResourceMetadata_Register.
s32 ResourceMetadata_RegisterFar(struct AnimationObject *state, s32 id);
/// Far call to ResourceMetadata_Unregister.
void ResourceMetadata_UnregisterFar(struct AnimationObject *state, s32 handle);
/// Far call to ResourceMetadata_ClearRecord.
void ResourceMetadata_ClearRecordFar(struct AnimationEntry *layer);
/// Far call to Animation_SetWorkEntry.
void Animation_SetWorkEntryFar(struct AnimationEntry *work, s32 no);
/// Far call to ObjectSystem_Initialize.
void ObjectSystem_InitializeFar(s32 mode);
/// Far call to ObjectDispatch_SelectAnimation.
void ObjectDispatch_SelectAnimationFar(void *raw, s32 no);
/// Far call to ObjectDispatch_SetAnimationSpeed.
void ObjectDispatch_SetAnimationSpeedFar(struct DispatchObject *obj, s32 val);
/// Far call to ObjectDispatch_Initialize.
void ObjectDispatch_InitializeFar(struct DispatchObject *obj,
                                  const s32 *script);
/// Far call to ObjectDispatch_StartPlayerControl.
void ObjectDispatch_StartPlayerControlFar(struct DispatchObject *obj);
/// Far call to ObjectDispatch_StartWorldMapControl.
void ObjectDispatch_StartWorldMapControlFar(struct DispatchObject *obj);
/// Far call to ObjectDispatch_StartMoveByKeys.
void ObjectDispatch_StartMoveByKeysFar(struct DispatchObject *obj);
/// Far call to ObjectDispatch_StartKeyCheck.
void ObjectDispatch_StartKeyCheckFar(struct DispatchObject *obj);
#ifndef OWN_Object_CreateFar
/// Far call to FieldObject_Create.
struct ObjectRuntime *Object_CreateFar(s32 id, s32 x, s32 y, s32 z);
#endif
/// Far call to ObjectDispatch_Release.
void ObjectDispatch_ReleaseFar(struct DispatchObject *obj);
/// Far call to ObjectDispatch_StartCamera.
void ObjectDispatch_StartCameraFar(struct DispatchObject *obj, s32 arg);
/// Far call to Object_SetPositionAndResetMotion.
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *obj, s32 x,
                                         s32 y, s32 z);
/// Far call to Map_RenderAnimatedTileFrame.
void render_animated_tile_frameFar(u8 *obj, u32 pos);
/// Far call to Map_RenderAnimatedTileFramesForObject.
void Map_RenderAnimatedTileFramesForObjectFar(u8 *obj);
/// Far call to Map_RenderAllAnimatedTileFrames.
void Map_RenderAllAnimatedTileFramesFar(u8 **table, s32 cnt);
/// Far call to Map_LoadLayeredScene.
s32 Map_LoadLayeredSceneFar(s32 index);
/// Far call to Map_InitializePerspectiveScene.
s32 Map_InitializePerspectiveSceneFar(void);
/// Far call to Map_CenterOnFollowedPoint.
void Map_CenterOnFollowedPointFar(void);
/// Far call to Map_UpdateCurrentTileBlock.
void Map_UpdateCurrentTileBlockFar(void);
/// Far call to WorldMap_LoadGraphics.
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
/// Far call to ObjectDispatch_StartIdle.
void ObjectDispatch_StartIdleFar(struct DispatchObject *obj);
/// Far call to ObjectDispatch_WaitForValue16.
void ObjectDispatch_WaitForValue16Far(void *raw);
#ifndef OWN_Object_SetMoveTargetFar
/// Far call to Object_SetMoveTarget.
void Object_SetMoveTargetFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
#endif
/// Far call to FieldObject_WaitForMove.
void FieldObject_WaitForMoveFar(void *fieldObject);
/// Far call to Map_LoadDefaultCellsAndUpdateBlock.
void Map_LoadDefaultCellsAndUpdateBlockFar(void);
/// Far call to Map_PlayMetatileCopySequence.
void Map_PlayMetatileCopySequenceFar(u16 *command, s32 destination_x,
                                     u32 destination_y);
/// Far call to Map_CopyMetatileIndicesRect.
void Map_CopyMetatileIndicesRectFar(s32 srcX, s32 srcY, u32 dst_x, u32 dst_y,
                                    u32 width, u32 height);
/// Far call to Map_ClearLayerEntryFlag.
void Map_ClearLayerEntryFlagFar(u32 no);
/// Far call to Map_SetLayerEntryFlag.
void Map_SetLayerEntryFlagFar(u32 no);
/// Far call to DisplayBlend_DisableRunScript.
void DisplayBlend_DisableRunScriptFar(void);
/// Far call to DisplayBlend_EnableRunScript.
void DisplayBlend_EnableRunScriptFar(void);
/// Far call to Map_GetTerrainHeight.
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 y);
/// Far call to GetMapCellCollision.
s32 GetMapCellCollisionFar(s32 layer, s32 x, s32 y);
/// Far call to Map_CopyMetatileCellsRect.
void Map_CopyMetatileCellsRectFar(s32 srcX, s32 srcY, s32 width, s32 height,
                                  s32 dst_x, s32 dst_y);
/// Far call to Map_CopyCellAttributeRect.
void Map_CopyCellAttributeRectFar(s32 srcX, s32 srcY, s32 width, s32 height,
                                  s32 dst_x, s32 dst_y);
/// Far call to Map_CopyCellIndexRect.
void Map_CopyCellIndexRectFar(s32 srcX, s32 srcY, s32 width, s32 height,
                              s32 dst_x, s32 dst_y);
/// Far call to Map_SetWindowCellTile.
void Map_SetWindowCellTileFar(s32 x, s32 y, s32 px, s32 py);
/// Far call to Map_CheckObjectStep.
s32 Map_CheckObjectStepFar(struct ObjectRuntime *object,
                           struct FieldPosition *position);
/// Far call to ObjectDispatch_SetSingleChildFlags.
void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *obj,
                                           u32 flags);
/// Far call to ObjectDispatch_SetChildBlendMode.
void ObjectDispatch_SetChildBlendModeFar(struct DispatchObject *obj, u32 mode);
/// Far call to Map_SetScreenShake.
void Map_SetScreenShakeFar(s32 value0, s32 value1, s32 value2);
/// Far call to Map_WaitForShakeToSettle.
void Map_WaitForShakeToSettleFar(void);
/// Far call to Graphics_EnableObjLayerAndCallbacks.
void Graphics_EnableObjLayerAndCallbacksFar(void);
/// Far call to ObjectDispatch_StopCallbacksAndHideLayers.
void ObjectDispatch_StopCallbacksAndHideLayersFar(void);
/// Far call to CheckMapPositionCellOccupied.
s32 CheckMapPositionCellOccupiedFar(struct WorldPosition *position);
/// Far call to ObjectDispatch_RegisterChildMetadata.
void ObjectDispatch_RegisterChildMetadataFar(struct DispatchObject *obj,
                                             s32 no);
/// Far call to MapAnimation_Start.
void Map_ResumeAnimationFar(void);
/// Far call to Map_LoadAreaGraphics.
void Map_LoadAreaGraphicsFar(void);
/// Far call to Animation_ApplyChildValues.
void Animation_ApplyChildValuesFar(struct DispatchObject *obj, u32 val);
/// Far call to ObjectGroup_SetChildValueUnlessFifteen.
void ObjectGroup_SetChildValueUnlessFifteenFar(void *raw_parent, u32 value);
/// Far call to ScriptObject_CheckOverlap.
s32 ScriptObject_CheckOverlapFar(struct ScriptObjectEntry *object, s32 *values);
/// Far call to Map_SetShownLayers.
void Map_SetShownLayersFar(u32 v);
/// Far call to ResourceMetadata_SumCommandLengths.
s32 ResourceMetadata_SumCommandLengthsFar(s32 id, u32 no, s32 cnt);
/// Far call to GetWorldMapTerrainBehavior.
s32 GetWorldMapTerrainBehaviorFar(struct WorldPosition *position,
                                  s32 *terrain_kind);
/// Far call to ResourceSlot_Load.
s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);
/// Far call to SetMapCellCollision.
void SetMapCellCollisionFar(u32 layer, s32 x, s32 y, u32 collisionCode);
/// Far call to AnimationObjects_SetActiveTimerSteps.
void AnimationObjects_SetActiveTimerStepsFar(struct AnimationObject *group,
                                             s32 val);
/// Far call to Runtime_RemapBytesByTable.
void Runtime_RemapBytesByTableFar(u8 *buf, s32 cnt);
/// Far call to Map_EnableUpdateCallback.
void Map_EnableUpdateCallbackFar(void);
/// Far call to Map_DisableUpdateCallback.
void Map_DisableUpdateCallbackFar(void);
struct ScriptObjectEntry *
/// Far call to ScriptObject_FindOverlappingEntry.
ScriptObject_FindOverlappingEntryFar(struct ScriptObjectEntry *object,
                                     s32 *values);
/// Far call to Map_GetCellAttributeLowNibble.
s32 Map_GetCellAttributeLowNibbleFar(s32 index, s32 x, s32 y);
