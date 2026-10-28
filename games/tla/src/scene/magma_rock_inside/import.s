.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
	overlay_veneer Func_080000d8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Func_02004c58
	.thumb_func
Func_02004c58:
	overlay_veneer Resource_DecodeType01Far
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
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
	.global Func_02004c98
	.thumb_func
Func_02004c98:
	overlay_veneer Actor_SetMode
	.global Func_02004ca0
	.thumb_func
Func_02004ca0:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02004cb0
	.thumb_func
Func_02004cb0:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02004cc0
	.thumb_func
Func_02004cc0:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02004cc8
	.thumb_func
Func_02004cc8:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02004cd0
	.thumb_func
Func_02004cd0:
	overlay_veneer Actor_CommitPosition
	.global Func_02004cd8
	.thumb_func
Func_02004cd8:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02004ce8
	.thumb_func
Func_02004ce8:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02004cf0
	.thumb_func
Func_02004cf0:
	overlay_veneer Func_080201f0
	.global Func_02004cf8
	.thumb_func
Func_02004cf8:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004d10
	.thumb_func
Func_02004d10:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02004d18
	.thumb_func
Func_02004d18:
	overlay_veneer Func_08020230
	.global Func_02004d20
	.thumb_func
Func_02004d20:
	overlay_veneer Func_080202f0
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02004d40
	.thumb_func
Func_02004d40:
	overlay_veneer Event_StartFar
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	overlay_veneer Event_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Party_LearnAbility
	.thumb_func
Party_LearnAbility:
	overlay_veneer Party_LearnAbilityFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02004da0
	.thumb_func
Func_02004da0:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02004da8
	.thumb_func
Func_02004da8:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Func_02004dc0
	.thumb_func
Func_02004dc0:
	overlay_veneer Actor_ConfigureSpriteColorFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02004dd0
	.thumb_func
Func_02004dd0:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02004de0
	.thumb_func
Func_02004de0:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02004df8
	.thumb_func
Func_02004df8:
	overlay_veneer Func_080c8208
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02004e10
	.thumb_func
Func_02004e10:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02004e20
	.thumb_func
Func_02004e20:
	overlay_veneer Func_080c8240
	.global Func_02004e28
	.thumb_func
Func_02004e28:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02004e30
	.thumb_func
Func_02004e30:
	overlay_veneer Func_080c8288
	.global Func_02004e38
	.thumb_func
Func_02004e38:
	overlay_veneer Func_080c82e0
	.global Func_02004e40
	.thumb_func
Func_02004e40:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02004e48
	.thumb_func
Func_02004e48:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02004e50
	.thumb_func
Func_02004e50:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Func_02004e58
	.thumb_func
Func_02004e58:
	overlay_veneer Func_080c83e0
	.global Func_02004e60
	.thumb_func
Func_02004e60:
	overlay_veneer Func_080c8480
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02004e70
	.thumb_func
Func_02004e70:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_02004e78
	.thumb_func
Func_02004e78:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global Func_02004e80
	.thumb_func
Func_02004e80:
	overlay_veneer BattleEffect_StopMainActorFar
	.global Func_02004e88
	.thumb_func
Func_02004e88:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Func_02004e90
	.thumb_func
Func_02004e90:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02004e98
	.thumb_func
Func_02004e98:
	overlay_veneer Func_080c8518
	.global Func_02004ea0
	.thumb_func
Func_02004ea0:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02004ea8
	.thumb_func
Func_02004ea8:
	overlay_veneer Func_080c8580
	.global Actor_SetActionCallback
	.thumb_func
Actor_SetActionCallback:
	overlay_veneer Actor_SetActionCallbackFar
	.global Func_02004eb8
	.thumb_func
Func_02004eb8:
	overlay_veneer Event_NoOpFar
	.global Func_02004ec0
	.thumb_func
Func_02004ec0:
	overlay_veneer Func_080c86d0
	.global Func_02004ec8
	.thumb_func
Func_02004ec8:
	overlay_veneer Func_080c86d8
	.global Func_02004ed0
	.thumb_func
Func_02004ed0:
	overlay_veneer Func_080c8710
	.global Func_02004ed8
	.thumb_func
Func_02004ed8:
	overlay_veneer Func_080c8718
	.global Func_02004ee0
	.thumb_func
Func_02004ee0:
	overlay_veneer Func_080c8720
	.global Func_02004ee8
	.thumb_func
Func_02004ee8:
	overlay_veneer Func_080c8728
	.global Func_02004ef0
	.thumb_func
Func_02004ef0:
	overlay_veneer Func_080c8730
	.global Func_02004ef8
	.thumb_func
Func_02004ef8:
	overlay_veneer Func_080c8738
	.global Func_02004f00
	.thumb_func
Func_02004f00:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02004f08
	.thumb_func
Func_02004f08:
	overlay_veneer Func_080c8780
	.global Func_02004f10
	.thumb_func
Func_02004f10:
	overlay_veneer Func_080c87c0
	.global Func_02004f18
	.thumb_func
Func_02004f18:
	overlay_veneer Func_080c87d0
	.global Func_02004f20
	.thumb_func
Func_02004f20:
	overlay_veneer BattleEffect_StepVector3Far
	.global Func_02004f28
	.thumb_func
Func_02004f28:
	overlay_veneer Func_080c88c8
	.global Func_02004f30
	.thumb_func
Func_02004f30:
	overlay_veneer Sound_PlayFar
