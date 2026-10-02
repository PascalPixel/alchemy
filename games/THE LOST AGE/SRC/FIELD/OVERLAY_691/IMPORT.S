@ Import veneers after the OVERLAY_691 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_691 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Func_02003a14
	.thumb_func
Func_02003a14:
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
	.global Func_02003a54
	.thumb_func
Func_02003a54:
	overlay_veneer Object_SetMode
	.global Func_02003a5c
	.thumb_func
Func_02003a5c:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02003a64
	.thumb_func
Func_02003a64:
	overlay_veneer Func_080200c0
	.global Func_02003a6c
	.thumb_func
Func_02003a6c:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003a7c
	.thumb_func
Func_02003a7c:
	overlay_veneer Func_08020120
	.global Func_02003a84
	.thumb_func
Func_02003a84:
	overlay_veneer Object_SetPosition
	.global Func_02003a8c
	.thumb_func
Func_02003a8c:
	overlay_veneer Object_CommitPosition
	.global Func_02003a94
	.thumb_func
Func_02003a94:
	overlay_veneer Func_08020178
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02003aa4
	.thumb_func
Func_02003aa4:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201b8
	.else
	overlay_veneer Func_080201e8
	.endif
	.endif
	.endif
	.endif
	.global Func_02003aac
	.thumb_func
Func_02003aac:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003ac4
	.thumb_func
Func_02003ac4:
	overlay_veneer Func_08020228
	.global Func_02003acc
	.thumb_func
Func_02003acc:
	overlay_veneer Func_08020230
	.global Func_02003ad4
	.thumb_func
Func_02003ad4:
	overlay_veneer Func_080202c0
	.global Func_02003adc
	.thumb_func
Func_02003adc:
	overlay_veneer Func_080202f0
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02003aec
	.thumb_func
Func_02003aec:
	overlay_veneer Func_080c8018
	.global Func_02003af4
	.thumb_func
Func_02003af4:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global Func_02003b0c
	.thumb_func
Func_02003b0c:
	overlay_veneer Func_080c80f8
	.global Func_02003b14
	.thumb_func
Func_02003b14:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetActionById
	.thumb_func
Object_SetActionById:
	overlay_veneer Object_SetActionByIdFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02003b3c
	.thumb_func
Func_02003b3c:
	overlay_veneer Func_080c8208
	.global Func_02003b44
	.thumb_func
Func_02003b44:
	overlay_veneer Func_080c8218
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Func_02003b5c
	.thumb_func
Func_02003b5c:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02003b6c
	.thumb_func
Func_02003b6c:
	overlay_veneer Func_080c8240
	.global Func_02003b74
	.thumb_func
Func_02003b74:
	overlay_veneer Func_080c8250
	.global Func_02003b7c
	.thumb_func
Func_02003b7c:
	overlay_veneer Func_080c8278
	.global Func_02003b84
	.thumb_func
Func_02003b84:
	overlay_veneer Func_080c8288
	.global Func_02003b8c
	.thumb_func
Func_02003b8c:
	overlay_veneer Func_080c82e0
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
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
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02003bac
	.thumb_func
Func_02003bac:
	overlay_veneer Func_080c84a0
	.global Func_02003bb4
	.thumb_func
Func_02003bb4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	overlay_veneer Func_080c84a8
	.endif
	.endif
	.global Func_02003bbc
	.thumb_func
Func_02003bbc:
	overlay_veneer Func_080c84b0
	.global Func_02003bc4
	.thumb_func
Func_02003bc4:
	overlay_veneer Func_080c84b8
	.global Func_02003bcc
	.thumb_func
Func_02003bcc:
	overlay_veneer Func_080c84e0
	.global Func_02003bd4
	.thumb_func
Func_02003bd4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8508
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8508
	.else
	overlay_veneer Func_080c8518
	.endif
	.endif
	.global Func_02003bdc
	.thumb_func
Func_02003bdc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetActionCallbackFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetActionCallbackFar
	.else
	overlay_veneer Func_080c8690
	.endif
	.endif
	.global Func_02003be4
	.thumb_func
Func_02003be4:
	overlay_veneer Func_080c86a8
	.global Func_02003bec
	.thumb_func
Func_02003bec:
	overlay_veneer Func_080c86d0
	.global Func_02003bf4
	.thumb_func
Func_02003bf4:
	overlay_veneer Func_080c86d8
	.global Func_02003bfc
	.thumb_func
Func_02003bfc:
	overlay_veneer Func_080c86e8
	.global Func_02003c04
	.thumb_func
Func_02003c04:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_02003c0c
	.thumb_func
Func_02003c0c:
	overlay_veneer Func_080c8718
	.global Func_02003c14
	.thumb_func
Func_02003c14:
	overlay_veneer Func_080c8720
	.global Func_02003c1c
	.thumb_func
Func_02003c1c:
	overlay_veneer Func_080c8728
	.global Func_02003c24
	.thumb_func
Func_02003c24:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8720
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8720
	.else
	overlay_veneer Func_080c8730
	.endif
	.endif
	.global Func_02003c2c
	.thumb_func
Func_02003c2c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8728
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8728
	.else
	overlay_veneer Func_080c8738
	.endif
	.endif
	.global Func_02003c34
	.thumb_func
Func_02003c34:
	overlay_veneer Func_080c87c0
	.global Func_02003c3c
	.thumb_func
Func_02003c3c:
	overlay_veneer Func_080c87d0
	.global Func_02003c44
	.thumb_func
Func_02003c44:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87d0
	.else
	overlay_veneer Func_080c87e0
	.endif
	.endif
	.global Func_02003c4c
	.thumb_func
Func_02003c4c:
	overlay_veneer Func_080c88c8
	.global Func_02003c54
	.thumb_func
Func_02003c54:
	overlay_veneer Audio_PlayCue
