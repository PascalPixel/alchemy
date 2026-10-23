@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.__modsi3, 1
	.set import.Engine_TaskWait, 1
	.set import.Engine_DebugCreateWindow, 1
	.set import.Engine_DebugFinalizeWindow, 1
	.ifndef TBS_EDITION_JA
	.set import.UiText_OpenMessageWindow, 1
	.endif
	.set import.UiText_ShowPositionedMessageAndWait, 1
	.ifndef TBS_EDITION_JA
	.set import.UiWork_IsComplete, 1
	.endif
	.set import.UiText_DrawResource, 1
	.set import.UiText_DrawCharacterAtOffset, 1
	.set import.UiText_DrawStringInWindow, 1
	.set import.UiText_DrawNumberAtOffset, 1
	.ifndef TBS_EDITION_JA
	.set import.UiWindow_CreateWithSideObject, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.UiWork_PushValueSlot, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.UiWork_FinalizePendingCore, 1
	.endif
	.set import.Engine_DebugRedrawWindow, 1
	.set import.Engine_DebugClearWindow, 1
	.set import.NamingScreen_EditMemberName, 1
	.set import.DebugMenu_BrowseIcons, 1
	.set import.Unit_Get, 1
	.set import.Unit_Calculate, 1
	.set import.Engine_DebugGetItem, 1
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
	.set import.Engine_ScriptSetMessage, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set import.Engine_ScriptSetMessage, 1
	.endif
	.ifdef TBS_EDITION_FR
	.set import.Engine_ScriptSetMessage, 1
	.endif
	.ifdef TBS_EDITION_IT
	.set import.Engine_ScriptSetMessage, 1
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
	.set import.Menu_OpenCharacterSelector, 1
	.endif
	.set import.Engine_DebugDrawItemDetails, 1
	.set import.Shop_OpenDebugShop, 1
	.ifdef TBS_EDITION_JA
	.set import.Shop_ConfirmAct, 1
	.endif
	.ifdef TBS_EDITION_EN
	.set import.Shop_ConfirmAct, 1
	.endif
	.ifdef TBS_EDITION_DE
	.set import.Shop_ConfirmAct, 1
	.endif
	.set import.Shop_PickUnitItem, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
