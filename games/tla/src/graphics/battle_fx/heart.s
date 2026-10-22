	.section .rodata
	.global BattleEffect_HeartSheet
BattleEffect_HeartSheet:
	.incbin "games/tla/src/graphics/battle_fx/heart.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/heart.bitmap.lz"
	.byte 0x09	@ PACKER: left in the buffer by BattleEffect_SkullSheet+0x297
