	overlay_veneer 0x03000380
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer 0x080000c1
	overlay_veneer 0x080000f9
	overlay_veneer 0x08000119
	overlay_veneer 0x08000121
	overlay_veneer 0x08000141
	overlay_veneer 0x08000151
	overlay_veneer 0x080001c9
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer 0x08009081
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
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
	overlay_veneer 0x080091c1
	.global Object_CheckMovementCollision
	.thumb_func
Object_CheckMovementCollision:
	overlay_veneer 0x080091d9
	overlay_veneer 0x080091e1
	overlay_veneer 0x080091e9
	overlay_veneer 0x080091f1
	overlay_veneer 0x080091f9
	overlay_veneer 0x08015041
	overlay_veneer 0x08015251
	overlay_veneer 0x080770c1
	overlay_veneer 0x080770c9
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer 0x0808a011
	overlay_veneer 0x0808a019
	overlay_veneer 0x0808a021
	overlay_veneer 0x0808a061
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
	overlay_veneer 0x0808a091
	overlay_veneer 0x0808a099
	overlay_veneer 0x0808a0c1
	overlay_veneer 0x0808a0c9
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer 0x0808a0e1
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer 0x0808a0e9
	overlay_veneer 0x0808a0f1
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer 0x0808a101
	overlay_veneer 0x0808a131
	overlay_veneer 0x0808a139
	overlay_veneer 0x0808a149
	overlay_veneer 0x0808a151
	overlay_veneer 0x0808a159
	.global Engine_ObjectSetPalette
	.thumb_func
Engine_ObjectSetPalette:
	overlay_veneer 0x0808a161
	overlay_veneer 0x0808a1b9
	overlay_veneer 0x0808a1e1
	overlay_veneer 0x0808a1e9
	overlay_veneer 0x0808a1f1
	overlay_veneer 0x0808a201
	overlay_veneer 0x0808a209
	overlay_veneer 0x0808a211
	overlay_veneer 0x0808a219
	overlay_veneer 0x0808a221
	overlay_veneer 0x0808a251
	overlay_veneer 0x0808a259
	overlay_veneer 0x0808a261
	overlay_veneer 0x0808a361
	overlay_veneer 0x0808a371
	overlay_veneer 0x0808a399
	.global BattleFx_PlayQueuedSound
	.thumb_func
BattleFx_PlayQueuedSound:
	overlay_veneer 0x0808a5e9
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	overlay_veneer 0x080f9011
