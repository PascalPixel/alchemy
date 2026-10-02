@ Import veneers after the WORLD_MAP overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — WORLD_MAP overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02004aa8
	.thumb_func
Func_02004aa8:
	overlay_veneer Func_080001a8
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02004ac8
	.thumb_func
Func_02004ac8:
	overlay_veneer Func_080001d8
	.global Func_02004ad0
	.thumb_func
Func_02004ad0:
	overlay_veneer Func_080001e8
	.global Func_02004ad8
	.thumb_func
Func_02004ad8:
	overlay_veneer Func_080001f0
	.global Render_ProjectPoint
	.thumb_func
Render_ProjectPoint:
	overlay_veneer Render_ProjectPointFar
	.global QueueIoWriteDelay2
	.thumb_func
QueueIoWriteDelay2:
	overlay_veneer QueueIoWriteDelay2Far
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Animation_ApplyChildArgument
	.thumb_func
Animation_ApplyChildArgument:
	overlay_veneer Animation_ApplyChildArgumentFar
	.global Func_02004b18
	.thumb_func
Func_02004b18:
	overlay_veneer Object_SetMode
	.global Func_02004b20
	.thumb_func
Func_02004b20:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02004b28
	.thumb_func
Func_02004b28:
	overlay_veneer Func_080200b0
	.global Func_02004b30
	.thumb_func
Func_02004b30:
	overlay_veneer Func_080200c0
	.global Func_02004b38
	.thumb_func
Func_02004b38:
	overlay_veneer Object_Destroy
	.global Func_02004b40
	.thumb_func
Func_02004b40:
	overlay_veneer Func_08020128
	.global Func_02004b48
	.thumb_func
Func_02004b48:
	overlay_veneer Object_ResetMotion
	.global Func_02004b50
	.thumb_func
Func_02004b50:
	overlay_veneer Object_SetPosition
	.global Func_02004b58
	.thumb_func
Func_02004b58:
	overlay_veneer Object_CommitPosition
	.global Func_02004b60
	.thumb_func
Func_02004b60:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080201d0
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201d0
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201d0
	.else
	overlay_veneer Func_08020200
	.endif
	.endif
	.endif
	.endif
	.global Func_02004b68
	.thumb_func
Func_02004b68:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080201d8
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201d8
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201d8
	.else
	overlay_veneer Func_08020208
	.endif
	.endif
	.endif
	.endif
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02004b78
	.thumb_func
Func_02004b78:
	overlay_veneer Func_08020228
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02004b88
	.thumb_func
Func_02004b88:
	overlay_veneer Func_08020318
	.global Func_02004b90
	.thumb_func
Func_02004b90:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020320
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020320
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020320
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020320
	.else
	overlay_veneer Func_08020350
	.endif
	.endif
	.endif
	.endif
	.global GetWorldMapCollision
	.thumb_func
GetWorldMapCollision:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020328
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020328
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020328
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020328
	.else
	overlay_veneer GetWorldMapCollisionFar
	.endif
	.endif
	.endif
	.endif
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02004ba8
	.thumb_func
Func_02004ba8:
	overlay_veneer UiText_DrawQuantity
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global Func_02004bb8
	.thumb_func
Func_02004bb8:
	overlay_veneer Func_08038348
	.global Menu_AnimateSelectionToEntry
	.thumb_func
Menu_AnimateSelectionToEntry:
	overlay_veneer Menu_AnimateSelectionToEntryFar
	.global Owner_AdjustSecondValue
	.thumb_func
Owner_AdjustSecondValue:
	overlay_veneer Owner_AdjustSecondValueFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer Party_AddActiveOwnerFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
	.global Djinn_AddToOwner
	.thumb_func
Djinn_AddToOwner:
	overlay_veneer Djinn_AddToOwnerFar
	.global Trade_AddOffer
	.thumb_func
Trade_AddOffer:
	overlay_veneer Trade_AddOfferFar
	.global Func_02004bf8
	.thumb_func
Func_02004bf8:
	overlay_veneer Func_080ad208
	.global Func_02004c00
	.thumb_func
Func_02004c00:
	overlay_veneer Func_080ad228
	.global Func_02004c08
	.thumb_func
Func_02004c08:
	overlay_veneer Func_080ad310
	.global Func_02004c10
	.thumb_func
Func_02004c10:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080ad2f8
	.else
	overlay_veneer Func_080ad318
	.endif
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02004c20
	.thumb_func
