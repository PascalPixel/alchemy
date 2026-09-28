	overlay_veneer 0x080000c1
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer 0x080000d1
	overlay_veneer 0x080000d9
	overlay_veneer 0x080000f9
	overlay_veneer 0x08000119
	overlay_veneer 0x08000121
	.global Engine_HeapAllocate
	.thumb_func
Engine_HeapAllocate:
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
	overlay_veneer 0x08009099
	overlay_veneer 0x080090c9
	overlay_veneer 0x080090d1
	overlay_veneer 0x08009129
	overlay_veneer 0x08009151
	overlay_veneer 0x08009159
	.global Engine_MapAnimateCells
	.thumb_func
Engine_MapAnimateCells:
	overlay_veneer 0x08009179
	overlay_veneer 0x08009181
	overlay_veneer 0x080091c1
	overlay_veneer 0x080091d9
	overlay_veneer 0x080091e1
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
	overlay_veneer 0x0808a061
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
	overlay_veneer 0x0808a0c9
	overlay_veneer 0x0808a0d1
	.global Engine_ActorCenterAndWalk
	.thumb_func
Engine_ActorCenterAndWalk:
	overlay_veneer 0x0808a0d9
	overlay_veneer 0x0808a0e1
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer 0x0808a0f1
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer 0x0808a101
	overlay_veneer 0x0808a111
	overlay_veneer 0x0808a129
	overlay_veneer 0x0808a131
	overlay_veneer 0x0808a139
	overlay_veneer 0x0808a151
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
	overlay_veneer 0x0808a159
	overlay_veneer 0x0808a161
	overlay_veneer 0x0808a171
	overlay_veneer 0x0808a179
	overlay_veneer 0x0808a181
	overlay_veneer 0x0808a189
	overlay_veneer 0x0808a191
	overlay_veneer 0x0808a1b9
	overlay_veneer 0x0808a1d9
	overlay_veneer 0x0808a1e1
	overlay_veneer 0x0808a1e9
	overlay_veneer 0x0808a1f1
	overlay_veneer 0x0808a209
	overlay_veneer 0x0808a211
	overlay_veneer 0x0808a229
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer 0x0808a249
	overlay_veneer 0x0808a361
	.global Engine_EventCloseScreen
	.thumb_func
Engine_EventCloseScreen:
	overlay_veneer 0x0808a369
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer 0x0808a371
	overlay_veneer 0x0808a399
	overlay_veneer 0x0808a409
	overlay_veneer 0x0808a4f1
	overlay_veneer 0x0808a4f9
	overlay_veneer 0x0808a5e1
	overlay_veneer 0x0808a5e9
	overlay_veneer 0x080b0009
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
