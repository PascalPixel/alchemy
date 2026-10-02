@ Import veneers after the OVERLAY_690 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_690 overlay import veneers
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
	.global Func_02001fa8
	.thumb_func
Func_02001fa8:
	overlay_veneer Func_08000130
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
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
	.global Func_02001fe0
	.thumb_func
Func_02001fe0:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02001ff0
	.thumb_func
Func_02001ff0:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02001ff8
	.thumb_func
Func_02001ff8:
	overlay_veneer Func_080200c0
	.global Func_02002000
	.thumb_func
Func_02002000:
	overlay_veneer Object_Destroy
	.global Func_02002008
	.thumb_func
Func_02002008:
	overlay_veneer Object_SetPosition
	.global Func_02002010
	.thumb_func
Func_02002010:
	overlay_veneer Object_CommitPosition
	.global Func_02002018
	.thumb_func
Func_02002018:
	overlay_veneer Func_08020178
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002028
	.thumb_func
Func_02002028:
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
	.global Func_02002030
	.thumb_func
Func_02002030:
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
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02002048
	.thumb_func
Func_02002048:
	overlay_veneer Func_080202f8
	.global Func_02002050
	.thumb_func
Func_02002050:
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
	.global Func_02002058
	.thumb_func
Func_02002058:
	overlay_veneer Func_08038248
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02002068
	.thumb_func
Func_02002068:
	overlay_veneer Func_080c8018
	.global Func_02002070
	.thumb_func
Func_02002070:
	overlay_veneer Func_080c8020
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
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
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
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
	.global Func_020020c0
	.thumb_func
Func_020020c0:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_020020e0
	.thumb_func
Func_020020e0:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_020020f0
	.thumb_func
Func_020020f0:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02002110
	.thumb_func
Func_02002110:
	overlay_veneer Func_080c8278
	.global Func_02002118
	.thumb_func
Func_02002118:
	overlay_veneer Func_080c82f8
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
	.global Func_02002130
	.thumb_func
Func_02002130:
	overlay_veneer Func_080c83e0
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
	.global Func_02002140
	.thumb_func
Func_02002140:
	overlay_veneer Func_080c8480
	.global Func_02002148
	.thumb_func
Func_02002148:
	overlay_veneer Func_080c84d8
	.global Func_02002150
	.thumb_func
Func_02002150:
	overlay_veneer Func_080c84e0
	.global Func_02002158
	.thumb_func
Func_02002158:
	overlay_veneer Func_080c8580
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_02002168
	.thumb_func
Func_02002168:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8678
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8678
	.else
	overlay_veneer Func_080c8688
	.endif
	.endif
	.global Func_02002170
	.thumb_func
Func_02002170:
	overlay_veneer Func_080c86a8
	.global Func_02002178
	.thumb_func
Func_02002178:
	overlay_veneer Func_080c86c0
	.global Func_02002180
	.thumb_func
Func_02002180:
	overlay_veneer Func_080c86e8
	.global Func_02002188
	.thumb_func
Func_02002188:
	overlay_veneer Func_080c8718
	.global Func_02002190
	.thumb_func
Func_02002190:
	overlay_veneer Func_080c8720
	.global Func_02002198
	.thumb_func
Func_02002198:
	overlay_veneer Func_080c8728
	.global Func_020021a0
	.thumb_func
Func_020021a0:
	overlay_veneer Func_080c8760
	.global Func_020021a8
	.thumb_func
Func_020021a8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_020021b0
	.thumb_func
Func_020021b0:
	overlay_veneer Audio_PlayCue
