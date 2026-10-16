// Field actors, movement, map drawing and scene services.
// Far-call interfaces; their ordered jump stubs live in
// games/tbs/src/system/far_call/FIELD.S.
void ObjectSystem_ConfigureFar(s32 mode);
void Object_ApplyProjectedPlacementFar(struct AnimationObject *sprite,
                                       const struct SpritePlacement *position,
                                       const struct SpriteScale *scale,
                                       s32 facing);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state,
                                        s32 flags);
struct AnimationObject *ResourceObject_CreateFar(s32 id);
void ResourceObject_ReleaseFar(struct AnimationObject *sprite);
s32 ResourceMetadata_RegisterFar(struct AnimationObject *state, s32 id);
void ResourceMetadata_UnregisterFar(struct AnimationObject *state, s32 handle);
void ResourceMetadata_ClearRecordFar(struct AnimationEntry *layer);
void Animation_SetWorkEntryFar(struct AnimationEntry *work, s32 no);
void ObjectSystem_InitializeFar(s32 mode);
void ObjectDispatch_ApplyArgumentToChildrenFar(void *raw, s32 no);
void ObjectDispatch_ApplyValueToChildrenFar(struct DispatchObject *obj,
                                            s32 val);
void ObjectDispatch_InitializeFar(struct DispatchObject *obj,
                                  const s32 *script);
void ObjectDispatch_InitFromTable2Far(struct DispatchObject *obj);
void ObjectDispatch_InitFromTable3Far(struct DispatchObject *obj);
void ObjectDispatch_InitFromTable1Far(struct DispatchObject *obj);
void ObjectDispatch_InitFromTable0Far(struct DispatchObject *obj);
#ifndef OWN_Object_CreateFar
struct ObjectRuntime *Object_CreateFar(s32 id, s32 x, s32 y, s32 z);
#endif
void ObjectDispatch_ReleaseFar(struct DispatchObject *obj);
void ObjectDispatch_InitFromTable4WithArgumentFar(struct DispatchObject *obj,
                                                  s32 arg);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *obj, s32 x,
                                         s32 y, s32 z);
void render_animated_tile_frameFar(u8 *obj, u32 pos);
void Map_RenderAnimatedTileFramesForObjectFar(u8 *obj);
void Map_RenderAllAnimatedTileFramesFar(u8 **table, s32 cnt);
s32 Map_LoadLayeredSceneFar(s32 index);
s32 Map_InitializePerspectiveSceneFar(void);
void Map_ApplyWorkOriginAndSpanFar(void);
void Map_UpdateCurrentTileBlockFar(void);
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
void ObjectDispatch_InitFromTable6Far(struct DispatchObject *obj);
void ObjectDispatch_WaitForValue16Far(void *raw);
#ifndef OWN_Object_SetMoveTargetFar
void Object_SetMoveTargetFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
#endif
void FieldObject_WaitForMoveFar(void *fieldObject);
void Map_LoadDefaultCellsAndUpdateBlockFar(void);
void Map_PlayMetatileCopySequenceFar(u16 *command, s32 destination_x,
                                     u32 destination_y);
void Map_CopyMetatileIndicesRectFar(s32 srcX, s32 srcY, u32 dst_x, u32 dst_y,
                                    u32 width, u32 height);
void Map_ClearLayerEntryFlagFar(u32 no);
void Map_SetLayerEntryFlagFar(u32 no);
void DisplayBlend_DisableRunScriptFar(void);
void DisplayBlend_EnableRunScriptFar(void);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 y);
s32 GetMapCellCollisionFar(s32 layer, s32 x, s32 y);
void Map_CopyMetatileCellsRectFar(s32 srcX, s32 srcY, s32 width, s32 height,
                                  s32 dst_x, s32 dst_y);
void Map_CopyCellAttributeRectFar(s32 srcX, s32 srcY, s32 width, s32 height,
                                  s32 dst_x, s32 dst_y);
void Map_CopyCellIndexRectFar(s32 srcX, s32 srcY, s32 width, s32 height,
                              s32 dst_x, s32 dst_y);
void Map_SetWindowCellTileFar(s32 x, s32 y, s32 px, s32 py);
s32 Map_CheckObjectStepFar(struct ObjectRuntime *object,
                           struct FieldPosition *position);
void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *obj,
                                           u32 flags);
void ObjectDispatch_SetChildBlendModeFar(struct DispatchObject *obj, u32 mode);
void Runtime_SetWorkTripleIfNonNegativeFar(s32 value0, s32 value1, s32 value2);
void Map_WaitWorkValuesBelow256Far(void);
void Graphics_EnableObjLayerAndCallbacksFar(void);
void ObjectDispatch_StopCallbacksAndHideLayersFar(void);
s32 CheckMapPositionCellOccupiedFar(struct WorldPosition *position);
void ObjectDispatch_RegisterChildMetadataFar(struct DispatchObject *obj,
                                             s32 no);
void Map_ResumeAnimationFar(void);
void Map_LoadAreaGraphicsFar(void);
void Animation_ApplyChildValuesFar(struct DispatchObject *obj, u32 val);
void ObjectGroup_SetChildValueUnlessFifteenFar(void *raw_parent, u32 value);
s32 ScriptObject_CheckOverlapFar(struct ScriptObjectEntry *object, s32 *values);
void Map_SetWorkFlagBits9To11Far(u32 v);
s32 ResourceMetadata_SumCommandLengthsFar(s32 id, u32 no, s32 cnt);
s32 GetWorldMapTerrainBehaviorFar(struct WorldPosition *position,
                                  s32 *terrain_kind);
s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);
void SetMapCellCollisionFar(u32 layer, s32 x, s32 y, u32 collisionCode);
void AnimationObjects_SetActiveTimerStepsFar(struct AnimationObject *group,
                                             s32 val);
void Runtime_RemapBytesByTableFar(u8 *buf, s32 cnt);
void Map_EnableUpdateCallbackFar(void);
void Map_DisableUpdateCallbackFar(void);
struct ScriptObjectEntry *
ScriptObject_FindOverlappingEntryFar(struct ScriptObjectEntry *object,
                                     s32 *values);
s32 Map_GetCellAttributeLowNibbleFar(s32 index, s32 x, s32 y);
