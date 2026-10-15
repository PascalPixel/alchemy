/* FarCall_FieldTable: one entry per 8-byte stub, in table order. */
FAR_CALL(ObjectSystem_ConfigureFar, ObjectSystem_Configure, void, (s32 mode))
FAR_CALL(Object_ApplyProjectedPlacementFar, Render_ApplyProjectedPlacement, void,
	(struct AnimationObject * sprite, s32 *pos, s32 *scale, s32 mode))
FAR_SLOT(Render_PlaceProjectedSprite)
FAR_SLOT(Animation_InitializeObjects)
FAR_CALL(AnimationObjects_SelectAnimationFar, AnimationObjects_SelectAnimation, s32,
	(struct AnimationObject * state, s32 flags))
FAR_SLOT(AnimationObjects_SetActiveFrameTimers)
FAR_CALL(ResourceObject_CreateFar, ResourceObject_Create, struct ResourceObject *, (s32 id))
FAR_CALL(
	ResourceObject_ReleaseFar, ResourceObject_Release, void, (struct ResourceObjectWork * work))
FAR_SLOT(Render_PlaceSpritePartPair)
FAR_CALL(ResourceMetadata_RegisterFar, ResourceMetadata_Register, s32,
	(struct AnimationObject * state, s32 id))
FAR_CALL(ResourceMetadata_UnregisterFar, ResourceMetadata_Unregister, void,
	(struct AnimationObject * state, s32 handle))
FAR_SLOT(ResourceMetadata_ReleaseSlot)
FAR_CALL(ResourceMetadata_ClearRecordFar, ResourceMetadata_ClearRecord, void, (void *dst))
FAR_SLOT(Animation_InitWorkFromMetadata)
FAR_CALL(
	Animation_SetWorkEntryFar, Animation_SetWorkEntry, void, (struct AnimationEntry * work, s32 no))
FAR_CALL(ObjectSystem_InitializeFar, ObjectSystem_Initialize, void, (s32 mode))
FAR_CALL(ObjectDispatch_ApplyArgumentToChildrenFar, ObjectDispatch_ApplyArgumentToChildren, void,
	(void *raw, s32 no))
FAR_CALL(ObjectDispatch_ApplyValueToChildrenFar, ObjectDispatch_ApplyValueToChildren, void,
	(struct DispatchObject * obj, s32 val))
FAR_SLOT(ObjectDispatch_ApplyPairToChildren)
FAR_CALL(ObjectDispatch_InitializeFar, ObjectDispatch_Initialize, void,
	(struct DispatchObject * obj, const s32 *script))
FAR_CALL(ObjectDispatch_InitFromTable2Far, ObjectDispatch_InitFromTable2, void,
	(struct DispatchObject * obj))
FAR_CALL(ObjectDispatch_InitFromTable3Far, ObjectDispatch_InitFromTable3, void,
	(struct DispatchObject * obj))
FAR_CALL(ObjectDispatch_InitFromTable1Far, ObjectDispatch_InitFromTable1, void,
	(struct DispatchObject * obj))
FAR_CALL(ObjectDispatch_InitFromTable0Far, ObjectDispatch_InitFromTable0, void,
	(struct DispatchObject * obj))
FAR_SLOT(ObjectDispatch_FindFreeObject)
#ifndef OWN_Object_CreateFar
FAR_CALL(
	Object_CreateFar, FieldObject_Create, struct ObjectRuntime *, (s32 id, s32 x, s32 y, s32 z))
#endif
FAR_CALL(ObjectDispatch_ReleaseFar, ObjectDispatch_Release, void, (struct DispatchObject * obj))
FAR_SLOT(ObjectDispatch_SetChildRotation)
FAR_CALL(ObjectDispatch_InitFromTable4WithArgumentFar, ObjectDispatch_InitFromTable4WithArgument,
	void, (struct DispatchObject * obj, s32 arg))
FAR_SLOT(ObjectDispatch_InitFromTable5WithArgument)
FAR_CALL(Object_SetPositionAndResetMotionFar, Object_SetPositionAndResetMotion, void,
	(struct ObjectRuntime * obj, s32 x, s32 y, s32 z))
