.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Audio_GetBackgroundCueFar
	.thumb_func
Audio_GetBackgroundCueFar:
	overlay_veneer Audio_GetBackgroundCue
	.global Sound_WaitForBgmRestoreFar
	.thumb_func
Sound_WaitForBgmRestoreFar:
	overlay_veneer Sound_WaitForBgmRestore
