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
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetFar
	.global Func_02001428
	.thumb_func
Func_02001428:
	overlay_veneer Actor_SetMode
	.global Actor_StartScript
	.thumb_func
Actor_StartScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Actor_CreateFar
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Map_CenterOnFollowedPoint
	.thumb_func
Map_CenterOnFollowedPoint:
	overlay_veneer Map_CenterOnFollowedPointFar
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
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global MapGrid_SetScreenShake
	.thumb_func
MapGrid_SetScreenShake:
	overlay_veneer MapGrid_SetScreenShakeFar
	.global Func_02001478
	.thumb_func
Func_02001478:
	overlay_veneer MapGrid_GetTerrainKindFar
	.global Func_02001480
	.thumb_func
Func_02001480:
	overlay_veneer Func_08020360
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
	.global Actor_MoveToObject
	.thumb_func
Actor_MoveToObject:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetSpriteColor
	.thumb_func
Actor_SetSpriteColor:
	overlay_veneer Actor_SetSpriteColorFar
	.global Func_020014c0
	.thumb_func
Func_020014c0:
	overlay_veneer Func_080c8208
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global FieldCamera_FollowActor
	.thumb_func
FieldCamera_FollowActor:
	overlay_veneer FieldCamera_FollowActorFar
	.global Func_020014d8
	.thumb_func
Func_020014d8:
	overlay_veneer Field_TryJumpForwardFar
	.global Func_020014e0
	.thumb_func
Func_020014e0:
	overlay_veneer Func_080c8480
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_020014f0
	.thumb_func
Func_020014f0:
	overlay_veneer Sprite_ShareVramCacheEntryFar
	.global Func_020014f8
	.thumb_func
Func_020014f8:
	overlay_veneer Func_080c86f8
	.global Func_02001500
	.thumb_func
Func_02001500:
	overlay_veneer Func_080c8708
	.global Func_02001508
	.thumb_func
Func_02001508:
	overlay_veneer Func_080c8710
	.global Func_02001510
	.thumb_func
Func_02001510:
	overlay_veneer Func_080c8718
	.global Func_02001518
	.thumb_func
Func_02001518:
	overlay_veneer Func_080c8720
	.global Func_02001520
	.thumb_func
Func_02001520:
	overlay_veneer Func_080c8728
	.global Func_02001528
	.thumb_func
Func_02001528:
	overlay_veneer Func_080c8730
	.global Func_02001530
	.thumb_func
Func_02001530:
	overlay_veneer Func_080c8738
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
