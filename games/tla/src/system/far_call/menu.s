.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Localization_LookupEntryIdFar
	.thumb_func
Localization_LookupEntryIdFar:
	overlay_veneer UiText_GetSpeakerPortrait
	.global Menu_RunTopSelectionFar
	.thumb_func
Menu_RunTopSelectionFar:
	overlay_veneer Menu_RunTopSelection
	.global Menu_SelectSaveSlotActionFar
	.thumb_func
Menu_SelectSaveSlotActionFar:
	overlay_veneer Menu_SelectSaveSlotAction
	.global Func_080381f8
	.thumb_func
Func_080381f8:
	overlay_veneer Func_0804d7fc
