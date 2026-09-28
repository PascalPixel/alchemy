	overlay_veneer 0x03000380
	overlay_veneer 0x080000c1
	overlay_veneer 0x080000d1
	overlay_veneer 0x080000d9
	overlay_veneer 0x080000f9
	.global Math_Atan2
	.thumb_func
Math_Atan2:
	overlay_veneer 0x08000101
	overlay_veneer 0x08000129
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer 0x08000141
	overlay_veneer 0x08000149
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer 0x08000151
	overlay_veneer 0x08000171
	overlay_veneer 0x08000179
	overlay_veneer 0x080001a9
	overlay_veneer 0x080001b9
	overlay_veneer 0x080001c1
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer 0x080001c9
	overlay_veneer 0x080001d1
	overlay_veneer 0x080001e1
	overlay_veneer 0x080001e9
	overlay_veneer 0x08000291
	.global ResourceMetadata_ClearRecordFar
	.thumb_func
ResourceMetadata_ClearRecordFar:
	overlay_veneer 0x08009061
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer 0x08009081
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer 0x08009099
	overlay_veneer 0x080090c9
	overlay_veneer 0x080090e1
	overlay_veneer 0x08009141
	overlay_veneer 0x08009149
	.global Engine_ObjectSetPosition
	.thumb_func
Engine_ObjectSetPosition:
	overlay_veneer 0x08009151
	overlay_veneer 0x08009159
	overlay_veneer 0x080091a9
	overlay_veneer 0x080091b9
	overlay_veneer 0x080091c1
	overlay_veneer 0x080091c9
	overlay_veneer 0x080091d9
	overlay_veneer 0x080091e1
	overlay_veneer 0x08015041
	overlay_veneer 0x08015121
	.global ItemIcon_LoadTilesFar
	.thumb_func
ItemIcon_LoadTilesFar:
	overlay_veneer 0x08015251
	overlay_veneer 0x080153a1
	overlay_veneer 0x080153b9
	overlay_veneer 0x08077009
	overlay_veneer 0x08077029
	overlay_veneer 0x08077051
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer 0x080770c1
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer 0x080770c9
	overlay_veneer 0x080770e1
	overlay_veneer 0x080770e9
	overlay_veneer 0x08077149
	overlay_veneer 0x08077151
	overlay_veneer 0x08077169
	overlay_veneer 0x08077261
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
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	.global Object_GetByIdFar
	.thumb_func
Object_GetByIdFar:
	overlay_veneer 0x0808a081
	overlay_veneer 0x0808a089
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer 0x0808a091
	overlay_veneer 0x0808a0a1
	overlay_veneer 0x0808a0b9
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
	overlay_veneer 0x0808a111
	overlay_veneer 0x0808a131
	overlay_veneer 0x0808a139
	overlay_veneer 0x0808a149
	overlay_veneer 0x0808a151
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer 0x0808a171
	overlay_veneer 0x0808a179
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer 0x0808a181
	overlay_veneer 0x0808a191
	.global Engine_ActorFaceDirection
	.thumb_func
Engine_ActorFaceDirection:
	overlay_veneer 0x0808a1b9
	overlay_veneer 0x0808a1e1
	.global Engine_ActorShowEmote
	.thumb_func
Engine_ActorShowEmote:
	overlay_veneer 0x0808a1e9
	.global Engine_CameraFollowActor
	.thumb_func
Engine_CameraFollowActor:
	overlay_veneer 0x0808a201
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
	overlay_veneer 0x0808a249
	overlay_veneer 0x0808a251
	overlay_veneer 0x0808a261
	overlay_veneer 0x0808a269
	overlay_veneer 0x0808a2a1
	overlay_veneer 0x0808a361
	overlay_veneer 0x0808a369
	overlay_veneer 0x0808a371
	overlay_veneer 0x0808a401
	overlay_veneer 0x0808a461
	overlay_veneer 0x0808a4f1
	overlay_veneer 0x0808a549
	overlay_veneer 0x0808a589
	overlay_veneer 0x080a1019
	overlay_veneer 0x080f9011
	overlay_veneer 0x080f9049
