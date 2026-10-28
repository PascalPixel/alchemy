.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
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
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
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
	.global HeapSlot_AllocEwram
	.thumb_func
HeapSlot_AllocEwram:
	overlay_veneer HeapSlot_AllocEwramFar
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02005770
	.thumb_func
Func_02005770:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_ReleaseVram
	.thumb_func
VramCache_ReleaseVram:
	overlay_veneer VramCache_ReleaseVramFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02005790
	.thumb_func
Func_02005790:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
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
	overlay_veneer Map_CenterOnFollowedPointFar
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
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02005818
	.thumb_func
Func_02005818:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02005820
	.thumb_func
Func_02005820:
	overlay_veneer Func_080201f0
	.global Func_02005828
	.thumb_func
Func_02005828:
	overlay_veneer Func_080201f8
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02005838
	.thumb_func
Func_02005838:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02005848
	.thumb_func
Func_02005848:
	overlay_veneer Func_080202f0
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_02005858
	.thumb_func
Func_02005858:
	overlay_veneer MessageArgument_PushFar
	.global Func_02005860
	.thumb_func
Func_02005860:
	overlay_veneer PartyInventory_DiscardFar
	.global Func_02005868
	.thumb_func
Func_02005868:
	overlay_veneer Func_080ad2e0
	.global Func_02005870
	.thumb_func
Func_02005870:
	overlay_veneer Func_080ad308
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02005880
	.thumb_func
Func_02005880:
	overlay_veneer Event_StartFar
	.global Func_02005888
	.thumb_func
Func_02005888:
	overlay_veneer Event_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
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
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_020058e8
	.thumb_func
Func_020058e8:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_020058f0
	.thumb_func
Func_020058f0:
	overlay_veneer ObjectMotion_MoveToObjectFar
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
	overlay_veneer Object_JumpDownLedgeFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_TurnToFaceEachOther
	.thumb_func
Object_TurnToFaceEachOther:
	overlay_veneer Object_LinkPairFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02005938
	.thumb_func
Func_02005938:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02005940
	.thumb_func
Func_02005940:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02005950
	.thumb_func
Func_02005950:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
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
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02005980
	.thumb_func
Func_02005980:
	overlay_veneer Func_080c8240
	.global Func_02005988
	.thumb_func
Func_02005988:
	overlay_veneer Event_SetDestinationFar
	.global Func_02005990
	.thumb_func
Func_02005990:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02005998
	.thumb_func
Func_02005998:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
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
	overlay_veneer Func_080c82c0
	.global Func_020059c0
	.thumb_func
Func_020059c0:
	overlay_veneer Func_080c82e0
	.global Func_020059c8
	.thumb_func
Func_020059c8:
	overlay_veneer Func_080c82e8
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	overlay_veneer ObjectTable_GetFar
	.global Func_020059e8
	.thumb_func
Func_020059e8:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_020059f0
	.thumb_func
Func_020059f0:
	overlay_veneer Func_080c8518
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
	overlay_veneer Func_080c8710
	.global Func_02005a20
	.thumb_func
Func_02005a20:
	overlay_veneer Func_080c8730
	.global Func_02005a28
	.thumb_func
Func_02005a28:
	overlay_veneer Func_080c8738
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
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02005a48
	.thumb_func
Func_02005a48:
	overlay_veneer Func_080c87a8
	.global Func_02005a50
	.thumb_func
Func_02005a50:
	overlay_veneer Func_080c88d0
	.global ObjectEffect_StartContextEffect26
	.thumb_func
ObjectEffect_StartContextEffect26:
	overlay_veneer ObjectEffect_StartContextEffect26Far
	.global Func_02005a60
	.thumb_func
Func_02005a60:
	overlay_veneer Func_080c8948
	.global Func_02005a68
	.thumb_func
Func_02005a68:
	overlay_veneer Sound_PlayFar
