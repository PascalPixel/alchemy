@ Import veneers after the OVERLAY_65A overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_65A overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
	overlay_veneer Func_08020178
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_020010dc
	.thumb_func
Func_020010dc:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
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
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_0200110c
	.thumb_func
Func_0200110c:
	overlay_veneer Func_080c8018
	.global Func_02001114
	.thumb_func
Func_02001114:
	overlay_veneer Func_080c8020
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
	overlay_veneer Func_080c80f8
	.global Func_0200113c
	.thumb_func
Func_0200113c:
	overlay_veneer Func_080c8110
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
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02001174
	.thumb_func
Func_02001174:
	overlay_veneer Func_080c8240
	.global Func_0200117c
	.thumb_func
Func_0200117c:
	overlay_veneer Func_080c8278
	.global Func_02001184
	.thumb_func
Func_02001184:
	overlay_veneer Func_080c82c0
	.global Func_0200118c
	.thumb_func
Func_0200118c:
	overlay_veneer Func_080c82d8
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_020011ac
	.thumb_func
Func_020011ac:
	overlay_veneer Func_080c84e0
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
