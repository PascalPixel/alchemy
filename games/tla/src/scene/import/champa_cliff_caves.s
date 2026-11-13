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
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Decompress_TaggedLz
	.thumb_func
Decompress_TaggedLz:
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
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetFar
	.global MapGrid_PlayMapEdit
	.thumb_func
MapGrid_PlayMapEdit:
	overlay_veneer MapGrid_PlayMapEditFar
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
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global MapGrid_SetScreenShake
	.thumb_func
MapGrid_SetScreenShake:
	overlay_veneer MapGrid_SetScreenShakeFar
	.global Func_0200343c
	.thumb_func
Func_0200343c:
	overlay_veneer Func_08020360
	.global PartyInventory_FindMember
	.thumb_func
PartyInventory_FindMember:
	overlay_veneer PartyInventory_FindMemberFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Event_Start
	.thumb_func
Event_Start:
	overlay_veneer Event_StartFar
	.global Event_End
	.thumb_func
Event_End:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Func_02003474
	.thumb_func
Func_02003474:
	overlay_veneer Func_080c8078
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
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_SetDestinationById
	.thumb_func
Actor_SetDestinationById:
	overlay_veneer Actor_SetDestinationByIdFar
	.global Actor_WalkToById
	.thumb_func
Actor_WalkToById:
	overlay_veneer Actor_WalkToByIdFar
	.global Actor_WalkToAndWaitById
	.thumb_func
Actor_WalkToAndWaitById:
	overlay_veneer Actor_WalkToAndWaitByIdFar
	.global Actor_CenterOnColumn
	.thumb_func
Actor_CenterOnColumn:
	overlay_veneer Actor_CenterOnColumnFar
	.global Actor_SetDestinationOffsetById
	.thumb_func
Actor_SetDestinationOffsetById:
	overlay_veneer Actor_SetDestinationOffsetByIdFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_FaceActorById
	.thumb_func
Actor_FaceActorById:
	overlay_veneer Actor_FaceActorByIdFar
	.global Event_SetMessageId
	.thumb_func
Event_SetMessageId:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Event_ShowMessageThenWait
	.thumb_func
Event_ShowMessageThenWait:
	overlay_veneer Event_ShowMessageThenWaitFar
	.global Event_ShowMessageAndWait
	.thumb_func
Event_ShowMessageAndWait:
	overlay_veneer Event_ShowMessageAndWaitFar
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
	.global Actor_ShowEmoteById
	.thumb_func
Actor_ShowEmoteById:
	overlay_veneer Actor_ShowEmoteByIdFar
	.global FieldEffect_SetAttached
	.thumb_func
FieldEffect_SetAttached:
	overlay_veneer FieldEffect_SetAttachedFar
	.global FieldCamera_FollowActor
	.thumb_func
FieldCamera_FollowActor:
	overlay_veneer FieldCamera_FollowActorFar
	.global FieldCamera_MoveWithinMapBounds
	.thumb_func
FieldCamera_MoveWithinMapBounds:
	overlay_veneer FieldCamera_MoveWithinMapBoundsFar
	.global Func_02003564
	.thumb_func
Func_02003564:
	overlay_veneer FieldCamera_WaitForArrivalFar
	.global Event_SetDestination
	.thumb_func
Event_SetDestination:
	overlay_veneer Event_SetDestinationFar
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global Field_RequestBattleFromEncounterTable
	.thumb_func
Field_RequestBattleFromEncounterTable:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Event_SetAfterBattleDestination
	.thumb_func
Event_SetAfterBattleDestination:
	overlay_veneer Event_SetAfterBattleDestinationFar
	.global Party_SetFields1f2And1f4
	.thumb_func
Party_SetFields1f2And1f4:
	overlay_veneer Party_SetFields1f2And1f4Far
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
	.global Func_020035ac
	.thumb_func
Func_020035ac:
	overlay_veneer Func_080c8480
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Scene_PlayMusic
	.thumb_func
Scene_PlayMusic:
	overlay_veneer Scene_PlayMusicFar
	.global Camera_WorldToScreen
	.thumb_func
Camera_WorldToScreen:
	overlay_veneer Camera_WorldToScreenFar
	.global Actor_LaunchFromLeader
	.thumb_func
Actor_LaunchFromLeader:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_WalkByById
	.thumb_func
Actor_WalkByById:
	overlay_veneer Actor_WalkByByIdFar
	.global Actor_WalkByAndWaitById
	.thumb_func
Actor_WalkByAndWaitById:
	overlay_veneer Actor_WalkByAndWaitByIdFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Actor_SetActionCallback
	.thumb_func
Actor_SetActionCallback:
	overlay_veneer Actor_SetActionCallbackFar
	.global Party_GetLeaderActor
	.thumb_func
Party_GetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_020035fc
	.thumb_func
Func_020035fc:
	overlay_veneer Func_080c87c0
	.global Func_02003604
	.thumb_func
Func_02003604:
	overlay_veneer Func_080c87c8
	.global Func_0200360c
	.thumb_func
Func_0200360c:
	overlay_veneer Field_StartColorTransitionFar
	.ifdef TLA_EDITION_ES
	.global Field_EndActorSpriteEffect
	.thumb_func
Field_EndActorSpriteEffect:
	overlay_veneer Field_StartPaletteTransitionFar
	.else
	.ifdef TLA_EDITION_IT
	.global Field_EndActorSpriteEffect
	.thumb_func
Field_EndActorSpriteEffect:
	overlay_veneer Field_StartPaletteTransitionFar
	.else
	.global Field_StartPaletteTransition
	.thumb_func
Field_StartPaletteTransition:
	overlay_veneer Field_StartPaletteTransitionFar
	.endif
	.endif
	.global Actor_ShowItemIconById
	.thumb_func
Actor_ShowItemIconById:
	overlay_veneer Actor_ShowItemIconByIdFar
	.global Func_02003624
	.thumb_func
Func_02003624:
	overlay_veneer Func_080c88f0
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
