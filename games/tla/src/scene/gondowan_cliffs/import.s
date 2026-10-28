.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __udivsi3
	.thumb_func
__udivsi3:
	overlay_veneer IwramUnsignedDivide
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
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
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02001184
	.thumb_func
Func_02001184:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_0200119c
	.thumb_func
Func_0200119c:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_020011b4
	.thumb_func
Func_020011b4:
	overlay_veneer Actor_SetMode
	.global Func_020011bc
	.thumb_func
Func_020011bc:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_020011d4
	.thumb_func
Func_020011d4:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_020011dc
	.thumb_func
Func_020011dc:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_020011f4
	.thumb_func
Func_020011f4:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	overlay_veneer Func_08020230
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_0200120c
	.thumb_func
Func_0200120c:
	overlay_veneer Event_StartFar
	.global Func_02001214
	.thumb_func
Func_02001214:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_0200124c
	.thumb_func
Func_0200124c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02001254
	.thumb_func
Func_02001254:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_0200128c
	.thumb_func
Func_0200128c:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02001294
	.thumb_func
Func_02001294:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_020012a4
	.thumb_func
Func_020012a4:
	overlay_veneer Func_080c8210
	.global Func_020012ac
	.thumb_func
Func_020012ac:
	overlay_veneer Func_080c8218
	.global Func_020012b4
	.thumb_func
Func_020012b4:
	overlay_veneer Event_SetDestinationFar
	.global Func_020012bc
	.thumb_func
Func_020012bc:
	overlay_veneer Event_SetExitRequestFar
	.global Func_020012c4
	.thumb_func
Func_020012c4:
	overlay_veneer Func_080c8288
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
	.global Func_020012e4
	.thumb_func
Func_020012e4:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_020012f4
	.thumb_func
Func_020012f4:
	overlay_veneer Func_080c86a8
	.global Func_020012fc
	.thumb_func
Func_020012fc:
	overlay_veneer Func_080c86d0
	.global Func_02001304
	.thumb_func
Func_02001304:
	overlay_veneer Func_080c86d8
	.global Func_0200130c
	.thumb_func
Func_0200130c:
	overlay_veneer Func_080c86e8
	.global Func_02001314
	.thumb_func
Func_02001314:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_0200131c
	.thumb_func
Func_0200131c:
	overlay_veneer Sound_PlayFar
