@ Import veneers after the OVERLAY_673 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_673 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_0200113c
	.thumb_func
Func_0200113c:
	overlay_veneer Func_080003c8
	.global Func_02001144
	.thumb_func
Func_02001144:
	overlay_veneer Func_080003d0
	.global Func_0200114c
	.thumb_func
Func_0200114c:
	overlay_veneer Object_SetMode
	.global Func_02001154
	.thumb_func
Func_02001154:
	overlay_veneer Object_SetCallback
	.global Func_0200115c
	.thumb_func
Func_0200115c:
	overlay_veneer Func_080200c0
	.global Func_02001164
	.thumb_func
Func_02001164:
	overlay_veneer Func_080201e8
	.global Func_0200116c
	.thumb_func
Func_0200116c:
	overlay_veneer Func_080201f0
	.global Func_02001174
	.thumb_func
Func_02001174:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_0200118c
	.thumb_func
Func_0200118c:
	overlay_veneer Func_08038210
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_0200119c
	.thumb_func
Func_0200119c:
	overlay_veneer Func_080c8018
	.global Func_020011a4
	.thumb_func
Func_020011a4:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020011cc
	.thumb_func
Func_020011cc:
	overlay_veneer Func_080c80f8
	.global Func_020011d4
	.thumb_func
Func_020011d4:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020011e4
	.thumb_func
Func_020011e4:
	overlay_veneer Func_080c8180
	.global Func_020011ec
	.thumb_func
Func_020011ec:
	overlay_veneer Func_080c81a0
	.global Func_020011f4
	.thumb_func
Func_020011f4:
	overlay_veneer Func_080c81d0
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	overlay_veneer Func_080c8210
	.global Func_02001204
	.thumb_func
Func_02001204:
	overlay_veneer Func_080c8218
	.global Func_0200120c
	.thumb_func
Func_0200120c:
	overlay_veneer Func_080c8238
	.global Func_02001214
	.thumb_func
Func_02001214:
	overlay_veneer Func_080c8248
	.global Func_0200121c
	.thumb_func
Func_0200121c:
	overlay_veneer Func_080c8278
	.global Func_02001224
	.thumb_func
Func_02001224:
	overlay_veneer Func_080c8280
	.global Func_0200122c
	.thumb_func
Func_0200122c:
	overlay_veneer Func_080c8288
	.global Func_02001234
	.thumb_func
Func_02001234:
	overlay_veneer Func_080c8368
	.global Func_0200123c
	.thumb_func
Func_0200123c:
	overlay_veneer Func_080c8378
	.global Func_02001244
	.thumb_func
Func_02001244:
	overlay_veneer Func_080c8380
	.global Func_0200124c
	.thumb_func
Func_0200124c:
	overlay_veneer Func_080c8390
	.global Func_02001254
	.thumb_func
Func_02001254:
	overlay_veneer Func_080c8480
	.global Func_0200125c
	.thumb_func
Func_0200125c:
	overlay_veneer Func_080c84e0
	.global Func_02001264
	.thumb_func
Func_02001264:
	overlay_veneer Func_080c8570
	.global Func_0200126c
	.thumb_func
Func_0200126c:
	overlay_veneer Func_080c8880
	.global Func_02001274
	.thumb_func
Func_02001274:
	overlay_veneer Func_080c88d0
	.global Func_0200127c
	.thumb_func
Func_0200127c:
	overlay_veneer Func_080c8948
	.global Func_02001284
	.thumb_func
Func_02001284:
	overlay_veneer Audio_PlayCue
