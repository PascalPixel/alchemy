@ Import veneers after the OVERLAY_6AA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6AA overlay import veneers
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
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
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
	.global Func_02004b80
	.thumb_func
Func_02004b80:
	overlay_veneer Func_080001e8
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global ResourceMetadata_ClearRecord
	.thumb_func
ResourceMetadata_ClearRecord:
	.ifdef TLA_EDITION_DE
	overlay_veneer GetBattleEffectObject
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer GetBattleEffectObject
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer GetBattleEffectObject
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer GetBattleEffectObject
	.else
	overlay_veneer ResourceMetadata_ClearRecordFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02004bc0
	.thumb_func
Func_02004bc0:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02004bd0
	.thumb_func
Func_02004bd0:
	overlay_veneer Object_SetCallback
	.global Func_02004bd8
	.thumb_func
Func_02004bd8:
	overlay_veneer Func_080200c0
	.global Func_02004be0
	.thumb_func
Func_02004be0:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02004bf0
	.thumb_func
Func_02004bf0:
	overlay_veneer Func_08020120
	.global Func_02004bf8
	.thumb_func
Func_02004bf8:
	overlay_veneer Object_SetPosition
	.global Func_02004c00
	.thumb_func
Func_02004c00:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02004c10
	.thumb_func
Func_02004c10:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020198
	.else
	overlay_veneer Func_080201c8
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c18
	.thumb_func
Func_02004c18:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080201a8
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201a8
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201a8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201a8
	.else
	overlay_veneer Func_080201d8
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c20
	.thumb_func
Func_02004c20:
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
	.global Func_02004c28
	.thumb_func
Func_02004c28:
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
	.global Func_02004c30
	.thumb_func
Func_02004c30:
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
	.global Func_02004c38
	.thumb_func
Func_02004c38:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004c50
	.thumb_func
Func_02004c50:
	overlay_veneer Func_08020228
	.global Func_02004c58
	.thumb_func
Func_02004c58:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02004c68
	.thumb_func
Func_02004c68:
	overlay_veneer Func_08020290
	.global Func_02004c70
	.thumb_func
Func_02004c70:
	overlay_veneer Func_080202f8
	.global Func_02004c78
	.thumb_func
Func_02004c78:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020318
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020318
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020318
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020318
	.else
	overlay_veneer Func_08020348
	.endif
	.endif
	.endif
	.endif
	.global Func_02004c80
	.thumb_func
Func_02004c80:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020330
	.else
	overlay_veneer Func_08020360
	.endif
	.endif
	.global Func_02004c88
	.thumb_func
Func_02004c88:
	overlay_veneer Func_08020390
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02004ca8
	.thumb_func
Func_02004ca8:
	overlay_veneer Func_080c8018
	.global Func_02004cb0
	.thumb_func
Func_02004cb0:
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
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global Func_02004cd8
	.thumb_func
Func_02004cd8:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02004ce8
	.thumb_func
Func_02004ce8:
	overlay_veneer Func_080c8150
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02004d00
	.thumb_func
Func_02004d00:
	overlay_veneer Func_080c8210
	.global Func_02004d08
	.thumb_func
Func_02004d08:
	overlay_veneer Func_080c8218
	.global Func_02004d10
	.thumb_func
Func_02004d10:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02004d20
	.thumb_func
Func_02004d20:
	overlay_veneer Func_080c8240
	.global Func_02004d28
	.thumb_func
Func_02004d28:
	overlay_veneer Func_080c8258
	.global Func_02004d30
	.thumb_func
Func_02004d30:
	overlay_veneer Func_080c8278
	.global Func_02004d38
	.thumb_func
Func_02004d38:
	overlay_veneer Func_080c8288
	.global Func_02004d40
	.thumb_func
Func_02004d40:
	overlay_veneer Func_080c82e0
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	overlay_veneer Func_080c82f8
	.global Func_02004d50
	.thumb_func
Func_02004d50:
	overlay_veneer Func_080c8350
	.global Func_02004d58
	.thumb_func
Func_02004d58:
	overlay_veneer Func_080c8378
	.global Func_02004d60
	.thumb_func
Func_02004d60:
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
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_SpawnObjectTableFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_SpawnObjectTableFar
	.else
	overlay_veneer ObjectTable_GetFar
	.endif
	.endif
	.global Func_02004d88
	.thumb_func
Func_02004d88:
	overlay_veneer Func_080c8480
	.global BattleFx_PrepareBufferInterpolation
	.thumb_func
BattleFx_PrepareBufferInterpolation:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer BattleFx_PrepareBufferInterpolationFar
	.endif
	.endif
	.global Func_02004d98
	.thumb_func
Func_02004d98:
	overlay_veneer Func_080c84d8
	.global Func_02004da0
	.thumb_func
Func_02004da0:
	overlay_veneer Func_080c84e0
	.global Func_02004da8
	.thumb_func
Func_02004da8:
	overlay_veneer Func_080c8548
	.global Func_02004db0
	.thumb_func
Func_02004db0:
	overlay_veneer Func_080c8628
	.global Func_02004db8
	.thumb_func
Func_02004db8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_02004dc0
	.thumb_func
Func_02004dc0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_02004dc8
	.thumb_func
Func_02004dc8:
	overlay_veneer Func_080c88d8
	.global Func_02004dd0
	.thumb_func
Func_02004dd0:
	overlay_veneer Func_080c88e8
	.global Func_02004dd8
	.thumb_func
Func_02004dd8:
	overlay_veneer Func_080c8918
	.global Func_02004de0
	.thumb_func
Func_02004de0:
	overlay_veneer Audio_PlayCue
