.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	overlay_veneer Trig_SinFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Trig_CosFar
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
	.global Actor_SelectAnimation
	.thumb_func
Actor_SelectAnimation:
	overlay_veneer Actor_SelectAnimationFar
	.global Actor_StartScript
	.thumb_func
Actor_StartScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Actor_CreateFar
	.global Actor_Release
	.thumb_func
Actor_Release:
	overlay_veneer Actor_ReleaseFar
	.global Actor_SetMoveTarget
	.thumb_func
Actor_SetMoveTarget:
	overlay_veneer Actor_SetMoveTargetFar
	.global MapGrid_CopyCellPictureIndicesRect
	.thumb_func
MapGrid_CopyCellPictureIndicesRect:
	overlay_veneer MapGrid_CopyCellPictureIndicesRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSpriteFlagsFar
	.global Actor_SetBlendMode
	.thumb_func
Actor_SetBlendMode:
	overlay_veneer Actor_SetBlendModeFar
	.global MapGrid_SetScreenShake
	.thumb_func
MapGrid_SetScreenShake:
	overlay_veneer MapGrid_SetScreenShakeFar
	.global MapGrid_WaitForShakeToSettle
	.thumb_func
MapGrid_WaitForShakeToSettle:
	overlay_veneer MapGrid_WaitForShakeToSettleFar
	.global Func_02001f9c
	.thumb_func
Func_02001f9c:
	overlay_veneer Func_080202f8
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer Event_WaitFar
	.global Event_Start
	.thumb_func
Event_Start:
	overlay_veneer Event_StartFar
	.global Func_02001fb4
	.thumb_func
Func_02001fb4:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_WalkToById
	.thumb_func
Actor_WalkToById:
	overlay_veneer Actor_WalkToByIdFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetSpriteColor
	.thumb_func
Actor_SetSpriteColor:
	overlay_veneer Actor_SetSpriteColorFar
	.global Camera_SetSpeedParameters
	.thumb_func
Camera_SetSpeedParameters:
	overlay_veneer Camera_SetSpeedParametersFar
	.global FieldCamera_MoveWithinMapBounds
	.thumb_func
FieldCamera_MoveWithinMapBounds:
	overlay_veneer FieldCamera_MoveWithinMapBoundsFar
	.global FieldCamera_WaitForArrival
	.thumb_func
FieldCamera_WaitForArrival:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global Field_RequestBattleFromEncounterTable
	.thumb_func
Field_RequestBattleFromEncounterTable:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Event_SetAfterBattleDestination
	.thumb_func
Event_SetAfterBattleDestination:
	overlay_veneer Event_SetAfterBattleDestinationFar
	.global BattleEffect_ApplyColorToTargetBuffer
	.thumb_func
BattleEffect_ApplyColorToTargetBuffer:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global BattleEffect_ApplyColorToSourceBuffer
	.thumb_func
BattleEffect_ApplyColorToSourceBuffer:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Scene_PlayMusic
	.thumb_func
Scene_PlayMusic:
	overlay_veneer Scene_PlayMusicFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
