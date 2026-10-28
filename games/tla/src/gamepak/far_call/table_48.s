.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Sound_SetBgmVolumeTargetFar
	.thumb_func
Sound_SetBgmVolumeTargetFar:
	overlay_veneer Sound_SetBgmVolumeTarget
	.global Sound_GetBgmRestoreDelayFar
	.thumb_func
Sound_GetBgmRestoreDelayFar:
	overlay_veneer Sound_GetBgmRestoreDelay
	.global Func_081c0050
	.thumb_func
Func_081c0050:
	overlay_veneer Func_081c1014
	.global Sound_StopAllMusicPlayersFar
	.thumb_func
Sound_StopAllMusicPlayersFar:
	overlay_veneer Sound_StopAllMusicPlayers
	.global Sound_ResumeAllMusicPlayersFar
	.thumb_func
Sound_ResumeAllMusicPlayersFar:
	overlay_veneer Sound_ResumeAllMusicPlayers
	.global Sound_UpdateCommandMaskFar
	.thumb_func
Sound_UpdateCommandMaskFar:
	overlay_veneer Sound_UpdateCommandMask
