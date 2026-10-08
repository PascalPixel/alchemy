	.section .bss,"aw",%nobits
	.global RamakanSabaku_SandVramSlot
RamakanSabaku_SandVramSlot:
	.space 16
	.global RamakanSabaku_SandWork
RamakanSabaku_SandWork:
	.space 0x50
	.global RamakanSabaku_SandTileBuffer
RamakanSabaku_SandTileBuffer:
	.space 0x900
	.global RamakanSabaku_SandPhase
RamakanSabaku_SandPhase:
	.space 2
