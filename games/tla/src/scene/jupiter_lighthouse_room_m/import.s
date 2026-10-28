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
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global MemAlloc
	.thumb_func
MemAlloc:
	overlay_veneer Runtime_BumpAllocateFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_0200468c
	.thumb_func
Func_0200468c:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020046a4
	.thumb_func
Func_020046a4:
	overlay_veneer DebugText_ClearFar
	.global Func_020046ac
	.thumb_func
Func_020046ac:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global Func_020046b4
	.thumb_func
Func_020046b4:
	overlay_veneer Ui_LoadWindowGraphicsFar
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
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_020046f4
	.thumb_func
Func_020046f4:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_02004704
	.thumb_func
Func_02004704:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_0200470c
	.thumb_func
Func_0200470c:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_0200471c
	.thumb_func
Func_0200471c:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02004724
	.thumb_func
Func_02004724:
	overlay_veneer Object_SetPosition
	.global Func_0200472c
	.thumb_func
Func_0200472c:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_0200473c
	.thumb_func
Func_0200473c:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02004744
	.thumb_func
Func_02004744:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_0200474c
	.thumb_func
Func_0200474c:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_0200475c
	.thumb_func
Func_0200475c:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02004764
	.thumb_func
Func_02004764:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02004774
	.thumb_func
Func_02004774:
	overlay_veneer Func_08020360
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02004784
	.thumb_func
Func_02004784:
	overlay_veneer Func_08038348
	.global Unit_Calculate
	.thumb_func
Unit_Calculate:
	overlay_veneer Unit_CalculateFar
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global Func_020047a4
	.thumb_func
Func_020047a4:
	overlay_veneer Inventory_EquipFar
	.global Unit_AddHp
	.thumb_func
Unit_AddHp:
	overlay_veneer Unit_AddHpFar
	.global Unit_AddPp
	.thumb_func
Unit_AddPp:
	overlay_veneer Unit_AddPpFar
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer Party_AddActiveOwnerFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020047d4
	.thumb_func
Func_020047d4:
	overlay_veneer EventRuntime_StartFar
	.global Func_020047dc
	.thumb_func
Func_020047dc:
	overlay_veneer EventRuntime_EndFar
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
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
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
	.global Func_02004844
	.thumb_func
Func_02004844:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_0200484c
	.thumb_func
Func_0200484c:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
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
	.global Object_TurnToFaceEachOther
	.thumb_func
Object_TurnToFaceEachOther:
	overlay_veneer Object_LinkPairFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_0200488c
	.thumb_func
Func_0200488c:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_0200489c
	.thumb_func
Func_0200489c:
	overlay_veneer Func_080c8198
	.global Func_020048a4
	.thumb_func
Func_020048a4:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_020048b4
	.thumb_func
Func_020048b4:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global Func_020048bc
	.thumb_func
Func_020048bc:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_020048cc
	.thumb_func
Func_020048cc:
	overlay_veneer Func_080c8210
	.global Func_020048d4
	.thumb_func
Func_020048d4:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_020048e4
	.thumb_func
Func_020048e4:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020048f4
	.thumb_func
Func_020048f4:
	overlay_veneer Func_080c8240
	.global Func_020048fc
	.thumb_func
Func_020048fc:
	overlay_veneer Func_080c8258
	.global Func_02004904
	.thumb_func
Func_02004904:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Party_SetFields1f2And1f4
	.thumb_func
Party_SetFields1f2And1f4:
	overlay_veneer Party_SetFields1f2And1f4Far
	.global Func_0200491c
	.thumb_func
Func_0200491c:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02004924
	.thumb_func
Func_02004924:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_0200492c
	.thumb_func
Func_0200492c:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_0200494c
	.thumb_func
Func_0200494c:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02004954
	.thumb_func
Func_02004954:
	overlay_veneer Camera_WorldToScreenFar
	.ifndef TLA_EDITION_JA
	.global Event_ClearInvalidPackedValues
	.thumb_func
Event_ClearInvalidPackedValues:
	overlay_veneer Event_ClearInvalidPackedValuesFar
	.endif
	.global Func_02004964
	.thumb_func
Func_02004964:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
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
	.global Func_02004984
	.thumb_func
Func_02004984:
	overlay_veneer Func_080c86a8
	.global Func_0200498c
	.thumb_func
Func_0200498c:
	overlay_veneer Func_080c86e8
	.global Func_02004994
	.thumb_func
Func_02004994:
	overlay_veneer Func_080c8780
	.global Func_0200499c
	.thumb_func
Func_0200499c:
	overlay_veneer Func_080c87e8
	.global Func_020049a4
	.thumb_func
Func_020049a4:
	overlay_veneer Func_080c8830
	.global Func_020049ac
	.thumb_func
Func_020049ac:
	overlay_veneer Func_080c8840
	.global Func_020049b4
	.thumb_func
Func_020049b4:
	overlay_veneer Func_080c8848
	.global Sound_WaitForFanfare
	.thumb_func
Sound_WaitForFanfare:
	overlay_veneer Sound_WaitForFanfareFar
	.global Func_020049c4
	.thumb_func
Func_020049c4:
	overlay_veneer Sound_PlayFar
