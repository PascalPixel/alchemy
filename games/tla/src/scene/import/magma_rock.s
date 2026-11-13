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
	.global Task_Remove
	.thumb_func
Task_Remove:
	overlay_veneer Task_RemoveFar
	.global Random16
	.thumb_func
Random16:
	overlay_veneer Random16Far
	.global Trig_Sin
	.thumb_func
Trig_Sin:
	overlay_veneer Trig_SinFar
	.global Trig_Cos
	.thumb_func
Trig_Cos:
	overlay_veneer Trig_CosFar
	.global Vector_AddPolarOffset
	.thumb_func
Vector_AddPolarOffset:
	overlay_veneer Vector_AddPolarOffsetFar
	.global HeapSlot_Alloc
	.thumb_func
HeapSlot_Alloc:
	overlay_veneer HeapSlot_AllocFar
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Import_DecompressTaggedLz
	.thumb_func
Import_DecompressTaggedLz:
	overlay_veneer Decompress_TaggedLzFar
	.global VramCache_FreeEntry
	.thumb_func
VramCache_FreeEntry:
	overlay_veneer VramCache_FreeEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global VramCache_FindFreeEntry
	.thumb_func
VramCache_FindFreeEntry:
	overlay_veneer VramCache_FindFreeEntryFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global GameFlag_Get
	.thumb_func
GameFlag_Get:
	overlay_veneer GameFlag_GetFar
	.global GameFlag_Set
	.thumb_func
GameFlag_Set:
	overlay_veneer GameFlag_SetFar
	.global GameFlag_Clear
	.thumb_func
GameFlag_Clear:
	overlay_veneer GameFlag_ClearFar
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
	overlay_veneer Actor_SetMoveTargetFar
	.global Actor_WaitForMove
	.thumb_func
Actor_WaitForMove:
	overlay_veneer Actor_WaitForMoveFar
	.global MapGrid_CopyCellPictureIndicesRect
	.thumb_func
MapGrid_CopyCellPictureIndicesRect:
	overlay_veneer MapGrid_CopyCellPictureIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer MapGrid_GetTerrainHeightFar
	.global Map_CopyCellAttributeRect
	.thumb_func
Map_CopyCellAttributeRect:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global MapGrid_CheckCollision
	.thumb_func
MapGrid_CheckCollision:
	overlay_veneer MapGrid_CheckCollisionFar
	.global Actor_SetSpriteFlags
	.thumb_func
Actor_SetSpriteFlags:
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
	.global MapGrid_SetCellBehaviorAt
	.thumb_func
MapGrid_SetCellBehaviorAt:
	overlay_veneer MapGrid_SetCellBehaviorAtFar
	.global MapGrid_GetTerrainKind
	.thumb_func
MapGrid_GetTerrainKind:
	overlay_veneer MapGrid_GetTerrainKindFar
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer Event_WaitFar
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
	.global Actor_WalkToById
	.thumb_func
Actor_WalkToById:
	overlay_veneer Actor_WalkToByIdFar
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
	.global Actor_SetSpriteColor
	.thumb_func
Actor_SetSpriteColor:
	overlay_veneer Actor_SetSpriteColorFar
	.global Actor_DisableById
	.thumb_func
Actor_DisableById:
	overlay_veneer Actor_DisableByIdFar
	.global FieldEffect_SetAttached
	.thumb_func
FieldEffect_SetAttached:
	overlay_veneer FieldEffect_SetAttachedFar
	.global Actor_CopyGraphics
	.thumb_func
Actor_CopyGraphics:
	overlay_veneer Actor_CopyGraphicsFar
	.global FieldCamera_FollowActor
	.thumb_func
FieldCamera_FollowActor:
	overlay_veneer FieldCamera_FollowActorFar
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
	.global FieldCamera_MoveToActor
	.thumb_func
FieldCamera_MoveToActor:
	overlay_veneer FieldCamera_MoveToActorFar
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global Field_RequestDjinnBattle
	.thumb_func
Field_RequestDjinnBattle:
	overlay_veneer Field_RequestDjinnBattleFar
	.global Field_TryJumpForward
	.thumb_func
Field_TryJumpForward:
	overlay_veneer Field_TryJumpForwardFar
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
	overlay_veneer FieldPsynergy_RunHandlerFar
	.global BattleEffect_LoadActionEffectResources
	.thumb_func
BattleEffect_LoadActionEffectResources:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Import_FieldPsynergySetupActorPair
	.thumb_func
Import_FieldPsynergySetupActorPair:
	overlay_veneer FieldPsynergy_SetupActorPairFar
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
	.global Sprite_ShareVramCacheEntry
	.thumb_func
Sprite_ShareVramCacheEntry:
	overlay_veneer Sprite_ShareVramCacheEntryFar
	.global Event_NoOp
	.thumb_func
Event_NoOp:
	overlay_veneer Event_NoOpFar
	.global Rope_Initialize
	.thumb_func
Rope_Initialize:
	overlay_veneer Rope_InitializeFar
	.global Tightrope_Initialize
	.thumb_func
Tightrope_Initialize:
	overlay_veneer Tightrope_InitializeFar
	.global Func_02003bf4
	.thumb_func
Func_02003bf4:
	overlay_veneer Tightrope_PlaceFar
	.global Func_02003bfc
	.thumb_func
Func_02003bfc:
	overlay_veneer Rope_PlaceFar
	.global Actor_GetCellIndex
	.thumb_func
Actor_GetCellIndex:
	overlay_veneer Actor_GetCellIndexFar
	.global Actor_PlaceBlocks
	.thumb_func
Actor_PlaceBlocks:
	overlay_veneer Actor_PlaceBlocksFar
	.global Actor_LiftTargetBlock
	.thumb_func
Actor_LiftTargetBlock:
	overlay_veneer Actor_LiftTargetBlockFar
	.global Actor_PlaceMovedBlock
	.thumb_func
Actor_PlaceMovedBlock:
	overlay_veneer Actor_PlaceMovedBlockFar
	.global MapCollision_FindOrAddTag
	.thumb_func
MapCollision_FindOrAddTag:
	overlay_veneer MapCollision_FindOrAddTagFar
	.global Actor_FindAtCell
	.thumb_func
Actor_FindAtCell:
	overlay_veneer Actor_FindAtCellFar
	.global EffectSprite_Init
	.thumb_func
EffectSprite_Init:
	overlay_veneer EffectSprite_InitFar
	.global EffectSprite_DrawAtWorldPosition
	.thumb_func
EffectSprite_DrawAtWorldPosition:
	overlay_veneer EffectSprite_DrawAtWorldPositionFar
	.global BattleEffect_StepVector3
	.thumb_func
BattleEffect_StepVector3:
	overlay_veneer BattleEffect_StepVector3Far
	.global Event_PlayCueNearFocus
	.thumb_func
Event_PlayCueNearFocus:
	overlay_veneer Event_PlayCueNearFocusFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
