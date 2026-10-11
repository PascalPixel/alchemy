@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/common/include/game/ed_asm.h"
	.include "games/tbs/src/system/overlay.inc"
	import __modsi3, IwramSignedRemainder
	import Engine_TaskWait, WaitFramesFar
	import UiWindow_Create, UiWindow_CreateFar
	import UiWork_Finalize, UiWork_FinalizeFar
	import UiText_DrawResource, UiText_DrawResourceFar
	import UiText_DrawCharacterAtOffset, UiText_DrawCharacterAtOffsetFar
	import UiText_DrawStringAtOffset, UiText_DrawStringAtOffsetFar
	import UiText_DrawStringInWindow, UiText_DrawStringInWindowFar
	import UiText_DrawNumberAtOffset, UiText_DrawNumberAtOffsetFar
	import UiText_DrawNumberInWindow, UiText_DrawNumberInWindowFar
	import RenderOutput_RedrawSavedRect, RenderOutput_RedrawSavedRectFar
	import Engine_DebugClearWindow, RenderOutput_ClearListFar
	import DebugMenu_BrowseIcons, DebugMenu_BrowseIconsFar
	import DebugMenu_BrowseEntryGlyphs, DebugMenu_BrowseEntryGlyphsFar
	import UnitGet, UnitGetFar
	import UnitCalc, UnitCalcFar
	import Engine_DebugGetItem, Item_GetDirectFar
	import PartyInventory_Add, PartyInventory_AddFar
	.if EDITION_INTERNATIONAL
	import Engine_DebugGetAbility, BtlActGet
	.endif
	import Party_ListActiveOwners, Party_ListActiveOwnersFar
	import Party_AdvanceOwnerCountToTarget, Party_AdvanceOwnerCountToTargetFar
	import PartyInventory_HasSpace, PartyInventory_HasSpaceFar
	.if EDITION_INTERNATIONAL
	import Object_GetById, Object_GetByIdFar
	import Engine_ActorSetAnimation, Object_SetModeByIdFar
	.endif
	.if EDITION_INTERNATIONAL
	import Engine_EventSetMessage, Event_SetMessageIdFar
	import Engine_EventShowMessage, BattleEv_RunWaitFar
	import Engine_ActorShowEmote, BattleFx_SpawnLinkedFar
	.else
	import Engine_EventBegin, Battle_ResetFar
	import Engine_EventEnd, BattleFx_FinishActionFar
	import Object_GetById, Object_GetByIdFar
	import Engine_ActorSetPosition, ObjectMotion_SetHorizontalPositionWithTerrainFar
	import Engine_ActorSetAnimation, Object_SetModeByIdFar
	import ObjectMotion_WaitForAnimationChange, ObjectMotion_WaitForAnimationChangeFar
	.endif
	import Engine_EventRequestExit, Event_SetExitRequestFar
	import Engine_DebugDrawItemDetails, ItemMenu_DrawItemDetailsFar
	import Engine_AudioPlayCue, AudioCommand_PlayFar
