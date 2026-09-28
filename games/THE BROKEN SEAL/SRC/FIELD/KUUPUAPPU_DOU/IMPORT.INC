	overlay_veneer 0x030003ac
	.global IwramUnsignedRemainder
	.thumb_func
IwramUnsignedRemainder:
	overlay_veneer 0x030003e0
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer 0x080000d1
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer 0x080000f9
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer 0x08000129
	overlay_veneer 0x08000319
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
	overlay_veneer 0x08009129
	.global Engine_ObjectSetPosition
	.thumb_func
Engine_ObjectSetPosition:
	.global Object_SetMoveTarget
	.thumb_func
Object_SetMoveTarget:
	overlay_veneer 0x08009151
	.global Engine_ObjectCommitPosition
	.thumb_func
Engine_ObjectCommitPosition:
	overlay_veneer 0x08009159
	.global Engine_MapCopyCells
	.thumb_func
Engine_MapCopyCells:
	overlay_veneer 0x080091b9
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
	overlay_veneer 0x080091f1
	overlay_veneer 0x080091f9
	.global Engine_MessageShowCentered
	.thumb_func
Engine_MessageShowCentered:
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
	overlay_veneer 0x0808a019
	overlay_veneer 0x0808a021
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer 0x0808a081
	overlay_veneer 0x0808a0f1
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer 0x0808a101
	.global Engine_ActorSetSpritePriority
	.thumb_func
Engine_ActorSetSpritePriority:
	overlay_veneer 0x0808a1e1
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
