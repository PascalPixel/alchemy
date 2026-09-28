	overlay_veneer 0x03000380
	overlay_veneer 0x030003ac
	.global IwramUnsignedDivide
	.thumb_func
IwramUnsignedDivide:
	overlay_veneer 0x030003f0
	.global WaitFrames
	.thumb_func
WaitFrames:
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer 0x080000c1
	overlay_veneer 0x080000d1
	overlay_veneer 0x080000d9
	.global Engine_RandomNext
	.thumb_func
Engine_RandomNext:
	overlay_veneer 0x080000f9
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer 0x08000129
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer 0x08009081
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
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer 0x080091c9
	.global Object_CheckMovementCollision
	.thumb_func
Object_CheckMovementCollision:
	overlay_veneer 0x080091d9
	.global Engine_ActorSetSpriteFlags
	.thumb_func
Engine_ActorSetSpriteFlags:
	overlay_veneer 0x080091e1
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer 0x080091f1
	.global Engine_MapWaitWorkValuesBelow256
	.thumb_func
Engine_MapWaitWorkValuesBelow256:
	overlay_veneer 0x080091f9
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer 0x08009241
	.global ObjectGroup_SetChildValueUnlessFifteen
	.thumb_func
ObjectGroup_SetChildValueUnlessFifteen:
	overlay_veneer 0x08009249
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer 0x080770c1
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer 0x080770c9
	overlay_veneer 0x080770d1
	.global Engine_PartyRemoveItem
	.thumb_func
Engine_PartyRemoveItem:
	overlay_veneer 0x080772e9
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
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
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
	.global Engine_ActorEnableActionCallback
	.thumb_func
Engine_ActorEnableActionCallback:
	overlay_veneer 0x0808a099
	.global Engine_ActorStartAction
	.thumb_func
Engine_ActorStartAction:
	overlay_veneer 0x0808a0a1
	.global Engine_ActorStop
	.thumb_func
Engine_ActorStop:
	overlay_veneer 0x0808a0a9
	.global Engine_ActorSetDestination
	.thumb_func
Engine_ActorSetDestination:
	overlay_veneer 0x0808a0b9
	.global Engine_ObjectMotionSetPositionAndCommit
	.thumb_func
Engine_ObjectMotionSetPositionAndCommit:
	overlay_veneer 0x0808a0c1
	.global Engine_ActorWalkToAndWait
	.thumb_func
Engine_ActorWalkToAndWait:
	overlay_veneer 0x0808a0d1
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
	.global Engine_ActorSetAnimationAndWait
	.thumb_func
Engine_ActorSetAnimationAndWait:
	overlay_veneer 0x0808a111
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
	overlay_veneer 0x0808a159
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
	.global Engine_CameraMoveTo
	.thumb_func
Engine_CameraMoveTo:
	overlay_veneer 0x0808a211
	.global Engine_CameraWaitForMove
	.thumb_func
Engine_CameraWaitForMove:
	overlay_veneer 0x0808a219
	.global Engine_CameraMoveToActor
	.thumb_func
Engine_CameraMoveToActor:
	overlay_veneer 0x0808a221
	.global BattleFx_SetPhaseRequest
	.thumb_func
BattleFx_SetPhaseRequest:
	overlay_veneer 0x0808a259
	.global DisplayTransition_InitializeBattleEffectState
	.thumb_func
DisplayTransition_InitializeBattleEffectState:
	overlay_veneer 0x0808a301
	.global Engine_PsynergyPlayEffect
	.thumb_func
Engine_PsynergyPlayEffect:
	overlay_veneer 0x0808a421
	.global Engine_PsynergyBegin
	.thumb_func
Engine_PsynergyBegin:
	overlay_veneer 0x0808a429
	.global Engine_PsynergySetTarget
	.thumb_func
Engine_PsynergySetTarget:
	overlay_veneer 0x0808a431
	.global Engine_PsynergyLowerHands
	.thumb_func
Engine_PsynergyLowerHands:
	overlay_veneer 0x0808a439
	.global Engine_PsynergyRaiseHands
	.thumb_func
Engine_PsynergyRaiseHands:
	overlay_veneer 0x0808a449
	.global Battle_ResetEffectCounter
	.thumb_func
Battle_ResetEffectCounter:
	overlay_veneer 0x0808a461
	.global Audio_PlayCueFromEventWork
	.thumb_func
Audio_PlayCueFromEventWork:
	overlay_veneer 0x0808a4f1
	.global BattleFx_HasReachedTarget
	.thumb_func
BattleFx_HasReachedTarget:
	overlay_veneer 0x0808a509
	.global EffectSlot_SetObjectMode
	.thumb_func
EffectSlot_SetObjectMode:
	overlay_veneer 0x0808a519
	.global EffectSlot_SetCallback
	.thumb_func
EffectSlot_SetCallback:
	overlay_veneer 0x0808a521
	.global EffectSlot_Initialize
	.thumb_func
EffectSlot_Initialize:
	overlay_veneer 0x0808a529
	.global BattleFx_ClearOwnedSlot
	.thumb_func
BattleFx_ClearOwnedSlot:
	overlay_veneer 0x0808a531
	.global Camera_WorldToScreen
	.thumb_func
Camera_WorldToScreen:
	overlay_veneer 0x0808a539
	.global BattleFx_InitializeSlots
	.thumb_func
BattleFx_InitializeSlots:
	overlay_veneer 0x0808a551
	.global BattleFx_ClearActiveSlotsAndScheduleUpdates
	.thumb_func
BattleFx_ClearActiveSlotsAndScheduleUpdates:
	overlay_veneer 0x0808a559
	.global Motion_LaunchFromFocusedObject
	.thumb_func
Motion_LaunchFromFocusedObject:
	overlay_veneer 0x0808a571
	.global Engine_ActorWalkBy
	.thumb_func
Engine_ActorWalkBy:
	overlay_veneer 0x0808a579
	.global Engine_ActorWalkByAndWait
	.thumb_func
Engine_ActorWalkByAndWait:
	overlay_veneer 0x0808a581
	.global Engine_ActorFollow
	.thumb_func
Engine_ActorFollow:
	overlay_veneer 0x0808a589
	.global BattleFx_PlayQueuedSound
	.thumb_func
BattleFx_PlayQueuedSound:
	overlay_veneer 0x0808a5e9
	.global Shop_RestoreSceneTiles
	.thumb_func
Shop_RestoreSceneTiles:
	overlay_veneer 0x080b0049
	.global Shop_InitEffect
	.thumb_func
Shop_InitEffect:
	overlay_veneer 0x080b0051
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer 0x080f9011
