	.section .bss,"aw",%nobits
	.global LamakanDesert_SandVramSlot
LamakanDesert_SandVramSlot:
	.space 16
	.global LamakanDesert_SandWork
LamakanDesert_SandWork:
	.space 0x50
	.global LamakanDesert_SandTileBuffer
LamakanDesert_SandTileBuffer:
	.space 0x900
	.global LamakanDesert_SandPhase
LamakanDesert_SandPhase:
	.space 2
