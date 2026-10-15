	.section .rodata
	.global Battle_SpikeHallBackdrop
Battle_SpikeHallBackdrop:
	.incbin "games/tla/src/graphics/backdrop/spike_hall.gbapal"
	.incbin "games/tla/src/graphics/backdrop/spike_hall.bitmap.d7"
	.byte 0x1c, 0x4d	@ PACKER: left in the buffer by Battle_GorgePoolBackdrop+0x428a
