	overlay_veneer 0x030003e0
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	overlay_veneer 0x080000d1
	overlay_veneer 0x080000d9
	overlay_veneer 0x080000f9
	overlay_veneer 0x08000119
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer 0x080001b9
	.global Object_InitializeMode
	.thumb_func
Object_InitializeMode:
	overlay_veneer 0x08009021
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer 0x080090c9
	overlay_veneer 0x080090d1
	overlay_veneer 0x08009129
	overlay_veneer 0x08009179
	overlay_veneer 0x08009181
	overlay_veneer 0x08009189
	overlay_veneer 0x08009191
	overlay_veneer 0x080091c1
	overlay_veneer 0x080091e1
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer 0x080091f1
	.global Engine_MapWaitWorkValuesBelow256
	.thumb_func
Engine_MapWaitWorkValuesBelow256:
	overlay_veneer 0x080091f9
	overlay_veneer 0x08009241
	overlay_veneer 0x08015041
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer 0x080770c1
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer 0x080770c9
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
	overlay_veneer 0x0808a049
	overlay_veneer 0x0808a059
	overlay_veneer 0x0808a071
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer 0x0808a081
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
	overlay_veneer 0x0808a0a9
	overlay_veneer 0x0808a0b1
	overlay_veneer 0x0808a0b9
	overlay_veneer 0x0808a0c1
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
	overlay_veneer 0x0808a129
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
	overlay_veneer 0x0808a159
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer 0x0808a171
	overlay_veneer 0x0808a179
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
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer 0x0808a1e1
	overlay_veneer 0x0808a1e9
	overlay_veneer 0x0808a1f1
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
	overlay_veneer 0x0808a229
	overlay_veneer 0x0808a249
	overlay_veneer 0x0808a251
	overlay_veneer 0x0808a2c1
	overlay_veneer 0x0808a2c9
	overlay_veneer 0x0808a2d1
	overlay_veneer 0x0808a2d9
	overlay_veneer 0x0808a2e1
	overlay_veneer 0x0808a331
	overlay_veneer 0x0808a349
	overlay_veneer 0x0808a361
	overlay_veneer 0x0808a371
	overlay_veneer 0x0808a429
	overlay_veneer 0x0808a441
	overlay_veneer 0x0808a5e1
	.global BattleFx_PlayQueuedSound
	.thumb_func
BattleFx_PlayQueuedSound:
	overlay_veneer 0x0808a5e9
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
