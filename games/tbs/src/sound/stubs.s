@ Audio mixing, playback commands and music-player updates.
	.include "games/tbs/src/system/stub_table.inc"
	.global StubsSound
StubsSound:
	stub m4aSoundMainStub, m4aSoundMain
	stub sySoundInitStub, sySoundInit
	stub sySoundPlayStub, sySoundPlay
	stub sySoundUpdateBGMRampsStub, sySoundUpdateBGMRamps
	stub sySoundSetBGMTempoStub, sySoundSetBGMTempo
	unnamed_stub sySoundSetBGMPitch
	unnamed_stub sySoundSetBGMTempoPitchTarget
	unnamed_stub sySoundSetBGMVolume
	unnamed_stub sySoundSetBGMVolumeTarget
	stub sySoundGetBGMRestoreDelayStub, sySoundGetBGMRestoreDelay
	unnamed_stub sySoundTestRun
	unnamed_stub sySoundStopAllMusicPlayers
	unnamed_stub sySoundResumeAllMusicPlayers
	unnamed_stub sySoundUpdateCommandMask
	stub sySoundGetCurrentBGMStub, sySoundGetCurrentBGM
	stub sySoundWaitForBGMRestoreStub, sySoundWaitForBGMRestore
