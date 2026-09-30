@ Import veneers after the OVERLAY_6A9 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6A9 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Func_0200105c
	.thumb_func
Func_0200105c:
	overlay_veneer Func_080003c8
	.global Func_02001064
	.thumb_func
Func_02001064:
	overlay_veneer Func_080003d0
	.global Func_0200106c
	.thumb_func
Func_0200106c:
	overlay_veneer Func_080003d8
	.global Func_02001074
	.thumb_func
Func_02001074:
	overlay_veneer Object_SetMode
	.global Func_0200107c
	.thumb_func
Func_0200107c:
	overlay_veneer Object_SetCallback
	.global Func_02001084
	.thumb_func
Func_02001084:
	overlay_veneer Func_080200c0
	.global Func_0200108c
	.thumb_func
Func_0200108c:
	overlay_veneer Func_080200c8
	.global Func_02001094
	.thumb_func
Func_02001094:
	overlay_veneer Func_08020178
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_020010a4
	.thumb_func
Func_020010a4:
	overlay_veneer Func_080201c8
	.global Func_020010ac
	.thumb_func
Func_020010ac:
	overlay_veneer Func_080201e8
	.global Func_020010b4
	.thumb_func
Func_020010b4:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_020010cc
	.thumb_func
Func_020010cc:
	overlay_veneer Func_08020360
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020010dc
	.thumb_func
Func_020010dc:
	overlay_veneer Func_080c8018
	.global Func_020010e4
	.thumb_func
Func_020010e4:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_0200110c
	.thumb_func
Func_0200110c:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_0200111c
	.thumb_func
Func_0200111c:
	overlay_veneer Func_080c8170
	.global Func_02001124
	.thumb_func
Func_02001124:
	overlay_veneer Func_080c81d0
	.global Func_0200112c
	.thumb_func
Func_0200112c:
	overlay_veneer Func_080c8228
	.global Func_02001134
	.thumb_func
Func_02001134:
	overlay_veneer Func_080c8278
	.global Func_0200113c
	.thumb_func
Func_0200113c:
	overlay_veneer Func_080c84e0
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_0200114c
	.thumb_func
Func_0200114c:
	overlay_veneer Func_080c86a8
	.global Func_02001154
	.thumb_func
Func_02001154:
	overlay_veneer Func_080c86e8
	.global Func_0200115c
	.thumb_func
Func_0200115c:
	overlay_veneer Func_080c8738
	.global Func_02001164
	.thumb_func
Func_02001164:
	overlay_veneer Audio_PlayCue
