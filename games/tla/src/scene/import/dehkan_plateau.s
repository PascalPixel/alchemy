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
	.global HeapSlot_Alloc
	.thumb_func
HeapSlot_Alloc:
	overlay_veneer HeapSlot_AllocFar
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
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
	.global Func_02003ba0
	.thumb_func
Func_02003ba0:
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
	.global Actor_SetSpriteFlags
	.thumb_func
Actor_SetSpriteFlags:
	overlay_veneer Actor_SetSpriteFlagsFar
	.global MapGrid_SetScreenShake
	.thumb_func
MapGrid_SetScreenShake:
	overlay_veneer MapGrid_SetScreenShakeFar
	.global MapGrid_WaitForShakeToSettle
	.thumb_func
MapGrid_WaitForShakeToSettle:
	overlay_veneer MapGrid_WaitForShakeToSettleFar
	.global MapGrid_GetTerrainKind
	.thumb_func
MapGrid_GetTerrainKind:
	overlay_veneer MapGrid_GetTerrainKindFar
	.global Icon_DrawItem
	.thumb_func
Icon_DrawItem:
	overlay_veneer Icon_DrawItemFar
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer Event_WaitFar
	.global Event_Start
	.thumb_func
Event_Start:
	overlay_veneer Event_StartFar
	.global Event_End
	.thumb_func
Event_End:
	overlay_veneer Event_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_MoveToAndWaitById
	.thumb_func
Actor_MoveToAndWaitById:
	overlay_veneer Actor_MoveToAndWaitByIdFar
	.global Actor_WalkToAndWaitById
	.thumb_func
Actor_WalkToAndWaitById:
	overlay_veneer Actor_WalkToAndWaitByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetHorizontalPositionAndAngleWithTerrain
	.thumb_func
Actor_SetHorizontalPositionAndAngleWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Actor_MoveToObject
	.thumb_func
Actor_MoveToObject:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_FaceActorById
	.thumb_func
Actor_FaceActorById:
	overlay_veneer Actor_FaceActorByIdFar
	.global Actor_SetSpriteColor
	.thumb_func
Actor_SetSpriteColor:
	overlay_veneer Actor_SetSpriteColorFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_FaceDirectionById
	.thumb_func
Actor_FaceDirectionById:
	overlay_veneer Actor_FaceDirectionByIdFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02003cc0
	.thumb_func
Func_02003cc0:
	overlay_veneer Actor_DisableByIdFar
	.global Actor_ShowEmoteById
	.thumb_func
Actor_ShowEmoteById:
	overlay_veneer Actor_ShowEmoteByIdFar
	.global FieldEffect_SetAttached
	.thumb_func
FieldEffect_SetAttached:
	overlay_veneer FieldEffect_SetAttachedFar
	.global Actor_CopyGraphics
	.thumb_func
Actor_CopyGraphics:
	overlay_veneer Actor_CopyGraphicsFar
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
	.global FieldCamera_GetActor
	.thumb_func
FieldCamera_GetActor:
	overlay_veneer FieldCamera_GetActorFar
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global Field_RequestDjinnBattle
	.thumb_func
Field_RequestDjinnBattle:
	overlay_veneer Field_RequestDjinnBattleFar
	.global Field_FindNearbyTalkTarget
	.thumb_func
Field_FindNearbyTalkTarget:
	overlay_veneer Field_FindNearbyTalkTargetFar
	.global Field_FindNearbyPsynergyTarget
	.thumb_func
Field_FindNearbyPsynergyTarget:
	overlay_veneer Field_FindNearbyPsynergyTargetFar
	.global Func_02003d20
	.thumb_func
Func_02003d20:
	overlay_veneer Field_TryJumpForwardFar
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
	.global ItemDisplay_ShowPicture
	.thumb_func
ItemDisplay_ShowPicture:
	overlay_veneer ItemDisplay_ShowPictureFar
	.global FieldPsynergy_RunHandler
	.thumb_func
FieldPsynergy_RunHandler:
	overlay_veneer FieldPsynergy_RunHandlerFar
	.global BattleEffect_LoadActionEffectResources
	.thumb_func
BattleEffect_LoadActionEffectResources:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global FieldPsynergy_SetupActorPair
	.thumb_func
FieldPsynergy_SetupActorPair:
	overlay_veneer FieldPsynergy_SetupActorPairFar
	.global BattleEffect_StopMainActor
	.thumb_func
BattleEffect_StopMainActor:
	overlay_veneer BattleEffect_StopMainActorFar
	.global BattleEffect_CleanupSceneObjects
	.thumb_func
BattleEffect_CleanupSceneObjects:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global EventObject_Initialize
	.thumb_func
EventObject_Initialize:
	overlay_veneer EventObject_InitializeFar
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Sprite_ShareVramCacheEntry
	.thumb_func
Sprite_ShareVramCacheEntry:
	overlay_veneer Sprite_ShareVramCacheEntryFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02003d90
	.thumb_func
Func_02003d90:
	overlay_veneer Rope_InitializeFar
	.global Func_02003d98
	.thumb_func
Func_02003d98:
	overlay_veneer Tightrope_InitializeFar
	.global Func_02003da0
	.thumb_func
Func_02003da0:
	overlay_veneer Tightrope_PlaceFar
	.global Func_02003da8
	.thumb_func
Func_02003da8:
	overlay_veneer Rope_PlaceFar
	.global Func_02003db0
	.thumb_func
Func_02003db0:
	overlay_veneer Actor_GetCellIndexFar
	.global Func_02003db8
	.thumb_func
Func_02003db8:
	overlay_veneer Actor_PlaceBlocksFar
	.global Func_02003dc0
	.thumb_func
Func_02003dc0:
	overlay_veneer Actor_LiftTargetBlockFar
	.global Func_02003dc8
	.thumb_func
Func_02003dc8:
	overlay_veneer Actor_PlaceMovedBlockFar
	.global Func_02003dd0
	.thumb_func
Func_02003dd0:
	overlay_veneer MapCollision_FindOrAddTagFar
	.global Func_02003dd8
	.thumb_func
Func_02003dd8:
	overlay_veneer Actor_FindAtCellFar
	.global Func_02003de0
	.thumb_func
Func_02003de0:
	overlay_veneer Leader_CanPushObjectAheadFar
	.global Func_02003de8
	.thumb_func
Func_02003de8:
	overlay_veneer Leader_PushObjectAheadFar
	.global Func_02003df0
	.thumb_func
Func_02003df0:
	overlay_veneer Leader_FindActorAheadFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
