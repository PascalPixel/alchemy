@ Import veneers after the OVERLAY_69E overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_69E overlay import veneers
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
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_020027b8
	.thumb_func
Func_020027b8:
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
	.global Func_020027d8
	.thumb_func
Func_020027d8:
	overlay_veneer Func_080001e8
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_02002810
	.thumb_func
Func_02002810:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02002820
	.thumb_func
Func_02002820:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02002828
	.thumb_func
Func_02002828:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02002838
	.thumb_func
Func_02002838:
	overlay_veneer Func_08020120
	.global Func_02002840
	.thumb_func
Func_02002840:
	overlay_veneer Object_SetPosition
	.global Func_02002848
	.thumb_func
Func_02002848:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	@ The localized scaffold exports this same complete resident routine
	@ under its own source label.
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020190
	.else
	overlay_veneer Map_GetTerrainHeightFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02002858
	.thumb_func
Func_02002858:
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
	.global Func_02002860
	.thumb_func
Func_02002860:
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
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02002870
	.thumb_func
Func_02002870:
	overlay_veneer Func_08020228
	.global Func_02002878
	.thumb_func
Func_02002878:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02002888
	.thumb_func
Func_02002888:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020330
	.else
	overlay_veneer Func_08020360
	.endif
	.endif
	.endif
	.endif
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02002898
	.thumb_func
Func_02002898:
	overlay_veneer Func_08038210
	.global Func_020028a0
	.thumb_func
Func_020028a0:
	overlay_veneer Func_080ad208
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020028b0
	.thumb_func
Func_020028b0:
	overlay_veneer Func_080c8018
	.global Func_020028b8
	.thumb_func
Func_020028b8:
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
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_020028f8
	.thumb_func
Func_020028f8:
	overlay_veneer Func_080c80f8
	.global Func_02002900
	.thumb_func
Func_02002900:
	overlay_veneer Func_080c8100
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Func_02002918
	.thumb_func
Func_02002918:
	overlay_veneer Func_080c8168
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02002928
	.thumb_func
Func_02002928:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02002938
	.thumb_func
Func_02002938:
	overlay_veneer Func_080c8198
	.global Func_02002940
	.thumb_func
Func_02002940:
	overlay_veneer Func_080c81d8
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02002950
	.thumb_func
Func_02002950:
	overlay_veneer Func_080c8210
	.global Func_02002958
	.thumb_func
Func_02002958:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02002970
	.thumb_func
Func_02002970:
	overlay_veneer Func_080c8258
	.global Func_02002978
	.thumb_func
Func_02002978:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	overlay_veneer Func_080c8378
	.endif
	.endif
	.global Func_02002980
	.thumb_func
Func_02002980:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8370
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8370
	.else
	overlay_veneer Func_080c8380
	.endif
	.endif
	.global Func_02002988
	.thumb_func
Func_02002988:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8380
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8380
	.else
	overlay_veneer Func_080c8390
	.endif
	.endif
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8398
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8398
	.else
	overlay_veneer Event_SetStatus1c6Far
	.endif
	.endif
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c83a0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c83a0
	.else
	overlay_veneer Event_ClearStatus1c6Far
	.endif
	.endif
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_DelayEffectFramesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_DelayEffectFramesFar
	.else
	overlay_veneer Event_WaitValue1c8FramesFar
	.endif
	.endif
	.global Func_020029a8
	.thumb_func
Func_020029a8:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer Func_080c84e0
	.endif
	.endif
	.global Func_020029b0
	.thumb_func
Func_020029b0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Camera_WorldToScreenFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Camera_WorldToScreenFar
	.else
	overlay_veneer Func_080c85c0
	.endif
	.endif
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_SetActionCallback
	.thumb_func
Object_SetActionCallback:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8670
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8670
	.else
	overlay_veneer Object_SetActionCallbackFar
	.endif
	.endif
	.global Func_020029c8
	.thumb_func
Func_020029c8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8708
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8708
	.else
	overlay_veneer Func_080c8718
	.endif
	.endif
	.global Func_020029d0
	.thumb_func
Func_020029d0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8710
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8710
	.else
	overlay_veneer Func_080c8720
	.endif
	.endif
	.global Func_020029d8
	.thumb_func
Func_020029d8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8718
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8718
	.else
	overlay_veneer Func_080c8728
	.endif
	.endif
	.global Func_020029e0
	.thumb_func
Func_020029e0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b0
	.else
	overlay_veneer Func_080c87c0
	.endif
	.endif
	.global Func_020029e8
	.thumb_func
Func_020029e8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b8
	.else
	overlay_veneer Func_080c87c8
	.endif
	.endif
	.global Func_020029f0
	.thumb_func
Func_020029f0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87d8
	.else
	overlay_veneer Func_080c87e8
	.endif
	.endif
	.global Func_020029f8
	.thumb_func
Func_020029f8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87e0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87e0
	.else
	overlay_veneer Func_080c87f0
	.endif
	.endif
	.global Func_02002a00
	.thumb_func
Func_02002a00:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87f8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87f8
	.else
	overlay_veneer Func_080c8808
	.endif
	.endif
	.global Func_02002a08
	.thumb_func
Func_02002a08:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_EndActorSpriteEffectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_EndActorSpriteEffectFar
	.else
	overlay_veneer Func_080c8810
	.endif
	.endif
	.global Func_02002a10
	.thumb_func
Func_02002a10:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8808
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8808
	.else
	overlay_veneer Func_080c8818
	.endif
	.endif
	.global Func_02002a18
	.thumb_func
Func_02002a18:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8810
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8810
	.else
	overlay_veneer Func_080c8820
	.endif
	.endif
	.global Func_02002a20
	.thumb_func
Func_02002a20:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8820
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8820
	.else
	overlay_veneer Func_080c8830
	.endif
	.endif
	.global Func_02002a28
	.thumb_func
Func_02002a28:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8830
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8830
	.else
	overlay_veneer Func_080c8840
	.endif
	.endif
	.global Func_02002a30
	.thumb_func
Func_02002a30:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8838
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8838
	.else
	overlay_veneer Func_080c8848
	.endif
	.endif
	.global AudioCommand_WaitForStateByteClear
	.thumb_func
AudioCommand_WaitForStateByteClear:
	overlay_veneer AudioCommand_WaitForStateByteClearFar
	.global Func_02002a40
	.thumb_func
Func_02002a40:
	overlay_veneer Audio_PlayCue
