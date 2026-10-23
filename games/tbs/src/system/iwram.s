	.section .sym_iwram,"aw",%nobits
	.ifdef TBS_EDITION_DE
	.space 0x00000010
	.endif
	.global gOamDepthLists
gOamDepthLists:
	.space 0x00000400
	.global gFrameTick
gFrameTick:
	.space 0x00000004
	.global gSavedStackSize
gSavedStackSize:
	.space 0x0000000c
	.global gVramCacheBlockOwners
gVramCacheBlockOwners:
	.space 0x00000200
	.global gTaskStatus
gTaskStatus:
	.space 0x00000010
	.global gTasks
gTasks:
	.space 0x000000a0
	.global gScreenFadeFramesLeft
gScreenFadeFramesLeft:
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
	.global gScreenFadeStartLevel
gScreenFadeStartLevel:
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
	.global gVramCacheEntries
gVramCacheEntries:
	.space 0x00000180
	.global gDecodeFillByte
gDecodeFillByte:
	.space 0x00000004
	.global gKeysPressed
gKeysPressed:
	.space 0x00000004
	.global gScreenFadeFrames
gScreenFadeFrames:
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
	.global gScreenFadeToWhite
gScreenFadeToWhite:
	.space 0x0000000c
	.global gProjection
gProjection:
	.space 0x00000014
	.global gKeysHeldLast
gKeysHeldLast:
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
	.global gTasksReady
gTasksReady:
	.ifdef TBS_EDITION_DE
	.space 0x00000004
	.else
	.space 0x0000000c
	.endif
	.global gOamMatrices
gOamMatrices:
	.space 0x00000100
	.global gFrameCount
gFrameCount:
	.global gFrameCount
gFrameCount:
	.space 0x00000004
	.global gFrameRenderPending
gFrameRenderPending:
	.space 0x0000000c
	.global gHeapSlots
gHeapSlots:
	.global gHeapSlots
	.type gHeapSlots, %object
	.size gHeapSlots, 0x100
gHeapSlots:
	.space 0x0000000c
	.global gSpriteAnims
gSpriteAnims:
	.space 0x00000004
	.global gSprites
gSprites:
	.space 0x00000004
	.global gObjects
gObjects:
	.space 0x00000004
	.global gObjectSystemWork
gObjectSystemWork:
	.space 0x00000004
	.global gTileAnimationPages
gTileAnimationPages:
	.space 0x00000004
	.global gMapGridWork
gMapGridWork:
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
	.global gSelectionScreenWork
gSelectionScreenWork:
	.space 0x00000004
	.global gSelectionWorkspace
gSelectionWorkspace:
	.space 0x00000004
	.global gOptionMenuWork
gOptionMenuWork:
	.space 0x00000008
	.global gBattleBgEffectWork
gBattleBgEffectWork:
	.space 0x00000014
	.global gFieldWork
gFieldWork:
	.space 0x00000004
	.global gPaletteCycleWork
gPaletteCycleWork:
	.space 0x00000004
	.global gParticleWork
gParticleWork:
	.space 0x00000004
	.global gLightningWork
gLightningWork:
	.space 0x00000004
	.global gScreenTransitionWork
gScreenTransitionWork:
	.space 0x00000004
	.global gPaletteFadeWork
gPaletteFadeWork:
	.space 0x00000008
	.global gScanlineEffect
gScanlineEffect:
	.space 0x00000008
	.global gObjectEventEffectWork
gObjectEventEffectWork:
	.space 0x00000004
	.global gBattleMessageWork
gBattleMessageWork:
	.space 0x00000008
	.global gBattleAnimWork
gBattleAnimWork:
	.space 0x0000000c
	.global gBattleEntryTimer
gBattleEntryTimer:
	.space 0x00000008
	.global gBattleViewWork
gBattleViewWork:
	.space 0x0000001c
	.global gSaveWork
gSaveWork:
	.space 0x0000000c
	.global gEnemyUnits
gEnemyUnits:
	.space 0x00000004
	.global gMenuWork
gMenuWork:
	.space 0x00000004
	.global gFieldEffectWork
gFieldEffectWork:
	.global gFieldEffectWork
gFieldEffectWork:
	.space 0x00000004
	.global gBattleMenuWork
gBattleMenuWork:
	.space 0x00000004
	.global gIconMenuWork
gIconMenuWork:
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
	.global gNumberBuffer
gNumberBuffer:
	.space 0x00000008
	.global gNumberBufferHexEnd
gNumberBufferHexEnd:
	.space 0x00000002
	.global gNumberBufferDecimalEnd
gNumberBufferDecimalEnd:
	.ifdef TBS_EDITION_DE
	.space 0x00000076
	.else
	.space 0x00000086
	.endif
	.global gIwramHeap
gIwramHeap:
