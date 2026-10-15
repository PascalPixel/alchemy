	.section .rodata
	.global Battle_SoruShindenBackdrop
Battle_SoruShindenBackdrop:
	.incbin "games/common/src/graphics/backdrop/sol_sanctum.gbapal"
	.incbin "games/common/src/graphics/backdrop/sol_sanctum.bitmap.d7"
	.byte 0x10, 0x15	@ PACKER: left in the buffer by Battle_GrasslandBackdrop+0x42b6
