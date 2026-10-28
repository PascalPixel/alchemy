.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
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
	.global Func_02000f7c
	.thumb_func
Func_02000f7c:
	overlay_veneer Actor_SetMode
	.global Func_02000f84
	.thumb_func
Func_02000f84:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02000f94
	.thumb_func
Func_02000f94:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02000f9c
	.thumb_func
Func_02000f9c:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02000fb4
	.thumb_func
Func_02000fb4:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02000fbc
	.thumb_func
Func_02000fbc:
	overlay_veneer Func_08038210
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02000fcc
	.thumb_func
Func_02000fcc:
	overlay_veneer Event_StartFar
	.global Func_02000fd4
	.thumb_func
Func_02000fd4:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Func_02000fec
	.thumb_func
Func_02000fec:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02001004
	.thumb_func
Func_02001004:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001014
	.thumb_func
Func_02001014:
	overlay_veneer Func_080c8240
	.global Func_0200101c
	.thumb_func
Func_0200101c:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_02001024
	.thumb_func
Func_02001024:
	overlay_veneer Func_080c8288
	.global Func_0200102c
	.thumb_func
Func_0200102c:
	overlay_veneer Func_080c82e0
	.global Func_02001034
	.thumb_func
Func_02001034:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_0200103c
	.thumb_func
Func_0200103c:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02001044
	.thumb_func
Func_02001044:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Func_0200104c
	.thumb_func
Func_0200104c:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02001054
	.thumb_func
Func_02001054:
	overlay_veneer Func_080c8580
	.global Func_0200105c
	.thumb_func
Func_0200105c:
	overlay_veneer Func_080c86d0
	.global Func_02001064
	.thumb_func
Func_02001064:
	overlay_veneer Func_080c86d8
	.global Func_0200106c
	.thumb_func
Func_0200106c:
	overlay_veneer Sound_PlayFar
