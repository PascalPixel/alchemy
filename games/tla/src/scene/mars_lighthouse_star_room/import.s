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
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
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
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
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
	.global Func_0200418c
	.thumb_func
Func_0200418c:
	overlay_veneer Oam_AddSpriteAtDepthFar
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
	.global ResourceMetadata_ClearRecord
	.thumb_func
ResourceMetadata_ClearRecord:
	overlay_veneer ResourceMetadata_ClearRecordFar
	.global Func_020041cc
	.thumb_func
Func_020041cc:
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_020041dc
	.thumb_func
Func_020041dc:
	overlay_veneer Actor_StartScriptFar
	.global Func_020041e4
	.thumb_func
Func_020041e4:
	overlay_veneer Func_080200c0
	.global Func_020041ec
	.thumb_func
Func_020041ec:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_020041fc
	.thumb_func
Func_020041fc:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02004204
	.thumb_func
Func_02004204:
	overlay_veneer Actor_SetTargetPosition
	.global Func_0200420c
	.thumb_func
Func_0200420c:
	overlay_veneer Actor_CommitPosition
	.global Func_02004214
	.thumb_func
Func_02004214:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02004224
	.thumb_func
Func_02004224:
	overlay_veneer GetMapCellCollisionFar
	.global Func_0200422c
	.thumb_func
Func_0200422c:
	overlay_veneer Func_080201d8
	.global Func_02004234
	.thumb_func
Func_02004234:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_0200423c
	.thumb_func
Func_0200423c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02004244
	.thumb_func
Func_02004244:
	overlay_veneer Func_080201f0
	.global Func_0200424c
	.thumb_func
Func_0200424c:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004264
	.thumb_func
Func_02004264:
	overlay_veneer Map_SetScreenShakeFar
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
	overlay_veneer Actor_SetSpritePriorityDirectFar
	.global Func_02004284
	.thumb_func
Func_02004284:
	overlay_veneer Func_080202f8
	.global Func_0200428c
	.thumb_func
Func_0200428c:
	overlay_veneer Field_CheckSpheresOverlapFar
	.global Func_02004294
	.thumb_func
Func_02004294:
	overlay_veneer Func_08020390
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
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
	overlay_veneer Inventory_EquipFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020042dc
	.thumb_func
Func_020042dc:
	overlay_veneer Event_StartFar
	.global Func_020042e4
	.thumb_func
Func_020042e4:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_SetMovementTypeAndWaitById
	.thumb_func
Actor_SetMovementTypeAndWaitById:
	overlay_veneer Actor_SetMovementTypeAndWaitByIdFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Func_0200432c
	.thumb_func
Func_0200432c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02004334
	.thumb_func
Func_02004334:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_WaitForAnimationChange
	.thumb_func
Actor_WaitForAnimationChange:
	overlay_veneer Actor_WaitForAnimationChangeFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_0200436c
	.thumb_func
Func_0200436c:
	overlay_veneer Actor_JumpDownLedgeFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_0200437c
	.thumb_func
Func_0200437c:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_0200438c
	.thumb_func
Func_0200438c:
	overlay_veneer Func_080c8198
	.global Func_02004394
	.thumb_func
Func_02004394:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_020043a4
	.thumb_func
Func_020043a4:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
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
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
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
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Func_020043f4
	.thumb_func
Func_020043f4:
	overlay_veneer Leader_StopMovingFar
	.global Func_020043fc
	.thumb_func
Func_020043fc:
	overlay_veneer ScreenTransition_StartFar
	.global Func_02004404
	.thumb_func
Func_02004404:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_0200440c
	.thumb_func
Func_0200440c:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02004414
	.thumb_func
Func_02004414:
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
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	overlay_veneer ObjectTable_GetFar
	.global Func_0200443c
	.thumb_func
Func_0200443c:
	overlay_veneer Func_080c8480
	.global Func_02004444
	.thumb_func
Func_02004444:
	overlay_veneer Event_ResolveAllPendingActionsFar
	.global Func_0200444c
	.thumb_func
Func_0200444c:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02004454
	.thumb_func
Func_02004454:
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.global Func_0200445c
	.thumb_func
Func_0200445c:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02004464
	.thumb_func
Func_02004464:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_0200446c
	.thumb_func
Func_0200446c:
	overlay_veneer Sound_PlayFar
