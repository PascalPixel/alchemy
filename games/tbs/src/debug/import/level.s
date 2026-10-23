@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.__modsi3, 1
	.set import.Engine_TaskWait, 1
	.set import.Window_Create, 1
	.set import.Window_Close, 1
	.set import.Text_DrawMessage, 1
	.set import.Text_DrawMessageAsTiles, 1
	.set import.Text_DrawStringAtOffset, 1
	.set import.Text_DrawStringInWindow, 1
	.set import.Text_DrawNumberAtOffset, 1
	.set import.Text_DrawNumberInWindow, 1
	.set import.WindowSprite_RedrawSavedRect, 1
	.set import.Engine_DebugClearWindow, 1
	.set import.DebugMenu_BrowseIcons, 1
	.set import.DebugMenu_BrowsePortraits, 1
	.set import.Unit_Get, 1
	.set import.Unit_Calculate, 1
	.set import.Engine_DebugGetItem, 1
	.set import.PartyInventory_Add, 1
	.ifndef TBS_EDITION_JA
	.set import.Engine_DebugGetAbility, 1
	.endif
	.set import.Party_ListMembers, 1
	.set import.Unit_LevelUpTo, 1
	.set import.PartyInventory_HasSpace, 1
	.ifdef TBS_EDITION_JA
	.set import.Engine_ScriptStart, 1
	.endif
	.ifdef TBS_EDITION_JA
	.set import.Engine_ScriptEnd, 1
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
	.set import.Engine_ScriptSetMessage, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.Engine_ScriptShowMessage, 1
	.endif
	.ifndef TBS_EDITION_JA
	.set import.Engine_ObjectEventShowEmote, 1
	.endif
	.set import.Engine_ScriptRequestWarp, 1
	.set import.Engine_DebugDrawItemDetails, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
