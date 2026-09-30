@ Import veneers after the OVERLAY_69D overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_69D overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02001d38
	.thumb_func
Func_02001d38:
	overlay_veneer Func_080000d0
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
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02001d68
	.thumb_func
Func_02001d68:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02001d80
	.thumb_func
Func_02001d80:
	overlay_veneer Func_080001e8
	.global Func_02001d88
	.thumb_func
Func_02001d88:
	overlay_veneer Func_080003c8
	.global Func_02001d90
	.thumb_func
Func_02001d90:
	overlay_veneer Func_080003d0
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
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02001db8
	.thumb_func
Func_02001db8:
	overlay_veneer Object_SetCallback
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
	overlay_veneer Func_08020120
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
	overlay_veneer Func_080201e0
	.global Func_02001df8
	.thumb_func
Func_02001df8:
	overlay_veneer Func_080201e8
	.global Func_02001e00
	.thumb_func
Func_02001e00:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02001e10
	.thumb_func
Func_02001e10:
	overlay_veneer Func_08020228
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
	.global Func_02001e48
	.thumb_func
Func_02001e48:
	overlay_veneer Func_080c8018
	.global Func_02001e50
	.thumb_func
Func_02001e50:
	overlay_veneer Func_080c8020
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
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02001e80
	.thumb_func
Func_02001e80:
	overlay_veneer Func_080c8170
	.global Func_02001e88
	.thumb_func
Func_02001e88:
	overlay_veneer Func_080c8200
	.global Func_02001e90
	.thumb_func
Func_02001e90:
	overlay_veneer Func_080c8218
	.global Func_02001e98
	.thumb_func
Func_02001e98:
	overlay_veneer Func_080c8228
	.global Func_02001ea0
	.thumb_func
Func_02001ea0:
	overlay_veneer Func_080c8238
	.global Func_02001ea8
	.thumb_func
Func_02001ea8:
	overlay_veneer Func_080c8258
	.global Func_02001eb0
	.thumb_func
Func_02001eb0:
	overlay_veneer Func_080c8278
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
	.global Func_02001ed0
	.thumb_func
Func_02001ed0:
	overlay_veneer Func_080c84e0
	.global Func_02001ed8
	.thumb_func
Func_02001ed8:
	overlay_veneer Func_080c85c0
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
	overlay_veneer Audio_PlayCue
