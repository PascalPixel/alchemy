	.section .rodata
	.global Battle_RockyBeachBackdrop
Battle_RockyBeachBackdrop:
	.incbin "games/tla/src/graphics/backdrop/rocky_beach.gbapal"
	.incbin "games/tla/src/graphics/backdrop/rocky_beach.bitmap.d7"
	.byte 0x20, 0x03	@ PACKER: left in the buffer by Battle_GrasslandBBackdrop+0x33ce
