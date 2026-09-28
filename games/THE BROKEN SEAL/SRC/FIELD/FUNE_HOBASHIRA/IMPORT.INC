	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	overlay_veneer 0x080000d1
	.global Random16
	.thumb_func
Random16:
	overlay_veneer 0x080000f9
	overlay_veneer 0x08000119
	overlay_veneer 0x08000121
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer 0x08009129
	.global Object_SetPosition
	.thumb_func
Object_SetPosition:
	overlay_veneer 0x08009151
	.global Object_CommitPosition
	.thumb_func
Object_CommitPosition:
	overlay_veneer 0x08009159
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer 0x080091e1
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
	.global Event_CallWithLastActiveObjectId
	.thumb_func
Event_CallWithLastActiveObjectId:
	overlay_veneer 0x0808a031
	.global Engine_PartyGiveItem
	.thumb_func
Engine_PartyGiveItem:
	overlay_veneer 0x0808a061
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer 0x0808a081
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer 0x0808a091
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer 0x0808a099
	.global Engine_ActorStop
	.thumb_func
Engine_ActorStop:
	overlay_veneer 0x0808a0a9
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer 0x0808a0b9
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer 0x0808a0d1
	.global Engine_ActorSetPosition
	.thumb_func
Engine_ActorSetPosition:
	overlay_veneer 0x0808a0f1
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
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
	overlay_veneer 0x0808a159
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer 0x0808a171
	.global Engine_EventShowMessageAndWait
	.thumb_func
Engine_EventShowMessageAndWait:
	overlay_veneer 0x0808a189
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
	overlay_veneer 0x0808a1f1
	overlay_veneer 0x0808a201
	.global Engine_CameraMoveTo
	.thumb_func
Engine_CameraMoveTo:
	overlay_veneer 0x0808a211
	.global Battle_GetWorkObject1e0
	.thumb_func
Battle_GetWorkObject1e0:
	overlay_veneer 0x0808a229
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer 0x0808a249
	.global FieldEffect_UpdateGridPlacement
	.thumb_func
FieldEffect_UpdateGridPlacement:
	overlay_veneer 0x0808a2a9
	overlay_veneer 0x0808a2b1
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
	.global Engine_ItemShowFound
	.thumb_func
Engine_ItemShowFound:
	overlay_veneer 0x0808a399
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
