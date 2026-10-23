	.section .rodata
	.global Battle_SolSanctumBackdrop
Battle_SolSanctumBackdrop:
	.incbin "games/tbs/src/graphics/battle_bg/sol_sanctum.gbapal"
	.incbin "games/tbs/src/graphics/battle_bg/sol_sanctum.bitmap.d7"
	.byte 0x10, 0x15	@ PACKER: left in the buffer by Battle_GrasslandBackdrop+0x42b6
