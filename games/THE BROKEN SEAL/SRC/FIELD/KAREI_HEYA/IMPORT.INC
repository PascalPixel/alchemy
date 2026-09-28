	.global Engine_MapAnimateCells
	.thumb_func
Engine_MapAnimateCells:
	overlay_veneer 0x08009179
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer 0x08009181
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer 0x080770c1
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer 0x080770c9
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
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer 0x0808a081
	.global Engine_ActorDestroy
	.thumb_func
Engine_ActorDestroy:
	overlay_veneer 0x0808a089
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer 0x0808a091
	.global Engine_ActorCenterAndWalk
	.thumb_func
Engine_ActorCenterAndWalk:
	overlay_veneer 0x0808a0d9
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer 0x0808a101
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
	overlay_veneer 0x0808a159
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer 0x0808a171
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer 0x0808a181
	.global Engine_EventAskYesNo
	.thumb_func
Engine_EventAskYesNo:
	overlay_veneer 0x0808a191
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer 0x0808a249
	.global Engine_EventCloseScreen
	.thumb_func
Engine_EventCloseScreen:
	overlay_veneer 0x0808a369
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer 0x0808a371
	.global Engine_SanctumOpen
	.thumb_func
Engine_SanctumOpen:
	overlay_veneer 0x080b0011
	.global Engine_InnOpen
	.thumb_func
Engine_InnOpen:
	overlay_veneer 0x080b0019
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
