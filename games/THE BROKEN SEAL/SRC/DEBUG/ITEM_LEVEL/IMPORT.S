	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Import veneers after the ITEM_LEVEL overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — ITEM_LEVEL overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global Engine_TaskWait
	.thumb_func
Engine_TaskWait:
	overlay_veneer WaitFramesFar
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
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
	.global RenderOutput_RedrawSavedRect
	.thumb_func
RenderOutput_RedrawSavedRect:
	overlay_veneer RenderOutput_RedrawSavedRectFar
	.global Engine_DebugClearWindow
	.thumb_func
Engine_DebugClearWindow:
	overlay_veneer RenderOutput_ClearListFar
	.global DebugMenu_BrowseIcons
	.thumb_func
DebugMenu_BrowseIcons:
	overlay_veneer DebugMenu_BrowseIconsFar
	.global DebugMenu_BrowseEntryGlyphs
	.thumb_func
DebugMenu_BrowseEntryGlyphs:
	overlay_veneer DebugMenu_BrowseEntryGlyphsFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global Owner_RecalculateStats
	.thumb_func
Owner_RecalculateStats:
	overlay_veneer Owner_RecalculateStatsFar
	.global Engine_DebugGetItem
	.thumb_func
Engine_DebugGetItem:
	overlay_veneer Func_08077018
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	.if EDITION_INTERNATIONAL
	.global Engine_DebugGetAbility
	.thumb_func
Engine_DebugGetAbility:
	overlay_veneer Func_08077080
	.endif
	.global Party_ListActiveOwners
	.thumb_func
Party_ListActiveOwners:
	overlay_veneer Party_ListActiveOwnersFar
	.global Party_AdvanceOwnerCountToTarget
	.thumb_func
Party_AdvanceOwnerCountToTarget:
	overlay_veneer Party_AdvanceOwnerCountToTargetFar
	.global PartyInventory_HasSpace
	.thumb_func
PartyInventory_HasSpace:
	overlay_veneer PartyInventory_HasSpaceFar
	.if EDITION_INTERNATIONAL
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer Object_SetModeByIdFar
	.endif
	.if EDITION_INTERNATIONAL
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer Event_SetValue1d8Far
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer BattleEv_RunWaitFar
	.global Engine_ActorShowEmote
	.thumb_func
Engine_ActorShowEmote:
	overlay_veneer BattleFx_SpawnLinkedFar
	.else
	.global Battle_Reset
	.thumb_func
Battle_Reset:
	overlay_veneer Battle_ResetFar
	.global BattleFx_FinishAction
	.thumb_func
BattleFx_FinishAction:
	overlay_veneer BattleFx_FinishActionFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetHorizontalPositionWithTerrain
	.thumb_func
ObjectMotion_SetHorizontalPositionWithTerrain:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.endif
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer Event_SetValue170Far
	.global Engine_DebugDrawItemDetails
	.thumb_func
Engine_DebugDrawItemDetails:
	overlay_veneer ItemMenu_DrawItemDetailsFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
