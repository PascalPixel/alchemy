@ Import veneers after the OVERLAY_6BA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6BA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02000c88
	.thumb_func
Func_02000c88:
	overlay_veneer Func_080200c0
	.global Func_02000c90
	.thumb_func
Func_02000c90:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080201b0
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201b0
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201b0
	.else
	overlay_veneer Func_080201e0
	.endif
	.endif
	.endif
	.endif
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	overlay_veneer UiText_OpenMessageWindowFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global UiWork_IsComplete
	.thumb_func
UiWork_IsComplete:
	overlay_veneer UiWork_IsCompleteFar
	.global RenderOutput_PrepareForRedraw
	.thumb_func
RenderOutput_PrepareForRedraw:
	overlay_veneer RenderOutput_PrepareForRedrawFar
	.global UiText_DrawResource
	.thumb_func
UiText_DrawResource:
	overlay_veneer UiText_DrawResourceFar
	.global UiText_DrawCharacterAtOffset
	.thumb_func
UiText_DrawCharacterAtOffset:
	overlay_veneer UiText_DrawCharacterAtOffsetFar
	.global UiText_DrawStringAtOffset
	.thumb_func
UiText_DrawStringAtOffset:
	overlay_veneer UiText_DrawStringAtOffsetFar
	.global UiText_DrawStringInWindow
	.thumb_func
UiText_DrawStringInWindow:
	overlay_veneer UiText_DrawStringInWindowFar
	.global UiText_DrawNumberAtOffset
	.thumb_func
UiText_DrawNumberAtOffset:
	overlay_veneer UiText_DrawNumberAtOffsetFar
	.global UiText_DrawNumberInWindow
	.thumb_func
UiText_DrawNumberInWindow:
	overlay_veneer UiText_DrawNumberInWindowFar
	.global Func_02000d00
	.thumb_func
Func_02000d00:
	overlay_veneer Func_080380f8
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	overlay_veneer UiText_DrawQuantity
	.global UiWork_FinalizePendingCore
	.thumb_func
UiWork_FinalizePendingCore:
	overlay_veneer UiWork_FinalizePendingCoreFar
	.global RenderOutput_RedrawSavedRect
	.thumb_func
RenderOutput_RedrawSavedRect:
	overlay_veneer RenderOutput_RedrawSavedRectFar
	.global Func_02000d20
	.thumb_func
Func_02000d20:
	overlay_veneer Func_080382f8
	.global Func_02000d28
	.thumb_func
Func_02000d28:
	overlay_veneer Func_08038318
	.global Func_02000d30
	.thumb_func
Func_02000d30:
	overlay_veneer Func_08038320
	.global Func_02000d38
	.thumb_func
Func_02000d38:
	overlay_veneer Func_08038340
	.global Owner_RecalculateStats
	.thumb_func
Owner_RecalculateStats:
	overlay_veneer Owner_RecalculateStatsFar
	.global Func_02000d48
	.thumb_func
Func_02000d48:
	overlay_veneer Item_Get
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer Inventory_AddItemFar
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	.global Inventory_Break
	.thumb_func
Inventory_Break:
	overlay_veneer Inventory_BreakFar
	.global Owner_AdjustFirstValue
	.thumb_func
Owner_AdjustFirstValue:
	overlay_veneer Owner_AdjustFirstValueFar
	.global Owner_AdjustSecondValue
	.thumb_func
Owner_AdjustSecondValue:
	overlay_veneer Owner_AdjustSecondValueFar
	.global Party_ListActiveOwners
	.thumb_func
Party_ListActiveOwners:
	overlay_veneer Party_ListActiveOwnersFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
	.global Djinn_AddToOwner
	.thumb_func
Djinn_AddToOwner:
	overlay_veneer Djinn_AddToOwnerFar
	.global Djinn_Activate
	.thumb_func
Djinn_Activate:
	overlay_veneer Djinn_ActivateFar
	.global Party_AdvanceOwnerCountToTarget
	.thumb_func
Party_AdvanceOwnerCountToTarget:
	overlay_veneer Party_AdvanceOwnerCountToTargetFar
	.global Party_AdjustSixDigitCounterA
	.thumb_func
Party_AdjustSixDigitCounterA:
	overlay_veneer Party_AdjustSixDigitCounterAFar
	.global PartyInventory_HasSpace
	.thumb_func
PartyInventory_HasSpace:
	overlay_veneer PartyInventory_HasSpaceFar
	.global Func_02000db0
	.thumb_func
Func_02000db0:
	overlay_veneer Func_080ad240
	.global Djinn_AddToLeastLoadedOwner
	.thumb_func
Djinn_AddToLeastLoadedOwner:
	overlay_veneer Djinn_AddToLeastLoadedOwnerFar
	.global Func_02000dc0
	.thumb_func
Func_02000dc0:
	overlay_veneer Func_080ad2b8
	.global Func_02000dc8
	.thumb_func
Func_02000dc8:
	overlay_veneer Func_080ad2f0
	.global Func_02000dd0
	.thumb_func
Func_02000dd0:
	overlay_veneer Func_080ad2f8
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
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
	.global Func_02000df8
	.thumb_func
Func_02000df8:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02000e08
	.thumb_func
Func_02000e08:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02000e18
	.thumb_func
Func_02000e18:
	overlay_veneer Func_080c81a0
	.global Func_02000e20
	.thumb_func
Func_02000e20:
	overlay_veneer Func_080c8280
	.global Func_02000e28
	.thumb_func
Func_02000e28:
	overlay_veneer Func_080c83e0
	.global Func_02000e30
	.thumb_func
Func_02000e30:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global ItemMenu_DrawItemDetails
	.thumb_func
ItemMenu_DrawItemDetails:
	overlay_veneer ItemMenu_DrawItemDetailsFar
	.global Func_02000e40
	.thumb_func
Func_02000e40:
	overlay_veneer Func_08108008
	.global Func_02000e48
	.thumb_func
Func_02000e48:
	overlay_veneer Func_08108010
	.global Func_02000e50
	.thumb_func
Func_02000e50:
	overlay_veneer Func_08108018
	.global Func_02000e58
	.thumb_func
Func_02000e58:
	overlay_veneer Func_08108020
	.global Func_02000e60
	.thumb_func
Func_02000e60:
	overlay_veneer Func_08108070
	.global Func_02000e68
	.thumb_func
Func_02000e68:
	overlay_veneer Func_08108098
	.global Func_02000e70
	.thumb_func
Func_02000e70:
	overlay_veneer Func_081080a0
	.global Func_02000e78
	.thumb_func
Func_02000e78:
	overlay_veneer Audio_PlayCue
