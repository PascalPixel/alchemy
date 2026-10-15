	.section .rodata
	.global BattleFx_CrescentMoonSheet
BattleFx_CrescentMoonSheet:
	.incbin "games/common/src/graphics/battle_fx/crescent_moon.gbapal"
	.incbin "games/common/src/graphics/battle_fx/crescent_moon.bitmap.lz"
	.byte 0x2b	@ PACKER: left in the buffer by BattleFx_VioletStreakSheet+0x4cf
