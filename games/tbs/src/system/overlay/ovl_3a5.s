	.section .bss,"aw",%nobits
	.global gLamakanDesert_SandVramSlot
gLamakanDesert_SandVramSlot:
	.space 16
	.global gLamakanDesert_SandWork
gLamakanDesert_SandWork:
	.space 0x50
	.global gLamakanDesert_SandTileBuffer
gLamakanDesert_SandTileBuffer:
	.space 0x900
	.global gLamakanDesert_SandPhase
gLamakanDesert_SandPhase:
	.space 2
