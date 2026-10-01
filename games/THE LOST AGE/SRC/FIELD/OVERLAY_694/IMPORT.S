@ Import veneers after the OVERLAY_694 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_694 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global Engine_MathModulo
	.thumb_func
Engine_MathModulo:
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
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
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
	overlay_veneer Object_SetCallback
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
	overlay_veneer Func_08020120
	.global Func_02004004
	.thumb_func
Func_02004004:
	overlay_veneer Func_08020178
	.global Func_0200400c
	.thumb_func
Func_0200400c:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_0200401c
	.thumb_func
Func_0200401c:
	overlay_veneer Func_08020228
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_0200402c
	.thumb_func
Func_0200402c:
	overlay_veneer Func_08020290
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
	overlay_veneer Func_080ad290
	.global PartyInventory_CountItem
	.thumb_func
PartyInventory_CountItem:
	overlay_veneer PartyInventory_CountItemFar
	.global PartyInventory_CountFreeSlots
	.thumb_func
PartyInventory_CountFreeSlots:
	overlay_veneer PartyInventory_CountFreeSlotsFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_0200406c
	.thumb_func
Func_0200406c:
	overlay_veneer Func_080c8018
	.global Func_02004074
	.thumb_func
Func_02004074:
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
	overlay_veneer Func_080c80f8
	.global Func_020040dc
	.thumb_func
Func_020040dc:
	overlay_veneer Func_080c8100
	.global Func_020040e4
	.thumb_func
Func_020040e4:
	overlay_veneer Func_080c8110
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
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_0200412c
	.thumb_func
Func_0200412c:
	overlay_veneer Func_080c8180
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
	overlay_veneer Func_080c81a0
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
	overlay_veneer Func_080c81d8
	.global Func_02004164
	.thumb_func
Func_02004164:
	overlay_veneer Func_080c81f0
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
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Func_0200418c
	.thumb_func
Func_0200418c:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
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
	overlay_veneer Event_SetPairWork1c0Far
	.global Func_020041b4
	.thumb_func
Func_020041b4:
	overlay_veneer Func_080c8278
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
	overlay_veneer Func_080c8378
	.global Func_020041d4
	.thumb_func
Func_020041d4:
	overlay_veneer Func_080c8380
	.global Func_020041dc
	.thumb_func
Func_020041dc:
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
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_020041fc
	.thumb_func
Func_020041fc:
	overlay_veneer Func_080c83e0
	.global Func_02004204
	.thumb_func
Func_02004204:
	overlay_veneer Func_080c84e0
	.global Func_0200420c
	.thumb_func
Func_0200420c:
	overlay_veneer Func_080c8570
	.global Func_02004214
	.thumb_func
Func_02004214:
	overlay_veneer Func_080c8580
	.global Func_0200421c
	.thumb_func
Func_0200421c:
	overlay_veneer Func_080c85c0
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_0200422c
	.thumb_func
Func_0200422c:
	overlay_veneer Func_080c8628
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
	overlay_veneer Func_080c8808
	.global Func_0200426c
	.thumb_func
Func_0200426c:
	overlay_veneer Func_080c8810
	.global Func_02004274
	.thumb_func
Func_02004274:
	overlay_veneer Func_080c8818
	.global Func_0200427c
	.thumb_func
Func_0200427c:
	overlay_veneer Func_080c8820
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
