@ Audio mixing, playback commands and music-player updates.
	.syntax unified
	.thumb
	.include "games/tla/src/system/stub.s"
	.global StubsSound
StubsSound:
	stub m4aSoundMainStub, m4aSoundMain
	stub sySndInitStub, sySndInit
	stub sySndPlayStub, sySndPlay
	stub sySndVSyncStub, sySndVSync
	stub sySndSetBGMTempoStub, sySndSetBGMTempo
	stub sySndSetBGMPitchStub, sySndSetBGMPitch
	stub sySndSetBGMTempoPitchTargetStub, sySndSetBGMTempoPitchTarget
	stub sySndSetBGMVolumeStub, sySndSetBGMVolume
	stub sySndSetBGMVolumeTargetStub, sySndSetBGMVolumeTarget
	stub sySndGetBGMRestoreDelayStub, sySndGetBGMRestoreDelay
	stub sySndTestRunStub, sySndTestRun
	stub sySndStopAllMusicPlayersStub, sySndStopAllMusicPlayers
	stub sySndResumeAllMusicPlayersStub, sySndResumeAllMusicPlayers
	stub sySndUpdateCommandMaskStub, sySndUpdateCommandMask
	stub sySndGetCurrentBGMStub, sySndGetCurrentBGM
	stub sySndWaitForBGMRestoreStub, sySndWaitForBGMRestore
	stub sySndSuspendDirectSndStub, sySndSuspendDirectSnd
	stub sySndUpdateBGMRampsStub, sySndUpdateBGMRamps
	stub sySndFlagTestStub, sySndFlagTest
	stub sySndFlagSetStub, sySndFlagSet
	stub sySndFlagClearStub, sySndFlagClear
	stub sySndNoOperationStub, sySndNoOperation
