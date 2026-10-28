.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
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
	.global MemAlloc
	.thumb_func
MemAlloc:
	overlay_veneer Runtime_BumpAllocateFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_02001d68
	.thumb_func
Func_02001d68:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02001d80
	.thumb_func
Func_02001d80:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_02001da8
	.thumb_func
Func_02001da8:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_02001db8
	.thumb_func
Func_02001db8:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02001dc0
	.thumb_func
Func_02001dc0:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02001dd0
	.thumb_func
Func_02001dd0:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02001dd8
	.thumb_func
Func_02001dd8:
	overlay_veneer Object_SetPosition
	.global Func_02001de0
	.thumb_func
Func_02001de0:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02001df0
	.thumb_func
Func_02001df0:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02001df8
	.thumb_func
Func_02001df8:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02001e00
	.thumb_func
Func_02001e00:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02001e10
	.thumb_func
Func_02001e10:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02001e18
	.thumb_func
Func_02001e18:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02001e28
	.thumb_func
Func_02001e28:
	overlay_veneer Func_08020360
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001e48
	.thumb_func
Func_02001e48:
	overlay_veneer EventRuntime_StartFar
	.global Func_02001e50
	.thumb_func
Func_02001e50:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global Func_02001e70
	.thumb_func
Func_02001e70:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02001e90
	.thumb_func
Func_02001e90:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001ea8
	.thumb_func
Func_02001ea8:
	overlay_veneer Func_080c8258
	.global Func_02001eb0
	.thumb_func
Func_02001eb0:
	overlay_veneer EventRuntime_SetCountdownFar
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
	.global Func_02001ed0
	.thumb_func
Func_02001ed0:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02001ed8
	.thumb_func
Func_02001ed8:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02001ee0
	.thumb_func
Func_02001ee0:
	overlay_veneer Func_080c87e8
	.global Func_02001ee8
	.thumb_func
Func_02001ee8:
	overlay_veneer Func_080c8830
	.global Func_02001ef0
	.thumb_func
Func_02001ef0:
	overlay_veneer Func_080c8840
	.global Func_02001ef8
	.thumb_func
Func_02001ef8:
	overlay_veneer Func_080c8848
	.global Func_02001f00
	.thumb_func
Func_02001f00:
	overlay_veneer Func_080c8888
	.global Func_02001f08
	.thumb_func
Func_02001f08:
	overlay_veneer Sound_PlayFar
