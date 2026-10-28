.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
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
	overlay_veneer Object_SetMode
	.global Func_02003120
	.thumb_func
Func_02003120:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02003128
	.thumb_func
Func_02003128:
	overlay_veneer Func_080200c0
	.global Func_02003130
	.thumb_func
Func_02003130:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003140
	.thumb_func
Func_02003140:
	overlay_veneer Object_SetPosition
	.global Func_02003148
	.thumb_func
Func_02003148:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02003150
	.thumb_func
Func_02003150:
	overlay_veneer Func_08020198
	.global Func_02003158
	.thumb_func
Func_02003158:
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02003168
	.thumb_func
Func_02003168:
	overlay_veneer Func_080201d8
	.global Func_02003170
	.thumb_func
Func_02003170:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02003178
	.thumb_func
Func_02003178:
	overlay_veneer Func_080201f0
	.global Func_02003180
	.thumb_func
Func_02003180:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003198
	.thumb_func
Func_02003198:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_020031a8
	.thumb_func
Func_020031a8:
	overlay_veneer ObjectDispatch_SetSpritePriorityFar
	.ifndef TLA_EDITION_JA
	.global Func_020031b0
	.thumb_func
Func_020031b0:
	overlay_veneer Runtime_CheckRadiusOverlapFar
	.endif
	.global Func_020031b8
	.thumb_func
Func_020031b8:
	overlay_veneer Func_08020390
	.global Func_020031c0
	.thumb_func
Func_020031c0:
	overlay_veneer Func_08038210
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020031e0
	.thumb_func
Func_020031e0:
	overlay_veneer EventRuntime_StartFar
	.global Func_020031e8
	.thumb_func
Func_020031e8:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020031f8
	.thumb_func
Func_020031f8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02003200
	.thumb_func
Func_02003200:
	overlay_veneer Object_JumpDownLedgeFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02003210
	.thumb_func
Func_02003210:
	overlay_veneer Func_080c8218
	.global Func_02003218
	.thumb_func
Func_02003218:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003228
	.thumb_func
Func_02003228:
	overlay_veneer Func_080c8240
	.global Func_02003230
	.thumb_func
Func_02003230:
	overlay_veneer Func_080c8248
	.global Func_02003238
	.thumb_func
Func_02003238:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02003240
	.thumb_func
Func_02003240:
	overlay_veneer DisplayTransition_StartFar
	.global Func_02003248
	.thumb_func
Func_02003248:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02003250
	.thumb_func
Func_02003250:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02003258
	.thumb_func
Func_02003258:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02003270
	.thumb_func
Func_02003270:
	overlay_veneer DisplayScroll_InitializeHBlankDmaFar
	.global Func_02003278
	.thumb_func
Func_02003278:
	overlay_veneer Func_080c8480
	.global Func_02003280
	.thumb_func
Func_02003280:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02003288
	.thumb_func
Func_02003288:
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.global Func_02003290
	.thumb_func
Func_02003290:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02003298
	.thumb_func
Func_02003298:
	overlay_veneer EventRuntime_GetControlledOwnerFar
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
	.global Func_020032b8
	.thumb_func
Func_020032b8:
	overlay_veneer Sound_PlayFar
	.global Sound_WaitForBgmRestore
	.thumb_func
Sound_WaitForBgmRestore:
	overlay_veneer Sound_WaitForBgmRestoreFar
