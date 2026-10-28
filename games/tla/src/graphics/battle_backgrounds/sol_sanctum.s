	.section .rodata
	.global Battle_SolSanctumBackdrop
Battle_SolSanctumBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/sol_sanctum.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/sol_sanctum.bitmap.d7"
	.byte 0x10, 0x15	@ PACKER: left in the buffer by Battle_GrasslandBackdrop+0x42b6
