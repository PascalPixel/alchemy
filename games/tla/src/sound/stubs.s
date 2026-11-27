@ Audio mixing, playback commands and music-player updates.
	.syntax unified
	.thumb
	.include "games/tla/src/system/stub.s"
	.global Stubs_Sound
Stubs_Sound:
	stub m4aSoundMainStub, m4aSoundMain
	stub sySndInitStub, sySndInit
	stub sySndPlayStub, sySndPlay
	stub sySndVSyncStub, sySndVSync
	stub sySndSetBgmTempoStub, sySndSetBgmTempo
	stub sySndSetBgmPitchStub, sySndSetBgmPitch
	stub sySndSetBgmTempoPitchTargetStub, sySndSetBgmTempoPitchTarget
	stub sySndSetBgmVolumeStub, sySndSetBgmVolume
	stub sySndSetBgmVolumeTargetStub, sySndSetBgmVolumeTarget
	stub sySndGetBgmRestoreDelayStub, sySndGetBgmRestoreDelay
	stub sySndTestRunStub, sySndTestRun
	stub sySndStopAllMusicPlayersStub, sySndStopAllMusicPlayers
	stub sySndResumeAllMusicPlayersStub, sySndResumeAllMusicPlayers
	stub sySndUpdateCommandMaskStub, sySndUpdateCommandMask
	stub sySndGetCurrentBgmStub, sySndGetCurrentBgm
	stub sySndWaitForBgmRestoreStub, sySndWaitForBgmRestore
	stub sySndSuspendDirectSndStub, sySndSuspendDirectSnd
	stub sySndUpdateBgmRampsStub, sySndUpdateBgmRamps
	stub sySndFlagTestStub, sySndFlagTest
	stub sySndFlagSetStub, sySndFlagSet
	stub sySndFlagClearStub, sySndFlagClear
	stub sySndNoOpStub, sySndNoOp
