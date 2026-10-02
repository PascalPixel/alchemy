@ Import veneers after the OVERLAY_64C overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_64C overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02001f54
	.thumb_func
Func_02001f54:
	overlay_veneer Func_080001a8
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02001f74
	.thumb_func
Func_02001f74:
	overlay_veneer Func_080001d8
	.global Func_02001f7c
	.thumb_func
Func_02001f7c:
	overlay_veneer Func_080001e8
	.global Render_ProjectPoint
	.thumb_func
Render_ProjectPoint:
	overlay_veneer Render_ProjectPointFar
	.global Func_02001f8c
	.thumb_func
Func_02001f8c:
	overlay_veneer Func_08000298
	.global QueueIoWriteDelay2
	.thumb_func
QueueIoWriteDelay2:
	overlay_veneer QueueIoWriteDelay2Far
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
	.global Animation_ApplyChildArgument
	.thumb_func
Animation_ApplyChildArgument:
	overlay_veneer Animation_ApplyChildArgumentFar
	.global Func_02001fbc
	.thumb_func
Func_02001fbc:
	overlay_veneer Object_SetMode
	.global Func_02001fc4
	.thumb_func
Func_02001fc4:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02001fcc
	.thumb_func
Func_02001fcc:
	overlay_veneer Func_080200c0
	.global Func_02001fd4
	.thumb_func
Func_02001fd4:
	overlay_veneer Object_Destroy
	.global Func_02001fdc
	.thumb_func
Func_02001fdc:
	overlay_veneer Func_08020128
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02001ff4
	.thumb_func
Func_02001ff4:
	overlay_veneer Func_08020228
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02002004
	.thumb_func
Func_02002004:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020320
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020320
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020320
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020320
	.else
	overlay_veneer Func_08020350
	.endif
	.endif
	.endif
	.endif
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	overlay_veneer Func_080ad310
	.global Func_02002014
	.thumb_func
Func_02002014:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080ad2f8
	.else
	overlay_veneer Func_080ad318
	.endif
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02002024
	.thumb_func
Func_02002024:
	overlay_veneer Func_080c8018
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_02002034
	.thumb_func
Func_02002034:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02002044
	.thumb_func
Func_02002044:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02002054
	.thumb_func
Func_02002054:
	overlay_veneer Event_SetPairWork1c0Far
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
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_SetStatus1c6Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_SetStatus1c6Far
	.else
	overlay_veneer Event_WaitValue1c8FramesFar
	.endif
	.endif
	.global Func_02002074
	.thumb_func
Func_02002074:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetWideSpriteFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetWideSpriteFar
	.else
	overlay_veneer Func_080c8430
	.endif
	.endif
	.global Func_0200207c
	.thumb_func
Func_0200207c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8428
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8428
	.else
	overlay_veneer Func_080c8438
	.endif
	.endif
	.global Func_02002084
	.thumb_func
Func_02002084:
	overlay_veneer Func_080c84e0
	.global Func_0200208c
	.thumb_func
Func_0200208c:
	overlay_veneer Func_080c8638
	.global Func_02002094
	.thumb_func
Func_02002094:
	overlay_veneer Func_080c86f0
	.global Func_0200209c
	.thumb_func
Func_0200209c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_020020a4
	.thumb_func
Func_020020a4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8958
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8958
	.else
	overlay_veneer Func_080c8960
	.endif
	.endif
	.global Func_020020ac
	.thumb_func
Func_020020ac:
	overlay_veneer Resource_FarCall00A + 1
	.global Func_020020b4
	.thumb_func
Func_020020b4:
	overlay_veneer Audio_PlayCue
