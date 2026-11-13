	.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall010
Resource_FarCall010:
	export m4aSoundMainFar, m4aSoundMain
	export Sound_InitializeFar, Sound_Initialize
	export Sound_PlayFar, Sound_Play
	export Sound_VSyncFar, Sound_VSync
	export Sound_SetBgmTempoFar, Sound_SetBgmTempo
	export Sound_SetBgmPitchFar, Sound_SetBgmPitch
	export Sound_SetBgmTempoPitchTargetFar, Sound_SetBgmTempoPitchTarget
	export Sound_SetBgmVolumeFar, Sound_SetBgmVolume
	export Sound_SetBgmVolumeTargetFar, Sound_SetBgmVolumeTarget
	export Sound_GetBgmRestoreDelayFar, Sound_GetBgmRestoreDelay
	export SoundTest_RunFar, SoundTest_Run
	export Sound_StopAllMusicPlayersFar, Sound_StopAllMusicPlayers
	export Sound_ResumeAllMusicPlayersFar, Sound_ResumeAllMusicPlayers
	export Sound_UpdateCommandMaskFar, Sound_UpdateCommandMask
	export Sound_GetCurrentBgmFar, Sound_GetCurrentBgm
	export Sound_WaitForBgmRestoreFar, Sound_WaitForBgmRestore
	export m4aSoundVSyncOffFar, Sound_SuspendDirectSound
	export Sound_UpdateBgmRampsFar, Sound_UpdateBgmRamps
	export SoundFlag_TestFar, SoundFlag_Test
	export SoundFlag_SetFar, SoundFlag_Set
	export SoundFlag_ClearFar, SoundFlag_Clear
	export Func_081c00a8, Func_081c0c18
