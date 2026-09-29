@ Far-call stub table heading the field engine at 0x08009000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_FieldTable
FarCall_FieldTable:
	.global Func_08009000
	.thumb_func
Func_08009000:
	overlay_veneer ObjectSystem_Configure
	.global Object_ApplyProjectedPlacementFar
	.thumb_func
Object_ApplyProjectedPlacementFar:
	overlay_veneer Render_ApplyProjectedPlacement
	overlay_veneer Render_PlaceProjectedSprite
	overlay_veneer Animation_InitializeObjects
	.global AnimationObjects_SelectAnimationFar
	.global Func_08009020
	.thumb_func
AnimationObjects_SelectAnimationFar:
Func_08009020:
	.global Object_InitializeMode
	.thumb_func
Object_InitializeMode:
	overlay_veneer AnimationObjects_SelectAnimation
	overlay_veneer AnimationObjects_SetHalfword02OnActive
	.global ResourceObject_CreateFar
	.global Func_08009030
	.thumb_func
ResourceObject_CreateFar:
Func_08009030:
	.global GetBattleEffectObject
	.thumb_func
GetBattleEffectObject:
	overlay_veneer ResourceObject_Create
	.global ResourceObject_ReleaseFar
	.thumb_func
ResourceObject_ReleaseFar:
	overlay_veneer ResourceObject_Release
	overlay_veneer Render_PlaceSpritePartPair
	.global ResourceMetadata_RegisterFar
	.global Func_08009048
	.thumb_func
ResourceMetadata_RegisterFar:
Func_08009048:
	overlay_veneer ResourceMetadata_Register
	.global ResourceMetadata_UnregisterFar
	.thumb_func
ResourceMetadata_UnregisterFar:
	overlay_veneer ResourceMetadata_Unregister
	overlay_veneer ResourceMetadata_ReleaseSlot
	.global ResourceMetadata_ClearRecordFar
	.global Func_08009060
	.thumb_func
ResourceMetadata_ClearRecordFar:
	.thumb_func
Func_08009060:
	overlay_veneer ResourceMetadata_ClearRecord
	overlay_veneer Animation_InitWorkFromMetadata
	.global Animation_SetWorkEntryFar
	.global Func_08009070
	.thumb_func
Animation_SetWorkEntryFar:
Func_08009070:
	overlay_veneer Animation_SetWorkEntry
	overlay_veneer ObjectSystem_Initialize
	.global Func_08009080
	.thumb_func
Func_08009080:
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	.global ObjectDispatch_ApplyArgumentToChildrenFar
	.thumb_func
ObjectDispatch_ApplyArgumentToChildrenFar:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildren
	.global ObjectDispatch_ApplyValueToChildrenFar
	.thumb_func
ObjectDispatch_ApplyValueToChildrenFar:
	overlay_veneer ObjectDispatch_ApplyValueToChildren
	overlay_veneer ObjectDispatch_ApplyPairToChildren
	.global ObjectDispatch_InitializeFar
	.global Func_08009098
	.thumb_func
ObjectDispatch_InitializeFar:
Func_08009098:
	overlay_veneer ObjectDispatch_Initialize
	overlay_veneer ObjectDispatch_InitFromTable2
	overlay_veneer ObjectDispatch_InitFromTable3
	overlay_veneer ObjectDispatch_InitFromTable1
	overlay_veneer ObjectDispatch_InitFromTable0
	overlay_veneer ObjectDispatch_FindFreeObject
	.global Object_CreateFar
	.global Func_080090c8
	.thumb_func
Object_CreateFar:
	.thumb_func
Func_080090c8:
	overlay_veneer Func_0800c150
	.global Func_080090d0
	.thumb_func
Func_080090d0:
	.global Object_Destroy
	.thumb_func
Object_Destroy:
	.global ObjectDispatch_ReleaseFar
	.thumb_func
ObjectDispatch_ReleaseFar:
	overlay_veneer ObjectDispatch_Release
	overlay_veneer ObjectDispatch_SetChildField1e
	.global ObjectDispatch_InitFromTable4WithArgumentFar
	.global Func_080090e0
	.thumb_func
ObjectDispatch_InitFromTable4WithArgumentFar:
Func_080090e0:
	overlay_veneer ObjectDispatch_InitFromTable4WithArgument
	overlay_veneer ObjectDispatch_InitFromTable5WithArgument
	.global Object_SetPositionAndResetMotionFar
	.global Func_080090f0
	.thumb_func
Object_SetPositionAndResetMotionFar:
Func_080090f0:
	overlay_veneer Object_SetPositionAndResetMotion
	.global render_animated_tile_frameFar
	.thumb_func
