	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer 0x08009129
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer 0x08009181
	.global Map_ClearLayerEntryFlag
	.thumb_func
Map_ClearLayerEntryFlag:
	overlay_veneer 0x08009189
	.global Map_SetLayerEntryFlag
	.thumb_func
Map_SetLayerEntryFlag:
	overlay_veneer 0x08009191
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer 0x080091c1
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
	overlay_veneer 0x0808a039
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer 0x0808a071
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
	.global Engine_ActorStartRepeatedMotion
	.thumb_func
Engine_ActorStartRepeatedMotion:
	overlay_veneer 0x0808a131
	.global Engine_ActorRunRepeatedMotion
	.thumb_func
Engine_ActorRunRepeatedMotion:
	overlay_veneer 0x0808a139
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
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer 0x0808a249
	.global Engine_ColorBufferApplyTarget
	.thumb_func
Engine_ColorBufferApplyTarget:
	overlay_veneer 0x0808a331
	.global Engine_ColorBufferApplySource
	.thumb_func
Engine_ColorBufferApplySource:
	overlay_veneer 0x0808a339
	.global Engine_ColorBufferInterpolate
	.thumb_func
Engine_ColorBufferInterpolate:
	overlay_veneer 0x0808a349
	.global Engine_EventOpenScreen
	.thumb_func
Engine_EventOpenScreen:
	overlay_veneer 0x0808a361
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer 0x0808a371
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
