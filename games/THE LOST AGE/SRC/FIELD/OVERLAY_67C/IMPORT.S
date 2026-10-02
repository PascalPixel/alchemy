@ Import veneers after the OVERLAY_67C overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_67C overlay import veneers
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
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02003c7c
	.thumb_func
Func_02003c7c:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02003c94
	.thumb_func
Func_02003c94:
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
	.global Func_02003cc4
	.thumb_func
Func_02003cc4:
	overlay_veneer Object_SetMode
	.global Func_02003ccc
	.thumb_func
Func_02003ccc:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02003cd4
	.thumb_func
Func_02003cd4:
	overlay_veneer Func_080200c0
	.global Func_02003cdc
	.thumb_func
Func_02003cdc:
	overlay_veneer Func_08020120
	.global Func_02003ce4
	.thumb_func
Func_02003ce4:
	overlay_veneer Object_SetPosition
	.global Func_02003cec
	.thumb_func
Func_02003cec:
	overlay_veneer Object_CommitPosition
	.global Func_02003cf4
	.thumb_func
Func_02003cf4:
	overlay_veneer Func_08020178
	.global Func_02003cfc
	.thumb_func
Func_02003cfc:
	overlay_veneer Func_080201a8
	.global Func_02003d04
	.thumb_func
Func_02003d04:
	overlay_veneer Func_080201b0
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02003d14
	.thumb_func
Func_02003d14:
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
	.global Func_02003d1c
	.thumb_func
Func_02003d1c:
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
	.global Func_02003d24
	.thumb_func
Func_02003d24:
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
	.global Func_02003d2c
	.thumb_func
Func_02003d2c:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003d44
	.thumb_func
Func_02003d44:
	overlay_veneer Func_08020228
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02003d54
	.thumb_func
Func_02003d54:
	overlay_veneer Func_08020378
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02003d64
	.thumb_func
Func_02003d64:
	overlay_veneer UiText_DrawQuantity
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02003d7c
	.thumb_func
Func_02003d7c:
	overlay_veneer Func_080c8018
	.global Func_02003d84
	.thumb_func
Func_02003d84:
	overlay_veneer Func_080c8020
	.global UiText_DrawQuantityPairWithCue
	.thumb_func
UiText_DrawQuantityPairWithCue:
	overlay_veneer UiText_DrawQuantityPairWithCueFar
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
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02003ddc
	.thumb_func
Func_02003ddc:
	overlay_veneer Func_080c80f8
	.global Func_02003de4
	.thumb_func
Func_02003de4:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02003df4
	.thumb_func
Func_02003df4:
	overlay_veneer Func_080c8150
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02003e04
	.thumb_func
Func_02003e04:
	overlay_veneer Func_080c8168
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02003e14
	.thumb_func
Func_02003e14:
	overlay_veneer Func_080c8180
	.global Func_02003e1c
	.thumb_func
Func_02003e1c:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02003e34
	.thumb_func
Func_02003e34:
	overlay_veneer Func_080c8210
	.global Func_02003e3c
	.thumb_func
Func_02003e3c:
	overlay_veneer Func_080c8218
	.global Func_02003e44
	.thumb_func
Func_02003e44:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02003e54
	.thumb_func
Func_02003e54:
	overlay_veneer Func_080c8240
	.global Func_02003e5c
	.thumb_func
Func_02003e5c:
	overlay_veneer Func_080c8278
	.global Func_02003e64
	.thumb_func
Func_02003e64:
	overlay_veneer Func_080c8288
	.global Func_02003e6c
	.thumb_func
Func_02003e6c:
	overlay_veneer Func_080c82e0
	.global Func_02003e74
	.thumb_func
Func_02003e74:
	overlay_veneer Func_080c8378
	.global Func_02003e7c
	.thumb_func
Func_02003e7c:
	overlay_veneer Func_080c8380
	.global Func_02003e84
	.thumb_func
Func_02003e84:
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
	.global Func_02003ea4
	.thumb_func
Func_02003ea4:
	overlay_veneer Func_080c8480
	.global Func_02003eac
	.thumb_func
Func_02003eac:
	overlay_veneer Func_080c84e0
	.global Func_02003eb4
	.thumb_func
Func_02003eb4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_SnapScaleToFullFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_SnapScaleToFullFar
	.else
	overlay_veneer Func_080c8540
	.endif
	.endif
	.global Func_02003ebc
	.thumb_func
Func_02003ebc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8560
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8560
	.else
	overlay_veneer Func_080c8570
	.endif
	.endif
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_02003ecc
	.thumb_func
Func_02003ecc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8678
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8678
	.else
	overlay_veneer Func_080c8688
	.endif
	.endif
	.global Func_02003ed4
	.thumb_func
Func_02003ed4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetActionCallbackFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetActionCallbackFar
	.else
	overlay_veneer Func_080c8690
	.endif
	.endif
	.global Func_02003edc
	.thumb_func
Func_02003edc:
	overlay_veneer Func_080c86d0
	.global Func_02003ee4
	.thumb_func
Func_02003ee4:
	overlay_veneer Func_080c86d8
	.global Func_02003eec
	.thumb_func
Func_02003eec:
	overlay_veneer Func_080c8860
	.global Func_02003ef4
	.thumb_func
Func_02003ef4:
	overlay_veneer Audio_PlayCue
	.global AudioCommand_WaitForCompletion
	.thumb_func
AudioCommand_WaitForCompletion:
	overlay_veneer AudioCommand_WaitForCompletionFar
