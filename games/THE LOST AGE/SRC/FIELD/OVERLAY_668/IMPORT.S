@ Import veneers after the OVERLAY_668 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_668 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
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
	.global Func_020049d4
	.thumb_func
Func_020049d4:
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
	.global Func_02004a0c
	.thumb_func
Func_02004a0c:
	overlay_veneer Object_SetMode
	.global Func_02004a14
	.thumb_func
Func_02004a14:
	overlay_veneer Object_SetCallback
	.global Func_02004a1c
	.thumb_func
Func_02004a1c:
	overlay_veneer Func_080200c0
	.global Func_02004a24
	.thumb_func
Func_02004a24:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02004a34
	.thumb_func
Func_02004a34:
	overlay_veneer Object_SetPosition
	.global Func_02004a3c
	.thumb_func
Func_02004a3c:
	overlay_veneer Object_CommitPosition
	.global Func_02004a44
	.thumb_func
Func_02004a44:
	overlay_veneer Func_08020170
	.global Func_02004a4c
	.thumb_func
Func_02004a4c:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080201b0
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201b0
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201b0
	.else
	overlay_veneer Func_080201e0
	.endif
	.endif
	.endif
	.endif
	.global Func_02004a54
	.thumb_func
Func_02004a54:
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
	.global Func_02004a5c
	.thumb_func
Func_02004a5c:
	.ifdef TLA_EDITION_DE
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Map_GetTerrainHeightFar
	.else
	overlay_veneer Func_080201f0
	.endif
	.endif
	.endif
	.endif
	.global Func_02004a64
	.thumb_func
Func_02004a64:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02004a74
	.thumb_func
Func_02004a74:
	overlay_veneer Func_08020290
	.global Func_02004a7c
	.thumb_func
Func_02004a7c:
	overlay_veneer Func_080202f8
	.global Func_02004a84
	.thumb_func
Func_02004a84:
	overlay_veneer Func_080ad2e8
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02004a94
	.thumb_func
Func_02004a94:
	overlay_veneer Func_080c8018
	.global Func_02004a9c
	.thumb_func
Func_02004a9c:
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
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
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
	.global Func_02004b04
	.thumb_func
Func_02004b04:
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
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_02004b3c
	.thumb_func
Func_02004b3c:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02004b4c
	.thumb_func
Func_02004b4c:
	overlay_veneer Func_080c8198
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02004b5c
	.thumb_func
Func_02004b5c:
	overlay_veneer Func_080c81d8
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02004b6c
	.thumb_func
Func_02004b6c:
	overlay_veneer Func_080c8210
	.global Func_02004b74
	.thumb_func
Func_02004b74:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Func_02004b84
	.thumb_func
Func_02004b84:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02004b94
	.thumb_func
Func_02004b94:
	overlay_veneer Func_080c8240
	.global Func_02004b9c
	.thumb_func
Func_02004b9c:
	overlay_veneer Func_080c8250
	.global Func_02004ba4
	.thumb_func
Func_02004ba4:
	overlay_veneer Event_SetPairWork1c0Far
	.global Func_02004bac
	.thumb_func
Func_02004bac:
	overlay_veneer Func_080c8278
	.global Func_02004bb4
	.thumb_func
Func_02004bb4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	overlay_veneer Func_080c82c0
	.endif
	.endif
	.global Func_02004bbc
	.thumb_func
Func_02004bbc:
	overlay_veneer Func_080c8378
	.global Func_02004bc4
	.thumb_func
Func_02004bc4:
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
	.global Func_02004be4
	.thumb_func
Func_02004be4:
	overlay_veneer Func_080c84e0
	.global Func_02004bec
	.thumb_func
Func_02004bec:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02004c0c
	.thumb_func
Func_02004c0c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetActionCallbackFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetActionCallbackFar
	.else
	overlay_veneer Func_080c8690
	.endif
	.endif
	.global Func_02004c14
	.thumb_func
Func_02004c14:
	overlay_veneer Func_080c86a8
	.global Func_02004c1c
	.thumb_func
Func_02004c1c:
	overlay_veneer Func_080c86d0
	.global Func_02004c24
	.thumb_func
Func_02004c24:
	overlay_veneer Func_080c86d8
	.global Func_02004c2c
	.thumb_func
Func_02004c2c:
	overlay_veneer Func_080c86e8
	.global Func_02004c34
	.thumb_func
Func_02004c34:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_02004c3c
	.thumb_func
Func_02004c3c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8728
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8728
	.else
	overlay_veneer Func_080c8738
	.endif
	.endif
	.global Func_02004c44
	.thumb_func
Func_02004c44:
	overlay_veneer Func_080c8740
	.global Func_02004c4c
	.thumb_func
Func_02004c4c:
	overlay_veneer Func_080c8748
	.global Func_02004c54
	.thumb_func
Func_02004c54:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8798
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8798
	.else
	overlay_veneer Func_080c87a8
	.endif
	.endif
	.global Func_02004c5c
	.thumb_func
Func_02004c5c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87a8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87a8
	.else
	overlay_veneer Func_080c87b8
	.endif
	.endif
	.global Func_02004c64
	.thumb_func
Func_02004c64:
	overlay_veneer Func_080c87c0
	.global Func_02004c6c
	.thumb_func
Func_02004c6c:
	overlay_veneer Func_080c87d0
	.global Func_02004c74
	.thumb_func
Func_02004c74:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87d0
	.else
	overlay_veneer Func_080c87e0
	.endif
	.endif
	.global Func_02004c7c
	.thumb_func
Func_02004c7c:
	overlay_veneer Func_080c8840
	.global Func_02004c84
	.thumb_func
Func_02004c84:
	overlay_veneer Func_080c8848
	.global Func_02004c8c
	.thumb_func
Func_02004c8c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8858
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8858
	.else
	overlay_veneer Func_080c8868
	.endif
	.endif
	.global Func_02004c94
	.thumb_func
Func_02004c94:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8860
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8860
	.else
	overlay_veneer Func_080c8870
	.endif
	.endif
	.global Func_02004c9c
	.thumb_func
Func_02004c9c:
	overlay_veneer Audio_PlayCue
