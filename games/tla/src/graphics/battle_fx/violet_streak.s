	.section .rodata
	.global BattleFx_VioletStreakSheet
BattleFx_VioletStreakSheet:
	.incbin "games/tla/src/graphics/battle_fx/violet_streak.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/violet_streak.bitmap.lz"
	.byte 0x5b	@ PACKER: left in the buffer by BattleFx_SpiralSheet+0xc8b
