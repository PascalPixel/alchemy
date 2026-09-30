@ Import veneers after the KOROSSEO_MARUTA overlay's code: fixed 8-byte
@ veneers through which the overlay calls main-image code, each loading its
@ target into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — KOROSSEO_MARUTA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathModulo
	.thumb_func
Engine_MathModulo:
	overlay_veneer IwramSignedRemainder
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Engine_TaskRemoveCallback
	.thumb_func
Engine_TaskRemoveCallback:
	overlay_veneer Scheduler_RemoveCallbackFar
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
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
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
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global AffineMatrix_BuildForEffect
	.thumb_func
AffineMatrix_BuildForEffect:
	overlay_veneer AffineMatrix_BuildForEffectFar
	.global Runtime_PushSlotEntry
	.thumb_func
Runtime_PushSlotEntry:
	overlay_veneer Runtime_PushSlotEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global ResourceMetadata_ClearRecordFar
	.thumb_func
ResourceMetadata_ClearRecordFar:
	overlay_veneer Func_08009060
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildrenFar
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Object_CreateFar
	.global ObjectDispatch_InitFromTable4WithArgument
	.thumb_func
ObjectDispatch_InitFromTable4WithArgument:
	overlay_veneer ObjectDispatch_InitFromTable4WithArgumentFar
	.global ObjectDispatch_InitFromTable6
	.thumb_func
ObjectDispatch_InitFromTable6:
	overlay_veneer ObjectDispatch_InitFromTable6Far
	.global ObjectDispatch_WaitForValue16
	.thumb_func
ObjectDispatch_WaitForValue16:
	overlay_veneer ObjectDispatch_WaitForValue16Far
	.global Object_SetPosition
	.thumb_func
Object_SetPosition:
	.global Engine_ObjectSetPosition
	.thumb_func
Engine_ObjectSetPosition:
	overlay_veneer Object_SetMoveTargetFar
	.global Object_CommitPosition
	.thumb_func
Object_CommitPosition:
	overlay_veneer Script_WaitForEventTimeoutFar
	.global Map_ClearLayerEntryFlag
	.thumb_func
Map_ClearLayerEntryFlag:
	overlay_veneer Map_ClearLayerEntryFlagFar
	.global Map_SetLayerEntryFlag
	.thumb_func
Map_SetLayerEntryFlag:
	overlay_veneer Map_SetLayerEntryFlagFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global GetMapCellCollision
	.thumb_func
GetMapCellCollision:
	overlay_veneer GetMapCellCollisionFar
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Func_080091c8
	.global Object_CheckMovementCollision
	.thumb_func
Object_CheckMovementCollision:
	overlay_veneer Func_080091d8
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Graphics_EnableObjLayerAndCallbacks
	.thumb_func
Graphics_EnableObjLayerAndCallbacks:
	overlay_veneer Graphics_EnableObjLayerAndCallbacksFar
	.global SetMapCellCollision
	.thumb_func
SetMapCellCollision:
	overlay_veneer SetMapCellCollisionFar
	.global Engine_MessageShowCentered
	.thumb_func
Engine_MessageShowCentered:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global UiText_DrawQuantity
	.thumb_func
UiText_DrawQuantity:
	overlay_veneer UiWork_PushValueSlotFar
	.global ItemIcon_LoadTilesFar
	.thumb_func
ItemIcon_LoadTilesFar:
	overlay_veneer Ui_PrepareTransferForItemFar
	.global PartyTalkMenu_Choose
	.thumb_func
PartyTalkMenu_Choose:
	overlay_veneer PartyTalkMenu_ChooseFar
	.global Resource_LoadFixedBlockBIntoFreeSlot
	.thumb_func
Resource_LoadFixedBlockBIntoFreeSlot:
	overlay_veneer Resource_LoadFixedBlockBIntoFreeSlotFar
	.global Engine_OwnerGetState
	.thumb_func
