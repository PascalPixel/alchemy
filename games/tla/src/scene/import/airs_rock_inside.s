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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Heap_ReleaseFar
	.global Decompress_TaggedLz
	.thumb_func
Decompress_TaggedLz:
	overlay_veneer Decompress_TaggedLzFar
	.global DebugText_Clear
	.thumb_func
DebugText_Clear:
	overlay_veneer DebugText_ClearFar
	.global Window_LoadGraphics
	.thumb_func
Window_LoadGraphics:
	overlay_veneer Window_LoadGraphicsFar
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
	.global Import_ActorSelectAnimation
	.thumb_func
Import_ActorSelectAnimation:
	overlay_veneer Actor_SelectAnimationFar
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Actor_StartScript
	.thumb_func
Actor_StartScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Actor_CreateFar
	.global Import_ActorDestroy
	.thumb_func
Import_ActorDestroy:
	overlay_veneer Actor_ReleaseFar
	.global Map_CenterOnFollowedPoint
	.thumb_func
Map_CenterOnFollowedPoint:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Import_ActorSetTargetPosition
	.thumb_func
Import_ActorSetTargetPosition:
	overlay_veneer Actor_SetMoveTargetFar
	.global Import_ActorCommitPosition
	.thumb_func
Import_ActorCommitPosition:
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
	.global MapGrid_CopyCellPictureCellsRect
	.thumb_func
MapGrid_CopyCellPictureCellsRect:
	overlay_veneer MapGrid_CopyCellPictureCellsRectFar
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
	.global Func_02005dd4
	.thumb_func
Func_02005dd4:
	overlay_veneer MapGrid_WaitForShakeToSettleFar
	.global TilesetAnim_SuspendAll
	.thumb_func
TilesetAnim_SuspendAll:
	overlay_veneer TilesetAnim_SuspendAllFar
	.global TilesetAnim_ResumeAll
	.thumb_func
TilesetAnim_ResumeAll:
	overlay_veneer TilesetAnim_ResumeAllFar
	.global Func_02005dec
	.thumb_func
Func_02005dec:
	overlay_veneer Func_080202f8
	.global Func_02005df4
	.thumb_func
Func_02005df4:
	overlay_veneer Func_08020360
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_02005e04
	.thumb_func
Func_02005e04:
	overlay_veneer WindowColor_ApplyFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer Event_WaitFar
	.global Event_Start
	.thumb_func
Event_Start:
	overlay_veneer Event_StartFar
	.global Event_End
	.thumb_func
Event_End:
	overlay_veneer Event_EndFar
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
	.global Actor_SetDestinationById
	.thumb_func
Actor_SetDestinationById:
	overlay_veneer Actor_SetDestinationByIdFar
	.global Actor_MoveToAndWaitById
	.thumb_func
Actor_MoveToAndWaitById:
	overlay_veneer Actor_MoveToAndWaitByIdFar
	.global Actor_WalkToById
	.thumb_func
Actor_WalkToById:
	overlay_veneer Actor_WalkToByIdFar
	.global Actor_WalkToAndWaitById
	.thumb_func
Actor_WalkToAndWaitById:
	overlay_veneer Actor_WalkToAndWaitByIdFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
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
	.global Actor_JumpDownLedge
	.thumb_func
Actor_JumpDownLedge:
	overlay_veneer Actor_JumpDownLedgeFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Actor_ConfigureSpriteColor
	.thumb_func
Actor_ConfigureSpriteColor:
	overlay_veneer Actor_ConfigureSpriteColorFar
	.global Actor_SetSpriteColor
	.thumb_func
Actor_SetSpriteColor:
	overlay_veneer Actor_SetSpriteColorFar
	.global Func_02005ea4
	.thumb_func
Func_02005ea4:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02005eac
	.thumb_func
Func_02005eac:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02005ec4
	.thumb_func
Func_02005ec4:
	overlay_veneer FieldEffect_SetAttachedFar
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
	.global FieldCamera_MoveToActorById
	.thumb_func
FieldCamera_MoveToActorById:
	overlay_veneer FieldCamera_MoveToActorByIdFar
	.global Func_02005ef4
	.thumb_func
Func_02005ef4:
	overlay_veneer FieldCamera_GetActorFar
	.global Event_SetExitRequest
	.thumb_func
Event_SetExitRequest:
	overlay_veneer Event_SetExitRequestFar
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
	overlay_veneer PaletteFade_StartInterpolationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Party_AddPp
	.thumb_func
Party_AddPp:
	overlay_veneer Party_AddPpFar
	.global Scene_PlayMusic
	.thumb_func
Scene_PlayMusic:
	overlay_veneer Scene_PlayMusicFar
	.global Actor_SetActionCallback
	.thumb_func
Actor_SetActionCallback:
	overlay_veneer Actor_SetActionCallbackFar
	.global Func_02005f4c
	.thumb_func
Func_02005f4c:
	overlay_veneer Scene_QueueMusicFar
	.global Func_02005f54
	.thumb_func
Func_02005f54:
	overlay_veneer Func_080c86d0
	.global Func_02005f5c
	.thumb_func
Func_02005f5c:
	overlay_veneer Func_080c86d8
	.global Func_02005f64
	.thumb_func
Func_02005f64:
	overlay_veneer Func_080c8710
	.global Func_02005f6c
	.thumb_func
Func_02005f6c:
	overlay_veneer Func_080c8718
	.global Func_02005f74
	.thumb_func
Func_02005f74:
	overlay_veneer Func_080c8720
	.global Func_02005f7c
	.thumb_func
Func_02005f7c:
	overlay_veneer Func_080c8728
	.global Party_GetLeaderActor
	.thumb_func
Party_GetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02005f8c
	.thumb_func
Func_02005f8c:
	overlay_veneer Func_080c88c8
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
