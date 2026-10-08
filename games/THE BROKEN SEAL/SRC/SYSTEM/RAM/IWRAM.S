	.section .sym_iwram,"aw",%nobits
	.ifdef TBS_EDITION_DE
	.space 0x00000010
	.endif
	.global Data_03001400
Data_03001400:
	.space 0x00000400
	.global gFrameTick
gFrameTick:
	.space 0x00000004
	.global gSavedStackSize
gSavedStackSize:
	.space 0x0000000c
	.global ResourceBlockOwners
ResourceBlockOwners:
	.space 0x00000200
	.global gSchedulerStatus
gSchedulerStatus:
	.space 0x00000010
	.global gSchedulerTaskTable
gSchedulerTaskTable:
	.space 0x000000a0
	.global gBlendFramesLeft
gBlendFramesLeft:
	.space 0x00000004
	.global gDebugTextEnabled
gDebugTextEnabled:
	.space 0x0000000c
	.global gBgScroll
gBgScroll:
	.space 0x00000018
	.global Data_03001ae8
Data_03001ae8:
	.global gKeysHeld
gKeysHeld:
	.space 0x00000004
	.global gBlendStartLevel
gBlendStartLevel:
	.space 0x00000004
	.global gCpuLoadTimer
gCpuLoadTimer:
	.space 0x00000004
	.global gScanlinePitch
gScanlinePitch:
	.space 0x00000004
	.global gKeysPressedLatch
gKeysPressedLatch:
	.space 0x00000004
	.global gKeysRepeatDirected
gKeysRepeatDirected:
	.space 0x00000004
	.global gKeyRepeatTimer
gKeyRepeatTimer:
	.space 0x00000004
	.global gKeysRepeat
gKeysRepeat:
	.space 0x0000000c
	.global gVramBlockCache
gVramBlockCache:
	.space 0x00000180
	.global gDecodeFillByte
gDecodeFillByte:
	.space 0x00000004
	.global Data_03001c94
Data_03001c94:
	.global gKeyState
gKeyState:
	.space 0x00000004
	.global gBlendDuration
gBlendDuration:
	.space 0x00000004
	.global gLoadedStateWord
gLoadedStateWord:
	.space 0x00000004
	.global gSleepDisabled
gSleepDisabled:
	.space 0x00000004
	.global gCpuLoadPeak
gCpuLoadPeak:
	.space 0x00000004
	.global gBlendTargetLevel
gBlendTargetLevel:
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
	.space 0x00000004
	.global Data_03001cc0
Data_03001cc0:
	.space 0x00000004
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
	.space 0x0000000c
	.global gProjection
gProjection:
	.space 0x00000014
	.global gKeysHeldLast
gKeysHeldLast:
	.space 0x00000004
	.global gBlendLayers
gBlendLayers:
	.space 0x00000004
	.global Data_03001cfc
Data_03001cfc:
	.space 0x00000004
	.global gObjAffineCount
gObjAffineCount:
	.space 0x00000004
	.global gKeysDirection
gKeysDirection:
	.ifdef TBS_EDITION_DE
	.space 0x0000000c
	.else
	.space 0x00000004
	.endif
	.global gOptionMirror
gOptionMirror:
	.space 0x00000004
	.global gKeysHeldAtVBlank
gKeysHeldAtVBlank:
	.space 0x0000000c
	.global gOamCopyEnabled
gOamCopyEnabled:
	.space 0x00000004
	.global Data_03001d1c
Data_03001d1c:
	.space 0x00000004
	.global gDebugPaused
gDebugPaused:
	.space 0x00000004
	.global gPostLoadCounter
gPostLoadCounter:
	.space 0x00000004
	.global gVBlankDone
gVBlankDone:
	.space 0x00000004
	.global gTransformStackTop
gTransformStackTop:
	.space 0x00000008
	.global gSchedulerTaskCount
gSchedulerTaskCount:
	.ifdef TBS_EDITION_DE
	.space 0x00000004
	.else
	.space 0x0000000c
	.endif
	.global gObjAffineMatrices
gObjAffineMatrices:
	.space 0x00000100
	.global Data_03001e40
Data_03001e40:
	.global gFrameCount
gFrameCount:
	.space 0x00000004
	.global gFrameRenderPending
gFrameRenderPending:
	.space 0x0000000c
	.global Data_03001e50