Func_02004c20:
	overlay_veneer Func_080c8018
	.global Func_02004c28
	.thumb_func
Func_02004c28:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Func_02004c38
	.thumb_func
Func_02004c38:
	overlay_veneer Func_080c8078
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02004c88
	.thumb_func
Func_02004c88:
	overlay_veneer Func_080c80f8
	.global Func_02004c90
	.thumb_func
Func_02004c90:
	overlay_veneer Func_080c8100
	.global Func_02004c98
	.thumb_func
Func_02004c98:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02004cd8
	.thumb_func
Func_02004cd8:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02004ce8
	.thumb_func
Func_02004ce8:
	overlay_veneer Func_080c8198
	.global Func_02004cf0
	.thumb_func
Func_02004cf0:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02004d00
	.thumb_func
Func_02004d00:
	overlay_veneer Func_080c81d8
	.global Func_02004d08
	.thumb_func
Func_02004d08:
	overlay_veneer Func_080c8210
	.global Func_02004d10
	.thumb_func
Func_02004d10:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Func_02004d20
	.thumb_func
Func_02004d20:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02004d30
	.thumb_func
Func_02004d30:
	overlay_veneer Func_080c8240
	.global Func_02004d38
	.thumb_func
Func_02004d38:
	overlay_veneer Event_SetPairWork1c0Far
	.global Func_02004d40
	.thumb_func
Func_02004d40:
	overlay_veneer Func_080c8278
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	overlay_veneer Func_080c8280
	.global Func_02004d50
	.thumb_func
Func_02004d50:
	overlay_veneer Func_080c8288
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Party_SetFields1f2And1f4
	.thumb_func
Party_SetFields1f2And1f4:
	overlay_veneer Party_SetFields1f2And1f4Far
	.global Func_02004d68
	.thumb_func
Func_02004d68:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	overlay_veneer Func_080c82c0
	.endif
	.endif
	.global Func_02004d70
	.thumb_func
Func_02004d70:
	overlay_veneer Func_080c8368
	.global Func_02004d78
	.thumb_func
Func_02004d78:
	overlay_veneer Func_080c8378
	.global Func_02004d80
	.thumb_func
Func_02004d80:
	overlay_veneer Func_080c8380
	.global Func_02004d88
	.thumb_func
Func_02004d88:
	overlay_veneer Func_080c8390
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_SetStatus1c6Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_SetStatus1c6Far
	.else
	overlay_veneer Event_WaitValue1c8FramesFar
	.endif
	.endif
	.global Func_02004da8
	.thumb_func
Func_02004da8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetWideSpriteFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetWideSpriteFar
	.else
	overlay_veneer Func_080c8430
	.endif
	.endif
	.global Func_02004db0
	.thumb_func
Func_02004db0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8428
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8428
	.else
	overlay_veneer Func_080c8438
	.endif
	.endif
	.global Func_02004db8
	.thumb_func
Func_02004db8:
	overlay_veneer Func_080c84e0
	.global Func_02004dc0
	.thumb_func
Func_02004dc0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8560
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8560
	.else
	overlay_veneer Func_080c8570
	.endif
	.endif
	.global Func_02004dc8
	.thumb_func
Func_02004dc8:
	overlay_veneer Func_080c8578
	.global Func_02004dd0
	.thumb_func
Func_02004dd0:
	overlay_veneer Func_080c8580
	.global Func_02004dd8
	.thumb_func
Func_02004dd8:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02004df8
	.thumb_func
Func_02004df8:
	overlay_veneer Func_080c8638
	.global Func_02004e00
	.thumb_func
Func_02004e00:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8630
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8630
	.else
	overlay_veneer Func_080c8640
	.endif
	.endif
	.global Func_02004e08
	.thumb_func
Func_02004e08:
	overlay_veneer Func_080c86f0
	.global Func_02004e10
	.thumb_func
Func_02004e10:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_02004e18
	.thumb_func
Func_02004e18:
	overlay_veneer Func_080c8860
	.global Func_02004e20
	.thumb_func
Func_02004e20:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8958
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8958
	.else
	overlay_veneer Func_080c8960
	.endif
	.endif
	.global Func_02004e28
	.thumb_func
Func_02004e28:
	overlay_veneer ItemMenu_Open
	.global Func_02004e30
	.thumb_func
Func_02004e30:
	overlay_veneer Audio_PlayCue
