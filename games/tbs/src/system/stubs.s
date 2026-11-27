@ The system library's module: frames and events, memory, resources and the
@ other services every team's code calls.
	.include "games/tbs/src/system/stub_table.inc"
	.global Stubs_System
Stubs_System:
	stub sySynchStub, sySynch
	unnamed_stub syInitEvent
	stub sySetEventStub, sySetEvent
	stub syCloseEventStub, syCloseEvent
	unnamed_stub sySuspendEvent
	unnamed_stub syResumeEvent
	unnamed_stub syExecuteEvent
	stub syRandomStub, syRandom
	stub syAtanStub, syAtan
	unnamed_stub sySquart
	stub sySquartFXStub, sySquartFX
	stub sySinStub, sySin
	stub syCosStub, syCos
	stub syVectorAddPolarOffsetStub, syVectorAddPolarOffset
	stub syIntFunctionStub, syIntFunction
	unnamed_stub syHeapInit
	stub syHeapSlotAllocStub, syHeapSlotAlloc
	stub syHeapSlotAllocEWRAMStub, syHeapSlotAllocEWRAM
	stub syHeapSlotReleaseStub, syHeapSlotRelease
	unnamed_stub syHeapGetRemainingIWRAM
	unnamed_stub syHeapGetRemainingEWRAM
	stub syHeapAllocStub, syHeapAlloc
	stub syHeapAllocEWRAMStub, syHeapAllocEWRAM
	stub syHeapReleaseStub, syHeapRelease
	unnamed_stub syPrint
	unnamed_stub syPrintHex
	unnamed_stub syPrintDec
	unnamed_stub syPrintBlanks
	stub syDecompressByteLZStub, syDecompressByteLZ
	stub syDecompressTaggedLZStub, syDecompressTaggedLZ
	unnamed_stub syVRAMCacheReset
	stub syVRAMCacheFreeEntryStub, syVRAMCacheFreeEntry
	stub syVRAMCacheReleaseVRAMStub, syVRAMCacheReleaseVRAM
	stub syVRAMCacheLoadStub, syVRAMCacheLoad
	stub syVRAMCacheFindFreeEntryStub, syVRAMCacheFindFreeEntry
	unnamed_stub syPrintClear
	stub syOAMMatrixAllocFromTransformStub, syOAMMatrixAllocFromTransform
	stub syOAMAddSprAtDepthStub, syOAMAddSprAtDepth
	stub syWindowLoadGraphicsStub, syWindowLoadGraphics
	unnamed_stub syPaletteLoadBasicSprCols
	unnamed_stub syInitMatrix
	unnamed_stub syUnitMatrixCurrent
	unnamed_stub syPushMatrix
	unnamed_stub syPopMatrix
	unnamed_stub syMatrixRotate
	unnamed_stub syRotX
	unnamed_stub syRotY
	unnamed_stub syRotZ
	unnamed_stub syMatrixTranslateVector
	unnamed_stub syMatrixScaleVector
	unnamed_stub syMatrixRotateTranslate
	unnamed_stub syMatrixRotateTranslateScale
	unnamed_stub syLookAtLoad
	unnamed_stub syConvert3Dto2D
	unnamed_stub syPerspective
	unnamed_stub sySetScreenDist
	unnamed_stub syCopyBuildStamp
	unnamed_stub syDummyReturnZero
	stub syResourceGetTableEntryStub, syResourceGetTableEntry
	stub syDummyNoOperationHookStub, syDummyNoOperationHook
	unnamed_stub syOverlayLoad
	stub syScreenFadeToBlackStub, syScreenFadeToBlack
	unnamed_stub syScreenFadeFromBlack
	unnamed_stub syScreenFadeToWhite
	unnamed_stub syScreenFadeFromWhite
	unnamed_stub syScreenFadeStart
	stub syScreenFadeWaitStub, syScreenFadeWait
	unnamed_stub syDummyReservedNoOperationA
	unnamed_stub syDummyReservedNoOperationB
	unnamed_stub syDummyReservedNoOperationC
	unnamed_stub syDummyReservedNoOperationD
	unnamed_stub syDummyReservedNoOperationE
	stub sySerialInitStub, sySerialInit
	stub sySerialPollAndTransferStub, sySerialPollAndTransfer
	unnamed_stub syIOWriteQueueStore8
	stub syIOWriteQueueStore16Stub, syIOWriteQueueStore16
	stub syIOWriteQueueStore32Stub, syIOWriteQueueStore32
	unnamed_stub syIOWriteQueueSetBits8
	unnamed_stub syIOWriteQueueSetBits16
	unnamed_stub syIOWriteQueueSetBits32
	unnamed_stub syIOWriteQueueClearBits8
	unnamed_stub syIOWriteQueueClearBits16
	unnamed_stub syIOWriteQueueClearBits32
	unnamed_stub syInputResetKeyRepeat
	unnamed_stub syLookAt
	unnamed_stub sySerialWaitForStatMask
	stub sySerialRemoveIRQHandlersStub, sySerialRemoveIRQHandlers
	stub sySndLoadReverbCodeStub, sySndLoadReverbCode
	stub sySerialStartSendStub, sySerialStartSend
	stub sySerialStartReceiveStub, sySerialStartReceive
	unnamed_stub sySerialWaitForSend
	unnamed_stub sySerialWaitForReceive
	unnamed_stub sySerialWaitForTransfers
	stub sySerialGetActiveTransfersStub, sySerialGetActiveTransfers
	unnamed_stub sySerialStepBlockTransfer
	stub sySetEventFlagsStub, sySetEventFlags
