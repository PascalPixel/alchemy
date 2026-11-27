@ Audio mixing, playback commands and music-player updates.
	.include "games/tbs/src/system/stub_table.inc"
	.global Stubs_Sound
Stubs_Sound:
	stub m4aSoundMainStub, m4aSoundMain
	stub sySndInitStub, sySndInit
	stub sySndPlayStub, sySndPlay
	stub sySndUpdateBgmRampsStub, sySndUpdateBgmRamps
	stub sySndSetBgmTempoStub, sySndSetBgmTempo
	unnamed_stub sySndSetBgmPitch
	unnamed_stub sySndSetBgmTempoPitchTarget
	unnamed_stub sySndSetBgmVolume
	unnamed_stub sySndSetBgmVolumeTarget
	stub sySndGetBgmRestoreDelayStub, sySndGetBgmRestoreDelay
	unnamed_stub sySndTestRun
	unnamed_stub sySndStopAllMusicPlayers
	unnamed_stub sySndResumeAllMusicPlayers
	unnamed_stub sySndUpdateCommandMask
	stub sySndGetCurrentBgmStub, sySndGetCurrentBgm
	stub sySndWaitForBgmRestoreStub, sySndWaitForBgmRestore
