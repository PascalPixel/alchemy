	overlay_veneer 0x03000380
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
	overlay_veneer 0x08009081
	overlay_veneer 0x08009099
	overlay_veneer 0x080090c9
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer 0x080091c1
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer 0x080091e1
	overlay_veneer 0x080091e9
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer 0x080091f1
	.global Engine_MapWaitWorkValuesBelow256
	.thumb_func
Engine_MapWaitWorkValuesBelow256:
	overlay_veneer 0x080091f9
	.global Owner_GetStateFar
	.thumb_func
Owner_GetStateFar:
	overlay_veneer 0x08077009
	.global Item_Get
	.thumb_func
Item_Get:
	overlay_veneer 0x08077019
	.global Inventory_AddItemFar
	.thumb_func
Inventory_AddItemFar:
	overlay_veneer 0x08077029
	.global Inventory_EquipFar
	.thumb_func
Inventory_EquipFar:
	overlay_veneer 0x08077051
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer 0x080770c1
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer 0x080770c9
	overlay_veneer 0x080770d1
	.global Inventory_Discard
	.thumb_func
Inventory_Discard:
	overlay_veneer 0x080772b1
	.global Engine_EventWait
	.thumb_func
Engine_EventWait:
	overlay_veneer 0x0808a011
	overlay_veneer 0x0808a019
	overlay_veneer 0x0808a021
	overlay_veneer 0x0808a049
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer 0x0808a081
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer 0x0808a091
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer 0x0808a0b9
	overlay_veneer 0x0808a0c9
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
	overlay_veneer 0x0808a111
	overlay_veneer 0x0808a129
	overlay_veneer 0x0808a131
	overlay_veneer 0x0808a139
	overlay_veneer 0x0808a149
	overlay_veneer 0x0808a151
	overlay_veneer 0x0808a161
	overlay_veneer 0x0808a171
	overlay_veneer 0x0808a181
	overlay_veneer 0x0808a189
	overlay_veneer 0x0808a1b9
	overlay_veneer 0x0808a1e9
	overlay_veneer 0x0808a1f1
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer 0x0808a421
	.global BattleFx_LoadActionEffectResources
	.thumb_func
BattleFx_LoadActionEffectResources:
	overlay_veneer 0x0808a429
	.global BattleFx_SetupObjectPair
	.thumb_func
BattleFx_SetupObjectPair:
	overlay_veneer 0x0808a431
	.global EffectRuntime_StopCurrentObject
	.thumb_func
EffectRuntime_StopCurrentObject:
	overlay_veneer 0x0808a439
	.global EventObject_Initialize
	.thumb_func
EventObject_Initialize:
	overlay_veneer 0x0808a449
	.global BattleFx_SetQueuedSoundAndPlay
	.thumb_func
BattleFx_SetQueuedSoundAndPlay:
	overlay_veneer 0x0808a5e1
	overlay_veneer 0x0808a5e9
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
