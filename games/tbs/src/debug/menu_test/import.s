@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/common/include/game/ed_asm.h"
	.include "games/tbs/src/system/overlay.inc"
	import __modsi3, IwramSignedRemainder
	import Engine_TaskWait, WaitFramesFar
	import Engine_DebugCreateWindow, UiWindow_CreateFar
	import Engine_DebugFinalizeWindow, UiWork_FinalizeFar
	.if EDITION_INTERNATIONAL
	import UiText_OpenMessageWindow, UiText_OpenMessageWindowFar
	.endif
	import UiText_ShowPositionedMessageAndWait, UiText_ShowPositionedMessageAndWaitFar
	.if EDITION_INTERNATIONAL
	import UiWork_IsComplete, UiWork_IsCompleteFar
	.endif
	import UiText_DrawResource, UiText_DrawResourceFar
	import UiText_DrawCharacterAtOffset, UiText_DrawCharacterAtOffsetFar
	import UiText_DrawStringInWindow, UiText_DrawStringInWindowFar
	import UiText_DrawNumberAtOffset, UiText_DrawNumberAtOffsetFar
	.if EDITION_INTERNATIONAL
	import UiWindow_CreateWithSideObject, UiWindow_CreateWithSideObjectFar
	import UiWork_PushValueSlot, UiWork_PushValueSlotFar
	import UiWork_FinalizePendingCore, UiWork_FinalizePendingCoreFar
	.endif
	import Engine_DebugRedrawWindow, RenderOutput_RedrawSavedRectFar
	import Engine_DebugClearWindow, RenderOutput_ClearListFar
	import NameEntry_EditOwnerName, NameEntry_EditOwnerNameFar
	import DebugMenu_BrowseIcons, DebugMenu_BrowseIconsFar
	import UnitGet, UnitGetFar
	import UnitCalc, UnitCalcFar
	import Engine_DebugGetItem, Item_GetDirectFar
	import Inventory_AddItem, Inventory_AddItemFar
	import PartyInventory_Add, PartyInventory_AddFar
	import Owner_AdjustFirstValue, Owner_AdjustFirstValueFar
	import UnitEpAdd, UnitEpAddFar
	import Party_AddActiveOwner, Party_AddActiveOwnerFar
	import Party_RemoveActiveOwner, Party_RemoveActiveOwnerFar
	import Djinn_AddToOwner, Djinn_AddToOwnerFar
	import Djinn_Activate, Djinn_ActivateFar
	import Party_AdvanceOwnerCountToTarget, Party_AdvanceOwnerCountToTargetFar
	import Item_AdjustCounter, Item_AdjustCounterFar
	import PartyInventory_HasSpace, PartyInventory_HasSpaceFar
	.ifndef TBS_EDITION_EN
	.if EDITION_INTERNATIONAL
	.ifndef TBS_EDITION_DE
	import PartyInventory_GiveItem, PartyInventory_GiveItemFar
	.endif
	.endif
	.endif
	.ifndef TBS_EDITION_EN
	.if EDITION_INTERNATIONAL
	import Engine_EventSetMessage, Event_SetMessageIdFar
	import BattleEv_RunWait, BattleEv_RunWaitFar
	.endif
	.endif
	.ifeq EDITION_INTERNATIONAL
	import Menu_OpenCharacterSelector, Menu_OpenCharacterSelectorFar
	.endif
	import Engine_DebugDrawItemDetails, ItemMenu_DrawItemDetailsFar
	import Battle_ApplyPresetItemsAndFlags, Battle_ApplyPresetItemsAndFlagsFar
	.ifdef TBS_EDITION_EN
	import Shop_ConfirmAct, Shop_ConfirmActFar
	.else
	.ifeq EDITION_INTERNATIONAL
	import Shop_ConfirmAct, Shop_ConfirmActFar
	.else
	.ifdef TBS_EDITION_DE
	import Shop_ConfirmAct, Shop_ConfirmActFar
	.endif
	.endif
	.endif
	import Shop_PickUnitItem, Shop_PickUnitItemFar
	import Engine_AudioPlayCue, AudioCommand_PlayFar
