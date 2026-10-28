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
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
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
	.global Func_02003fdc
	.thumb_func
Func_02003fdc:
	overlay_veneer Object_SetMode
	.global Func_02003fe4
	.thumb_func
Func_02003fe4:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02003fec
	.thumb_func
Func_02003fec:
	overlay_veneer Func_080200c0
	.global Func_02003ff4
	.thumb_func
Func_02003ff4:
	overlay_veneer Object_Destroy
	.global Func_02003ffc
	.thumb_func
Func_02003ffc:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02004004
	.thumb_func
Func_02004004:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_0200400c
	.thumb_func
Func_0200400c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_0200401c
	.thumb_func
Func_0200401c:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_0200402c
	.thumb_func
Func_0200402c:
	overlay_veneer ObjectDispatch_SetSpritePriorityFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_0200403c
	.thumb_func
Func_0200403c:
	overlay_veneer UiText_DrawQuantity
	.global Func_02004044
	.thumb_func
Func_02004044:
	overlay_veneer Func_080ad240
	.global Func_0200404c
	.thumb_func
Func_0200404c:
	overlay_veneer Party_GetAverageLevelFar
	.global PartyInventory_CountItem
	.thumb_func
PartyInventory_CountItem:
	overlay_veneer PartyInventory_CountItemFar
	.global PartyInventory_CountFreeSlots
	.thumb_func
PartyInventory_CountFreeSlots:
	overlay_veneer PartyInventory_CountFreeSlotsFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_0200406c
	.thumb_func
Func_0200406c:
	overlay_veneer EventRuntime_StartFar
	.global Func_02004074
	.thumb_func
Func_02004074:
	overlay_veneer EventRuntime_EndFar
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
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
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
	.global Func_020040d4
	.thumb_func
Func_020040d4:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_020040dc
	.thumb_func
Func_020040dc:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_020040e4
	.thumb_func
Func_020040e4:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
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
	.global Func_0200412c
	.thumb_func
Func_0200412c:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_0200413c
	.thumb_func
Func_0200413c:
	overlay_veneer Func_080c8198
	.global Func_02004144
	.thumb_func
Func_02004144:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_0200414c
	.thumb_func
Func_0200414c:
	overlay_veneer Func_080c81a8
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_0200415c
	.thumb_func
Func_0200415c:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global Func_02004164
	.thumb_func
Func_02004164:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02004174
	.thumb_func
Func_02004174:
	overlay_veneer Func_080c8210
	.global Func_0200417c
	.thumb_func
Func_0200417c:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_0200418c
	.thumb_func
Func_0200418c:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_0200419c
	.thumb_func
Func_0200419c:
	overlay_veneer Func_080c8240
	.global Func_020041a4
	.thumb_func
Func_020041a4:
	overlay_veneer Func_080c8260
	.global Func_020041ac
	.thumb_func
Func_020041ac:
	overlay_veneer Event_SetDestinationFar
	.global Func_020041b4
	.thumb_func
Func_020041b4:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Field_SetDestination
	.thumb_func
Field_SetDestination:
	overlay_veneer Field_SetDestinationFar
	.global Func_020041cc
	.thumb_func
Func_020041cc:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_020041d4
	.thumb_func
Func_020041d4:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_020041dc
	.thumb_func
Func_020041dc:
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
	.global Func_020041fc
	.thumb_func
Func_020041fc:
	overlay_veneer Func_080c83e0
	.global Func_02004204
	.thumb_func
Func_02004204:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_0200420c
	.thumb_func
Func_0200420c:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02004214
	.thumb_func
Func_02004214:
	overlay_veneer Func_080c8580
	.global Func_0200421c
	.thumb_func
Func_0200421c:
	overlay_veneer Camera_WorldToScreenFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_0200422c
	.thumb_func
Func_0200422c:
	overlay_veneer ObjectTable_SnapshotFar
	.global Object_Spawn
	.thumb_func
Object_Spawn:
	overlay_veneer Object_SpawnFar
	.global Func_0200423c
	.thumb_func
Func_0200423c:
	overlay_veneer Func_080c8760
	.global Func_02004244
	.thumb_func
Func_02004244:
	overlay_veneer Func_080c87c0
	.global Func_0200424c
	.thumb_func
Func_0200424c:
	overlay_veneer Func_080c87c8
	.global Func_02004254
	.thumb_func
Func_02004254:
	overlay_veneer Func_080c87e8
	.global Func_0200425c
	.thumb_func
Func_0200425c:
	overlay_veneer Func_080c87f0
	.global Func_02004264
	.thumb_func
Func_02004264:
	overlay_veneer BattleEffect_StartArcEffectFar
	.global Func_0200426c
	.thumb_func
Func_0200426c:
	overlay_veneer BattleEffect_EndArcEffectFar
	.global Func_02004274
	.thumb_func
Func_02004274:
	overlay_veneer BattleEffect_WaitForArcGroupsFar
	.global Func_0200427c
	.thumb_func
Func_0200427c:
	overlay_veneer BattleEffect_SetObjectUpdateModeFar
	.global Func_02004284
	.thumb_func
Func_02004284:
	overlay_veneer Func_080c88d8
	.global Func_0200428c
	.thumb_func
Func_0200428c:
	overlay_veneer Func_080c88e8
	.global Func_02004294
	.thumb_func
Func_02004294:
	overlay_veneer Func_08108008
	.global Func_0200429c
	.thumb_func
Func_0200429c:
	overlay_veneer Func_08108010
	.global Func_020042a4
	.thumb_func
Func_020042a4:
	overlay_veneer Func_08108018
	.global AudioCommand_WaitForStateByteClear
	.thumb_func
AudioCommand_WaitForStateByteClear:
	overlay_veneer AudioCommand_WaitForStateByteClearFar
	.global Func_020042b4
	.thumb_func
Func_020042b4:
	overlay_veneer Audio_PlayCue
