	.section .sym_iwram_slots,"aw",%nobits
	.global gHeapSlots
gHeapSlots:
	.space 0x00000070
	.section .sym_iwram,"aw",%nobits
	.space 0x00000060
	.global gVBlankCount
gVBlankCount:
	.space 0x00000008
	.global gTasksRunning
gTasksRunning:
	.space 0x00000004
	.global gScreenFadeVBlanksLeft
gScreenFadeVBlanksLeft:
	.space 0x00000004
	.global gDebugTextEnabled
gDebugTextEnabled:
	.space 0x00000010
	.global gBgScroll
gBgScroll:
	.space 0x00000018
	.global gSoundVBlankState
gSoundVBlankState:
	.space 0x00000004
	.global gScreenFadeStartLevel
gScreenFadeStartLevel:
	.space 0x00000004
	.global gCpuLoadTimer
gCpuLoadTimer:
	.space 0x00000004
	.global gScanlinePitch
gScanlinePitch:
	.space 0x0000000c
	.global gInput
gInput:
	.space 0x00000024
	.global gDecodeFillByte
gDecodeFillByte:
	.space 0x00000004
	.global gScreenFadeVBlanks
gScreenFadeVBlanks:
	.space 0x00000004
	.global gPlayTimeFrames
gPlayTimeFrames:
	.space 0x00000004
	.global gSleepDisabled
gSleepDisabled:
	.space 0x00000004
	.global gCpuLoadPeak
gCpuLoadPeak:
	.space 0x0000000c
	.global gOamDepthMasks
gOamDepthMasks:
	.space 0x00000020
	.global gScreenFadeTargetLevel
gScreenFadeTargetLevel:
	.space 0x00000008
	.global gSerialExchangeActive
gSerialExchangeActive:
	.space 0x00000004
	.global gRandomSeed
gRandomSeed:
	.space 0x00000004
	.global gResetRequested
gResetRequested:
	.space 0x00000004
	.global gDebugTextCursor
gDebugTextCursor:
	.space 0x00000008
	.global gMatrixStackDepth
gMatrixStackDepth:
	.space 0x00000004
	.global gSleepRequested
gSleepRequested:
	.space 0x00000004
	.global gFrameVBlanks
gFrameVBlanks:
	.space 0x00000004
	.global gLastFrameVBlanks
gLastFrameVBlanks:
	.space 0x00000004
	.global gScreenFadeToWhite
gScreenFadeToWhite:
	.space 0x00000004
	.global gProjection
gProjection:
	.space 0x0000000c
	.global Data_030011ec
Data_030011ec:
	.space 0x00000004
	.global Data_030011f0
Data_030011f0:
	.space 0x00000004
	.global gScreenFadeLayers
gScreenFadeLayers:
	.space 0x00000004
	.global gVBlankCallback
gVBlankCallback:
	.space 0x00000004
	.global gOamMatrixCount
gOamMatrixCount:
	.space 0x00000004
	.global gIdleSleepEnabled
gIdleSleepEnabled:
	.space 0x0000000c
	.global gOamCopyEnabled
gOamCopyEnabled:
	.space 0x00000008
	.global gDebugPaused
gDebugPaused:
	.space 0x00000004
	.global gIdleFrames
gIdleFrames:
	.space 0x00000004
	.global gVBlankDone
gVBlankDone:
	.space 0x00000004
	.global gMatrixStackTop
gMatrixStackTop:
	.space 0x00000008
	.global gTasksInitialized
gTasksInitialized:
	.space 0x00000004
	.global gFrameCount
gFrameCount:
	.space 0x00000004
	.global gFrameRenderPending
gFrameRenderPending:
	.space 0x00000008
	.global gDebugMode
gDebugMode:
	.space 0x00000004
	.global gCpuLoadDisplayEnabled
gCpuLoadDisplayEnabled:
	.space 0x00000008
	.global gScanlineAngle
gScanlineAngle:
	.space 0x00000004
	.global gOamUsage
gOamUsage:
	.space 0x00000004
	.global gLinkStatus
gLinkStatus:
	.space 0x00000004
	.global gNumberTextBuffer
gNumberTextBuffer:
	.space 0x00000008
	.global gNumberTextHexEnd
gNumberTextHexEnd:
	.space 0x00000002
	.global gNumberTextDecimalEnd
gNumberTextDecimalEnd:
	.space 0x000000a6
	.global gIwramHeap
gIwramHeap:
