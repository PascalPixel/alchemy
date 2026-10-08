	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_ExplosionSheet
BattleFx_ExplosionSheet:
	.incbin "GRAPHICS/BATTLE_FX/EXPLOSION.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/EXPLOSION.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0xec, 0x10, 0x3c	@ PACKER: left in the buffer by BattleFx_WaterSpraySheet+0xdd5
	.endif
