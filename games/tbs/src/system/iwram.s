	.section .sym_iwram,"aw",%nobits
	.ifdef TBS_EDITION_DE
	.space 16
	.endif
	.global gOamDepthLists
gOamDepthLists:
	.space 1024
	.global gVBlankCount
gVBlankCount:
	.space 4
	.global gSavedStackSize
gSavedStackSize:
	.space 12
	.global gVramCacheBlockOwners
gVramCacheBlockOwners:
	.space 512
	.global gTasksRunning
gTasksRunning:
	.space 16
	.global gTasks
gTasks:
	.space 160
	.global gScreenFadeVBlanksLeft
gScreenFadeVBlanksLeft:
	.space 4
	.global gDebugTextEnabled
gDebugTextEnabled:
	.space 12
	.global gBgScroll
gBgScroll:
	.space 24
	.global gKeysHeld
gKeysHeld:
	.space 4
	.global gScreenFadeStartLevel
gScreenFadeStartLevel:
	.space 4
	.global gCpuLoadTimer
gCpuLoadTimer:
	.space 4
	.global gScanlinePitch
gScanlinePitch:
	.space 4
	.global gKeysPressedLatch
gKeysPressedLatch:
	.space 4
	.global gKeysRepeatDirected
gKeysRepeatDirected:
	.space 4
	.global gKeyRepeatTimer
gKeyRepeatTimer:
	.space 4
	.global gKeysRepeat
gKeysRepeat:
	.space 12
	.global gVramCacheEntries
gVramCacheEntries:
	.space 384
	.global gDecodeFillByte
gDecodeFillByte:
	.space 4
	.global gKeysPressed
gKeysPressed:
	.space 4
	.global gScreenFadeVBlanks
gScreenFadeVBlanks:
	.space 4
	.global gPlayTimeFrames
gPlayTimeFrames:
	.space 4
	.global gSleepDisabled
gSleepDisabled:
	.space 4
	.global gCpuLoadPeak
gCpuLoadPeak:
	.space 4
	.global gScreenFadeTargetLevel
gScreenFadeTargetLevel:
	.space 8
	.global gSerialExchangeActive
gSerialExchangeActive:
	.space 4
	.global gRandomSeed
gRandomSeed:
	.space 4
	.global gResetRequested
gResetRequested:
	.space 4
	.global gDebugTextCursor
gDebugTextCursor:
	.space 4
	.global gCamera_UnreadWord1
gCamera_UnreadWord1:
	.space 4
	.global gTransformStackDepth
gTransformStackDepth:
	.space 4
	.global gSleepRequested
gSleepRequested:
	.space 4
	.global gFrameVBlanks
gFrameVBlanks:
	.space 4
	.global gLastFrameVBlanks
gLastFrameVBlanks:
	.space 4
	.global gScreenFadeToWhite
gScreenFadeToWhite:
	.space 12
	.global gProjection
gProjection:
	.space 20
	.global gKeysHeldLast
gKeysHeldLast:
	.space 4
	.global gScreenFadeLayers
gScreenFadeLayers:
	.space 4
	.global gVBlankCallback
gVBlankCallback:
	.space 4
	.global gOamMatrixCount
gOamMatrixCount:
	.space 4
	.global gKeysDirection
gKeysDirection:
	.ifdef TBS_EDITION_DE
	.space 12
	.else
	.space 4
	.endif
	.global gIdleSleepEnabled
gIdleSleepEnabled:
	.space 4
	.global gKeysHeldAtVBlank
gKeysHeldAtVBlank:
	.space 12
	.global gOamCopyEnabled
gOamCopyEnabled:
	.space 4
	.global gCamera_UnreadWord2
gCamera_UnreadWord2:
	.space 4
	.global gDebugPaused
gDebugPaused:
	.space 4
	.global gIdleFrames
gIdleFrames:
	.space 4
	.global gVBlankDone
gVBlankDone:
	.space 4
	.global gTransformStackTop
