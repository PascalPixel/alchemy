@ Import veneers after the OVERLAY_679 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_679 overlay import veneers
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
	.global Func_02001bf4
	.thumb_func
Func_02001bf4:
	overlay_veneer Func_080000e0
	.global Func_02001bfc
	.thumb_func
Func_02001bfc:
	overlay_veneer Func_080000e8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Func_02001c0c
	.thumb_func
Func_02001c0c:
	overlay_veneer Func_08000130
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Func_02001c24
	.thumb_func
Func_02001c24:
	overlay_veneer Func_080001a0
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02001c3c
	.thumb_func
Func_02001c3c:
	overlay_veneer Func_080001e8
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
	.global ResourceMetadata_Register
	.thumb_func
ResourceMetadata_Register:
	.ifdef TLA_EDITION_DE
	overlay_veneer InitializeAnimationObjectsFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer InitializeAnimationObjectsFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer InitializeAnimationObjectsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer InitializeAnimationObjectsFar
	.else
	overlay_veneer ResourceMetadata_RegisterFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02001c6c
	.thumb_func
Func_02001c6c:
	overlay_veneer Object_SetMode
	.global Func_02001c74
	.thumb_func
Func_02001c74:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02001c7c
	.thumb_func
Func_02001c7c:
	overlay_veneer Func_080200c0
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	overlay_veneer Object_SetPosition
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02001c9c
	.thumb_func
Func_02001c9c:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	overlay_veneer Func_080c8018
	.global Func_02001ccc
	.thumb_func
Func_02001ccc:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_SnapHeadingAndOffset
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02001d04
	.thumb_func
Func_02001d04:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	overlay_veneer Func_080c8150
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001d44
	.thumb_func
Func_02001d44:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02001d54
	.thumb_func
Func_02001d54:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02001d6c
	.thumb_func
Func_02001d6c:
	overlay_veneer Func_080c8210
	.global Func_02001d74
	.thumb_func
Func_02001d74:
	overlay_veneer Func_080c8218
	.global Func_02001d7c
	.thumb_func
Func_02001d7c:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02001d8c
	.thumb_func
Func_02001d8c:
	overlay_veneer Func_080c8240
	.global Func_02001d94
	.thumb_func
Func_02001d94:
	overlay_veneer Func_080c8278
	.global Func_02001d9c
	.thumb_func
Func_02001d9c:
	overlay_veneer Func_080c82e0
	.global Func_02001da4
	.thumb_func
Func_02001da4:
	overlay_veneer Func_080c8378
	.global Func_02001dac
	.thumb_func
Func_02001dac:
	overlay_veneer Func_080c8390
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
	.global Func_02001dcc
	.thumb_func
Func_02001dcc:
	overlay_veneer Func_080c84e0
	.global Func_02001dd4
	.thumb_func
Func_02001dd4:
	overlay_veneer Func_080c8580
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_02001de4
	.thumb_func
Func_02001de4:
	overlay_veneer Func_080c8628
	.global Func_02001dec
	.thumb_func
Func_02001dec:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8678
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8678
	.else
	overlay_veneer Func_080c8688
	.endif
	.endif
	.global Func_02001df4
	.thumb_func
Func_02001df4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetActionCallbackFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetActionCallbackFar
	.else
	overlay_veneer Func_080c8690
	.endif
	.endif
	.global Func_02001dfc
	.thumb_func
Func_02001dfc:
	overlay_veneer Func_080c86a8
	.global Func_02001e04
	.thumb_func
Func_02001e04:
	overlay_veneer Func_080c86e8
	.global Func_02001e0c
	.thumb_func
Func_02001e0c:
	overlay_veneer Func_080c8760
	.global Func_02001e14
	.thumb_func
Func_02001e14:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_02001e1c
	.thumb_func
Func_02001e1c:
	overlay_veneer Func_08108008
	.global Func_02001e24
	.thumb_func
Func_02001e24:
	overlay_veneer Func_08108018
	.global Func_02001e2c
	.thumb_func
Func_02001e2c:
	overlay_veneer Audio_PlayCue
