.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
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
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02000e24
	.thumb_func
Func_02000e24:
	overlay_veneer Actor_SetMode
	.global Func_02000e2c
	.thumb_func
Func_02000e2c:
	overlay_veneer Actor_StartScriptFar
	.global Func_02000e34
	.thumb_func
Func_02000e34:
	overlay_veneer Func_080200c0
	.global Func_02000e3c
	.thumb_func
Func_02000e3c:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02000e44
	.thumb_func
Func_02000e44:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02000e54
	.thumb_func
Func_02000e54:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02000e5c
	.thumb_func
Func_02000e5c:
	overlay_veneer Actor_SetSpritePriorityDirectFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02000e74
	.thumb_func
Func_02000e74:
	overlay_veneer Event_StartFar
	.global Func_02000e7c
	.thumb_func
Func_02000e7c:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Func_02000e8c
	.thumb_func
Func_02000e8c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Func_02000ea4
	.thumb_func
Func_02000ea4:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02000eac
	.thumb_func
Func_02000eac:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Func_02000eb4
	.thumb_func
Func_02000eb4:
	overlay_veneer Func_080c81a8
	.global Func_02000ebc
	.thumb_func
Func_02000ebc:
	overlay_veneer Func_080c8218
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	overlay_veneer Func_080c8240
	.global Func_02000ed4
	.thumb_func
Func_02000ed4:
	overlay_veneer Func_080c8248
	.global Func_02000edc
	.thumb_func
Func_02000edc:
	overlay_veneer Func_080c8368
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02000ef4
	.thumb_func
Func_02000ef4:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Actor_Spawn
	.thumb_func
Actor_Spawn:
	overlay_veneer Actor_SpawnFar
	.global Func_02000f04
	.thumb_func
Func_02000f04:
	overlay_veneer Func_080c8760
	.global Func_02000f0c
	.thumb_func
Func_02000f0c:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02000f14
	.thumb_func
Func_02000f14:
	overlay_veneer Func_080c8780
	.global Func_02000f1c
	.thumb_func
Func_02000f1c:
	overlay_veneer Func_080c88c8
	.global Func_02000f24
	.thumb_func
Func_02000f24:
	overlay_veneer Sound_PlayFar
