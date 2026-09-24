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
	.irp target, 0x0800bb21, 0x0800b169, 0x0800b389, 0x0800b7c1
	overlay_veneer \target
	.endr
	.global AnimationObjects_SelectAnimationFar
	.global Func_08009020
	.thumb_func
AnimationObjects_SelectAnimationFar:
Func_08009020:
	.irp target, 0x0800ba31, 0x0800bacd
	overlay_veneer \target
	.endr
	.global ResourceObject_CreateFar
	.global Func_08009030
	.thumb_func
ResourceObject_CreateFar:
Func_08009030:
	.irp target, 0x0800bc71, 0x0800bdd5, 0x0800b075
	overlay_veneer \target
	.endr
	.global ResourceMetadata_RegisterFar
	.global Func_08009048
	.thumb_func
ResourceMetadata_RegisterFar:
Func_08009048:
	.irp target, 0x0800b8ad, 0x0800b93d, 0x0800b9a5
	overlay_veneer \target
	.endr
	.global ResourceMetadata_ClearRecordFar
	.global Func_08009060
	.thumb_func
ResourceMetadata_ClearRecordFar:
Func_08009060:
	.irp target, 0x0800bc49, 0x0800b869
	overlay_veneer \target
	.endr
	.global Animation_SetWorkEntryFar
	.global Func_08009070
	.thumb_func
Animation_SetWorkEntryFar:
Func_08009070:
	.irp target, 0x0800b9f5, 0x0800c005, 0x0800c301, 0x0800c345
	overlay_veneer \target
	.endr
	.irp target, 0x0800c389
	overlay_veneer \target
	.endr
	.global ObjectDispatch_InitializeFar
	.global Func_08009098
	.thumb_func
ObjectDispatch_InitializeFar:
Func_08009098:
	.irp target, 0x0800c2d9, 0x0800c48d, 0x0800c49d, 0x0800c47d
	overlay_veneer \target
	.endr
	.irp target, 0x0800c46d, 0x0800c0cd
	overlay_veneer \target
	.endr
	.global Object_CreateFar
	.global Func_080090c8
	.thumb_func
Object_CreateFar:
Func_080090c8:
	.irp target, 0x0800c151, 0x0800c0f5, 0x0800c3ed
	overlay_veneer \target
	.endr
	.global ObjectDispatch_InitFromTable4WithArgumentFar
	.global Func_080090e0
	.thumb_func
ObjectDispatch_InitFromTable4WithArgumentFar:
Func_080090e0:
	.irp target, 0x0800c4bd, 0x0800c455
	overlay_veneer \target
	.endr
	.global Object_SetPositionAndResetMotionFar
	.global Func_080090f0
	.thumb_func
Object_SetPositionAndResetMotionFar:
Func_080090f0:
	.irp target, 0x0800d131, 0x0800be71, 0x0800befd, 0x0800bf35
	overlay_veneer \target
	.endr
	.irp target, 0x0800fb39, 0x080109e9, 0x08010231
	overlay_veneer \target
	.endr
	.global Map_ApplyWorkOriginAndSpanFar
	.global Func_08009128
	.thumb_func
Map_ApplyWorkOriginAndSpanFar:
Func_08009128:
	.irp target, 0x0800fe9d, 0x080113e5, 0x08010e15, 0x0800c4ad
	overlay_veneer \target
	.endr
	.irp target, 0x0800c4ed, 0x0800d14d, 0x0800ca6d, 0x0800bfa5
	overlay_veneer \target
	.endr
	.irp target, 0x080111b5, 0x0801173d, 0x08010561, 0x08010425
	overlay_veneer \target
	.endr
	.irp target, 0x080118a9, 0x080118c1, 0x08011af1, 0x08011ae1
	overlay_veneer \target
	.endr
	.global Map_GetTerrainHeightFar
	.global Func_080091a8
	.thumb_func
Map_GetTerrainHeightFar:
Func_080091a8:
	.irp target, 0x08011f55, 0x08012039, 0x080105d5, 0x08010705
	overlay_veneer \target
	.endr
	.irp target, 0x08010789, 0x08010d49
	overlay_veneer \target
	.endr
	.global Object_CheckMovementCollision
	.global Func_080091d8
	.thumb_func
Object_CheckMovementCollision:
Func_080091d8:
	.irp target, 0x080120dd, 0x0800c529, 0x0800c549
	overlay_veneer \target
	.endr
	.global Func_080091f0
	.thumb_func
Func_080091f0:
	.irp target, 0x08012331, 0x08012351, 0x0800c409, 0x0800c5b5
	overlay_veneer \target
	.endr
	.irp target, 0x0800c5fd, 0x0800c571
	overlay_veneer \target
	.endr
	.global CheckMapPositionCellOccupiedFar
	.global Func_08009220
	.thumb_func
CheckMapPositionCellOccupiedFar:
Func_08009220:
	.irp target, 0x0801219d, 0x0800c431, 0x08011591, 0x08011645
	overlay_veneer \target
	.endr
	.irp target, 0x0800c599, 0x0800b685, 0x0800d925, 0x080108c5
	overlay_veneer \target
	.endr
	.global ResourceMetadata_SumCommandLengthsFar
	.global Func_08009260
	.thumb_func
ResourceMetadata_SumCommandLengthsFar:
Func_08009260:
	.irp target, 0x0800be21, 0x080122c9, 0x0800b6b9, 0x08012079
	overlay_veneer \target
	.endr
	.global Func_08009280
	.thumb_func
Func_08009280:
	.irp target, 0x0800baf9, 0x0800f9cd, 0x08011985, 0x080119a9
	overlay_veneer \target
	.endr
	.irp target, 0x0800d98d, 0x08011fd9, 0x08012519
	overlay_veneer \target
	.endr
