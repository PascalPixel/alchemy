@ Import veneers after the KOROSSEO_KAWA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — KOROSSEO_KAWA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_ScheduleCallback
	.thumb_func
Engine_ScheduleCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer Random16Far
	.global Math_Atan2
	.thumb_func
Math_Atan2:
	overlay_veneer ArcTan2Far
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer Vector_AddPolarOffsetFar
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Engine_AllocateBlock
	.thumb_func
Engine_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Runtime_BumpFree
	.thumb_func
Runtime_BumpFree:
	overlay_veneer Sys_FreeFar
	.global Resource_DecodeType01
	.thumb_func
Resource_DecodeType01:
	overlay_veneer Resource_DecodeType01Far
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	overlay_veneer Resource_ActivateEntryFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	.global Engine_VramLoad
	.thumb_func
Engine_VramLoad:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntryFar
	.thumb_func
Resource_FindFreeEntryFar:
	overlay_veneer Func_080001d0
	overlay_veneer AffineMatrix_BuildForEffectFar
	overlay_veneer Runtime_PushSlotEntryFar
	overlay_veneer Resource_GetTableEntryFar
	.global ResourceMetadata_ClearRecordFar
	.thumb_func
ResourceMetadata_ClearRecordFar:
	overlay_veneer Func_08009060
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildrenFar
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Object_CreateFar
	.thumb_func
Object_CreateFar:
	overlay_veneer Func_080090c8
	overlay_veneer ObjectDispatch_InitFromTable4WithArgumentFar
	.global ObjectDispatch_InitFromTable6
	.thumb_func
ObjectDispatch_InitFromTable6:
	overlay_veneer ObjectDispatch_InitFromTable6Far
	.global ObjectDispatch_WaitForValue16
	.thumb_func
ObjectDispatch_WaitForValue16:
	overlay_veneer ObjectDispatch_WaitForValue16Far
	.global Engine_ObjectSetPosition
	.thumb_func
Engine_ObjectSetPosition:
	.global Object_SetMoveTarget
	.thumb_func
Object_SetMoveTarget:
	overlay_veneer Object_SetMoveTargetFar
	.global Script_WaitForEventTimeout
	.thumb_func
Script_WaitForEventTimeout:
	overlay_veneer Script_WaitForEventTimeoutFar
	.global Map_GetTerrainHeightFar
	.thumb_func
Map_GetTerrainHeightFar:
	overlay_veneer Func_080091a8
	overlay_veneer Map_CopyMetatileCellsRectFar
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Map_UpdateCellRect
	.thumb_func
Map_UpdateCellRect:
	overlay_veneer Func_080091c8
	.global Object_CheckMovementCollision
	.thumb_func
Object_CheckMovementCollision:
	overlay_veneer Func_080091d8
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Engine_MessageShowCentered
	.thumb_func
Engine_MessageShowCentered:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global UiWork_PushValueSlot
	.thumb_func
UiWork_PushValueSlot:
	overlay_veneer UiWork_PushValueSlotFar
	.global ItemIcon_LoadTilesFar
	.thumb_func
ItemIcon_LoadTilesFar:
	overlay_veneer Ui_PrepareTransferForItemFar
	overlay_veneer PartyTalkMenu_ChooseFar
	.global Resource_LoadFixedBlockBIntoFreeSlot
	.thumb_func
Resource_LoadFixedBlockBIntoFreeSlot:
	overlay_veneer Resource_LoadFixedBlockBIntoFreeSlotFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer Inventory_AddItemFar
	.global Inventory_EquipFar
	.thumb_func
Inventory_EquipFar:
	overlay_veneer Func_08077050
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_GetByteFar
	.thumb_func
GameFlag_GetByteFar:
	overlay_veneer Func_080770e0
	.global GameFlag_SetByteFar
	.thumb_func
GameFlag_SetByteFar:
	overlay_veneer Func_080770e8
	.global Party_CountActiveOwnersFar
	.thumb_func
Party_CountActiveOwnersFar:
	.global GetPartyMemberCount
	.thumb_func
GetPartyMemberCount:
	overlay_veneer Func_08077148
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer Party_AddActiveOwnerFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
	.global Owner_RefreshActiveRatios
	.thumb_func
Owner_RefreshActiveRatios:
	overlay_veneer Owner_RefreshActiveRatiosFar
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
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	.global Object_GetByIdFar
	.thumb_func
Object_GetByIdFar:
	.global Object_GetById
	.thumb_func
Object_GetById:
	.global GetPartyInteractionRecord
	.thumb_func
GetPartyInteractionRecord:
	overlay_veneer Func_0808a080
	.global Engine_ActorDestroy
	.thumb_func
Engine_ActorDestroy:
	overlay_veneer ObjectTable_DestroyByIdFar
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer Object_RefreshSelectorByIdFar
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global Engine_ActorWalkTo
	.thumb_func
Engine_ActorWalkTo:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer Object_SetModeByIdFar
	.global Engine_ActorSetAnimationAndWait
	.thumb_func
Engine_ActorSetAnimationAndWait:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Engine_ActorStartRepeatedMotion
	.thumb_func
Engine_ActorStartRepeatedMotion:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Engine_ActorRunRepeatedMotion
	.thumb_func
Engine_ActorRunRepeatedMotion:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Engine_ActorFaceActor
	.thumb_func
Engine_ActorFaceActor:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Engine_ActorFaceEachOther
	.thumb_func
Engine_ActorFaceEachOther:
	overlay_veneer Object_LinkPairFar
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
	.global Engine_EventAskYesNo
	.thumb_func
Engine_EventAskYesNo:
	overlay_veneer BattleEventRuntime_ProcessActionFar
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Engine_ActorSetSpritePriority
	.thumb_func
Engine_ActorSetSpritePriority:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Engine_ActorShowEmote
	.thumb_func
Engine_ActorShowEmote:
	overlay_veneer BattleFx_SpawnLinkedFar
	.global Engine_CameraFollowActor
	.thumb_func
Engine_CameraFollowActor:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Engine_CameraSetSpeed
	.thumb_func
Engine_CameraSetSpeed:
	overlay_veneer ObjectTable_AllocateAndSetObjectSpeedFar
	.global Engine_CameraMoveTo
	.thumb_func
Engine_CameraMoveTo:
	overlay_veneer Motion_CamBoundsFar
	.global Engine_CameraWaitForMove
	.thumb_func
Engine_CameraWaitForMove:
	overlay_veneer BattleFx_CommitObjectPositionAndWaitFar
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer Event_SetValue170Far
	overlay_veneer BattleFx_SetWeightedResultFar
	overlay_veneer Party_SetFields1ceAnd1d0Far
	overlay_veneer Event_SetPair1d4Far
	.global Engine_LeaderCheckAhead
	.thumb_func
Engine_LeaderCheckAhead:
	overlay_veneer Field_TryJumpForwardFar
	.global Engine_EventOpenScreen
	.thumb_func
Engine_EventOpenScreen:
	overlay_veneer Event_SetStatus1c6Far
	.global Engine_EventCloseScreen
	.thumb_func
Engine_EventCloseScreen:
	overlay_veneer Event_ClearStatus1c6Far
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global ObjectTable_GetFar
	.thumb_func
ObjectTable_GetFar:
	overlay_veneer Func_0808a400
	overlay_veneer Battle_ResetEffectCounterFar
	.global Audio_PlayCueFromEventWork
	.thumb_func
Audio_PlayCueFromEventWork:
	overlay_veneer Audio_PlayCueFromEventWorkFar
	overlay_veneer Event_ClearInvalidPackedValuesFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Menu_OpenCharacterSelector
	.thumb_func
Menu_OpenCharacterSelector:
	overlay_veneer Menu_OpenCharacterSelectorFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
	.global AudioCommand_GetStateByte
	.thumb_func
AudioCommand_GetStateByte:
	overlay_veneer AudioCommand_GetStateByteFar
