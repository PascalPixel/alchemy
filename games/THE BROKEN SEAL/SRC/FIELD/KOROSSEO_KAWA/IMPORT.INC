	overlay_veneer 0x03000380
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	.global Engine_ScheduleCallback
	.thumb_func
Engine_ScheduleCallback:
	overlay_veneer 0x080000d1
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer 0x080000d9
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer 0x080000f9
	.global Math_Atan2
	.thumb_func
Math_Atan2:
	overlay_veneer 0x08000101
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer 0x08000129
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer 0x08000141
	.global Engine_AllocateBlock
	.thumb_func
Engine_AllocateBlock:
	overlay_veneer 0x08000149
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer 0x08000151
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer 0x08000171
	.global Runtime_BumpFree
	.thumb_func
Runtime_BumpFree:
	overlay_veneer 0x08000179
	.global Resource_DecodeType01
	.thumb_func
Resource_DecodeType01:
	overlay_veneer 0x080001a9
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer 0x080001b9
	overlay_veneer 0x080001c1
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	.global Engine_VramLoad
	.thumb_func
Engine_VramLoad:
	overlay_veneer 0x080001c9
	.global Resource_FindFreeEntryFar
	.thumb_func
Resource_FindFreeEntryFar:
	overlay_veneer 0x080001d1
	overlay_veneer 0x080001e1
	overlay_veneer 0x080001e9
	overlay_veneer 0x08000291
	.global ResourceMetadata_ClearRecordFar
	.thumb_func
ResourceMetadata_ClearRecordFar:
	overlay_veneer 0x08009061
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer 0x08009081
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer 0x08009099
	.global Object_CreateFar
	.thumb_func
Object_CreateFar:
	overlay_veneer 0x080090c9
	overlay_veneer 0x080090e1
	.global ObjectDispatch_InitFromTable6
	.thumb_func
ObjectDispatch_InitFromTable6:
	overlay_veneer 0x08009141
	.global ObjectDispatch_WaitForValue16
	.thumb_func
ObjectDispatch_WaitForValue16:
	overlay_veneer 0x08009149
	.global Engine_ObjectSetPosition
	.thumb_func
Engine_ObjectSetPosition:
	.global Object_SetMoveTarget
	.thumb_func
Object_SetMoveTarget:
	overlay_veneer 0x08009151
	.global Script_WaitForEventTimeout
	.thumb_func
Script_WaitForEventTimeout:
	overlay_veneer 0x08009159
	.global Map_GetTerrainHeightFar
	.thumb_func
Map_GetTerrainHeightFar:
	overlay_veneer 0x080091a9
	overlay_veneer 0x080091b9
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer 0x080091c1
	.global Map_UpdateCellRect
	.thumb_func
Map_UpdateCellRect:
	overlay_veneer 0x080091c9
	.global Object_CheckMovementCollision
	.thumb_func
Object_CheckMovementCollision:
	overlay_veneer 0x080091d9
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer 0x080091e1
	.global Engine_MessageShowCentered
	.thumb_func
Engine_MessageShowCentered:
	overlay_veneer 0x08015041
	.global UiWork_PushValueSlot
	.thumb_func
UiWork_PushValueSlot:
	overlay_veneer 0x08015121
	.global ItemIcon_LoadTilesFar
	.thumb_func
ItemIcon_LoadTilesFar:
	overlay_veneer 0x08015251
	overlay_veneer 0x080153a1
	.global Resource_LoadFixedBlockBIntoFreeSlot
	.thumb_func
Resource_LoadFixedBlockBIntoFreeSlot:
	overlay_veneer 0x080153b9
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer 0x08077009
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer 0x08077029
	.global Inventory_EquipFar
	.thumb_func
Inventory_EquipFar:
	overlay_veneer 0x08077051
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer 0x080770c1
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer 0x080770c9
	.global GameFlag_GetByteFar
	.thumb_func
GameFlag_GetByteFar:
	overlay_veneer 0x080770e1
	.global GameFlag_SetByteFar
	.thumb_func
GameFlag_SetByteFar:
	overlay_veneer 0x080770e9
	.global Party_CountActiveOwnersFar
	.thumb_func
