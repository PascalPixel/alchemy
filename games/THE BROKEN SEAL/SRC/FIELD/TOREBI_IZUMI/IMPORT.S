@ Import veneers after the TOREBI_IZUMI overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — TOREBI_IZUMI overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer Random16Far
	overlay_veneer Trig_SinFar
	overlay_veneer Trig_CosFar
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildrenFar
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	overlay_veneer Map_CopyMetatileIndicesRectFar
	overlay_veneer UiWindow_CreateFar
	overlay_veneer UiWork_FinalizeFar
	overlay_veneer UiText_DrawCharacterAtOffsetFar
	overlay_veneer UiText_DrawNumberInWindowFar
	.global UiWork_PushValueSlot
	.thumb_func
UiWork_PushValueSlot:
	overlay_veneer UiWork_PushValueSlotFar
	overlay_veneer UiWork_FinalizePendingCoreFar
	overlay_veneer Menu_SelectEntry20To21Far
	overlay_veneer PartyInventory_RemoveFar
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	overlay_veneer Party_AdjustSixDigitCounterAFar
	.global Party_GetAverageLevel
	.thumb_func
Party_GetAverageLevel:
	overlay_veneer Party_GetAverageLevelFar
	.global PartyInventory_CountItem
	.thumb_func
PartyInventory_CountItem:
	overlay_veneer PartyInventory_CountItemFar
	.global PartyInventory_CountFreeSlots
	.thumb_func
PartyInventory_CountFreeSlots:
	overlay_veneer PartyInventory_CountFreeSlotsFar
	.global Engine_EventWait
	.thumb_func
Engine_EventWait:
	overlay_veneer Battle_WaitMode0Far
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer Battle_ResetFar
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer BattleFx_FinishActionFar
	.global Engine_PartyGiveItem
	.thumb_func
Engine_PartyGiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer Object_GetByIdFar
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer Object_SetModeByIdFar
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer Event_SetValue1d8Far
	.global Engine_EventOpenMessage
	.thumb_func
Engine_EventOpenMessage:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer BattleEv_RunWaitFar
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Party_SetFields1ceAnd1d0
	.thumb_func
Party_SetFields1ceAnd1d0:
	overlay_veneer Party_SetFields1ceAnd1d0Far
	.global Event_SetPair1c0AndSetValue170
	.thumb_func
Event_SetPair1c0AndSetValue170:
	overlay_veneer Event_SetPair1c0AndSetValue170Far
	.global Engine_EventOpenScreen
	.thumb_func
Engine_EventOpenScreen:
	overlay_veneer Event_SetStatus1c6Far
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Engine_ItemShowFound
	.thumb_func
Engine_ItemShowFound:
	overlay_veneer BattleFx_StartEffectObject22Far
	overlay_veneer Battle_ResetEffectCounterFar
	.global ObjectTable_Snapshot
	.thumb_func
ObjectTable_Snapshot:
	overlay_veneer SaveState_CaptureObjectTableFar
	.global AudioCommand_WaitForStateByteClear
	.thumb_func
AudioCommand_WaitForStateByteClear:
	overlay_veneer AudioCommand_WaitForStateByteClearFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
