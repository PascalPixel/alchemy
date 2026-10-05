.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08118058
	.thumb_func
Func_08118058:
	overlay_veneer Summon_IsEntrySecondaryFlagged
	.global Func_08118060
	.thumb_func
Func_08118060:
	overlay_veneer Battle_GetSpriteFlagHighBits
	.global Func_08118068
	.thumb_func
Func_08118068:
	overlay_veneer Summon_IsEntryFlagged
