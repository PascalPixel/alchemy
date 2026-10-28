.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02001248
	.thumb_func
Func_02001248:
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
	.global Func_02001268
	.thumb_func
Func_02001268:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Func_02001278
	.thumb_func
Func_02001278:
	overlay_veneer Fade_ToWhiteFar
	.global Func_02001280
	.thumb_func
Func_02001280:
	overlay_veneer Fade_FromWhiteFar
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
	.global Func_020012a0
	.thumb_func
Func_020012a0:
	overlay_veneer Map_PlayMapEditFar
	.global Func_020012a8
	.thumb_func
Func_020012a8:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_020012b0
	.thumb_func
Func_020012b0:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_020012b8
	.thumb_func
Func_020012b8:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_020012c8
	.thumb_func
Func_020012c8:
	overlay_veneer Map_SetScreenShakeFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020012e0
	.thumb_func
Func_020012e0:
	overlay_veneer Event_StartFar
	.global Func_020012e8
	.thumb_func
Func_020012e8:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
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
	.global Actor_RefreshSelectorById
	.thumb_func
Actor_RefreshSelectorById:
	overlay_veneer Actor_RefreshSelectorByIdFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02001348
	.thumb_func
Func_02001348:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02001350
	.thumb_func
Func_02001350:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Func_02001380
	.thumb_func
Func_02001380:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02001390
	.thumb_func
Func_02001390:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_020013a0
	.thumb_func
Func_020013a0:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Func_020013a8
	.thumb_func
Func_020013a8:
	overlay_veneer Func_080c8210
	.global Func_020013b0
	.thumb_func
Func_020013b0:
	overlay_veneer Func_080c8218
	.global Func_020013b8
	.thumb_func
Func_020013b8:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020013c8
	.thumb_func
Func_020013c8:
	overlay_veneer Func_080c8240
	.global Func_020013d0
	.thumb_func
Func_020013d0:
	overlay_veneer Event_SetExitRequestFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Actor_SetWideSprite
	.thumb_func
Actor_SetWideSprite:
	overlay_veneer Actor_SetWideSpriteFar
	.global Func_020013f0
	.thumb_func
Func_020013f0:
	overlay_veneer Actor_ReturnHomeFar
	.global Func_020013f8
	.thumb_func
Func_020013f8:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02001400
	.thumb_func
Func_02001400:
	overlay_veneer Func_080c8580
	.global Actor_OffsetPositionAndReset
	.thumb_func
Actor_OffsetPositionAndReset:
	overlay_veneer Actor_OffsetPositionAndResetFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02001420
	.thumb_func
Func_02001420:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02001428
	.thumb_func
Func_02001428:
	overlay_veneer Func_080c8860
	.global Func_02001430
	.thumb_func
Func_02001430:
	overlay_veneer Func_08108008
	.global Func_02001438
	.thumb_func
Func_02001438:
	overlay_veneer Func_08108010
	.global Func_02001440
	.thumb_func
Func_02001440:
	overlay_veneer Func_08108018
	.global Func_02001448
	.thumb_func
Func_02001448:
	overlay_veneer Sound_PlayFar
