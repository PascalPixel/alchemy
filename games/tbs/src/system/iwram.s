	.section .sym_iwram,"aw",%nobits
	.ifdef TBS_EDITION_DE
	.space 0x00000010
	.endif
	.global gRenderSlotHeads
gRenderSlotHeads:
	.space 0x00000400
	.global gFrameTick
gFrameTick:
	.space 0x00000004
	.global gSavedStackSize
gSavedStackSize:
	.space 0x0000000c
	.global gResourceBlockOwners
gResourceBlockOwners:
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
	.global gKeysPressed
gKeysPressed:
	.space 0x00000004
	.global gBlendDuration
gBlendDuration:
	.space 0x00000004
	.global gPlayTimeFrames
gPlayTimeFrames:
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
	.global gIwram_Data1
gIwram_Data1:
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
	.global gVBlankHook
gVBlankHook:
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
	.global gIdleSleepEnabled
gIdleSleepEnabled:
	.space 0x00000004
	.global gKeysHeldAtVBlank
gKeysHeldAtVBlank:
	.space 0x0000000c
	.global gOamCopyEnabled
gOamCopyEnabled:
	.space 0x00000004
	.global gIwram_Data2
gIwram_Data2:
	.space 0x00000004
	.global gDebugPaused
gDebugPaused:
	.space 0x00000004
	.global gIdleFrames
gIdleFrames:
	.space 0x00000004
	.global gVBlankDone
gVBlankDone:
	.space 0x00000004
	.global gTransformStackTop
gTransformStackTop:
	.space 0x00000008
	.global gSchedulerReady
gSchedulerReady:
	.ifdef TBS_EDITION_DE
	.space 0x00000004
	.else
	.space 0x0000000c
	.endif
	.global gObjAffineMatrices
gObjAffineMatrices:
	.space 0x00000100
	.global gFrameCount
gFrameCount:
	.global gFrameCount
gFrameCount:
	.space 0x00000004
	.global gFrameRenderPending
gFrameRenderPending:
	.space 0x0000000c
	.global gWorkSlot
gWorkSlot:
	.global gWorkSlot
	.type gWorkSlot, %object
	.size gWorkSlot, 0x100
gWorkSlot:
	.space 0x0000000c
	.global gAnimationObjects
gAnimationObjects:
	.space 0x00000004
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
	.global gBattleWork
gBattleWork:
	.space 0x0000000c
	.global gCameraWork
gCameraWork:
	.space 0x0000000c
	.global gWindowWork
gWindowWork:
	.global gWindowWork
gWindowWork:
	.space 0x00000004
	.global gPartyStatusWork
gPartyStatusWork:
	.space 0x00000004
	.global gGlyphWork
gGlyphWork:
	.space 0x00000004
	.global gResQueueWork
gResQueueWork:
	.space 0x00000004
	.global gSelectionWorkspace
gSelectionWorkspace:
	.space 0x00000004
	.global gSelectionWork
gSelectionWork:
	.space 0x00000008
	.global gBattleBgFxWork
gBattleBgFxWork:
	.space 0x00000014
	.global gEventWork
gEventWork:
	.space 0x00000004
	.global gPaletteWork
gPaletteWork:
	.space 0x00000004
	.global gParticleWork
gParticleWork:
	.space 0x00000004
	.global gFieldBlendWork
gFieldBlendWork:
	.space 0x00000004
	.global gDisplayTransition
gDisplayTransition:
	.space 0x00000004
	.global gPaletteBufferWork
gPaletteBufferWork:
	.space 0x00000008
	.global gHBlankScrollWork
gHBlankScrollWork:
	.space 0x00000008
	.global gActorEffectWork
gActorEffectWork:
	.space 0x00000004
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
	.global gSaveWorkspace
gSaveWorkspace:
	.space 0x0000000c
	.global gBattleOwnerStates
gBattleOwnerStates:
	.space 0x00000004
	.global gMenuWork
gMenuWork:
	.space 0x00000004
	.global gEffectWork
gEffectWork:
	.global gEffectWork
gEffectWork:
	.space 0x00000004
	.global gLinkCountdownWork
gLinkCountdownWork:
	.space 0x00000004
	.global gMenuSelectWork
gMenuSelectWork:
	.space 0x00000004
	.global gColossoWork
gColossoWork:
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