Engine_OwnerGetState:
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer Inventory_AddItemFar
	.global Inventory_Equip
	.thumb_func
Inventory_Equip:
	overlay_veneer Inventory_EquipFar
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global Engine_GameFlagClear
	.thumb_func
Engine_GameFlagClear:
	overlay_veneer GameFlag_ClearBitFar
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global GetPartyMemberCount
	.thumb_func
GetPartyMemberCount:
	overlay_veneer Party_CountActiveOwnersFar
	.global Engine_PartyAddActiveOwner
	.thumb_func
Engine_PartyAddActiveOwner:
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer Party_AddActiveOwnerFar
	.global Engine_PartyRemoveActiveOwner
	.thumb_func
Engine_PartyRemoveActiveOwner:
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
	.global Object_GetByIdFar
	.thumb_func
Object_GetByIdFar:
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Func_0808a080
	.global Engine_ActorDestroy
	.thumb_func
Engine_ActorDestroy:
	overlay_veneer ObjectTable_DestroyByIdFar
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
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
	.global Engine_ActorSetDestinationOffset
	.thumb_func
Engine_ActorSetDestinationOffset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global Engine_ActorWaitForMove
	.thumb_func
Engine_ActorWaitForMove:
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
	.global BattleFx_RunRisingObjectSequence
	.thumb_func
BattleFx_RunRisingObjectSequence:
	overlay_veneer BattleFx_RunRisingObjectSequenceFar
	.global Engine_ActorFaceActor
	.thumb_func
Engine_ActorFaceActor:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Engine_ActorFaceEachOther
	.thumb_func
Engine_ActorFaceEachOther:
	overlay_veneer Object_LinkPairFar
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
	.global Engine_ActorSetAttachedEffect
	.thumb_func
Engine_ActorSetAttachedEffect:
	overlay_veneer BattleFx_ConfigureLinkedObjectFar
	.global Engine_ObjectAttachWorkTarget
	.thumb_func
Engine_ObjectAttachWorkTarget:
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
	.global BattleFx_SetWeightedResult
	.thumb_func
BattleFx_SetWeightedResult:
	overlay_veneer BattleFx_SetWeightedResultFar
	.global Party_SetFields1ceAnd1d0
	.thumb_func
Party_SetFields1ceAnd1d0:
	overlay_veneer Party_SetFields1ceAnd1d0Far
	.global Event_SetPair1d4
	.thumb_func
Event_SetPair1d4:
	overlay_veneer Event_SetPair1d4Far
	.global Engine_LeaderCheckAhead
	.thumb_func
Engine_LeaderCheckAhead:
	overlay_veneer Field_TryJumpForwardFar
	.global battle_owner_69
	.thumb_func
battle_owner_69:
	overlay_veneer battle_owner_69Far
	.global Engine_ColorBufferApplyTarget
	.thumb_func
Engine_ColorBufferApplyTarget:
	overlay_veneer BattleFx_ApplyColorToTargetBufferFar
	.global Engine_ColorBufferInterpolate
	.thumb_func
Engine_ColorBufferInterpolate:
	overlay_veneer BattleFx_StartBufferInterpolationFar
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
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	overlay_veneer ObjectTable_GetFar
	.global Battle_ResetEffectCounter
	.thumb_func
Battle_ResetEffectCounter:
	overlay_veneer Battle_ResetEffectCounterFar
	.global Audio_PlayCueFromEventWork
	.thumb_func
Audio_PlayCueFromEventWork:
	overlay_veneer Audio_PlayCueFromEventWorkFar
	.global Engine_EventClearInvalidPackedValues
	.thumb_func
Engine_EventClearInvalidPackedValues:
	overlay_veneer Event_ClearInvalidPackedValuesFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global BattleEffect_PauseObject
	.thumb_func
BattleEffect_PauseObject:
	overlay_veneer BattleEffect_PauseObjectFar
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
