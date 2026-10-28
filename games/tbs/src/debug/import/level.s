	.set overlay_imports, 1
	.set import.__modsi3, 1
	.set import.wait, 1
	.set import.Window_Create, 1
	.set import.Window_Close, 1
	.set import.Text_DrawMessage, 1
	.set import.Text_DrawMessageAsTiles, 1
	.set import.Text_DrawStringAtOffset, 1
	.set import.Text_DrawStringInWindow, 1
	.set import.Text_DrawNumberAtOffset, 1
	.set import.Text_DrawNumberInWindow, 1
	.set import.WindowSprite_RedrawSavedRect, 1
	.set import.WindowSprite_ClearList, 1
	.set import.DebugBrowser_Icons, 1
	.set import.DebugBrowser_Portraits, 1
	.set import.Unit_Get, 1
	.set import.Unit_Calculate, 1
	.set import.Item_GetDirect, 1
	.set import.PartyInventory_Add, 1
	.ifndef TBS_EDITION_JA
	.set import.Ability_GetDirect, 1
	.endif
	.set import.Party_ListMembers, 1
	.set import.Unit_LevelUpTo, 1
	.set import.PartyInventory_HasSpace, 1
	.ifdef TBS_EDITION_JA
	.set import.Event_Start, 1
	.endif
	.ifdef TBS_EDITION_JA
	.set import.Event_End, 1
	.endif
	.set import.Actor_GetById, 1
	.ifdef TBS_EDITION_JA
	.set import.Import_ActorSetPosition, 1
	.endif
	.set import.Import_ActorSetAnimation, 1
	.ifdef TBS_EDITION_JA
	.set import.Actor_WaitForAnimationChange, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.Event_SetMessageId, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.Event_ShowMessageAndWait, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.FieldEffect_SpawnAttached, 1
	.endif
	.set import.Import_EventSetExitRequest, 1
	.set import.ItemMenu_DrawItemDetails, 1
	.set import.Sound_Play, 1
	.include "games/tbs/src/cartridge/overlay.inc"
