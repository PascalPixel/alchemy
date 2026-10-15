	.section .rodata
	.global BattleFx_ExplosionSheet
BattleFx_ExplosionSheet:
	.incbin "games/common/src/graphics/battle_fx/explosion.gbapal"
	.incbin "games/common/src/graphics/battle_fx/explosion.bitmap.lz"
	.byte 0xec, 0x10, 0x3c	@ PACKER: left in the buffer by BattleFx_WaterSpraySheet+0xdd5
