@ Import veneers after the OVERLAY_68E overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_68E overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_020012b8
	.thumb_func
Func_020012b8:
	overlay_veneer Func_080000d0
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
	.global Func_020012d8
	.thumb_func
Func_020012d8:
	overlay_veneer Func_080003c8
	.global Func_020012e0
	.thumb_func
Func_020012e0:
	overlay_veneer Func_080003d0
	.global Func_020012e8
	.thumb_func
Func_020012e8:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_020012f8
	.thumb_func
Func_020012f8:
	overlay_veneer Func_08020178
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global UiText_DrawCharacterAtOffset
	.thumb_func
UiText_DrawCharacterAtOffset:
	overlay_veneer UiText_DrawCharacterAtOffsetFar
	.global UiText_DrawNumberInWindow
	.thumb_func
UiText_DrawNumberInWindow:
	overlay_veneer UiText_DrawNumberInWindowFar
	.global Func_02001328
	.thumb_func
Func_02001328:
	overlay_veneer UiText_DrawQuantity
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global Func_02001338
	.thumb_func
Func_02001338:
	overlay_veneer Func_08038340
	.global Func_02001340
	.thumb_func
Func_02001340:
	overlay_veneer Func_08038380
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global Party_AdjustSixDigitCounterA
	.thumb_func
Party_AdjustSixDigitCounterA:
	overlay_veneer Party_AdjustSixDigitCounterAFar
	.global Func_02001358
	.thumb_func
Func_02001358:
	overlay_veneer Func_080ad208
	.global PartyInventory_CountItem
	.thumb_func
PartyInventory_CountItem:
	overlay_veneer PartyInventory_CountItemFar
	.global PartyInventory_CountFreeSlots
	.thumb_func
PartyInventory_CountFreeSlots:
	overlay_veneer PartyInventory_CountFreeSlotsFar
	.global Func_02001370
	.thumb_func
Func_02001370:
	overlay_veneer Func_080ad2f8
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001380
	.thumb_func
Func_02001380:
	overlay_veneer Func_080c8018
	.global Func_02001388
	.thumb_func
Func_02001388:
	overlay_veneer Func_080c8020
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_020013b0
	.thumb_func
Func_020013b0:
	overlay_veneer Func_080c80f8
	.global Func_020013b8
	.thumb_func
Func_020013b8:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020013c8
	.thumb_func
Func_020013c8:
	overlay_veneer Func_080c8180
	.global Func_020013d0
	.thumb_func
Func_020013d0:
	overlay_veneer Func_080c8188
	.global Func_020013d8
	.thumb_func
Func_020013d8:
	overlay_veneer Func_080c81a0
	.global Func_020013e0
	.thumb_func
Func_020013e0:
	overlay_veneer Func_080c81d0
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Func_020013f0
	.thumb_func
Func_020013f0:
	overlay_veneer Func_080c83e0
	.global Func_020013f8
	.thumb_func
Func_020013f8:
	overlay_veneer Func_080c84e0
	.global Func_02001400
	.thumb_func
Func_02001400:
	overlay_veneer Func_080c8928
	.global Func_02001408
	.thumb_func
Func_02001408:
	overlay_veneer Audio_PlayCue
