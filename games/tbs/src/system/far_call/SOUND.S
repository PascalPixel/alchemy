@ Audio mixing, playback commands and music-player updates.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_SoundTable
FarCall_SoundTable:
	export m4aSoundMainFar, m4aSoundMain
	export Sound_InitializeFar, Sound_Initialize
	export Sound_PlaySongFar, Sound_PlaySong
	export Sound_UpdateBgmRampsFar, Sound_UpdateBgmRamps
	export Sound_SetBgmTempoFar, Sound_SetBgmTempo
	overlay_veneer Sound_SetBgmPitch
	overlay_veneer Sound_SetBgmTempoPitchTarget
	overlay_veneer Sound_SetBgmVolume
	overlay_veneer Sound_SetBgmVolumeTarget
	export Sound_GetBgmRestoreDelayFar, Sound_GetBgmRestoreDelay
	overlay_veneer SoundTest_Run
	overlay_veneer Sound_StopAllMusicPlayers
	overlay_veneer Sound_ResumeAllMusicPlayers
	overlay_veneer Sound_UpdateCommandMask
	export Sound_GetCurrentBgmFar, Sound_GetCurrentBgm
	export Sound_WaitForSongFar, Sound_WaitForSong
