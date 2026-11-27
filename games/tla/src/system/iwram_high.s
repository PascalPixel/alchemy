	.section .sym_iwram_high,"aw",%nobits
	.space 0x00000c1b
	.global Data_03006fbf
Data_03006fbf:
	.space 0x00000001
	.global IwramSoundMixWorkspace
IwramSoundMixWorkspace:
	.space 0x00000840
	.global CartridgeResetMarker
CartridgeResetMarker:
	.space 0x00000004
	.global BuildStampWork
BuildStampWork:
	.space 0x000007ec
	.global SoundInfoPointer
SoundInfoPointer:
	.space 0x0000000c
	.global IrqVector
IrqVector:
