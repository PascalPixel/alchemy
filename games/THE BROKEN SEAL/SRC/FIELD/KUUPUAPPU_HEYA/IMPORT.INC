	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	.global Engine_ScheduleCallback
	.thumb_func
Engine_ScheduleCallback:
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer 0x080000d1
	.global Scheduler_RemoveCallback
	.thumb_func
Scheduler_RemoveCallback:
	overlay_veneer 0x080000d9
	overlay_veneer 0x080000f9
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer 0x08000101
	.global Runtime_AllocateHeapBlock
	.thumb_func
Runtime_AllocateHeapBlock:
	overlay_veneer 0x08000141
	.global Engine_HeapRelease
	.thumb_func
Engine_HeapRelease:
	overlay_veneer 0x08000151
	.global Engine_VramLoad
	.thumb_func
Engine_VramLoad:
	overlay_veneer 0x080001c9
	overlay_veneer 0x08009081
	.global Object_CreateFar
	.thumb_func
Object_CreateFar:
	overlay_veneer 0x080090c9
	overlay_veneer 0x080090d1
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer 0x08009129
	.global Object_SetMoveTarget
	.thumb_func
Object_SetMoveTarget:
	overlay_veneer 0x08009151
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer 0x08009181
	.global Map_CopyMetatileCellsRect
	.thumb_func
Map_CopyMetatileCellsRect:
	overlay_veneer 0x080091b9
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer 0x080091c1
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer 0x080091e1
	.global Engine_MessageShowCentered
	.thumb_func
Engine_MessageShowCentered:
	overlay_veneer 0x08015041
	.global Engine_ItemLoadIcon
	.thumb_func
Engine_ItemLoadIcon:
	overlay_veneer 0x08015251
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer 0x08077009
	.global Owner_RecalculateStats
	.thumb_func
Owner_RecalculateStats:
	overlay_veneer 0x08077011
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer 0x080770c1
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer 0x080770c9
	.global Engine_GameFlagClear
	.thumb_func
Engine_GameFlagClear:
	overlay_veneer 0x080770d1
	.global Djinn_Transfer
	.thumb_func
Djinn_Transfer:
	overlay_veneer 0x080771d1
	.global PartyInventory_HasSpace
	.thumb_func
PartyInventory_HasSpace:
	overlay_veneer 0x08077251
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
	overlay_veneer 0x0808a039
	.global Event_PrepareObjectAndApplyValue
	.thumb_func
Event_PrepareObjectAndApplyValue:
	overlay_veneer 0x0808a049
	overlay_veneer 0x0808a059
	.global Engine_PartyGiveItem
	.thumb_func
Engine_PartyGiveItem:
	overlay_veneer 0x0808a061
	.global Engine_UiWorkWaitThenFinalizeCapacity
	.thumb_func
Engine_UiWorkWaitThenFinalizeCapacity:
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer 0x0808a071
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer 0x0808a081
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer 0x0808a091
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer 0x0808a099
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer 0x0808a0a1
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer 0x0808a0b9
	overlay_veneer 0x0808a0c1
	.global Engine_ActorWalkTo
	.thumb_func
Engine_ActorWalkTo:
	overlay_veneer 0x0808a0c9
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer 0x0808a0d1
	overlay_veneer 0x0808a0e1
	.global Engine_ActorWaitForMove
	.thumb_func
Engine_ActorWaitForMove:
	overlay_veneer 0x0808a0e9
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer 0x0808a0f1
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer 0x0808a101
	.global Engine_ActorSetAnimationAndWait
	.thumb_func
Engine_ActorSetAnimationAndWait:
	overlay_veneer 0x0808a111
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer 0x0808a119
	.global Engine_ActorJump
	.thumb_func
Engine_ActorJump:
	overlay_veneer 0x0808a129
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
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer 0x0808a1b9
	.global Engine_ActorSetSpritePriority
	.thumb_func
Engine_ActorSetSpritePriority:
	overlay_veneer 0x0808a1e1
	.global Engine_ActorShowEmote
	.thumb_func
Engine_ActorShowEmote:
	overlay_veneer 0x0808a1e9
	.global Engine_ActorSetAttachedEffect
	.thumb_func
Engine_ActorSetAttachedEffect:
	overlay_veneer 0x0808a1f1
	.global Engine_CameraFollowActor
	.thumb_func
Engine_CameraFollowActor:
	overlay_veneer 0x0808a201
	overlay_veneer 0x0808a211
	.global Engine_CameraWaitForMove
	.thumb_func
Engine_CameraWaitForMove:
	overlay_veneer 0x0808a219
	.global Map_SetWorkFourValues
	.thumb_func
Map_SetWorkFourValues:
	overlay_veneer 0x0808a231
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
	.global Battle_InitializeRenderObject
	.thumb_func
Battle_InitializeRenderObject:
	overlay_veneer 0x0808a2b9
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
	overlay_veneer 0x0808a389
	.global Engine_ItemShowFound
	.thumb_func
Engine_ItemShowFound:
	overlay_veneer 0x0808a399
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	.global Engine_ActorLookup
	.thumb_func
Engine_ActorLookup:
	overlay_veneer 0x0808a401
	.global Engine_PsynergyPlayEffect
	.thumb_func
Engine_PsynergyPlayEffect:
	overlay_veneer 0x0808a421
	.global Engine_PsynergyBegin
	.thumb_func
Engine_PsynergyBegin:
	overlay_veneer 0x0808a429
	.global Engine_PsynergySetTarget
	.thumb_func
Engine_PsynergySetTarget:
	overlay_veneer 0x0808a431
	.global Engine_PsynergyLowerHands
	.thumb_func
Engine_PsynergyLowerHands:
	overlay_veneer 0x0808a439
	.global BattleEffect_CleanupSceneObjects
	.thumb_func
BattleEffect_CleanupSceneObjects:
	overlay_veneer 0x0808a441
	.global Engine_PsynergyRaiseHands
	.thumb_func
Engine_PsynergyRaiseHands:
	overlay_veneer 0x0808a449
	.global Ui_SetBank15PaletteAndClearRenderMode
	.thumb_func
Ui_SetBank15PaletteAndClearRenderMode:
	overlay_veneer 0x0808a481
	.global Engine_AudioPlaySceneCue
	.thumb_func
Engine_AudioPlaySceneCue:
	.global Audio_PlayCueFromEventWork
	.thumb_func
Audio_PlayCueFromEventWork:
	overlay_veneer 0x0808a4f1
	.global Engine_ShopOpen
	.thumb_func
Engine_ShopOpen:
	overlay_veneer 0x080b0009
	.global Engine_InnOpen
	.thumb_func
Engine_InnOpen:
	overlay_veneer 0x080b0019
	.global BattlePlacement_UpdateTimedEntriesTwentyTimes
	.thumb_func
BattlePlacement_UpdateTimedEntriesTwentyTimes:
	overlay_veneer 0x080b5109
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
