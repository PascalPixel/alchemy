@ Import veneers after the OVERLAY_6A6 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6A6 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Func_02001238
	.thumb_func
Func_02001238:
	overlay_veneer Func_08000170
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02001248
	.thumb_func
Func_02001248:
	overlay_veneer Func_080001a8
	.global Func_02001250
	.thumb_func
Func_02001250:
	overlay_veneer Func_080001b8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02001268
	.thumb_func
Func_02001268:
	overlay_veneer Func_080001e8
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Func_02001278
	.thumb_func
Func_02001278:
	overlay_veneer Func_080002b8
	.global Func_02001280
	.thumb_func
Func_02001280:
	overlay_veneer Func_080002c0
	.global Func_02001288
	.thumb_func
Func_02001288:
	overlay_veneer Func_080003c8
	.global Func_02001290
	.thumb_func
Func_02001290:
	overlay_veneer Func_080003d0
	.global Func_02001298
	.thumb_func
Func_02001298:
	overlay_veneer Func_080003d8
	.global Func_020012a0
	.thumb_func
Func_020012a0:
	overlay_veneer Func_08020170
	.global Func_020012a8
	.thumb_func
Func_020012a8:
	overlay_veneer Func_08020178
	.global Func_020012b0
	.thumb_func
Func_020012b0:
	overlay_veneer Func_080201e8
	.global Func_020012b8
	.thumb_func
Func_020012b8:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_020012c8
	.thumb_func
Func_020012c8:
	overlay_veneer Func_08020228
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020012e0
	.thumb_func
Func_020012e0:
	overlay_veneer Func_080c8018
	.global Func_020012e8
	.thumb_func
Func_020012e8:
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
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02001348
	.thumb_func
Func_02001348:
	overlay_veneer Func_080c80f8
	.global Func_02001350
	.thumb_func
Func_02001350:
	overlay_veneer Func_080c8100
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_02001370
	.thumb_func
Func_02001370:
	overlay_veneer Func_080c8158
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02001380
	.thumb_func
Func_02001380:
	overlay_veneer Func_080c8180
	.global Func_02001388
	.thumb_func
Func_02001388:
	overlay_veneer Func_080c8188
	.global Func_02001390
	.thumb_func
Func_02001390:
	overlay_veneer Func_080c81a0
	.global Func_02001398
	.thumb_func
Func_02001398:
	overlay_veneer Func_080c81d0
	.global Func_020013a0
	.thumb_func
Func_020013a0:
	overlay_veneer Func_080c81d8
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
	overlay_veneer Func_080c8230
	.global Func_020013c0
	.thumb_func
Func_020013c0:
	overlay_veneer Func_080c8238
	.global Func_020013c8
	.thumb_func
Func_020013c8:
	overlay_veneer Func_080c8240
	.global Func_020013d0
	.thumb_func
Func_020013d0:
	overlay_veneer Func_080c8278
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_020013e8
	.thumb_func
Func_020013e8:
	overlay_veneer Func_080c8420
	.global Func_020013f0
	.thumb_func
Func_020013f0:
	overlay_veneer Func_080c8428
	.global Func_020013f8
	.thumb_func
Func_020013f8:
	overlay_veneer Func_080c84e0
	.global Func_02001400
	.thumb_func
Func_02001400:
	overlay_veneer Func_080c8580
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
	.global Func_02001420
	.thumb_func
Func_02001420:
	overlay_veneer Func_080c8778
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
	overlay_veneer Audio_PlayCue