render_animated_tile_frameFar:
	overlay_veneer Map_RenderAnimatedTileFrame
	.global Map_RenderAnimatedTileFramesForObjectFar
	.thumb_func
Map_RenderAnimatedTileFramesForObjectFar:
	overlay_veneer Map_RenderAnimatedTileFramesForObject
	.global Map_RenderAllAnimatedTileFramesFar
	.thumb_func
Map_RenderAllAnimatedTileFramesFar:
	overlay_veneer Map_RenderAllAnimatedTileFrames
	overlay_veneer Map_LoadLayeredScene
	overlay_veneer Map_InitializePerspectiveScene
	overlay_veneer Map_SetCameraCenter
	.global Map_ApplyWorkOriginAndSpanFar
	.global Func_08009128
	.thumb_func
Map_ApplyWorkOriginAndSpanFar:
Func_08009128:
	overlay_veneer Map_ApplyWorkOriginAndSpan
	overlay_veneer Map_UpdateCurrentTileBlock
	overlay_veneer WorldMap_LoadGraphics
	.global Object_ResetMotion
	.thumb_func
Object_ResetMotion:
	.global ObjectDispatch_InitFromTable6Far
	.thumb_func
ObjectDispatch_InitFromTable6Far:
	overlay_veneer ObjectDispatch_InitFromTable6
	.global ObjectDispatch_WaitForValue16Far
	.thumb_func
ObjectDispatch_WaitForValue16Far:
	overlay_veneer ObjectDispatch_WaitForValue16
	.global Object_SetPosition
	.thumb_func
Object_SetPosition:
	.global Object_SetMoveTargetFar
	.thumb_func
Object_SetMoveTargetFar:
	overlay_veneer Object_SetMoveTarget
	.global Object_CommitPosition
	.thumb_func
Object_CommitPosition:
	.global Script_WaitForEventTimeoutFar
	.thumb_func
Script_WaitForEventTimeoutFar:
	overlay_veneer Script_WaitForEventTimeout
	overlay_veneer Map_GetScreenRelativePosition
	overlay_veneer WorldMap_UpdateView
	.global Map_LoadDefaultCellsAndUpdateBlockFar
	.thumb_func
Map_LoadDefaultCellsAndUpdateBlockFar:
	overlay_veneer Map_LoadDefaultCellsAndUpdateBlock
	.global Map_PlayMetatileCopySequenceFar
	.thumb_func
Map_PlayMetatileCopySequenceFar:
	overlay_veneer Map_PlayMetatileCopySequence
	.global Map_CopyMetatileIndicesRectFar
	.thumb_func
Map_CopyMetatileIndicesRectFar:
	overlay_veneer Map_CopyMetatileIndicesRect
	.global Map_ClearLayerEntryFlagFar
	.thumb_func
Map_ClearLayerEntryFlagFar:
	.global Func_08009188
	.thumb_func
Func_08009188:
	overlay_veneer Map_ClearLayerEntryFlag
	.global Map_SetLayerEntryFlagFar
	.thumb_func
Map_SetLayerEntryFlagFar:
	.global Func_08009190
	.thumb_func
Func_08009190:
	overlay_veneer Map_SetLayerEntryFlag
	.global DisplayBlend_DisableRunScriptFar
	.thumb_func
DisplayBlend_DisableRunScriptFar:
	overlay_veneer DisplayBlend_DisableRunScript
	.global DisplayBlend_EnableRunScriptFar
	.thumb_func
DisplayBlend_EnableRunScriptFar:
	overlay_veneer DisplayBlend_EnableRunScript
	.global Map_GetTerrainHeightFar
	.global Func_080091a8
	.thumb_func
Map_GetTerrainHeightFar:
	.thumb_func
Func_080091a8:
	overlay_veneer Func_08011f54
	.global Func_080091b0
	.thumb_func
Func_080091b0:
	.global GetMapCellCollisionFar
	.thumb_func
GetMapCellCollisionFar:
	overlay_veneer GetMapCellCollision
	.global Map_CopyMetatileCellsRectFar
	.thumb_func
Map_CopyMetatileCellsRectFar:
	overlay_veneer Map_CopyMetatileCellsRect
	.global Map_CopyCellAttributeRectFar
	.thumb_func
Map_CopyCellAttributeRectFar:
	overlay_veneer Map_CopyCellAttributeRect
	.global Func_080091c8
	.thumb_func
Func_080091c8:
	overlay_veneer Func_08010788
	.global Map_SetWindowCellTileFar
	.thumb_func
Map_SetWindowCellTileFar:
	overlay_veneer Map_SetWindowCellTile
	.global Object_CheckMovementCollision
	.global Func_080091d8
	.thumb_func
