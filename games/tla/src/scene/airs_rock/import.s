.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02003af0
	.thumb_func
Func_02003af0:
	overlay_veneer Resource_DecodeType01Far
	.global Func_02003af8
	.thumb_func
Func_02003af8:
	overlay_veneer DebugText_ClearFar
	.global Func_02003b00
	.thumb_func
Func_02003b00:
	overlay_veneer Window_LoadGraphicsFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
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
	.global Func_02003b28
	.thumb_func
Func_02003b28:
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_02003b38
	.thumb_func
Func_02003b38:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02003b48
	.thumb_func
Func_02003b48:
	overlay_veneer Actor_Destroy
	.global Func_02003b50
	.thumb_func
Func_02003b50:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02003b58
	.thumb_func
Func_02003b58:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02003b60
	.thumb_func
Func_02003b60:
	overlay_veneer Actor_CommitPosition
	.global Func_02003b68
	.thumb_func
Func_02003b68:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02003b70
	.thumb_func
Func_02003b70:
	overlay_veneer Func_08020198
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02003b80
	.thumb_func
Func_02003b80:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003b98
	.thumb_func
Func_02003b98:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02003ba0
	.thumb_func
Func_02003ba0:
	overlay_veneer Func_08020230
	.global Func_02003ba8
	.thumb_func
Func_02003ba8:
	overlay_veneer Func_080202f8
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_02003bb8
	.thumb_func
Func_02003bb8:
	overlay_veneer Func_08038348
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02003bc8
	.thumb_func
Func_02003bc8:
	overlay_veneer Event_StartFar
	.global Func_02003bd0
	.thumb_func
Func_02003bd0:
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
	.global Func_02003bf8
	.thumb_func
Func_02003bf8:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02003c20
	.thumb_func
Func_02003c20:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003c30
	.thumb_func
Func_02003c30:
	overlay_veneer Func_080c8240
	.global Func_02003c38
	.thumb_func
Func_02003c38:
	overlay_veneer Func_080c8248
	.global Func_02003c40
	.thumb_func
Func_02003c40:
	overlay_veneer Func_080c8258
	.global Func_02003c48
	.thumb_func
Func_02003c48:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02003c50
	.thumb_func
Func_02003c50:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02003c58
	.thumb_func
Func_02003c58:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02003c60
	.thumb_func
Func_02003c60:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
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
	.global Func_02003c80
	.thumb_func
Func_02003c80:
	overlay_veneer Func_080c8480
	.global Func_02003c88
	.thumb_func
Func_02003c88:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02003c90
	.thumb_func
Func_02003c90:
	overlay_veneer Func_080c8518
	.global Func_02003c98
	.thumb_func
Func_02003c98:
	overlay_veneer Event_NoOpFar
	.global Func_02003ca0
	.thumb_func
Func_02003ca0:
	overlay_veneer Func_080c86f8
	.global Func_02003ca8
	.thumb_func
Func_02003ca8:
	overlay_veneer Func_080c8708
	.global Func_02003cb0
	.thumb_func
Func_02003cb0:
	overlay_veneer Func_080c8718
	.global Func_02003cb8
	.thumb_func
Func_02003cb8:
	overlay_veneer Func_080c8720
	.global Func_02003cc0
	.thumb_func
Func_02003cc0:
	overlay_veneer Func_080c8728
	.global Func_02003cc8
	.thumb_func
Func_02003cc8:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02003cd0
	.thumb_func
Func_02003cd0:
	overlay_veneer Func_080c88c8
	.global Func_02003cd8
	.thumb_func
Func_02003cd8:
	overlay_veneer Sound_PlayFar
