	.include "games/tbs/include/edition.inc"
	.section .sym_iwram,"aw",%nobits
	.if IWRAM_LAYOUT_SHIFT
	.space 16
	.endif
	.global OamDepthLists
OamDepthLists:
	.space 1024
	.global VBlankCount
VBlankCount:
	.space 4
	.global SavedStackSize
SavedStackSize:
	.space 12
	.global VramCacheBlockOwners
VramCacheBlockOwners:
	.space 512
	.global EventsRunning
EventsRunning:
	.space 16
	.global EventBuf
EventBuf:
	.space 160
	.global ScreenFadeVBlanksLeft
ScreenFadeVBlanksLeft:
	.space 4
	.global DebugTextEnabled
DebugTextEnabled:
	.space 12
	.global BgScrollWork
BgScrollWork:
	.space 24
	.global PerData
PerData:
	.space 4
	.global ScreenFadeStartLevel
ScreenFadeStartLevel:
	.space 4
	.global CpuLoadTimer
CpuLoadTimer:
	.space 4
	.global ScanlinePitch
ScanlinePitch:
	.space 4
	.global PerPushLatch
PerPushLatch:
	.space 4
	.global PerRepeatDirected
PerRepeatDirected:
	.space 4
	.global PerRepeatTimer
PerRepeatTimer:
	.space 4
	.global PerRepeat
PerRepeat:
	.space 12
	.global VramCacheEntries
VramCacheEntries:
	.space 384
	.global DecodeFillByte
DecodeFillByte:
	.space 4
	.global PerPush
PerPush:
	.space 4
	.global ScreenFadeVBlanks
ScreenFadeVBlanks:
	.space 4
	.global PlayTimeFrames
PlayTimeFrames:
	.space 4
	.global SleepDisabled
SleepDisabled:
	.space 4
	.global CpuLoadPeak
CpuLoadPeak:
	.space 4
	.global ScreenFadeTargetLevel
ScreenFadeTargetLevel:
	.space 8
	.global SerialExchangeActive
SerialExchangeActive:
	.space 4
	.global RandomSeed
RandomSeed:
	.space 4
	.global ResetRequested
ResetRequested:
	.space 4
	.global DebugTextCursor
DebugTextCursor:
	.space 4
	.global Camera_UnreadWordA
Camera_UnreadWordA:
	.space 4
	.global MatrixStackDepth
MatrixStackDepth:
	.space 4
	.global SleepRequested
SleepRequested:
	.space 4
	.global FrameVBlanks
FrameVBlanks:
	.space 4
	.global LastFrameVBlanks
LastFrameVBlanks:
	.space 4
	.global ScreenFadeToWhite
ScreenFadeToWhite:
	.space 12
	.global ProjectionWork
ProjectionWork:
	.space 20
	.global PerDataLast
PerDataLast:
	.space 4
	.global ScreenFadeLayers
ScreenFadeLayers:
	.space 4
	.global VBlankCallback
VBlankCallback:
	.space 4
	.global OamMatrixCount
OamMatrixCount:
	.space 4
	.global PerDirection
PerDirection:
	.if IWRAM_GAP_AFTER_KEYS_DIRECTION
	.space 12
	.else
	.space 4
	.endif
	.global IdleSleepEnabled
IdleSleepEnabled:
	.space 4
	.global PerDataAtVBlank
PerDataAtVBlank:
	.space 12
	.global OamCopyEnabled
OamCopyEnabled:
	.space 4
	.global Camera_UnreadWordB
Camera_UnreadWordB:
	.space 4
	.global DebugPaused
DebugPaused:
	.space 4
	.global IdleFrames
IdleFrames:
	.space 4
	.global VBlankDone
VBlankDone:
	.space 4
	.global MatrixStackTop
MatrixStackTop:
	.space 8
	.global EventsInitialized
EventsInitialized:
	.if IWRAM_GAP_AFTER_KEYS_DIRECTION
	.space 4
	.else
	.space 12
	.endif
	.global OamMatrices
OamMatrices:
	.space 256
	.global FrameCount
FrameCount:
	.global FrameCount
FrameCount:
	.space 4
	.global FrameRenderPending
FrameRenderPending:
	.space 12
	.global HeapSlots
HeapSlots:
	.global HeapSlots
	.type HeapSlots, %object
	.size HeapSlots, 256
HeapSlots:
	.space 12
	.global SpriteAnimations
SpriteAnimations:
	.space 4
	.global SpritesWork
SpritesWork:
	.space 4
	.global ActorsWork
ActorsWork:
	.space 4
	.global ActorSystemStateWork
ActorSystemStateWork:
	.space 4
	.global WorldMapAffineStateWork
WorldMapAffineStateWork:
	.space 4
	.global MapGridState
MapGridState:
	.space 4
	.global BattleStateWork
BattleStateWork:
	.space 12
	.global BattleCameraWork
BattleCameraWork:
	.space 12
	.global WindowStateWork
WindowStateWork:
	.global WindowStateWork
WindowStateWork:
	.space 4
	.global PartyStatusState
PartyStatusState:
	.space 4
	.global GlyphState
GlyphState:
	.space 4
	.global SelectionMenuStateWork
SelectionMenuStateWork:
	.space 4
	.global SelectionStateWork
SelectionStateWork:
	.space 4
	.global OptionMenuState
OptionMenuState:
	.space 8
	.global MindReadStateWork
MindReadStateWork:
	.space 20
	.global FieldStateWork
FieldStateWork:
	.space 4
	.global PaletteCycleStateWork
PaletteCycleStateWork:
	.space 4
	.global WeatherStateWork
WeatherStateWork:
	.space 4
	.global FieldBlendStateWork
FieldBlendStateWork:
	.space 4
	.global ScreenTransitionStateWork
ScreenTransitionStateWork:
	.space 4
	.global PaletteFadeState
PaletteFadeState:
	.space 8
	.global LineScroll
LineScroll:
	.space 8
	.global GroundMaskStateWork
GroundMaskStateWork:
	.space 4
	.global BattleMessageStateWork
BattleMessageStateWork:
	.space 8
	.global BattleEffectStateWork
BattleEffectStateWork:
	.space 12
	.global BattleEntryTimer
BattleEntryTimer:
	.space 8
	.global BattleBackgroundStateWork
BattleBackgroundStateWork:
	.space 28
	.global BupStateWork
BupStateWork:
	.space 12
	.global EnemyUnits
EnemyUnits:
	.space 4
	.global MenuState
MenuState:
	.space 4
	.global FieldEffectStateWork
FieldEffectStateWork:
	.global FieldEffectStateWork
FieldEffectStateWork:
	.space 4
	.global BattleMenuStateWork
BattleMenuStateWork:
	.space 4
	.global IconMenuStateWork
IconMenuStateWork:
	.space 4
	.global ColossoState
ColossoState:
	.space 24
	.global DebugMode
DebugMode:
	.space 4
	.global CpuLoadDisplayEnabled
CpuLoadDisplayEnabled:
	.space 4
	.global SleepComboFrames
SleepComboFrames:
	.space 4
	.global ScanlineAngle
ScanlineAngle:
	.space 4
	.global LinkStatusWork
LinkStatusWork:
	.space 12
	.global NumberBuffer
NumberBuffer:
	.space 8
	.global NumberBufferHexEnd
NumberBufferHexEnd:
	.space 2
	.global NumberBufferDecimalEnd
NumberBufferDecimalEnd:
	.if IWRAM_LAYOUT_SHIFT
	.space 118
	.else
	.space 134
	.endif
	.global IwramHeap
IwramHeap:
