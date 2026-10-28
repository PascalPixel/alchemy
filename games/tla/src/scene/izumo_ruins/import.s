.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
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
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02001428
	.thumb_func
Func_02001428:
	overlay_veneer Actor_SetMode
	.global Func_02001430
	.thumb_func
Func_02001430:
	overlay_veneer Actor_StartScriptFar
	.global Func_02001438
	.thumb_func
Func_02001438:
	overlay_veneer Func_080200c0
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02001448
	.thumb_func
Func_02001448:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02001450
	.thumb_func
Func_02001450:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02001460
	.thumb_func
Func_02001460:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02001470
	.thumb_func
Func_02001470:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02001478
	.thumb_func
Func_02001478:
	overlay_veneer Func_080202f0
	.global Func_02001480
	.thumb_func
Func_02001480:
	overlay_veneer Func_08020360
	.global Func_02001488
	.thumb_func
Func_02001488:
	overlay_veneer Event_StartFar
	.global Func_02001490
	.thumb_func
Func_02001490:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Func_020014a0
	.thumb_func
Func_020014a0:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_020014a8
	.thumb_func
Func_020014a8:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_020014c0
	.thumb_func
Func_020014c0:
	overlay_veneer Func_080c8208
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_020014d8
	.thumb_func
Func_020014d8:
	overlay_veneer Func_080c82e0
	.global Func_020014e0
	.thumb_func
Func_020014e0:
	overlay_veneer Func_080c8480
	.global Func_020014e8
	.thumb_func
Func_020014e8:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_020014f0
	.thumb_func
Func_020014f0:
	overlay_veneer Func_080c8518
	.global Func_020014f8
	.thumb_func
Func_020014f8:
	overlay_veneer Func_080c86f8
	.global Func_02001500
	.thumb_func
Func_02001500:
	overlay_veneer Func_080c8708
	.global Func_02001508
	.thumb_func
Func_02001508:
	overlay_veneer Func_080c8710
	.global Func_02001510
	.thumb_func
Func_02001510:
	overlay_veneer Func_080c8718
	.global Func_02001518
	.thumb_func
Func_02001518:
	overlay_veneer Func_080c8720
	.global Func_02001520
	.thumb_func
Func_02001520:
	overlay_veneer Func_080c8728
	.global Func_02001528
	.thumb_func
Func_02001528:
	overlay_veneer Func_080c8730
	.global Func_02001530
	.thumb_func
Func_02001530:
	overlay_veneer Func_080c8738
	.global Func_02001538
	.thumb_func
Func_02001538:
	overlay_veneer Sound_PlayFar
