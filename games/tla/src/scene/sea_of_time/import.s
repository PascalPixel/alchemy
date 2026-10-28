.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
	overlay_veneer Func_080000d8
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
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02001f4c
	.thumb_func
Func_02001f4c:
	overlay_veneer Actor_SetMode
	.global Func_02001f54
	.thumb_func
Func_02001f54:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02001f64
	.thumb_func
Func_02001f64:
	overlay_veneer Actor_Destroy
	.global Func_02001f6c
	.thumb_func
Func_02001f6c:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02001f74
	.thumb_func
Func_02001f74:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02001f8c
	.thumb_func
Func_02001f8c:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02001f94
	.thumb_func
Func_02001f94:
	overlay_veneer Func_08020230
	.global Func_02001f9c
	.thumb_func
Func_02001f9c:
	overlay_veneer Func_080202f8
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001fac
	.thumb_func
Func_02001fac:
	overlay_veneer Event_StartFar
	.global Func_02001fb4
	.thumb_func
Func_02001fb4:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02001fdc
	.thumb_func
Func_02001fdc:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02001fec
	.thumb_func
Func_02001fec:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001ffc
	.thumb_func
Func_02001ffc:
	overlay_veneer Func_080c8240
	.global Func_02002004
	.thumb_func
Func_02002004:
	overlay_veneer Event_SetExitRequestFar
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Func_0200201c
	.thumb_func
Func_0200201c:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02002024
	.thumb_func
Func_02002024:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02002044
	.thumb_func
Func_02002044:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_0200204c
	.thumb_func
Func_0200204c:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02002054
	.thumb_func
Func_02002054:
	overlay_veneer Sound_PlayFar
