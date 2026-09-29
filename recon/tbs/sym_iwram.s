@ IWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
	.section .sym_iwram,"aw",%nobits
	.global Data_03001400
Data_03001400:
	.space 0x00000400
	.global Data_03001800
Data_03001800:
	.global gFrameTick
gFrameTick:
	.space 0x00000010
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
	.global Data_03001ac4
Data_03001ac4:
	.space 0x0000000c
	.global Data_03001ad0
Data_03001ad0:
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
	.space 0x00000008
	.global Data_03001af4
Data_03001af4:
	.space 0x00000004
	.global Data_03001af8
Data_03001af8:
	.space 0x00000004
	.global Data_03001afc
Data_03001afc:
	.space 0x00000004
	.global Data_03001b00
Data_03001b00:
	.space 0x00000004
	.global Data_03001b04
Data_03001b04:
	.global gKeysRepeat
gKeysRepeat:
	.space 0x0000000c
	.global Data_03001b10
Data_03001b10:
	.global ResourceTableEntries
ResourceTableEntries:
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
	.global Data_03001c98
Data_03001c98:
	.global gBlendDuration
gBlendDuration:
	.space 0x00000004
	.global gLoadedStateWord
gLoadedStateWord:
	.space 0x00000004
	.global Data_03001ca0
Data_03001ca0:
	.space 0x00000008
	.global gBlendTargetLevel
gBlendTargetLevel:
	.space 0x00000008
	.global gSerialExchangeActive
gSerialExchangeActive:
	.space 0x00000004
	.global Data_03001cb4
Data_03001cb4:
	.space 0x00000004
	.global Data_03001cb8
Data_03001cb8:
	.space 0x00000004
	.global Data_03001cbc
Data_03001cbc:
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
	.global Data_03001ccc
Data_03001ccc:
	.space 0x00000008
	.global gBlendBrighten
gBlendBrighten:
	.space 0x0000000c
	.global Data_03001ce0
Data_03001ce0:
	.global gProjection
gProjection:
	.space 0x00000014
	.global Data_03001cf4
Data_03001cf4:
	.space 0x00000004
	.global gBlendLayers
gBlendLayers:
	.space 0x00000004
	.global Data_03001cfc
Data_03001cfc:
	.space 0x00000004
	.global Data_03001d00
Data_03001d00:
	.global gObjAffineCount
gObjAffineCount:
	.space 0x00000004
	.global Data_03001d04
Data_03001d04:
	.space 0x00000004
	.global Data_03001d08
Data_03001d08:
	.global gOptionMirror
gOptionMirror:
	.space 0x00000004
	.global Data_03001d0c
Data_03001d0c:
	.space 0x0000000c
	.global Data_03001d18
Data_03001d18:
	.space 0x00000004
	.global Data_03001d1c
Data_03001d1c:
	.space 0x00000008
	.global gPostLoadCounter
gPostLoadCounter:
	.space 0x00000004
	.global Data_03001d28
Data_03001d28:
	.space 0x00000004
	.global gTransformStackTop
gTransformStackTop:
	.space 0x00000008
	.global gSchedulerTaskCount
gSchedulerTaskCount:
	.space 0x0000000c
	.global gObjAffineMatrices
gObjAffineMatrices:
	.space 0x00000100
	.global Data_03001e40
Data_03001e40:
	.global gFrameCount
gFrameCount:
	.space 0x00000004
	.global Data_03001e44
Data_03001e44:
	.space 0x0000000c
	.global Data_03001e50
Data_03001e50:
	.global gWorkSlot
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
	.global Data_03001e64
Data_03001e64:
	.global gObjectSlots
gObjectSlots:
	.space 0x00000004
	.global Data_03001e68
Data_03001e68:
	.global gMenuCtrlWork
gMenuCtrlWork:
	.space 0x00000004
	.global gMapAnimationPages
gMapAnimationPages:
	.space 0x00000004
	.global Data_03001e70
Data_03001e70:
	.global gCam
gCam:
	.global gMapWork
gMapWork:
	.space 0x00000004
	.global Data_03001e74
Data_03001e74:
	.global Data_03001e74_a
Data_03001e74_a:
	.global gBattleWork
gBattleWork:
	.space 0x0000000c
	.global Data_03001e80
Data_03001e80:
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
	.global Data_03001ea0
Data_03001ea0:
	.global gSelectionWork
gSelectionWork:
	.space 0x00000008
	.global Data_03001ea8
Data_03001ea8:
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
	.global Data_03001ecc
Data_03001ecc:
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
	.global Data_03001eec
Data_03001eec:
	.global gBattleFxWork
gBattleFxWork:
	.space 0x0000000c
	.global gDisp
gDisp:
	.space 0x00000008
	.global Data_03001f00
Data_03001f00:
	.global gTransitionWork
gTransitionWork:
	.space 0x0000001c
	.global Data_03001f1c
Data_03001f1c:
	.global gSaveWorkspace
gSaveWorkspace:
	.space 0x00000010
	.global Data_03001f2c
Data_03001f2c:
	.global Data_03001f2c_a
Data_03001f2c_a:
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
	.global Data_03001f58
Data_03001f58:
	.space 0x00000008
	.global Data_03001f60
Data_03001f60:
	.space 0x00000004
	.global Data_03001f64
Data_03001f64:
	.global gLinkStatus
gLinkStatus:
	.space 0x0000000c
	.global Data_03001f70
Data_03001f70:
	.global gNumberTextBuffer
gNumberTextBuffer:
	.space 0x00000008
	.global Data_03001f78
Data_03001f78:
	.space 0x00000002
	.global Data_03001f7a
Data_03001f7a:
	.space 0x00000086
	.global gIwramHeap
gIwramHeap:
