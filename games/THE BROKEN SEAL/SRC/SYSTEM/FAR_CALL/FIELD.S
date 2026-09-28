@ Far-call stub table heading the field engine at 0x08009000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08009000
	.thumb_func
Func_08009000:
	overlay_veneer 0x0800bb21
	.global Object_ApplyProjectedPlacementFar
	.thumb_func
Object_ApplyProjectedPlacementFar:
	overlay_veneer 0x0800b169
	overlay_veneer 0x0800b389
	overlay_veneer 0x0800b7c1
	.global AnimationObjects_SelectAnimationFar
	.global Func_08009020
	.thumb_func
AnimationObjects_SelectAnimationFar:
Func_08009020:
	.global Object_InitializeMode
	.thumb_func
Object_InitializeMode:
	overlay_veneer 0x0800ba31
	overlay_veneer 0x0800bacd
	.global ResourceObject_CreateFar
	.global Func_08009030
	.thumb_func
ResourceObject_CreateFar:
Func_08009030:
	.global GetBattleEffectObject
	.thumb_func
GetBattleEffectObject:
	overlay_veneer 0x0800bc71
	.global ResourceObject_ReleaseFar
	.thumb_func
ResourceObject_ReleaseFar:
	overlay_veneer 0x0800bdd5
	overlay_veneer 0x0800b075
	.global ResourceMetadata_RegisterFar
	.global Func_08009048
	.thumb_func
ResourceMetadata_RegisterFar:
Func_08009048:
	overlay_veneer 0x0800b8ad
	.global ResourceMetadata_UnregisterFar
	.thumb_func
ResourceMetadata_UnregisterFar:
	overlay_veneer 0x0800b93d
	overlay_veneer 0x0800b9a5
	.global ResourceMetadata_ClearRecordFar
	.global Func_08009060
	.thumb_func
ResourceMetadata_ClearRecordFar:
Func_08009060:
	overlay_veneer 0x0800bc49
	overlay_veneer 0x0800b869
	.global Animation_SetWorkEntryFar
	.global Func_08009070
	.thumb_func
Animation_SetWorkEntryFar:
Func_08009070:
	overlay_veneer 0x0800b9f5
	overlay_veneer 0x0800c005
	.global Func_08009080
	.thumb_func
Func_08009080:
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	overlay_veneer 0x0800c301
	.global ObjectDispatch_ApplyValueToChildrenFar
	.thumb_func
ObjectDispatch_ApplyValueToChildrenFar:
	overlay_veneer 0x0800c345
	overlay_veneer 0x0800c389
	.global ObjectDispatch_InitializeFar
	.global Func_08009098
	.thumb_func
ObjectDispatch_InitializeFar:
Func_08009098:
	overlay_veneer 0x0800c2d9
	overlay_veneer 0x0800c48d
	overlay_veneer 0x0800c49d
	overlay_veneer 0x0800c47d
	overlay_veneer 0x0800c46d
	overlay_veneer 0x0800c0cd
	.global Object_CreateFar
	.global Func_080090c8
	.thumb_func
Object_CreateFar:
Func_080090c8:
	overlay_veneer 0x0800c151
	.global Func_080090d0
	.thumb_func
Func_080090d0:
	.global Object_Destroy
	.thumb_func
Object_Destroy:
	overlay_veneer 0x0800c0f5
	overlay_veneer 0x0800c3ed
	.global ObjectDispatch_InitFromTable4WithArgumentFar
	.global Func_080090e0
	.thumb_func
ObjectDispatch_InitFromTable4WithArgumentFar:
Func_080090e0:
	overlay_veneer 0x0800c4bd
	overlay_veneer 0x0800c455
	.global Object_SetPositionAndResetMotionFar
	.global Func_080090f0
	.thumb_func
Object_SetPositionAndResetMotionFar:
Func_080090f0:
	overlay_veneer 0x0800d131
	.global render_animated_tile_frameFar
	.thumb_func
render_animated_tile_frameFar:
	overlay_veneer 0x0800be71
	.global Map_RenderAnimatedTileFramesForObjectFar
	.thumb_func
Map_RenderAnimatedTileFramesForObjectFar:
	overlay_veneer 0x0800befd
	.global Map_RenderAllAnimatedTileFramesFar
	.thumb_func
Map_RenderAllAnimatedTileFramesFar:
	overlay_veneer 0x0800bf35
	overlay_veneer 0x0800fb39
	overlay_veneer 0x080109e9
	overlay_veneer 0x08010231
	.global Map_ApplyWorkOriginAndSpanFar
	.global Func_08009128
	.thumb_func
Map_ApplyWorkOriginAndSpanFar:
Func_08009128:
	overlay_veneer 0x0800fe9d
	overlay_veneer 0x080113e5
	overlay_veneer 0x08010e15
	.global Object_ResetMotion
	.thumb_func
Object_ResetMotion:
	overlay_veneer 0x0800c4ad
	.global ObjectDispatch_WaitForValue16Far
	.thumb_func
ObjectDispatch_WaitForValue16Far:
	overlay_veneer 0x0800c4ed
	.global Object_SetPosition
	.thumb_func
Object_SetPosition:
	overlay_veneer 0x0800d14d
	.global Object_CommitPosition
	.thumb_func
Object_CommitPosition:
	overlay_veneer 0x0800ca6d
	overlay_veneer 0x0800bfa5
	overlay_veneer 0x080111b5
	overlay_veneer 0x0801173d
	overlay_veneer 0x08010561
	overlay_veneer 0x08010425
	overlay_veneer 0x080118a9
	overlay_veneer 0x080118c1
	overlay_veneer 0x08011af1
	overlay_veneer 0x08011ae1
	.global Map_GetTerrainHeightFar
	.global Func_080091a8
	.thumb_func
Map_GetTerrainHeightFar:
Func_080091a8:
	overlay_veneer 0x08011f55
	.global Func_080091b0
	.thumb_func
Func_080091b0:
	overlay_veneer 0x08012039
	overlay_veneer 0x080105d5
	overlay_veneer 0x08010705
	overlay_veneer 0x08010789
	overlay_veneer 0x08010d49
	.global Object_CheckMovementCollision
	.global Func_080091d8
	.thumb_func
Object_CheckMovementCollision:
Func_080091d8:
	overlay_veneer 0x080120dd
	.global ObjectDispatch_SetSingleChildField26Far
	.thumb_func
ObjectDispatch_SetSingleChildField26Far:
	overlay_veneer 0x0800c529
	overlay_veneer 0x0800c549
	.global Func_080091f0
	.thumb_func
Func_080091f0:
	overlay_veneer 0x08012331
	overlay_veneer 0x08012351
	overlay_veneer 0x0800c409
	overlay_veneer 0x0800c5b5
	overlay_veneer 0x0800c5fd
	overlay_veneer 0x0800c571
	.global CheckMapPositionCellOccupiedFar
	.global Func_08009220
	.thumb_func
CheckMapPositionCellOccupiedFar:
Func_08009220:
	overlay_veneer 0x0801219d
	overlay_veneer 0x0800c431
	.global Map_ResumeAnimationFar
	.thumb_func
Map_ResumeAnimationFar:
	overlay_veneer 0x08011591
	.global Map_LoadAreaGraphicsFar
	.thumb_func
Map_LoadAreaGraphicsFar:
	overlay_veneer 0x08011645
	.global Animation_ApplyChildValuesFar
	.thumb_func
Animation_ApplyChildValuesFar:
	overlay_veneer 0x0800c599
	.global Func_08009248
	.thumb_func
Func_08009248:
	.global ObjectGroup_SetChildValueUnlessFifteenFar
	.thumb_func
ObjectGroup_SetChildValueUnlessFifteenFar:
	overlay_veneer 0x0800b685
	.global ScriptObject_CheckOverlapFar
	.thumb_func
ScriptObject_CheckOverlapFar:
	overlay_veneer 0x0800d925
	overlay_veneer 0x080108c5
	.global ResourceMetadata_SumCommandLengthsFar
	.global Func_08009260
	.thumb_func
ResourceMetadata_SumCommandLengthsFar:
Func_08009260:
	overlay_veneer 0x0800be21
	.global Func_08009268
	.thumb_func
Func_08009268:
	overlay_veneer 0x080122c9
	.global ResourceSlot_LoadFar
	.thumb_func
ResourceSlot_LoadFar:
	overlay_veneer 0x0800b6b9
	overlay_veneer 0x08012079
	.global Func_08009280
	.thumb_func
Func_08009280:
	overlay_veneer 0x0800baf9
	.global Runtime_RemapBytesByTableFar
	.thumb_func
Runtime_RemapBytesByTableFar:
	overlay_veneer 0x0800f9cd
	.global Map_EnableUpdateCallbackFar
	.thumb_func
Map_EnableUpdateCallbackFar:
	overlay_veneer 0x08011985
	.global Map_DisableUpdateCallbackFar
	.thumb_func
Map_DisableUpdateCallbackFar:
	overlay_veneer 0x080119a9
	overlay_veneer 0x0800d98d
	overlay_veneer 0x08011fd9
	overlay_veneer 0x08012519
