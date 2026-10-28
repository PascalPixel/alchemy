	.section .sym_iwram,"aw",%nobits
	.ifdef TBS_EDITION_DE
	.space 0x00000010
	.endif
	.global gOamDepthLists
gOamDepthLists:
	.space 0x00000400
	.global gVBlankCount
gVBlankCount:
	.space 0x00000004
	.global gSavedStackSize
gSavedStackSize:
	.space 0x0000000c
	.global gVramCacheBlockOwners
gVramCacheBlockOwners:
	.space 0x00000200
	.global gTasksRunning
gTasksRunning:
	.space 0x00000010
	.global gTasks
gTasks:
	.space 0x000000a0
	.global gScreenFadeVBlanksLeft
gScreenFadeVBlanksLeft:
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
	.global gCamera_UnreadWord1
gCamera_UnreadWord1:
	.space 0x00000004
	.global gTransformStackDepth
gTransformStackDepth:
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
	.global gCamera_UnreadWord2
gCamera_UnreadWord2:
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
	.global gTasksInitialized
gTasksInitialized:
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
	.global gActors
gActors:
	.space 0x00000004
	.global gActorSystemWork
gActorSystemWork:
	.space 0x00000004
	.global gWorldMapAffineWork
gWorldMapAffineWork:
	.space 0x00000004
	.global gMapGridWork
gMapGridWork:
	.space 0x00000004
	.global gBattleWork
gBattleWork:
	.space 0x0000000c
	.global gBattleCamera
gBattleCamera:
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
	.global gSelectionMenuWork
gSelectionMenuWork:
	.space 0x00000004
	.global gSelectionWorkspace
gSelectionWorkspace:
	.space 0x00000004
	.global gOptionMenuWork
gOptionMenuWork:
	.space 0x00000008
	.global gMindReadWork
gMindReadWork:
	.space 0x00000014
	.global gFieldWork
gFieldWork:
	.space 0x00000004
	.global gPaletteCycleWork
gPaletteCycleWork:
	.space 0x00000004
	.global gWeatherWork
gWeatherWork:
	.space 0x00000004
	.global gFieldBlendWork
gFieldBlendWork:
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
	.global gFadeOverlayWork
gFadeOverlayWork:
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
	.global gBattleBackgroundWork
gBattleBackgroundWork:
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
