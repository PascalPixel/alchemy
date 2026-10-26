	.set overlay_imports, 1
	.set import.__modsi3, 1
	.set import.WaitFrames, 1
	.set import.Window_Create, 1
	.set import.Window_Close, 1
	.ifndef TBS_EDITION_JA
	.set import.Message_OpenWindow, 1
	.endif
	.set import.FieldMessage_Show, 1
	.ifndef TBS_EDITION_JA
	.set import.TextPrinter_AllFinished, 1
	.endif
	.set import.Text_DrawMessage, 1
	.set import.Text_DrawMessageAsTiles, 1
	.set import.Text_DrawStringInWindow, 1
	.set import.Text_DrawNumberAtOffset, 1
	.ifndef TBS_EDITION_JA
	.set import.Portrait_OpenWindow, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.MessageArgument_Push, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.TextPrinter_CloseFinishedWindows, 1
	.endif
	.set import.WindowSprite_RedrawSavedRect, 1
	.set import.WindowSprite_ClearList, 1
	.set import.NamingScreen_EditMemberName, 1
	.set import.DebugBrowser_Icons, 1
	.set import.Unit_Get, 1
	.set import.Unit_Calculate, 1
	.set import.Item_GetDirect, 1
	.set import.Inventory_AddItem, 1
	.set import.PartyInventory_Add, 1
	.set import.Unit_AddHp, 1
	.set import.Unit_AddPp, 1
	.set import.Party_AddMember, 1
	.set import.Party_RemoveMember, 1
	.set import.Djinn_Give, 1
	.set import.Djinn_Set, 1
	.set import.Unit_LevelUpTo, 1
	.set import.Item_AdjustCounter, 1
	.set import.PartyInventory_HasSpace, 1
	.ifdef TBS_EDITION_ES
	.set import.PartyInventory_GiveItem, 1
	.endif
	.ifdef TBS_EDITION_FR
	.set import.PartyInventory_GiveItem, 1
	.endif
	.ifdef TBS_EDITION_IT
	.set import.PartyInventory_GiveItem, 1
	.endif
	.ifdef TBS_EDITION_DE
	.set import.FieldScript_SetMessageId, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set import.FieldScript_SetMessageId, 1
	.endif
	.ifdef TBS_EDITION_FR
	.set import.FieldScript_SetMessageId, 1
	.endif
	.ifdef TBS_EDITION_IT
	.set import.FieldScript_SetMessageId, 1
	.endif
	.ifdef TBS_EDITION_DE
	.set import.FieldScript_ShowMessageAndWait, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set import.FieldScript_ShowMessageAndWait, 1
	.endif
	.ifdef TBS_EDITION_FR
	.set import.FieldScript_ShowMessageAndWait, 1
	.endif
	.ifdef TBS_EDITION_IT
	.set import.FieldScript_ShowMessageAndWait, 1
	.endif
	.ifdef TBS_EDITION_JA
	.set import.MemberChooser_Open, 1
	.endif
	.set import.ItemMenu_DrawItemDetails, 1
	.set import.Shop_OpenDebugShop, 1
	.ifdef TBS_EDITION_JA
	.set import.Sanctum_Run, 1
	.endif
	.ifdef TBS_EDITION_EN
	.set import.Sanctum_Run, 1
	.endif
	.ifdef TBS_EDITION_DE
	.set import.Sanctum_Run, 1
	.endif
	.set import.ItemPicker_Run, 1
	.set import.Sound_PlaySong, 1
	.include "games/tbs/src/system/overlay.inc"