Data_03001e50:
	.global gWorkSlot
	.type gWorkSlot, %object
	.size gWorkSlot, 0x100
gWorkSlot:
	.space 0x0000000c
	.global gAnimationObjects
gAnimationObjects:
	.space 0x00000004
	.global Data_03001e60
Data_03001e60:
	.global gSpriteObjects
gSpriteObjects:
	.space 0x00000004
	.global gObjectSlots
gObjectSlots:
	.space 0x00000004
	.global gMenuCtrlWork
gMenuCtrlWork:
	.space 0x00000004
	.global gMapAnimationPages
gMapAnimationPages:
	.space 0x00000004
	.global gMapWork
gMapWork:
	.space 0x00000004
	.global Data_03001e74
Data_03001e74:
	.global Data_03001e74_a
Data_03001e74_a:
	.global BtlWk
BtlWk:
	.space 0x0000000c
	.global gCameraWork
gCameraWork:
	.space 0x0000000c
	.global Data_03001e8c
Data_03001e8c:
	.global gWindowWork
gWindowWork:
	.space 0x00000004
	.global Data_03001e90
Data_03001e90:
	.space 0x00000004
	.global gGlyphWork
gGlyphWork:
	.space 0x00000004
	.global Data_03001e98
Data_03001e98:
	.global gResQueueWork
gResQueueWork:
	.space 0x00000004
	.global Data_03001e9c
Data_03001e9c:
	.space 0x00000004
	.global gSelectionWork
gSelectionWork:
	.space 0x00000008
	.global gBattleBgFxWork
gBattleBgFxWork:
	.space 0x00000014
	.global Data_03001ebc
Data_03001ebc:
	.global gEventWork
gEventWork:
	.global gWork
gWork:
	.space 0x00000004
	.global gPaletteWork
gPaletteWork:
	.space 0x00000004
	.global gParticleWork
gParticleWork:
	.space 0x00000004
	.global Data_03001ec8
Data_03001ec8:
	.space 0x00000004
	.global gDisplayTransition
gDisplayTransition:
	.space 0x00000004
	.global Data_03001ed0
Data_03001ed0:
	.space 0x00000008
	.global gHBlankScrollWork
gHBlankScrollWork:
	.space 0x00000008
	.global gActorEffectWork
gActorEffectWork:
	.space 0x00000004
	.global Data_03001ee4
Data_03001ee4:
	.global gBattleDisplayWork
gBattleDisplayWork:
	.space 0x00000008
	.global gBattleFxWork
gBattleFxWork:
	.space 0x0000000c
	.global gDisp
gDisp:
	.space 0x00000008
	.global gTransitionWork
gTransitionWork:
	.space 0x0000001c
	.global Data_03001f1c
Data_03001f1c:
	.global gSaveWorkspace
gSaveWorkspace:
	.space 0x0000000c
	.global gBattleOwnerStates
gBattleOwnerStates:
	.space 0x00000004
	.global Data_03001f2c
Data_03001f2c:
	.global gMenuWork
gMenuWork:
	.space 0x00000004
	.global Data_03001f30
Data_03001f30:
	.global gEffectWork
gEffectWork:
	.space 0x00000004
	.global gLinkCountdownWork
gLinkCountdownWork:
	.space 0x00000004
	.global Data_03001f38
Data_03001f38:
	.global gMenuSelectWork
gMenuSelectWork:
	.space 0x00000004
	.global gKorosseoWork
gKorosseoWork:
	.space 0x00000018
	.global gDebugMode
gDebugMode:
	.space 0x00000004
	.global gCpuLoadDisplayEnabled
gCpuLoadDisplayEnabled:
	.space 0x00000004
	.global gSleepComboFrames
gSleepComboFrames:
	.space 0x00000004
	.global gScanlineAngle
gScanlineAngle:
	.space 0x00000004
	.global gLinkStatus
gLinkStatus:
	.space 0x0000000c
	.global gNumberTextBuffer
gNumberTextBuffer:
	.space 0x00000008
	.global gNumberTextHexEnd
gNumberTextHexEnd:
	.space 0x00000002
	.global gNumberTextDecimalEnd
gNumberTextDecimalEnd:
	.ifdef TBS_EDITION_DE
	.space 0x00000076
	.else
	.space 0x00000086
	.endif
	.global gIwramHeap
gIwramHeap:
