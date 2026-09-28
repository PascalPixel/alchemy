	overlay_veneer 0x03000380
	.global WaitFrames
	.thumb_func
WaitFrames:
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	.global Engine_MathSin
	.thumb_func
Engine_MathSin:
	overlay_veneer 0x08000119
	.global Engine_MathCos
	.thumb_func
Engine_MathCos:
	overlay_veneer 0x08000121
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	overlay_veneer 0x08009081
	overlay_veneer 0x08009099
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer 0x080090c9
	.global Object_SetPosition
	.thumb_func
Object_SetPosition:
	overlay_veneer 0x08009151
	.global Object_CommitPosition
	.thumb_func
Object_CommitPosition:
	overlay_veneer 0x08009159
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
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
	.global Engine_ObjectSetBlendMode
	.thumb_func
Engine_ObjectSetBlendMode:
	overlay_veneer 0x080091e9
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer 0x080091f1
	.global Map_WaitWorkValuesBelow256
	.thumb_func
Map_WaitWorkValuesBelow256:
	overlay_veneer 0x080091f9
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
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
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
	overlay_veneer 0x0808a071
	.global Object_GetById
	.thumb_func
Object_GetById:
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer 0x0808a081
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer 0x0808a091
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer 0x0808a0b9
	overlay_veneer 0x0808a0c9
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer 0x0808a0e1
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	.global Engine_ActorWaitForMove
	.thumb_func
Engine_ActorWaitForMove:
	overlay_veneer 0x0808a0e9
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer 0x0808a0f1
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer 0x0808a101
	.global Engine_ActorFaceActor
	.thumb_func
Engine_ActorFaceActor:
	overlay_veneer 0x0808a149
	.global Engine_ActorFaceEachOther
	.thumb_func
Engine_ActorFaceEachOther:
	overlay_veneer 0x0808a151
	.global Engine_ObjectSetPalette
	.thumb_func
Engine_ObjectSetPalette:
	overlay_veneer 0x0808a161
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer 0x0808a171
	overlay_veneer 0x0808a179
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer 0x0808a181
	overlay_veneer 0x0808a1b9
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer 0x0808a249
	.global Engine_LeaderCheckAhead
	.thumb_func
Engine_LeaderCheckAhead:
	overlay_veneer 0x0808a2a1
	.global Engine_EventCloseScreen
	.thumb_func
Engine_EventCloseScreen:
	overlay_veneer 0x0808a369
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer 0x0808a371
	.global BattleFx_PlayQueuedSound
	.thumb_func
BattleFx_PlayQueuedSound:
	overlay_veneer 0x0808a5e9
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