Object_CheckMovementCollision:
	.thumb_func
Func_080091d8:
	overlay_veneer Func_080120dc
	.global ObjectDispatch_SetSingleChildField26Far
	.thumb_func
ObjectDispatch_SetSingleChildField26Far:
	.global Func_080091e0
	.thumb_func
Func_080091e0:
	overlay_veneer ObjectDispatch_SetSingleChildField26
	.global Animation_SetStateField5Bits2To3Far
	.thumb_func
Animation_SetStateField5Bits2To3Far:
	overlay_veneer Animation_SetStateField5Bits2To3
	.global Func_080091f0
	.thumb_func
Func_080091f0:
	.global Runtime_SetWorkTripleIfNonNegativeFar
	.thumb_func
Runtime_SetWorkTripleIfNonNegativeFar:
	overlay_veneer Runtime_SetWorkTripleIfNonNegative
	.global Map_WaitWorkValuesBelow256Far
	.thumb_func
Map_WaitWorkValuesBelow256Far:
	overlay_veneer Map_WaitWorkValuesBelow256
	overlay_veneer Animation_SetIndexAndInitObjects
	.global Graphics_EnableObjLayerAndCallbacksFar
	.thumb_func
Graphics_EnableObjLayerAndCallbacksFar:
	overlay_veneer Graphics_EnableObjLayerAndCallbacks
	.global ObjectDispatch_StopCallbacksAndHideLayersFar
	.thumb_func
ObjectDispatch_StopCallbacksAndHideLayersFar:
	overlay_veneer ObjectDispatch_StopCallbacksAndHideLayers
	overlay_veneer Animation_SetStateField1dBit1
	.global CheckMapPositionCellOccupiedFar
	.global Func_08009220
	.thumb_func
CheckMapPositionCellOccupiedFar:
Func_08009220:
	overlay_veneer CheckMapPositionCellOccupied
	.global ObjectDispatch_RegisterChildMetadataFar
	.thumb_func
ObjectDispatch_RegisterChildMetadataFar:
	overlay_veneer ObjectDispatch_RegisterChildMetadata
	.global Map_ResumeAnimationFar
	.thumb_func
Map_ResumeAnimationFar:
	overlay_veneer MapAnimation_Start
	.global Map_LoadAreaGraphicsFar
	.thumb_func
Map_LoadAreaGraphicsFar:
	overlay_veneer Map_LoadAreaGraphics
	.global Animation_ApplyChildValuesFar
	.thumb_func
Animation_ApplyChildValuesFar:
	overlay_veneer Animation_ApplyChildValues
	.global Func_08009248
	.thumb_func
Func_08009248:
	.global ObjectGroup_SetChildValueUnlessFifteenFar
	.thumb_func
ObjectGroup_SetChildValueUnlessFifteenFar:
	overlay_veneer ObjectGroup_SetChildValueUnlessFifteen
	.global ScriptObject_CheckOverlapFar
	.thumb_func
ScriptObject_CheckOverlapFar:
	overlay_veneer ScriptObject_CheckOverlap
	.global Map_SetWorkFlagBits9To11Far
	.thumb_func
Map_SetWorkFlagBits9To11Far:
	overlay_veneer Map_SetWorkFlagBits9To11
	.global ResourceMetadata_SumCommandLengthsFar
	.global Func_08009260
	.thumb_func
ResourceMetadata_SumCommandLengthsFar:
Func_08009260:
	overlay_veneer ResourceMetadata_SumCommandLengths
	.global Func_08009268
	.thumb_func
Func_08009268:
	overlay_veneer GetWorldMapTerrainBehavior
	.global ResourceSlot_LoadFar
	.thumb_func
ResourceSlot_LoadFar:
	overlay_veneer ResourceSlot_Load
	.global SetMapCellCollisionFar
	.thumb_func
SetMapCellCollisionFar:
	overlay_veneer SetMapCellCollision
	.global Func_08009280
	.thumb_func
Func_08009280:
	overlay_veneer AnimationObjects_SetField15OnActive
	.global Runtime_RemapBytesByTableFar
	.thumb_func
Runtime_RemapBytesByTableFar:
	overlay_veneer Runtime_RemapBytesByTable
	.global Map_EnableUpdateCallbackFar
	.thumb_func
Map_EnableUpdateCallbackFar:
	overlay_veneer Map_EnableUpdateCallback
	.global Map_DisableUpdateCallbackFar
	.thumb_func
Map_DisableUpdateCallbackFar:
	overlay_veneer Map_DisableUpdateCallback
	overlay_veneer ScriptObject_FindOverlappingEntry
	overlay_veneer Map_GetCellAttributeLowNibble
	overlay_veneer Ui_RunIconMonitor