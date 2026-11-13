.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
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
	.global Sound_VSyncFar
	.thumb_func
Sound_VSyncFar:
	overlay_veneer Sound_VSync
	.global Sound_SetBgmTempoFar
	.thumb_func
Sound_SetBgmTempoFar:
	overlay_veneer Sound_SetBgmTempo
	.global Sound_SetBgmPitchFar
	.thumb_func
Sound_SetBgmPitchFar:
	overlay_veneer Sound_SetBgmPitch
	.global Sound_SetBgmTempoPitchTargetFar
	.thumb_func
Sound_SetBgmTempoPitchTargetFar:
	overlay_veneer Sound_SetBgmTempoPitchTarget
	.global Sound_SetBgmVolumeFar
	.thumb_func
Sound_SetBgmVolumeFar:
	overlay_veneer Sound_SetBgmVolume