FAR_CALL(render_animated_tile_frameFar, Map_RenderAnimatedTileFrame, void, (u8 * obj, u32 pos))
FAR_CALL(Map_RenderAnimatedTileFramesForObjectFar, Map_RenderAnimatedTileFramesForObject, void,
	(u8 * obj))
FAR_CALL(
	Map_RenderAllAnimatedTileFramesFar, Map_RenderAllAnimatedTileFrames, void, (u8 * *table, s32 cnt))
FAR_CALL(Map_LoadLayeredSceneFar, Map_LoadLayeredScene, s32, (s32 index))
FAR_CALL(Map_InitializePerspectiveSceneFar, Map_InitializePerspectiveScene, s32, (void))
FAR_SLOT(Map_SetCameraCenter)
FAR_CALL(Map_ApplyWorkOriginAndSpanFar, Map_ApplyWorkOriginAndSpan, void, (void))
FAR_CALL(Map_UpdateCurrentTileBlockFar, Map_UpdateCurrentTileBlock, void, (void))
FAR_CALL(WorldMap_LoadGraphicsFar, WorldMap_LoadGraphics, void, (s32 x, s32 z))
FAR_CALL(ObjectDispatch_InitFromTable6Far, ObjectDispatch_InitFromTable6, void,
	(struct DispatchObject * obj))
FAR_CALL(ObjectDispatch_WaitForValue16Far, ObjectDispatch_WaitForValue16, void, (void *raw))
#ifndef OWN_Object_SetMoveTargetFar
FAR_CALL(Object_SetMoveTargetFar, Object_SetMoveTarget, void,
	(struct ObjectRuntime * object, s32 x, s32 y, s32 z))
#endif
FAR_CALL(Script_WaitForEventTimeoutFar, Script_WaitForEventTimeout, void, (s32 arg0))
FAR_SLOT(Map_GetScreenRelativePosition)
FAR_SLOT(WorldMap_UpdateView)
FAR_CALL(Map_LoadDefaultCellsAndUpdateBlockFar, Map_LoadDefaultCellsAndUpdateBlock, void, (void))
FAR_CALL(Map_PlayMetatileCopySequenceFar, Map_PlayMetatileCopySequence, void,
	(u16 * command, s32 destination_x, u32 destination_y))
FAR_CALL(Map_CopyMetatileIndicesRectFar, Map_CopyMetatileIndicesRect, void,
	(s32 srcX, s32 srcY, u32 dst_x, u32 dst_y, u32 width, u32 height))
FAR_CALL(Map_ClearLayerEntryFlagFar, Map_ClearLayerEntryFlag, void, (u32 no))
FAR_CALL(Map_SetLayerEntryFlagFar, Map_SetLayerEntryFlag, void, (u32 no))
FAR_CALL(DisplayBlend_DisableRunScriptFar, DisplayBlend_DisableRunScript, void, (void))
FAR_CALL(DisplayBlend_EnableRunScriptFar, DisplayBlend_EnableRunScript, void, (void))
FAR_CALL(Map_GetTerrainHeightFar, Map_GetTerrainHeight, s32, (s32 layer, s32 x, s32 y))
FAR_CALL(GetMapCellCollisionFar, GetMapCellCollision, s32, (s32 layer, s32 x, s32 y))
FAR_CALL(Map_CopyMetatileCellsRectFar, Map_CopyMetatileCellsRect, void,
	(s32 srcX, s32 srcY, s32 width, s32 height, s32 dst_x, s32 dst_y))
FAR_CALL(Map_CopyCellAttributeRectFar, Map_CopyCellAttributeRect, void,
	(s32 srcX, s32 srcY, s32 width, s32 height, s32 dst_x, s32 dst_y))
FAR_CALL(Map_CopyCellIndexRectFar, Map_CopyCellIndexRect, void,
	(s32 srcX, s32 srcY, s32 width, s32 height, s32 dst_x, s32 dst_y))
FAR_CALL(Map_SetWindowCellTileFar, Map_SetWindowCellTile, void, (s32 x, s32 y, s32 px, s32 py))
FAR_CALL(Map_CheckObjectStepFar, Map_CheckObjectStep, s32,
	(struct ObjectRuntime * object, struct FieldPosition *position))
