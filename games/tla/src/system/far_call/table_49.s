.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Sound_GetCurrentBgmFar
	.thumb_func
Sound_GetCurrentBgmFar:
	overlay_veneer Sound_GetCurrentBgm
	.global Sound_WaitForBgmRestoreFar
	.thumb_func
Sound_WaitForBgmRestoreFar:
	overlay_veneer Sound_WaitForBgmRestore
