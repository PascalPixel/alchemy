.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global HeapSlot_AllocEwram
	.thumb_func
HeapSlot_AllocEwram:
	overlay_veneer HeapSlot_AllocEwramFar
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_020010a0
	.thumb_func
Func_020010a0:
	overlay_veneer Func_08020300
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_020010b0
	.thumb_func
Func_020010b0:
	overlay_veneer MessageArgument_PushFar
	.global Func_020010b8
	.thumb_func
Func_020010b8:
	overlay_veneer Item_Get
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global Party_AdjustCoins
	.thumb_func
Party_AdjustCoins:
	overlay_veneer Party_AdjustCoinsFar
	.global Func_020010d0
	.thumb_func
Func_020010d0:
	overlay_veneer Party_FindOwnerWithActionFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_020010e0
	.thumb_func
Func_020010e0:
	overlay_veneer Event_StartFar
	.global Func_020010e8
	.thumb_func
Func_020010e8:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Func_02001108
	.thumb_func
Func_02001108:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_WaitForAnimationChange
	.thumb_func
Actor_WaitForAnimationChange:
	overlay_veneer Actor_WaitForAnimationChangeFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02001138
	.thumb_func
Func_02001138:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02001148
	.thumb_func
Func_02001148:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02001158
	.thumb_func
Func_02001158:
	overlay_veneer Func_080c8208
	.global Func_02001160
	.thumb_func
Func_02001160:
	overlay_veneer Func_080c8210
	.global Func_02001168
	.thumb_func
Func_02001168:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02001170
	.thumb_func
Func_02001170:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02001178
	.thumb_func
Func_02001178:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Func_02001180
	.thumb_func
Func_02001180:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02001188
	.thumb_func
Func_02001188:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02001190
	.thumb_func
Func_02001190:
	overlay_veneer Func_08108008
	.global Func_02001198
	.thumb_func
Func_02001198:
	overlay_veneer Func_08108018
	.global Func_020011a0
	.thumb_func
Func_020011a0:
	overlay_veneer Func_08108070
	.global Func_020011a8
	.thumb_func
Func_020011a8:
	overlay_veneer Sound_PlayFar
