	.section .sym_iwram_high,"aw",%nobits
	.space 0x0000045c
	.global gCartridgeResetMarker
gCartridgeResetMarker:
	.space 0x00000004
	.global IwramHi_Data1
IwramHi_Data1:
	.space 0x000001fc
	.global gFrameWaitStackTop
gFrameWaitStackTop:
	.space 0x000005fc
	.global gIrqVector
gIrqVector:
