.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Localization_LookupEntryIdFar
	.thumb_func
Localization_LookupEntryIdFar:
	overlay_veneer Localization_LookupEntryId
	.global Func_080381e8
	.thumb_func
Func_080381e8:
	overlay_veneer Func_0803f778
	.global Menu_SelectSaveSlotActionFar
	.thumb_func
Menu_SelectSaveSlotActionFar:
	overlay_veneer Menu_SelectSaveSlotAction
	.global Func_080381f8
	.thumb_func
Func_080381f8:
	overlay_veneer Func_0804d7fc
