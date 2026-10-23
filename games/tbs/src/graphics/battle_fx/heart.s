	.section .rodata
	.global BattleAnim_HeartSheet
BattleAnim_HeartSheet:
	.incbin "games/tbs/src/graphics/battle_fx/heart.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/heart.bitmap.lz"
	.byte 0x09	@ PACKER: left in the buffer by BattleAnim_SkullSheet+0x297
