	overlay_veneer 0x080000c1
	.global Engine_ScheduleCallback
	.thumb_func
Engine_ScheduleCallback:
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer 0x080000d1
	.global Scheduler_RemoveCallback
	.thumb_func
Scheduler_RemoveCallback:
	overlay_veneer 0x080000d9
	overlay_veneer 0x080000f9
	overlay_veneer 0x08000101
	overlay_veneer 0x08000141
	overlay_veneer 0x08000151
	overlay_veneer 0x080001c9
	overlay_veneer 0x08009081
	overlay_veneer 0x080090c9
	overlay_veneer 0x080090d1
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer 0x08009129
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
	overlay_veneer 0x08015251
	overlay_veneer 0x08077009
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
	overlay_veneer 0x080771d1
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
	overlay_veneer 0x0808a0a1
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
	overlay_veneer 0x0808a101
	.global Engine_ActorSetAnimationAndWait
	.thumb_func
Engine_ActorSetAnimationAndWait:
	overlay_veneer 0x0808a111
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
	overlay_veneer 0x0808a2a1
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
	overlay_veneer 0x0808a399
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	.global Engine_ActorLookup
	.thumb_func
Engine_ActorLookup:
	overlay_veneer 0x0808a401
	overlay_veneer 0x0808a421
	overlay_veneer 0x0808a429
	overlay_veneer 0x0808a431
	overlay_veneer 0x0808a439
	overlay_veneer 0x0808a441
	overlay_veneer 0x0808a449
	.global Ui_SetBank15PaletteAndClearRenderMode
	.thumb_func
Ui_SetBank15PaletteAndClearRenderMode:
	overlay_veneer 0x0808a481
	.global Engine_AudioPlaySceneCue
	.thumb_func
Engine_AudioPlaySceneCue:
	overlay_veneer 0x0808a4f1
	overlay_veneer 0x080b0009
	overlay_veneer 0x080b0019
	overlay_veneer 0x080b5109
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
