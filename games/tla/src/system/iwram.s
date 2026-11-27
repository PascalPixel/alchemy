	.section .sym_iwram_slots,"aw",%nobits
	.global HeapSlotsWork
HeapSlotsWork:
	.space 0x00000070
	.section .sym_iwram,"aw",%nobits
	.space 0x00000060
	.global VBlankCount
VBlankCount:
	.space 0x00000008
	.global EventsRunning
EventsRunning:
	.space 0x00000004
	.global ScreenFadeVBlanksLeft
ScreenFadeVBlanksLeft:
	.space 0x00000004
	.global DebugTextEnabled
DebugTextEnabled:
	.space 0x00000010
	.global BgScrollWork
BgScrollWork:
	.space 0x00000018
	.global SoundVBlankStateWork
SoundVBlankStateWork:
	.space 0x00000004
	.global ScreenFadeStartLevel
ScreenFadeStartLevel:
	.space 0x00000004
	.global CpuLoadTimer
CpuLoadTimer:
	.space 0x00000004
	.global ScanlinePitch
ScanlinePitch:
	.space 0x0000000c
	.global PerWork
PerWork:
	.space 0x00000024
	.global DecodeFillByte
DecodeFillByte:
	.space 0x00000004
	.global ScreenFadeVBlanks
ScreenFadeVBlanks:
	.space 0x00000004
	.global PlayTimeFrames
PlayTimeFrames:
	.space 0x00000004
	.global SleepDisabled
SleepDisabled:
	.space 0x00000004
	.global CpuLoadPeak
CpuLoadPeak:
	.space 0x0000000c
	.global OamDepthMasks
OamDepthMasks:
	.space 0x00000020
	.global ScreenFadeTargetLevel
ScreenFadeTargetLevel:
	.space 0x00000008
	.global SerialExchangeActive
SerialExchangeActive:
	.space 0x00000004
	.global RandomSeed
RandomSeed:
	.space 0x00000004
	.global ResetRequested
ResetRequested:
	.space 0x00000004
	.global DebugTextCursor
DebugTextCursor:
	.space 0x00000008
	.global MatrixStackDepth
MatrixStackDepth:
	.space 0x00000004
	.global SleepRequested
SleepRequested:
	.space 0x00000004
	.global FrameVBlanks
FrameVBlanks:
	.space 0x00000004
	.global LastFrameVBlanks
LastFrameVBlanks:
	.space 0x00000004
	.global ScreenFadeToWhite
ScreenFadeToWhite:
	.space 0x00000004
	.global ProjectionWork
ProjectionWork:
	.space 0x0000000c
	.global Data_030011ec
Data_030011ec:
	.space 0x00000004
	.global Data_030011f0
Data_030011f0:
	.space 0x00000004
	.global ScreenFadeLayers
ScreenFadeLayers:
	.space 0x00000004
	.global VBlankCallback
VBlankCallback:
	.space 0x00000004
	.global OamMatrixCount
OamMatrixCount:
	.space 0x00000004
	.global IdleSleepEnabled
IdleSleepEnabled:
	.space 0x0000000c
	.global OamCopyEnabled
OamCopyEnabled:
	.space 0x00000008
	.global DebugPaused
DebugPaused:
	.space 0x00000004
	.global IdleFrames
IdleFrames:
	.space 0x00000004
	.global VBlankDone
VBlankDone:
	.space 0x00000004
	.global MatrixStackTop
MatrixStackTop:
	.space 0x00000008
	.global EventsInitialized
EventsInitialized:
	.space 0x00000004
	.global FrameCount
FrameCount:
	.space 0x00000004
	.global FrameRenderPending
FrameRenderPending:
	.space 0x00000008
	.global DebugMode
DebugMode:
	.space 0x00000004
	.global CpuLoadDisplayEnabled
CpuLoadDisplayEnabled:
	.space 0x00000008
	.global ScanlineAngle
ScanlineAngle:
	.space 0x00000004
	.global OamUsageWork
OamUsageWork:
	.space 0x00000004
	.global LinkStatusWork
LinkStatusWork:
	.space 0x00000004
	.global NumberTextBuffer
NumberTextBuffer:
	.space 0x00000008
	.global NumberTextHexEnd
NumberTextHexEnd:
	.space 0x00000002
	.global NumberTextDecimalEnd
NumberTextDecimalEnd:
	.space 0x000000a6
	.global IwramHeap
IwramHeap:
