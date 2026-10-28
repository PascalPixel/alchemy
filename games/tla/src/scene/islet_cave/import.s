.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
	overlay_veneer Func_080000d8
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
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02001214
	.thumb_func
Func_02001214:
	overlay_veneer Actor_SetMode
	.global Func_0200121c
	.thumb_func
Func_0200121c:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02001224
	.thumb_func
Func_02001224:
	overlay_veneer Actor_CommitPosition
	.global Func_0200122c
	.thumb_func
Func_0200122c:
	overlay_veneer Map_PlayMapEditFar
	.global Func_02001234
	.thumb_func
Func_02001234:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02001244
	.thumb_func
Func_02001244:
	overlay_veneer Func_08038210
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001254
	.thumb_func
Func_02001254:
	overlay_veneer Event_StartFar
	.global Func_0200125c
	.thumb_func
Func_0200125c:
	overlay_veneer Event_EndFar
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
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Func_0200129c
	.thumb_func
Func_0200129c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Func_020012bc
	.thumb_func
Func_020012bc:
	overlay_veneer Actor_ConfigureSpriteColorFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_020012cc
	.thumb_func
Func_020012cc:
	overlay_veneer Event_SetExitRequestFar
	.global Func_020012d4
	.thumb_func
Func_020012d4:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_020012dc
	.thumb_func
Func_020012dc:
	overlay_veneer Func_080c8288
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Func_020012ec
	.thumb_func
Func_020012ec:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_020012f4
	.thumb_func
Func_020012f4:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_020012fc
	.thumb_func
Func_020012fc:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
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
	.global Func_0200131c
	.thumb_func
Func_0200131c:
	overlay_veneer Func_080c8480
	.global Func_02001324
	.thumb_func
Func_02001324:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_0200132c
	.thumb_func
Func_0200132c:
	overlay_veneer Func_080c88d8
	.global Func_02001334
	.thumb_func
Func_02001334:
	overlay_veneer Func_080c88e8
	.global Func_0200133c
	.thumb_func
Func_0200133c:
	overlay_veneer Func_080c8968
	.global Func_02001344
	.thumb_func
Func_02001344:
	overlay_veneer Sound_PlayFar
