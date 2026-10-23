@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.__modsi3, 1
	.set import.WaitFrames, 1
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
	.set import.FieldScript_Start, 1
	.endif
	.ifdef TBS_EDITION_JA
	.set import.FieldScript_End, 1
	.endif
	.set import.ObjectEvent_GetById, 1
	.ifdef TBS_EDITION_JA
	.set import.Engine_ObjectEventSetPosition, 1
	.endif
	.set import.Engine_ObjectEventSetAnimation, 1
	.ifdef TBS_EDITION_JA
	.set import.ObjectMotion_WaitForAnimationChange, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.FieldScript_SetMessageId, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.FieldScript_ShowMessageAndWait, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.FieldEffect_SpawnLinked, 1
	.endif
	.set import.Engine_ScriptRequestWarp, 1
	.set import.ItemMenu_DrawItemDetails, 1
	.set import.Sound_PlaySong, 1
	.include "games/tbs/src/system/overlay.inc"
