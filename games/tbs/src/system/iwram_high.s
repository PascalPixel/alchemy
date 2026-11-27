	.section .sym_iwram_high,"aw",%nobits
	.space 1116
	.global CartridgeResetMarker
CartridgeResetMarker:
	.space 4
	.global BuildStampWork
BuildStampWork:
	.space 508
	.global TickWaitStackTop
TickWaitStackTop:
	.space 1532
	.global IrqVector
IrqVector:
