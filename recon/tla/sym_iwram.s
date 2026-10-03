@ IWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
@ The heap's slot words open IWRAM, below the resident modules.
	.section .sym_iwram_slots,"aw",%nobits
	.global gHeapSlots
gHeapSlots:
	.space 0x00000070
	.section .sym_iwram,"aw",%nobits
	.space 0x00000060
	.global gFrameTick
gFrameTick:
	.space 0x00000008
	.global gSchedulerStatus
gSchedulerStatus:
	.space 0x00000004
	.global gBlendFramesLeft
gBlendFramesLeft:
	.space 0x00000004
	.global Data_03001110
Data_03001110:
	.space 0x00000010
	.global Data_03001120
Data_03001120:
	.space 0x00000018
	.global Data_03001138
Data_03001138:
	.space 0x00000004
	.global gBlendStartLevel
gBlendStartLevel:
	.space 0x00000004
	.global gCpuLoadTimer
gCpuLoadTimer:
	.space 0x00000004
	.global Data_03001144
Data_03001144:
	.space 0x0000000c
	.global gInput
gInput:
	.space 0x00000024
	.global gDecodeFillByte
gDecodeFillByte:
	.space 0x00000004
	.global gBlendDuration
gBlendDuration:
	.space 0x00000004
	.global gFrameWaitCount
gFrameWaitCount:
	.space 0x00000004
	.global gSleepDisabled
gSleepDisabled:
	.space 0x00000004
	.global gCpuLoadPeak
gCpuLoadPeak:
	.space 0x0000000c
	.global gOamBucketMasks
gOamBucketMasks:
	.space 0x00000020
	.global gBlendTargetLevel
gBlendTargetLevel:
	.space 0x00000008
	.global gSerialExchangeActive
gSerialExchangeActive:
	.space 0x00000004
	.global gRandomState
gRandomState:
	.space 0x00000004
	.global gResetRequested
gResetRequested:
	.space 0x00000004
	.global Data_030011c4
Data_030011c4:
	.space 0x00000008
	.global gTransformStackDepth
gTransformStackDepth:
	.space 0x00000004
	.global gSleepRequested
gSleepRequested:
	.space 0x00000004
	.global gLagFrameCount
gLagFrameCount:
	.space 0x00000004
	.global gLagFramesShown
gLagFramesShown:
	.space 0x00000004
	.global gBlendBrighten
gBlendBrighten:
	.space 0x00000004
	.global gCameraSceneParameters
gCameraSceneParameters:
	.space 0x0000000c
	.global Data_030011ec
Data_030011ec:
	.space 0x00000004
	.global Data_030011f0
Data_030011f0:
	.space 0x00000004
	.global gBlendLayers
gBlendLayers:
	.space 0x00000004
	.global Data_030011f8
Data_030011f8:
	.space 0x00000004
	.global gObjAffineCount
gObjAffineCount:
	.space 0x00000004
	.global gAutoSleepEnabled
gAutoSleepEnabled:
	.space 0x0000000c
	.global gRenderOamEnabled
gRenderOamEnabled:
	.space 0x00000008
	.global gDebugPaused
gDebugPaused:
	.space 0x00000004
	.global gIdleFrameCount
gIdleFrameCount:
	.space 0x00000004
	.global Data_0300121c
Data_0300121c:
	.space 0x00000004
	.global gTransformStackTop
gTransformStackTop:
	.space 0x00000008
	.global gSchedulerTaskCount
gSchedulerTaskCount:
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
	.global Data_03001244
Data_03001244:
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
	.global Data_03001258
Data_03001258:
	.space 0x00000002
	.global Data_0300125a
Data_0300125a:
	.space 0x000000a6
	.global Data_03001300
Data_03001300:
