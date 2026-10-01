@ Import veneers after the OVERLAY_6AB overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6AB overlay import veneers
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
	.global Func_0200418c
	.thumb_func
Func_0200418c:
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
	.global Func_020041cc
	.thumb_func
Func_020041cc:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_020041dc
	.thumb_func
Func_020041dc:
	overlay_veneer Object_SetCallback
	.global Func_020041e4
	.thumb_func
Func_020041e4:
	overlay_veneer Func_080200c0
	.global Func_020041ec
	.thumb_func
Func_020041ec:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_020041fc
	.thumb_func
Func_020041fc:
	overlay_veneer Func_08020120
	.global Func_02004204
	.thumb_func
Func_02004204:
	overlay_veneer Object_SetPosition
	.global Func_0200420c
	.thumb_func
Func_0200420c:
	overlay_veneer Object_CommitPosition
	.global Func_02004214
	.thumb_func
Func_02004214:
	overlay_veneer Func_08020178
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02004224
	.thumb_func
Func_02004224:
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
	.global Func_0200422c
	.thumb_func
Func_0200422c:
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
	.global Func_02004234
	.thumb_func
Func_02004234:
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
	.global Func_0200423c
	.thumb_func
Func_0200423c:
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
	.global Func_02004244
	.thumb_func
Func_02004244:
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
	.global Func_0200424c
	.thumb_func
Func_0200424c:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004264
	.thumb_func
Func_02004264:
	overlay_veneer Func_08020228
	.global Func_0200426c
	.thumb_func
Func_0200426c:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_0200427c
	.thumb_func
Func_0200427c:
	overlay_veneer Func_08020290
	.global Func_02004284
	.thumb_func
Func_02004284:
	overlay_veneer Func_080202f8
	.global Func_0200428c
	.thumb_func
Func_0200428c:
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
	.global Func_02004294
	.thumb_func
Func_02004294:
	overlay_veneer Func_08020390
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_020042a4
	.thumb_func
Func_020042a4:
	overlay_veneer Func_08038210
	.global Func_020042ac
	.thumb_func
Func_020042ac:
	overlay_veneer Func_08038248
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer Inventory_AddItemFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global Func_020042c4
	.thumb_func
Func_020042c4:
	overlay_veneer Func_080ad048
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020042dc
	.thumb_func
Func_020042dc:
	overlay_veneer Func_080c8018
	.global Func_020042e4
	.thumb_func
Func_020042e4:
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
	.global Func_0200432c
	.thumb_func
Func_0200432c:
	overlay_veneer Func_080c80f8
	.global Func_02004334
	.thumb_func
Func_02004334:
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
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_0200436c
	.thumb_func
Func_0200436c:
	overlay_veneer Func_080c8150
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_0200437c
	.thumb_func
Func_0200437c:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_0200438c
	.thumb_func
Func_0200438c:
	overlay_veneer Func_080c8198
	.global Func_02004394
	.thumb_func
Func_02004394:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_020043a4
	.thumb_func
Func_020043a4:
	overlay_veneer Func_080c81d8
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_020043b4
	.thumb_func
Func_020043b4:
	overlay_veneer Func_080c8210
	.global Func_020043bc
	.thumb_func
Func_020043bc:
	overlay_veneer Func_080c8218
	.global Func_020043c4
	.thumb_func
Func_020043c4:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_020043d4
	.thumb_func
Func_020043d4:
	overlay_veneer Func_080c8240
	.global Func_020043dc
	.thumb_func
Func_020043dc:
	overlay_veneer Func_080c8258
	.global Func_020043e4
	.thumb_func
Func_020043e4:
	overlay_veneer Func_080c8280
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Func_020043f4
	.thumb_func
Func_020043f4:
	overlay_veneer Func_080c82f8
	.global Func_020043fc
	.thumb_func
Func_020043fc:
	overlay_veneer Func_080c8350
	.global Func_02004404
	.thumb_func
Func_02004404:
	overlay_veneer Func_080c8378
	.global Func_0200440c
	.thumb_func
Func_0200440c:
	overlay_veneer Func_080c8380
	.global Func_02004414
	.thumb_func
Func_02004414:
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
	.global Func_0200443c
	.thumb_func
Func_0200443c:
	overlay_veneer Func_080c8480
	.global Func_02004444
	.thumb_func
Func_02004444:
	overlay_veneer Func_080c84d8
	.global Func_0200444c
	.thumb_func
Func_0200444c:
	overlay_veneer Func_080c84e0
	.global Func_02004454
	.thumb_func
Func_02004454:
	overlay_veneer Func_080c8548
	.global Func_0200445c
	.thumb_func
Func_0200445c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8560
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8560
	.else
	overlay_veneer Func_080c8570
	.endif
	.endif
	.global Func_02004464
	.thumb_func
Func_02004464:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_0200446c
	.thumb_func
Func_0200446c:
	overlay_veneer Audio_PlayCue
