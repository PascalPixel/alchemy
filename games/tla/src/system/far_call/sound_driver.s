.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall010
Resource_FarCall010:
	.global m4aSoundMainFar
	.thumb_func
m4aSoundMainFar:
	overlay_veneer m4aSoundMain
	.global Sound_InitializeFar
	.thumb_func
Sound_InitializeFar:
	overlay_veneer Sound_Initialize
	.global Sound_PlayFar
	.thumb_func
Sound_PlayFar:
	overlay_veneer Sound_Play
	.global Func_081c0018
	.thumb_func
Func_081c0018:
	overlay_veneer Func_081c0f3c
	.global Func_081c0020
	.thumb_func
Func_081c0020:
	overlay_veneer Sound_SetBgmTempo
	.global Sound_SetBgmPitchFar
	.thumb_func
Sound_SetBgmPitchFar:
	overlay_veneer Sound_SetBgmPitch
	.global Sound_SetBgmTempoPitchTargetFar
	.thumb_func
Sound_SetBgmTempoPitchTargetFar:
	overlay_veneer Func_081c0f70
	.global Sound_SetBgmVolumeFar
	.thumb_func
Sound_SetBgmVolumeFar:
	overlay_veneer Sound_SetBgmVolume
