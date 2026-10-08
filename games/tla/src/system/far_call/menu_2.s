.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Menu_SelectEntry11To14Far
	.thumb_func
Menu_SelectEntry11To14Far:
	overlay_veneer Menu_SelectEntry11To14
	.global Menu_SelectEntry19To1cFar
	.thumb_func
Menu_SelectEntry19To1cFar:
	overlay_veneer Menu_SelectEntry19To1c
	.global Menu_RunConfirmSelectionFar
	.thumb_func
Menu_RunConfirmSelectionFar:
	overlay_veneer Menu_RunConfirmSelection
	.global Menu_SelectEntry20To21Far
	.thumb_func
Menu_SelectEntry20To21Far:
	overlay_veneer Menu_SelectEntry20To21
	.global Func_08038388
	.thumb_func
Func_08038388:
	overlay_veneer Func_08044a54
