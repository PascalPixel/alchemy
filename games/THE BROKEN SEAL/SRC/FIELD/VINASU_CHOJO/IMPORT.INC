	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer 0x03000380
	.global IwramUnsignedRemainder
	.thumb_func
IwramUnsignedRemainder:
	overlay_veneer 0x030003e0
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer 0x080000d1
	.global Scheduler_RemoveCallback
	.thumb_func
Scheduler_RemoveCallback:
	overlay_veneer 0x080000d9
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer 0x080000f9
	.global Engine_MathSin
	.thumb_func
Engine_MathSin:
	overlay_veneer 0x08000119
	.global Engine_MathCos
	.thumb_func
Engine_MathCos:
	overlay_veneer 0x08000121
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer 0x08000129
	overlay_veneer 0x08000141
	overlay_veneer 0x08000151
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer 0x080001b9
	overlay_veneer 0x080001c9
	.global AnimationObjects_SelectAnimation
	.thumb_func
AnimationObjects_SelectAnimation:
	overlay_veneer 0x08009021
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
	.global Engine_ObjectDispatchRelease
	.thumb_func
Engine_ObjectDispatchRelease:
	overlay_veneer 0x080090d1
	.global Engine_MapRedraw
	.thumb_func
Engine_MapRedraw:
	overlay_veneer 0x08009129
	overlay_veneer 0x08009151
	overlay_veneer 0x08009159
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer 0x08009181
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
	.global OverlayObject_SetValue1
	.thumb_func
OverlayObject_SetValue1:
	overlay_veneer 0x080091e9
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer 0x080091f1
	.global Graphics_EnableObjLayerAndCallbacks
	.thumb_func
Graphics_EnableObjLayerAndCallbacks:
	overlay_veneer 0x08009209
	.global ObjectDispatch_StopCallbacksAndHideLayers
	.thumb_func
ObjectDispatch_StopCallbacksAndHideLayers:
	overlay_veneer 0x08009211
	.global Engine_ObjectSetPartPalettes
	.thumb_func
Engine_ObjectSetPartPalettes:
	overlay_veneer 0x08009241
	.global Engine_MessageShowCentered
	.thumb_func
Engine_MessageShowCentered:
	overlay_veneer 0x08015041
	.global UiText_ShowCenteredMessage
	.thumb_func
UiText_ShowCenteredMessage:
	overlay_veneer 0x08015211
	overlay_veneer 0x08015251
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer 0x08077029
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer 0x08077041
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer 0x08077049
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
	overlay_veneer 0x08077269
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
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer 0x0808a071
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer 0x0808a081
	.global Engine_ActorDestroy
	.thumb_func
Engine_ActorDestroy:
	overlay_veneer 0x0808a089
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer 0x0808a091
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer 0x0808a099
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer 0x0808a0a1
	.global Engine_ActorStop
	.thumb_func
Engine_ActorStop:
	overlay_veneer 0x0808a0a9
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer 0x0808a0b1
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer 0x0808a0b9
	.global Engine_ActorMoveToAndWait
	.thumb_func
Engine_ActorMoveToAndWait:
	overlay_veneer 0x0808a0c1
	.global Engine_ActorWalkTo
	.thumb_func
Engine_ActorWalkTo:
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
	.global Engine_ActorSetChildValue
	.thumb_func
Engine_ActorSetChildValue:
	overlay_veneer 0x0808a159
	.global Engine_ObjectSetPalette
	.thumb_func
Engine_ObjectSetPalette:
	overlay_veneer 0x0808a161
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
	.global Engine_EventGetViewCenter
	.thumb_func
Engine_EventGetViewCenter:
	overlay_veneer 0x0808a229
	.global Event_SetPairWork1c0
	.thumb_func
Event_SetPairWork1c0:
	overlay_veneer 0x0808a239
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer 0x0808a249
	.global BattleFx_SetWeightedResult
	.thumb_func
BattleFx_SetWeightedResult:
	overlay_veneer 0x0808a251
	.global Party_SetFields1ceAnd1d0
	.thumb_func
Party_SetFields1ceAnd1d0:
	overlay_veneer 0x0808a261
	.global Event_SetPair1d4
	.thumb_func
Event_SetPair1d4:
	overlay_veneer 0x0808a269
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
	.global Engine_EventCloseScreen
	.thumb_func
Engine_EventCloseScreen:
	overlay_veneer 0x0808a369
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer 0x0808a371
	.global Engine_PsynergyBegin
	.thumb_func
Engine_PsynergyBegin:
	overlay_veneer 0x0808a429
	.global BattleEffect_CleanupSceneObjects
	.thumb_func
BattleEffect_CleanupSceneObjects:
	overlay_veneer 0x0808a441
	.global ObjectTable_Snapshot
	.thumb_func
ObjectTable_Snapshot:
	overlay_veneer 0x0808a5b1
	.global Engine_ActorsRefresh
	.thumb_func
Engine_ActorsRefresh:
	overlay_veneer 0x0808a5b9
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
