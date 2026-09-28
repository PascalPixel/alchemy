	overlay_veneer 0x03000380
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer 0x080000d1
	.global Engine_TaskRemoveCallback
	.thumb_func
Engine_TaskRemoveCallback:
	overlay_veneer 0x080000d9
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer 0x08000101
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer 0x08000129
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer 0x08009081
	overlay_veneer 0x08009099
	overlay_veneer 0x080090c9
	overlay_veneer 0x08009179
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
	overlay_veneer 0x080091e9
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
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
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
	.global Engine_ObjectMotionSetPositionAndCommit
	.thumb_func
Engine_ObjectMotionSetPositionAndCommit:
	overlay_veneer 0x0808a0c1
	overlay_veneer 0x0808a0c9
	overlay_veneer 0x0808a0d1
	overlay_veneer 0x0808a0d9
	.global Engine_ActorSetDestinationOffset
	.thumb_func
Engine_ActorSetDestinationOffset:
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
	overlay_veneer 0x0808a101
	overlay_veneer 0x0808a131
	.global Engine_ActorRunRepeatedMotion
	.thumb_func
Engine_ActorRunRepeatedMotion:
	overlay_veneer 0x0808a139
	.global Engine_ActorFaceActor
	.thumb_func
Engine_ActorFaceActor:
	overlay_veneer 0x0808a149
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
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer 0x0808a181
	.global Engine_EventShowMessageAndWait
	.thumb_func
Engine_EventShowMessageAndWait:
	overlay_veneer 0x0808a189
	overlay_veneer 0x0808a191
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer 0x0808a1b9
	overlay_veneer 0x0808a1e1
	overlay_veneer 0x0808a1e9
	overlay_veneer 0x0808a249
	overlay_veneer 0x0808a4f9
	.global BattleFx_SetQueuedSoundAndPlay
	.thumb_func
BattleFx_SetQueuedSoundAndPlay:
	overlay_veneer 0x0808a5e1
	.global BattleFx_PlayQueuedSound
	.thumb_func
BattleFx_PlayQueuedSound:
	overlay_veneer 0x0808a5e9
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
