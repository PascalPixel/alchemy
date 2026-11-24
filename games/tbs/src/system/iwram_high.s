	.section .sym_iwram_high,"aw",%nobits
	.space 1116
	.global gCartridgeResetMarker
gCartridgeResetMarker:
	.space 4
	.global gBuildStamp
gBuildStamp:
	.space 508
	.global gFrameWaitStackTop
gFrameWaitStackTop:
	.space 1532
	.global gIrqVector
gIrqVector:
