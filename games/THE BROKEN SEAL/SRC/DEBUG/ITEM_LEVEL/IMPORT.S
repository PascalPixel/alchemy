@ Import veneers after the ITEM_LEVEL overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — ITEM_LEVEL overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedRemainder
	overlay_veneer WaitFramesFar
	overlay_veneer UiWindow_CreateFar
	overlay_veneer UiWork_FinalizeFar
	overlay_veneer UiText_DrawResourceFar
	overlay_veneer UiText_DrawCharacterAtOffsetFar
	overlay_veneer UiText_DrawStringAtOffsetFar
	overlay_veneer UiText_DrawStringInWindowFar
	overlay_veneer UiText_DrawNumberAtOffsetFar
	overlay_veneer UiText_DrawNumberInWindowFar
	overlay_veneer RenderOutput_RedrawSavedRectFar
	overlay_veneer RenderOutput_ClearListFar
	overlay_veneer DebugMenu_BrowseIconsFar
	overlay_veneer DebugMenu_BrowseEntryGlyphsFar
	overlay_veneer Owner_GetStateFar
	overlay_veneer Owner_RecalculateStatsFar
	overlay_veneer Func_08077018
	overlay_veneer PartyInventory_AddFar
	overlay_veneer Func_08077080
	overlay_veneer Party_ListActiveOwnersFar
	overlay_veneer Party_AdvanceOwnerCountToTargetFar
	overlay_veneer PartyInventory_HasSpaceFar
	overlay_veneer Object_GetByIdFar
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Event_SetValue1d8Far
	overlay_veneer BattleEv_RunWaitFar
	overlay_veneer BattleFx_SpawnLinkedFar
	overlay_veneer Event_SetValue170Far
	overlay_veneer ItemMenu_DrawItemDetailsFar
	overlay_veneer AudioCommand_PlayFar
