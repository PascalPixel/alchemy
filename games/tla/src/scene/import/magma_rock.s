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
	.global Func_02003a14
	.thumb_func
Func_02003a14:
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
	.global Func_02003a54
	.thumb_func
Func_02003a54:
	overlay_veneer Actor_SetMode
	.global Actor_StartScript
	.thumb_func
Actor_StartScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02003a6c
	.thumb_func
Func_02003a6c:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Map_CenterOnFollowedPoint
	.thumb_func
Map_CenterOnFollowedPoint:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Actor_SetMoveTarget
	.thumb_func
Actor_SetMoveTarget:
	overlay_veneer Actor_SetTargetPosition
	.global Script_WaitForEventTimeout
	.thumb_func
Script_WaitForEventTimeout:
	overlay_veneer Actor_CommitPosition
	.global Map_CopyCellPictureIndicesRect
	.thumb_func
Map_CopyCellPictureIndicesRect:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer MapGrid_GetTerrainHeightFar
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02003aac
	.thumb_func
Func_02003aac:
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
	.global Map_WaitForShakeToSettle
	.thumb_func
Map_WaitForShakeToSettle:
	overlay_veneer Map_WaitForShakeToSettleFar
	.global SetMapCellCollision
	.thumb_func
SetMapCellCollision:
	overlay_veneer SetMapCellCollisionFar
	.global Func_02003adc
	.thumb_func
Func_02003adc:
	overlay_veneer MapGrid_GetTerrainKindFar
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Event_Start
	.thumb_func
Event_Start:
	overlay_veneer Event_StartFar
	.global Func_02003af4
	.thumb_func
Func_02003af4:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_MoveToObject
	.thumb_func
Actor_MoveToObject:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetActionById
	.thumb_func
Actor_SetActionById:
	overlay_veneer Actor_SetActionByIdFar
	.global Actor_WaitForAnimationChange
	.thumb_func
Actor_WaitForAnimationChange:
	overlay_veneer Actor_WaitForAnimationChangeFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02003b3c
	.thumb_func
Func_02003b3c:
	overlay_veneer Func_080c8208
	.global FieldEffect_SetAttached
	.thumb_func
FieldEffect_SetAttached:
	overlay_veneer FieldEffect_SetAttachedFar
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_SetSpeedParameters
	.thumb_func
Camera_SetSpeedParameters:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003b6c
	.thumb_func
Func_02003b6c:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global Func_080d452c
	.thumb_func
Func_080d452c:
	overlay_veneer Func_080c8250
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global Func_080d2b4c
	.thumb_func
Func_080d2b4c:
	overlay_veneer Func_080c8288
	.global Func_02003b8c
	.thumb_func
Func_02003b8c:
	overlay_veneer Func_080c82e0
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global FieldPsynergy_RunHandler
	.thumb_func
FieldPsynergy_RunHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global BattleEffect_LoadActionEffectResources
	.thumb_func
BattleEffect_LoadActionEffectResources:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_02003bb4
	.thumb_func
Func_02003bb4:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global BattleEffect_StopMainActor
	.thumb_func
BattleEffect_StopMainActor:
	overlay_veneer BattleEffect_StopMainActorFar
	.global BattleEffect_CleanupSceneObjects
	.thumb_func
BattleEffect_CleanupSceneObjects:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02003bd4
	.thumb_func
Func_02003bd4:
	overlay_veneer Sprite_ShareVramCacheEntryFar
	.global Event_NoOp
	.thumb_func
Event_NoOp:
	overlay_veneer Event_NoOpFar
	.global Func_02003be4
	.thumb_func
Func_02003be4:
	overlay_veneer Func_080c86a8
	.global Func_02003bec
	.thumb_func
Func_02003bec:
	overlay_veneer Func_080c86d0
	.global Func_02003bf4
	.thumb_func
Func_02003bf4:
	overlay_veneer Func_080c86d8
	.global Func_02003bfc
	.thumb_func
Func_02003bfc:
	overlay_veneer Func_080c86e8
	.global Func_02003c04
	.thumb_func
Func_02003c04:
	overlay_veneer Func_080c8710
	.global Func_02003c0c
	.thumb_func
Func_02003c0c:
	overlay_veneer Func_080c8718
	.global Func_02003c14
	.thumb_func
Func_02003c14:
	overlay_veneer Func_080c8720
	.global Func_02003c1c
	.thumb_func
Func_02003c1c:
	overlay_veneer Func_080c8728
	.global Func_02003c24
	.thumb_func
Func_02003c24:
	overlay_veneer Func_080c8730
	.global Func_02003c2c
	.thumb_func
Func_02003c2c:
	overlay_veneer Func_080c8738
	.global Func_02003c34
	.thumb_func
Func_02003c34:
	overlay_veneer Func_080c87c0
	.global Func_02003c3c
	.thumb_func
Func_02003c3c:
	overlay_veneer EffectSprite_DrawAtWorldPositionFar
	.global BattleEffect_StepVector3
	.thumb_func
BattleEffect_StepVector3:
	overlay_veneer BattleEffect_StepVector3Far
	.global Func_02003c4c
	.thumb_func
Func_02003c4c:
	overlay_veneer Func_080c88c8
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