Party_CountActiveOwnersFar:
	.global GetPartyMemberCount
	.thumb_func
GetPartyMemberCount:
	overlay_veneer 0x08077149
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer 0x08077151
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer 0x08077169
	.global Owner_RefreshActiveRatios
	.thumb_func
Owner_RefreshActiveRatios:
	overlay_veneer 0x08077261
	.global Engine_EventWait
	.thumb_func
Engine_EventWait:
	overlay_veneer 0x0808a011
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer 0x0808a019
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer 0x0808a021
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer 0x0808a071
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
	overlay_veneer 0x0808a081
	.global Engine_ActorDestroy
	.thumb_func
Engine_ActorDestroy:
	overlay_veneer 0x0808a089
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer 0x0808a091
	overlay_veneer 0x0808a0a1
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer 0x0808a0b9
	.global Engine_ActorWalkTo
	.thumb_func
Engine_ActorWalkTo:
	overlay_veneer 0x0808a0c9
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer 0x0808a0d1
	overlay_veneer 0x0808a0e9
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer 0x0808a0f1
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer 0x0808a101
	.global Engine_ActorSetAnimationAndWait
	.thumb_func
Engine_ActorSetAnimationAndWait:
	overlay_veneer 0x0808a111
	.global Engine_ActorStartRepeatedMotion
	.thumb_func
Engine_ActorStartRepeatedMotion:
	overlay_veneer 0x0808a131
	.global Engine_ActorRunRepeatedMotion
	.thumb_func
Engine_ActorRunRepeatedMotion:
	overlay_veneer 0x0808a139
	.global Engine_ActorFaceActor
	.thumb_func
Engine_ActorFaceActor:
	overlay_veneer 0x0808a149
	.global Engine_ActorFaceEachOther
	.thumb_func
Engine_ActorFaceEachOther:
	overlay_veneer 0x0808a151
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer 0x0808a171
	.global Engine_EventOpenMessage
	.thumb_func
Engine_EventOpenMessage:
	overlay_veneer 0x0808a179
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer 0x0808a181
	.global Engine_EventAskYesNo
	.thumb_func
Engine_EventAskYesNo:
	overlay_veneer 0x0808a191
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer 0x0808a1b9
	overlay_veneer 0x0808a1e1
	.global Engine_ActorShowEmote
	.thumb_func
Engine_ActorShowEmote:
	overlay_veneer 0x0808a1e9
	.global Engine_CameraFollowActor
	.thumb_func
Engine_CameraFollowActor:
	overlay_veneer 0x0808a201
	.global Engine_CameraSetSpeed
	.thumb_func
Engine_CameraSetSpeed:
	overlay_veneer 0x0808a209
	.global Engine_CameraMoveTo
	.thumb_func
Engine_CameraMoveTo:
	overlay_veneer 0x0808a211
	.global Engine_CameraWaitForMove
	.thumb_func
Engine_CameraWaitForMove:
	overlay_veneer 0x0808a219
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer 0x0808a249
	overlay_veneer 0x0808a251
	overlay_veneer 0x0808a261
	overlay_veneer 0x0808a269
	.global Engine_LeaderCheckAhead
	.thumb_func
Engine_LeaderCheckAhead:
	overlay_veneer 0x0808a2a1
	.global Engine_EventOpenScreen
	.thumb_func
Engine_EventOpenScreen:
	overlay_veneer 0x0808a361
	.global Engine_EventCloseScreen
	.thumb_func
Engine_EventCloseScreen:
	overlay_veneer 0x0808a369
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer 0x0808a371
	.global ObjectTable_GetFar
	.thumb_func
ObjectTable_GetFar:
	overlay_veneer 0x0808a401
	overlay_veneer 0x0808a461
	.global Audio_PlayCueFromEventWork
	.thumb_func
Audio_PlayCueFromEventWork:
	overlay_veneer 0x0808a4f1
	overlay_veneer 0x0808a549
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer 0x0808a589
	.global Menu_OpenCharacterSelector
	.thumb_func
Menu_OpenCharacterSelector:
	overlay_veneer 0x080a1019
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
	.global AudioCommand_GetStateByte
	.thumb_func
AudioCommand_GetStateByte:
	overlay_veneer 0x080f9049