gTransformStackTop:
	.space 8
	.global gTasksInitialized
gTasksInitialized:
	.ifdef TBS_EDITION_DE
	.space 4
	.else
	.space 12
	.endif
	.global gOamMatrices
gOamMatrices:
	.space 256
	.global gFrameCount
gFrameCount:
	.global gFrameCount
gFrameCount:
	.space 4
	.global gFrameRenderPending
gFrameRenderPending:
	.space 12
	.global gHeapSlots
gHeapSlots:
	.global gHeapSlots
	.type gHeapSlots, %object
	.size gHeapSlots, 256
gHeapSlots:
	.space 12
	.global gSpriteAnimations
gSpriteAnimations:
	.space 4
	.global gSprites
gSprites:
	.space 4
	.global gActors
gActors:
	.space 4
	.global gActorSystemWork
gActorSystemWork:
	.space 4
	.global gWorldMapAffineWork
gWorldMapAffineWork:
	.space 4
	.global gMapGridWork
gMapGridWork:
	.space 4
	.global gBattleWork
gBattleWork:
	.space 12
	.global gBattleCamera
gBattleCamera:
	.space 12
	.global gWindowWork
gWindowWork:
	.global gWindowWork
gWindowWork:
	.space 4
	.global gPartyStatusWork
gPartyStatusWork:
	.space 4
	.global gGlyphWork
gGlyphWork:
	.space 4
	.global gSelectionMenuWork
gSelectionMenuWork:
	.space 4
	.global gSelectionWorkspace
gSelectionWorkspace:
	.space 4
	.global gOptionMenuWork
gOptionMenuWork:
	.space 8
	.global gMindReadWork
gMindReadWork:
	.space 20
	.global gFieldWork
gFieldWork:
	.space 4
	.global gPaletteCycleWork
gPaletteCycleWork:
	.space 4
	.global gWeatherWork
gWeatherWork:
	.space 4
	.global gFieldBlendWork
gFieldBlendWork:
	.space 4
	.global gScreenTransitionWork
gScreenTransitionWork:
	.space 4
	.global gPaletteFadeWork
gPaletteFadeWork:
	.space 8
	.global gLineScroll
gLineScroll:
	.space 8
	.global gFadeOverlayWork
gFadeOverlayWork:
	.space 4
	.global gBattleMessageWork
gBattleMessageWork:
	.space 8
	.global gBattleEffectWork
gBattleEffectWork:
	.space 12
	.global gBattleEntryTimer
gBattleEntryTimer:
	.space 8
	.global gBattleBackgroundWork
gBattleBackgroundWork:
	.space 28
	.global gSaveWork
gSaveWork:
	.space 12
	.global gEnemyUnits
gEnemyUnits:
	.space 4
	.global gMenuWork
gMenuWork:
	.space 4
	.global gFieldEffectWork
gFieldEffectWork:
	.global gFieldEffectWork
gFieldEffectWork:
	.space 4
	.global gBattleMenuWork
gBattleMenuWork:
	.space 4
	.global gIconMenuWork
gIconMenuWork:
	.space 4
	.global gColossoWork
gColossoWork:
	.space 24
	.global gDebugMode
gDebugMode:
	.space 4
	.global gCpuLoadDisplayEnabled
gCpuLoadDisplayEnabled:
	.space 4
	.global gSleepComboFrames
gSleepComboFrames:
	.space 4
	.global gScanlineAngle
gScanlineAngle:
	.space 4
	.global gLinkStatus
gLinkStatus:
	.space 12
	.global gNumberBuffer
gNumberBuffer:
	.space 8
	.global gNumberBufferHexEnd
gNumberBufferHexEnd:
	.space 2
	.global gNumberBufferDecimalEnd
gNumberBufferDecimalEnd:
	.ifdef TBS_EDITION_DE
	.space 118
	.else
	.space 134
	.endif
	.global gIwramHeap
gIwramHeap:
