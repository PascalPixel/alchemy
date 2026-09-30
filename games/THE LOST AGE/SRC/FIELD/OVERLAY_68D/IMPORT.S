@ Import veneers after the OVERLAY_68D overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_68D overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global Engine_MathModulo
	.thumb_func
Engine_MathModulo:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02001f4c
	.thumb_func
Func_02001f4c:
	overlay_veneer Func_080000d0
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02001f6c
	.thumb_func
Func_02001f6c:
	overlay_veneer Func_080001a8
	.global Func_02001f74
	.thumb_func
Func_02001f74:
	overlay_veneer Func_080001b8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02001f8c
	.thumb_func
Func_02001f8c:
	overlay_veneer Func_080003c8
	.global Func_02001f94
	.thumb_func
Func_02001f94:
	overlay_veneer Func_080003d0
	.global Func_02001f9c
	.thumb_func
Func_02001f9c:
	overlay_veneer Func_080200c8
	.global Func_02001fa4
	.thumb_func
Func_02001fa4:
	overlay_veneer Object_SetPosition
	.global Func_02001fac
	.thumb_func
Func_02001fac:
	overlay_veneer Object_CommitPosition
	.global Func_02001fb4
	.thumb_func
Func_02001fb4:
	overlay_veneer Func_08020170
	.global Party_AddActiveOwner
	.thumb_func
Party_AddActiveOwner:
	overlay_veneer Party_AddActiveOwnerFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001fcc
	.thumb_func
Func_02001fcc:
	overlay_veneer Func_080c8018
	.global Func_02001fd4
	.thumb_func
Func_02001fd4:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_SnapHeadingAndOffset
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02002024
	.thumb_func
Func_02002024:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_02002054
	.thumb_func
Func_02002054:
	overlay_veneer Func_080c8158
	.global Func_0200205c
	.thumb_func
Func_0200205c:
	overlay_veneer Func_080c8180
	.global Func_02002064
	.thumb_func
Func_02002064:
	overlay_veneer Func_080c8188
	.global Func_0200206c
	.thumb_func
Func_0200206c:
	overlay_veneer Func_080c8198
	.global Func_02002074
	.thumb_func
Func_02002074:
	overlay_veneer Func_080c81d0
	.global Func_0200207c
	.thumb_func
Func_0200207c:
	overlay_veneer Func_080c81d8
	.global Func_02002084
	.thumb_func
Func_02002084:
	overlay_veneer Func_080c8200
	.global Func_0200208c
	.thumb_func
Func_0200208c:
	overlay_veneer Func_080c8210
	.global Func_02002094
	.thumb_func
Func_02002094:
	overlay_veneer Func_080c8218
	.global Func_0200209c
	.thumb_func
Func_0200209c:
	overlay_veneer Func_080c8228
	.global Func_020020a4
	.thumb_func
Func_020020a4:
	overlay_veneer Func_080c8238
	.global Func_020020ac
	.thumb_func
Func_020020ac:
	overlay_veneer Func_080c8240
	.global Func_020020b4
	.thumb_func
Func_020020b4:
	overlay_veneer Func_080c8278
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_020020cc
	.thumb_func
Func_020020cc:
	overlay_veneer Func_080c84e0
	.global Func_020020d4
	.thumb_func
Func_020020d4:
	overlay_veneer Func_080c85c0
	.global Func_020020dc
	.thumb_func
Func_020020dc:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_020020fc
	.thumb_func
Func_020020fc:
	overlay_veneer Func_080c8758
	.global Func_02002104
	.thumb_func
Func_02002104:
	overlay_veneer Func_080c87c0
	.global Func_0200210c
	.thumb_func
Func_0200210c:
	overlay_veneer Func_080c87c8
	.global Func_02002114
	.thumb_func
Func_02002114:
	overlay_veneer Func_080c87f8
	.global Field_BeginPaletteTransition
	.thumb_func
Field_BeginPaletteTransition:
	overlay_veneer Field_BeginPaletteTransitionFar
	.global Func_02002124
	.thumb_func
Func_02002124:
	overlay_veneer Audio_PlayCue
