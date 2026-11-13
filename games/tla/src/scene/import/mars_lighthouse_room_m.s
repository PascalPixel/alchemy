.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
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
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
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
	.global ResourceMetadata_ClearRecord
	.thumb_func
ResourceMetadata_ClearRecord:
	overlay_veneer ResourceMetadata_ClearRecordFar
	.global Func_02003118
	.thumb_func
Func_02003118:
	overlay_veneer Actor_SetMode
	.global Actor_StartScript
	.thumb_func
Actor_StartScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02003130
	.thumb_func
Func_02003130:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Actor_SetMoveTarget
	.thumb_func
Actor_SetMoveTarget:
	overlay_veneer Actor_SetTargetPosition
	.global Map_CopyCellPictureIndicesRect
	.thumb_func
Map_CopyCellPictureIndicesRect:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02003150
	.thumb_func
Func_02003150:
	overlay_veneer Map_ClearLayerEntryFlagFar
	.global DisplayBlend_EnableRunScript
	.thumb_func
DisplayBlend_EnableRunScript:
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer MapGrid_GetTerrainHeightFar
	.global Func_02003168
	.thumb_func
Func_02003168:
	overlay_veneer Func_080201d8
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_0802b450
	.thumb_func
Func_0802b450:
	overlay_veneer Func_080201f0
	.global Func_02003180
	.thumb_func
Func_02003180:
	overlay_veneer MapGrid_CheckCollisionFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Map_SetScreenShake
	.thumb_func
Map_SetScreenShake:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Actor_SetSpritePriorityDirect
	.thumb_func
Actor_SetSpritePriorityDirect:
	overlay_veneer Actor_SetSpritePriorityDirectFar
	.ifndef TLA_EDITION_JA
	.global Func_020031b0
	.thumb_func
Func_020031b0:
	overlay_veneer Field_CheckSpheresOverlapFar
	.endif
	.global Func_020031b8
	.thumb_func
Func_020031b8:
	overlay_veneer Func_08020390
	.global Func_0803ce64
	.thumb_func
Func_0803ce64:
	overlay_veneer Func_08038210
	.global PartyInventory_FindMember
	.thumb_func
PartyInventory_FindMember:
	overlay_veneer PartyInventory_FindMemberFar
	.global Party_CountMembers
	.thumb_func
Party_CountMembers:
	overlay_veneer Party_CountMembersFar
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Event_Start
	.thumb_func
Event_Start:
	overlay_veneer Event_StartFar
	.global Event_End
	.thumb_func
Event_End:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_JumpDownLedge
	.thumb_func
Actor_JumpDownLedge:
	overlay_veneer Actor_JumpDownLedgeFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global FieldEffect_SetAttached
	.thumb_func
FieldEffect_SetAttached:
	overlay_veneer FieldEffect_SetAttachedFar
	.global Camera_SetSpeedParameters
	.thumb_func
Camera_SetSpeedParameters:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003228
	.thumb_func
Func_02003228:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global Func_02003230
	.thumb_func
Func_02003230:
	overlay_veneer FieldCamera_MoveToActorByIdFar
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global ScreenTransition_Start
	.thumb_func
ScreenTransition_Start:
	overlay_veneer ScreenTransition_StartFar
	.global BattleEffect_ApplyColorToTargetBuffer
	.thumb_func
BattleEffect_ApplyColorToTargetBuffer:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global BattleEffect_ApplyColorToSourceBuffer
	.thumb_func
BattleEffect_ApplyColorToSourceBuffer:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global PaletteFade_StartInterpolation
	.thumb_func
PaletteFade_StartInterpolation:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global DisplayScroll_InitializeHBlankDma
	.thumb_func
DisplayScroll_InitializeHBlankDma:
	overlay_veneer DisplayScroll_InitializeHBlankDmaFar
	.global Func_02003278
	.thumb_func
Func_02003278:
	overlay_veneer Func_080c8480
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global BattleParty_ApplyHealthDelta
	.thumb_func
BattleParty_ApplyHealthDelta:
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.global Party_PlaySavedMusic
	.thumb_func
Party_PlaySavedMusic:
	overlay_veneer Party_PlaySavedMusicFar
	.global Party_GetLeaderActor
	.thumb_func
Party_GetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_020032a0
	.thumb_func
Func_020032a0:
	overlay_veneer Func_080c88d8
	.global Func_020032a8
	.thumb_func
Func_020032a8:
	overlay_veneer Func_080c88e8
	.global Func_020032b0
	.thumb_func
Func_020032b0:
	overlay_veneer Func_080c8918
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
	.global Sound_WaitForBgmRestore
	.thumb_func
Sound_WaitForBgmRestore:
	overlay_veneer Sound_WaitForBgmRestoreFar
