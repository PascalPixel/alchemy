@ Import veneers after the OVERLAY_697 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_697 overlay import veneers
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
	.global __udivsi3
	.thumb_func
__udivsi3:
	overlay_veneer IwramUnsignedDivide
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global Task_Wait
	.thumb_func
Task_Wait:
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
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02005770
	.thumb_func
Func_02005770:
	overlay_veneer Func_080001a8
	.global Resource_ActivateEntry
	.thumb_func
Resource_ActivateEntry:
	overlay_veneer Resource_ActivateEntryFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02005790
	.thumb_func
Func_02005790:
	overlay_veneer Func_080001e8
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
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
	.global Func_020057c8
	.thumb_func
Func_020057c8:
	overlay_veneer Object_SetMode
	.global Func_020057d0
	.thumb_func
Func_020057d0:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_020057d8
	.thumb_func
Func_020057d8:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_020057e8
	.thumb_func
Func_020057e8:
	overlay_veneer Func_08020120
	.global Func_020057f0
	.thumb_func
Func_020057f0:
	overlay_veneer Object_ResetMotion
	.global Func_020057f8
	.thumb_func
Func_020057f8:
	overlay_veneer Object_SetPosition
	.global Func_02005800
	.thumb_func
Func_02005800:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02005810
	.thumb_func
Func_02005810:
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
	.global Func_02005818
	.thumb_func
Func_02005818:
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
	.global Func_02005820
	.thumb_func
Func_02005820:
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
	.global Func_02005828
	.thumb_func
Func_02005828:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080201c8
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201c8
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201c8
	.else
	overlay_veneer Func_080201f8
	.endif
	.endif
	.endif
	.endif
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02005838
	.thumb_func
Func_02005838:
	overlay_veneer Func_08020228
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02005848
	.thumb_func
Func_02005848:
	overlay_veneer Func_080202f0
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02005858
	.thumb_func
Func_02005858:
	overlay_veneer UiText_DrawQuantity
	.global Func_02005860
	.thumb_func
Func_02005860:
	overlay_veneer Func_080ad2b0
	.global Func_02005868
	.thumb_func
Func_02005868:
	overlay_veneer Func_080ad2e0
	.global Func_02005870
	.thumb_func
Func_02005870:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080ad2e8
	.else
	overlay_veneer Func_080ad308
	.endif
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02005880
	.thumb_func
Func_02005880:
	overlay_veneer Func_080c8018
	.global Func_02005888
	.thumb_func
Func_02005888:
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
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
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
	.global Func_020058e0
	.thumb_func
Func_020058e0:
	overlay_veneer Func_080c80f8
	.global Func_020058e8
	.thumb_func
Func_020058e8:
	overlay_veneer Func_080c8100
	.global Func_020058f0
	.thumb_func
Func_020058f0:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_02005918
	.thumb_func
Func_02005918:
	overlay_veneer Func_080c8150
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02005938
	.thumb_func
Func_02005938:
	overlay_veneer Func_080c8180
	.global Func_02005940
	.thumb_func
Func_02005940:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02005950
	.thumb_func
Func_02005950:
	overlay_veneer Func_080c81d8
	.global Func_02005958
	.thumb_func
Func_02005958:
	overlay_veneer Func_080c8208
	.global Func_02005960
	.thumb_func
Func_02005960:
	overlay_veneer Func_080c8210
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Func_02005970
	.thumb_func
Func_02005970:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02005980
	.thumb_func
Func_02005980:
	overlay_veneer Func_080c8240
	.global Func_02005988
	.thumb_func
Func_02005988:
	overlay_veneer Event_SetPairWork1c0Far
	.global Func_02005990
	.thumb_func
Func_02005990:
	overlay_veneer Func_080c8278
	.global Func_02005998
	.thumb_func
Func_02005998:
	overlay_veneer Func_080c8280
	.global Func_020059a0
	.thumb_func
Func_020059a0:
	overlay_veneer Func_080c8288
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Party_SetFields1f2And1f4
	.thumb_func
Party_SetFields1f2And1f4:
	overlay_veneer Party_SetFields1f2And1f4Far
	.global Func_020059b8
	.thumb_func
Func_020059b8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	overlay_veneer Func_080c82c0
	.endif
	.endif
	.global Func_020059c0
	.thumb_func
Func_020059c0:
	overlay_veneer Func_080c82e0
	.global Func_020059c8
	.thumb_func
Func_020059c8:
	overlay_veneer Func_080c82e8
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
	.global Func_020059e8
	.thumb_func
Func_020059e8:
	overlay_veneer Func_080c84e0
	.global Func_020059f0
	.thumb_func
Func_020059f0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8508
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8508
	.else
	overlay_veneer Func_080c8518
	.endif
	.endif
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_02005a08
	.thumb_func
Func_02005a08:
	overlay_veneer Func_080c86d0
	.global Func_02005a10
	.thumb_func
Func_02005a10:
	overlay_veneer Func_080c86d8
	.global Func_02005a18
	.thumb_func
Func_02005a18:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_02005a20
	.thumb_func
Func_02005a20:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8720
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8720
	.else
	overlay_veneer Func_080c8730
	.endif
	.endif
	.global Func_02005a28
	.thumb_func
Func_02005a28:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8728
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8728
	.else
	overlay_veneer Func_080c8738
	.endif
	.endif
	.global Func_02005a30
	.thumb_func
Func_02005a30:
	overlay_veneer Func_080c8740
	.global Func_02005a38
	.thumb_func
Func_02005a38:
	overlay_veneer Func_080c8748
	.global Func_02005a40
	.thumb_func
Func_02005a40:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_02005a48
	.thumb_func
Func_02005a48:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8798
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8798
	.else
	overlay_veneer Func_080c87a8
	.endif
	.endif
	.global Func_02005a50
	.thumb_func
Func_02005a50:
	overlay_veneer Func_080c88d0
	.global ObjectEffect_BeginContextEffect26
	.thumb_func
ObjectEffect_BeginContextEffect26:
	overlay_veneer ObjectEffect_BeginContextEffect26Far
	.global Func_02005a60
	.thumb_func
Func_02005a60:
	overlay_veneer Func_080c8948
	.global Func_02005a68
	.thumb_func
Func_02005a68:
	overlay_veneer Audio_PlayCue
