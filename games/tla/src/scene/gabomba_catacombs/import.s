.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_0200114c
	.thumb_func
Func_0200114c:
	overlay_veneer Actor_SetMode
	.global Func_02001154
	.thumb_func
Func_02001154:
	overlay_veneer Actor_StartScriptFar
	.global Func_0200115c
	.thumb_func
Func_0200115c:
	overlay_veneer Func_080200c0
	.global Func_02001164
	.thumb_func
Func_02001164:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_0200116c
	.thumb_func
Func_0200116c:
	overlay_veneer Func_080201f0
	.global Func_02001174
	.thumb_func
Func_02001174:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_0200118c
	.thumb_func
Func_0200118c:
	overlay_veneer Func_08038210
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_0200119c
	.thumb_func
Func_0200119c:
	overlay_veneer Event_StartFar
	.global Func_020011a4
	.thumb_func
Func_020011a4:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_020011cc
	.thumb_func
Func_020011cc:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_020011d4
	.thumb_func
Func_020011d4:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Func_020011e4
	.thumb_func
Func_020011e4:
	overlay_veneer Event_SetMessageIdFar
	.global Func_020011ec
	.thumb_func
Func_020011ec:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	overlay_veneer Func_080c8210
	.global Func_02001204
	.thumb_func
Func_02001204:
	overlay_veneer Func_080c8218
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001214
	.thumb_func
Func_02001214:
	overlay_veneer Func_080c8248
	.global Func_0200121c
	.thumb_func
Func_0200121c:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02001224
	.thumb_func
Func_02001224:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_0200122c
	.thumb_func
Func_0200122c:
	overlay_veneer Func_080c8288
	.global Func_02001234
	.thumb_func
Func_02001234:
	overlay_veneer Func_080c8368
	.global Func_0200123c
	.thumb_func
Func_0200123c:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02001244
	.thumb_func
Func_02001244:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_0200124c
	.thumb_func
Func_0200124c:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Func_02001254
	.thumb_func
Func_02001254:
	overlay_veneer Func_080c8480
	.global Func_0200125c
	.thumb_func
Func_0200125c:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02001264
	.thumb_func
Func_02001264:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_0200126c
	.thumb_func
Func_0200126c:
	overlay_veneer Func_080c8880
	.global Func_02001274
	.thumb_func
Func_02001274:
	overlay_veneer Func_080c88d0
	.global Func_0200127c
	.thumb_func
Func_0200127c:
	overlay_veneer Func_080c8948
	.global Func_02001284
	.thumb_func
Func_02001284:
	overlay_veneer Sound_PlayFar