FAR_CALL(ObjectDispatch_SetSingleChildFlagsFar, ObjectDispatch_SetSingleChildFlags, void,
	(struct DispatchObject * obj, u32 flags))
FAR_CALL(ObjectDispatch_SetChildBlendModeFar, ObjectDispatch_SetChildBlendMode, void,
	(struct DispatchObject * obj, u32 mode))
FAR_CALL(Runtime_SetWorkTripleIfNonNegativeFar, Runtime_SetWorkTripleIfNonNegative, void,
	(s32 value0, s32 value1, s32 value2))
FAR_CALL(Map_WaitWorkValuesBelow256Far, Map_WaitWorkValuesBelow256, void, (void))
FAR_SLOT(Animation_SetIndexAndInitObjects)
FAR_CALL(Graphics_EnableObjLayerAndCallbacksFar, Graphics_EnableObjLayerAndCallbacks, void, (void))
FAR_CALL(ObjectDispatch_StopCallbacksAndHideLayersFar, ObjectDispatch_StopCallbacksAndHideLayers,
	void, (void))
FAR_SLOT(ObjectDispatch_SetChildDisplayFlag1)
FAR_CALL(CheckMapPositionCellOccupiedFar, CheckMapPositionCellOccupied, s32,
	(struct WorldPosition * position))
FAR_CALL(ObjectDispatch_RegisterChildMetadataFar, ObjectDispatch_RegisterChildMetadata, void,
	(struct DispatchObject * obj, s32 no))
FAR_CALL(Map_ResumeAnimationFar, MapAnimation_Start, void, (void))
FAR_CALL(Map_LoadAreaGraphicsFar, Map_LoadAreaGraphics, void, (void))
FAR_CALL(Animation_ApplyChildValuesFar, Animation_ApplyChildValues, void,
	(struct DispatchObject * obj, u32 val))
FAR_CALL(ObjectGroup_SetChildValueUnlessFifteenFar, ObjectGroup_SetChildValueUnlessFifteen, void,
	(void *raw_parent, u32 value))
FAR_CALL(ScriptObject_CheckOverlapFar, ScriptObject_CheckOverlap, s32,
	(struct ScriptObjectEntry * object, s32 *values))
FAR_CALL(Map_SetWorkFlagBits9To11Far, Map_SetWorkFlagBits9To11, void, (u32 v))
FAR_CALL(ResourceMetadata_SumCommandLengthsFar, ResourceMetadata_SumCommandLengths, s32,
	(s32 id, u32 no, s32 cnt))
FAR_CALL(GetWorldMapTerrainBehaviorFar, GetWorldMapTerrainBehavior, s32,
	(struct WorldPosition * position, s32 *terrain_kind))
FAR_CALL(
	ResourceSlot_LoadFar, ResourceSlot_Load, s32, (u32 slot, u32 *buffer, s32 number, u32 variant))
FAR_CALL(SetMapCellCollisionFar, SetMapCellCollision, void,
	(u32 layer, s32 x, s32 y, u32 collisionCode))
FAR_CALL(AnimationObjects_SetActiveTimerStepsFar, AnimationObjects_SetActiveTimerSteps, void,
	(struct AnimationObject * group, s32 val))
FAR_CALL(Runtime_RemapBytesByTableFar, Runtime_RemapBytesByTable, void, (u8 * buf, s32 cnt))
FAR_CALL(Map_EnableUpdateCallbackFar, Map_EnableUpdateCallback, void, (void))
FAR_CALL(Map_DisableUpdateCallbackFar, Map_DisableUpdateCallback, void, (void))
FAR_CALL(ScriptObject_FindOverlappingEntryFar, ScriptObject_FindOverlappingEntry,
	struct ScriptObjectEntry *, (struct ScriptObjectEntry * object, s32 *values))
FAR_CALL(
	Map_GetCellAttributeLowNibbleFar, Map_GetCellAttributeLowNibble, s32, (s32 index, s32 x, s32 y))
FAR_SLOT(Ui_RunIconMonitor)
