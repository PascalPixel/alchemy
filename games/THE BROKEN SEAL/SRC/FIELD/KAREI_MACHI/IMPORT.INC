	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
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
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer 0x080000f9
	.global Engine_MathSin
	.thumb_func
Engine_MathSin:
	overlay_veneer 0x08000119
	.global Engine_MathCos
	.thumb_func
Engine_MathCos:
	overlay_veneer 0x08000121
	.global Engine_HeapAllocate
	.thumb_func
Engine_HeapAllocate:
	.global AllocateEffectTransfer
	.thumb_func
AllocateEffectTransfer:
	overlay_veneer 0x08000141
	.global Engine_HeapRelease
	.thumb_func
Engine_HeapRelease:
	overlay_veneer 0x08000151
	.global Engine_VramLoad
	.thumb_func
Engine_VramLoad:
	overlay_veneer 0x080001c9
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
	.global Object_Destroy
	.thumb_func
Object_Destroy:
	overlay_veneer 0x080090d1
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer 0x08009129
	.global Object_SetPosition
	.thumb_func
Object_SetPosition:
	overlay_veneer 0x08009151
	.global Object_CommitPosition
	.thumb_func
Object_CommitPosition:
	overlay_veneer 0x08009159
	.global Engine_MapAnimateCells
	.thumb_func
Engine_MapAnimateCells:
	overlay_veneer 0x08009179
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer 0x08009181
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer 0x080091c1
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
	overlay_veneer 0x08015211
	.global Engine_ItemLoadIcon
	.thumb_func
Engine_ItemLoadIcon:
	overlay_veneer 0x08015251
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
	overlay_veneer 0x0808a031
	.global Engine_PartyGiveItem
	.thumb_func
Engine_PartyGiveItem:
	overlay_veneer 0x0808a061
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer 0x0808a071
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	.global GetOrbitingSceneObject
	.thumb_func
GetOrbitingSceneObject:
	.global Object_GetByIdFar
	.thumb_func
Object_GetByIdFar:
	overlay_veneer 0x0808a081
	.global Engine_ActorDestroy
	.thumb_func
Engine_ActorDestroy:
	overlay_veneer 0x0808a089
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer 0x0808a091
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer 0x0808a099
	overlay_veneer 0x0808a0a1
	.global Engine_ActorStop
	.thumb_func
Engine_ActorStop:
	overlay_veneer 0x0808a0a9
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer 0x0808a0b1
	.global Engine_ActorWalkTo
	.thumb_func
Engine_ActorWalkTo:
	overlay_veneer 0x0808a0c9
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer 0x0808a0d1
	.global Engine_ActorCenterAndWalk
	.thumb_func
Engine_ActorCenterAndWalk:
	overlay_veneer 0x0808a0d9
	.global Engine_ActorSetDestinationOffset
	.thumb_func
Engine_ActorSetDestinationOffset:
	overlay_veneer 0x0808a0e1
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
	.global Engine_ActorFaceEachOther
	.thumb_func
Engine_ActorFaceEachOther:
	overlay_veneer 0x0808a151
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
	overlay_veneer 0x0808a159
	.global Engine_ObjectSetPalette
	.thumb_func
Engine_ObjectSetPalette:
	overlay_veneer 0x0808a161
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
	.global Engine_EventShowMessageAndWait
	.thumb_func
Engine_EventShowMessageAndWait:
	overlay_veneer 0x0808a189
	.global Engine_EventAskYesNo
	.thumb_func
Engine_EventAskYesNo:
	overlay_veneer 0x0808a191
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer 0x0808a1b9
	overlay_veneer 0x0808a1d9
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
	.global Engine_CameraSetSpeed
	.thumb_func
Engine_CameraSetSpeed:
	overlay_veneer 0x0808a209
	.global Engine_CameraMoveTo
	.thumb_func
Engine_CameraMoveTo:
	overlay_veneer 0x0808a211
	.global Battle_GetWorkObject1e0
	.thumb_func
Battle_GetWorkObject1e0:
	overlay_veneer 0x0808a229
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer 0x0808a249
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
	.global Engine_ItemShowFound
	.thumb_func
Engine_ItemShowFound:
	overlay_veneer 0x0808a399
	.global BattleFx_StartFadeOverlay
	.thumb_func
BattleFx_StartFadeOverlay:
	overlay_veneer 0x0808a409
	overlay_veneer 0x0808a4f1
	.global BattleFx_RunPageEffectForSlot
	.thumb_func
BattleFx_RunPageEffectForSlot:
	overlay_veneer 0x0808a4f9
	.global BattleFx_SetQueuedSoundAndPlay
	.thumb_func
BattleFx_SetQueuedSoundAndPlay:
	overlay_veneer 0x0808a5e1
	.global BattleFx_PlayQueuedSound
	.thumb_func
BattleFx_PlayQueuedSound:
	overlay_veneer 0x0808a5e9
	.global Engine_ShopOpen
	.thumb_func
Engine_ShopOpen:
	overlay_veneer 0x080b0009
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
