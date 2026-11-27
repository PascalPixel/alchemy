@ The system library's module: frames and tasks, memory, resources and the
@ other services every team's code calls.
	.include "games/tbs/src/system/stub_table.inc"
	.global Stubs_System
Stubs_System:
	stub sySynchStub, sySynch
	unnamed_stub syInitEvent
	stub syTaskAddStub, sySetEvent
	stub syTaskRemoveStub, syCloseEvent
	unnamed_stub syTaskSuspend
	unnamed_stub syTaskResume
	unnamed_stub syExecuteEvent
	stub syRandomStub, syRandom
	stub syTrigAtanStub, syAtan
	unnamed_stub syMathIntegerSqrt
	stub syMathSqrtFXStub, sySquartFX
	stub sySinStub, sySin
	stub syCosStub, syCos
	stub syVectorAddPolarOffsetStub, syVectorAddPolarOffset
	stub syIrqSetHandlerStub, syIntFunction
	unnamed_stub syHeapInit
	stub syHeapSlotAllocStub, syHeapSlotAlloc
	stub syHeapSlotAllocEwramStub, syHeapSlotAllocEwram
	stub syHeapSlotReleaseStub, syHeapSlotRelease
	unnamed_stub syHeapGetRemainingIwram
	unnamed_stub syHeapGetRemainingEwram
	stub syHeapAllocStub, syHeapAlloc
	stub syHeapAllocEwramStub, syHeapAllocEwram
	stub syHeapReleaseStub, syHeapRelease
	unnamed_stub syPrint
	unnamed_stub syPrintHex
	unnamed_stub syDebugTextPrintDecimal
	unnamed_stub syDebugTextPrintBlanks
	stub fdDecompressByteLzStub, fdDecompressByteLz
	stub fdDecompressTaggedLzStub, fdDecompressTaggedLz
	unnamed_stub syVramCacheReset
	stub syVramCacheFreeEntryStub, syVramCacheFreeEntry
	stub syVramCacheReleaseVramStub, syVramCacheReleaseVram
	stub syVramCacheLoadStub, syVramCacheLoad
	stub syVramCacheFindFreeEntryStub, syVramCacheFindFreeEntry
	unnamed_stub syDebugTextClear
	stub syOamMatrixAllocFromTransformStub, syOamMatrixAllocFromTransform
	stub syOamAddSprAtDepthStub, syOamAddSprAtDepth
	stub mnWindowLoadGraphicsStub, syWindowLoadGraphics
	unnamed_stub syPaletteLoadBasicSprCols
	unnamed_stub syInitMatrix
	unnamed_stub syMatrixResetUnit
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
	unnamed_stub syMatrixSetLookAt
	unnamed_stub syConvert3Dto2D
	unnamed_stub syPerspective
	unnamed_stub sySetScreenDist
	unnamed_stub syCopyBuildStamp
	unnamed_stub syDummyReturnZero
	stub syResourceGetTableEntryStub, syResourceGetTableEntry
	stub syDummyNoOpHookStub, syDummyNoOpHook
	unnamed_stub syOverlayLoad
	stub syScrFadeToBlackStub, syScrFadeToBlack
	unnamed_stub syScrFadeFromBlack
	unnamed_stub syScrFadeToWhite
	unnamed_stub syScrFadeFromWhite
	unnamed_stub syScrFadeStart
	stub syScrFadeWaitStub, syScrFadeWait
	unnamed_stub syDummyReservedNoOpA
	unnamed_stub syDummyReservedNoOpB
	unnamed_stub syDummyReservedNoOpC
	unnamed_stub syDummyReservedNoOpD
	unnamed_stub syDummyReservedNoOpE
	stub sySerialInitStub, sySerialInit
	stub sySerialPollAndTransferStub, sySerialPollAndTransfer
	unnamed_stub syIoWriteQueueStore8
	stub syIoWriteQueueStore16Stub, syIoWriteQueueStore16
	stub syIoWriteQueueStore32Stub, syIoWriteQueueStore32
	unnamed_stub syIoWriteQueueSetBits8
	unnamed_stub syIoWriteQueueSetBits16
	unnamed_stub syIoWriteQueueSetBits32
	unnamed_stub syIoWriteQueueClearBits8
	unnamed_stub syIoWriteQueueClearBits16
	unnamed_stub syIoWriteQueueClearBits32
	unnamed_stub syInputResetKeyRepeat
	unnamed_stub syLookAt
	unnamed_stub sySerialWaitForStatMask
	stub sySerialRemoveIrqHandlersStub, sySerialRemoveIrqHandlers
	stub sySndLoadReverbCodeStub, sySndLoadReverbCode
	stub sySerialStartSendStub, sySerialStartSend
	stub sySerialStartReceiveStub, sySerialStartReceive
	unnamed_stub sySerialWaitForSend
	unnamed_stub sySerialWaitForReceive
	unnamed_stub sySerialWaitForTransfers
	stub sySerialGetActiveTransfersStub, sySerialGetActiveTransfers
	unnamed_stub sySerialStepBlockTransfer
	stub syTaskSetFlagsStub, syTaskSetFlags
