.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
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
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_020010bc
	.thumb_func
Func_020010bc:
	overlay_veneer Object_SetPosition
	.global Func_020010c4
	.thumb_func
Func_020010c4:
	overlay_veneer Object_CommitPosition
	.global Func_020010cc
	.thumb_func
Func_020010cc:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_020010dc
	.thumb_func
Func_020010dc:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_020010ec
	.thumb_func
Func_020010ec:
	overlay_veneer Func_080202f0
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_0200110c
	.thumb_func
Func_0200110c:
	overlay_veneer EventRuntime_StartFar
	.global Func_02001114
	.thumb_func
Func_02001114:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Func_02001134
	.thumb_func
Func_02001134:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_0200113c
	.thumb_func
Func_0200113c:
	overlay_veneer ObjectMotion_MoveToObjectFar
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
	.global Func_0200115c
	.thumb_func
Func_0200115c:
	overlay_veneer Func_080c8208
	.global Func_02001164
	.thumb_func
Func_02001164:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001174
	.thumb_func
Func_02001174:
	overlay_veneer Func_080c8240
	.global Func_0200117c
	.thumb_func
Func_0200117c:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02001184
	.thumb_func
Func_02001184:
	overlay_veneer Func_080c82c0
	.global Func_0200118c
	.thumb_func
Func_0200118c:
	overlay_veneer BattleEffect_FindNearestObjectFar
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
	.global Func_020011ac
	.thumb_func
Func_020011ac:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_020011b4
	.thumb_func
Func_020011b4:
	overlay_veneer Func_080c86a8
	.global Func_020011bc
	.thumb_func
Func_020011bc:
	overlay_veneer Func_080c86e8
	.global Func_020011c4
	.thumb_func
Func_020011c4:
	overlay_veneer Func_080c86f8
	.global Func_020011cc
	.thumb_func
Func_020011cc:
	overlay_veneer Func_080c8708
	.global Func_020011d4
	.thumb_func
Func_020011d4:
	overlay_veneer Func_080c8710
	.global Func_020011dc
	.thumb_func
Func_020011dc:
	overlay_veneer Func_080c8718
	.global Func_020011e4
	.thumb_func
Func_020011e4:
	overlay_veneer Func_080c8720
	.global Func_020011ec
	.thumb_func
Func_020011ec:
	overlay_veneer Func_080c8728
	.global Func_020011f4
	.thumb_func
Func_020011f4:
	overlay_veneer Func_080c8730
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	overlay_veneer Func_080c8740
	.global Func_02001204
	.thumb_func
Func_02001204:
	overlay_veneer Func_080c8748
	.global Func_0200120c
	.thumb_func
Func_0200120c:
	overlay_veneer Func_080c8750
	.global Func_02001214
	.thumb_func
Func_02001214:
	overlay_veneer Audio_PlayCue
