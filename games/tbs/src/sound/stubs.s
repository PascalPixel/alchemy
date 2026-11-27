@ Audio mixing, playback commands and music-player updates.
	.include "games/tbs/src/system/stub_table.inc"
	.global StubsSound
StubsSound:
	stub m4aSoundMainStub, m4aSoundMain
	stub sySndInitStub, sySndInit
	stub sySndPlayStub, sySndPlay
	stub sySndUpdateBGMRampsStub, sySndUpdateBGMRamps
	stub sySndSetBGMTempoStub, sySndSetBGMTempo
	unnamed_stub sySndSetBGMPitch
	unnamed_stub sySndSetBGMTempoPitchTarget
	unnamed_stub sySndSetBGMVolume
	unnamed_stub sySndSetBGMVolumeTarget
	stub sySndGetBGMRestoreDelayStub, sySndGetBGMRestoreDelay
	unnamed_stub sySndTestRun
	unnamed_stub sySndStopAllMusicPlayers
	unnamed_stub sySndResumeAllMusicPlayers
	unnamed_stub sySndUpdateCommandMask
	stub sySndGetCurrentBGMStub, sySndGetCurrentBGM
	stub sySndWaitForBGMRestoreStub, sySndWaitForBGMRestore
