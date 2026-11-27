@ Audio mixing, playback commands and music-player updates.
	.syntax unified
	.thumb
	.include "games/tla/src/system/stub.s"
	.global StubsSound
StubsSound:
	stub m4aSoundMainStub, m4aSoundMain
	stub sySoundInitStub, sySoundInit
	stub sySoundPlayStub, sySoundPlay
	stub sySoundVSyncStub, sySoundVSync
	stub sySoundSetBGMTempoStub, sySoundSetBGMTempo
	stub sySoundSetBGMPitchStub, sySoundSetBGMPitch
	stub sySoundSetBGMTempoPitchTargetStub, sySoundSetBGMTempoPitchTarget
	stub sySoundSetBGMVolumeStub, sySoundSetBGMVolume
	stub sySoundSetBGMVolumeTargetStub, sySoundSetBGMVolumeTarget
	stub sySoundGetBGMRestoreDelayStub, sySoundGetBGMRestoreDelay
	stub sySoundTestRunStub, sySoundTestRun
	stub sySoundStopAllMusicPlayersStub, sySoundStopAllMusicPlayers
	stub sySoundResumeAllMusicPlayersStub, sySoundResumeAllMusicPlayers
	stub sySoundUpdateCommandMaskStub, sySoundUpdateCommandMask
	stub sySoundGetCurrentBGMStub, sySoundGetCurrentBGM
	stub sySoundWaitForBGMRestoreStub, sySoundWaitForBGMRestore
	stub sySoundSuspendDirectSoundStub, sySoundSuspendDirectSound
	stub sySoundUpdateBGMRampsStub, sySoundUpdateBGMRamps
	stub sySoundFlagTestStub, sySoundFlagTest
	stub sySoundFlagSetStub, sySoundFlagSet
	stub sySoundFlagClearStub, sySoundFlagClear
	stub sySoundNoOperationStub, sySoundNoOperation
