	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	overlay_veneer 0x080000d1
	overlay_veneer 0x080000d9
	overlay_veneer 0x080000f9
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
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer 0x080091c1
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer 0x080091f1
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
	.global Engine_UiWorkWaitThenFinalizeCapacity
	.thumb_func
Engine_UiWorkWaitThenFinalizeCapacity:
	overlay_veneer 0x0808a071
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer 0x0808a081
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer 0x0808a091
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
	.global Engine_ActorJump
	.thumb_func
Engine_ActorJump:
	overlay_veneer 0x0808a129
	.global Engine_ActorRunRepeatedMotion
	.thumb_func
Engine_ActorRunRepeatedMotion:
	overlay_veneer 0x0808a139
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer 0x0808a151
	.global ObjectGroup_ConfigureChildValue
	.thumb_func
ObjectGroup_ConfigureChildValue:
	overlay_veneer 0x0808a159
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer 0x0808a171
	.global Engine_EventOpenMessage
	.thumb_func
Engine_EventOpenMessage:
	overlay_veneer 0x0808a179
	.global BattleEv_RunWait
	.thumb_func
BattleEv_RunWait:
	overlay_veneer 0x0808a181
	overlay_veneer 0x0808a189
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer 0x0808a1b9
	.global BattleFx_SpawnLinked
	.thumb_func
BattleFx_SpawnLinked:
	overlay_veneer 0x0808a1e9
	.global Engine_ActorSetAttachedEffect
	.thumb_func
Engine_ActorSetAttachedEffect:
	overlay_veneer 0x0808a1f1
	overlay_veneer 0x0808a201
	.global ObjectTable_AllocateAndSetObjectSpeed
	.thumb_func
ObjectTable_AllocateAndSetObjectSpeed:
	overlay_veneer 0x0808a209
	.global Engine_CameraMoveTo
	.thumb_func
Engine_CameraMoveTo:
	overlay_veneer 0x0808a211
	overlay_veneer 0x0808a219
	.global Event_SetValue170
	.thumb_func
Event_SetValue170:
	overlay_veneer 0x0808a249
	.global InitializeSceneRecordBuffer
	.thumb_func
InitializeSceneRecordBuffer:
	overlay_veneer 0x0808a2f9
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
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer 0x0808a369
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer 0x0808a371
	.global BattleFx_SetQueuedSoundAndPlay
	.thumb_func
BattleFx_SetQueuedSoundAndPlay:
	overlay_veneer 0x0808a5e1
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
