	.section .rodata
	.global BattleAnim_DaggerSheet
BattleAnim_DaggerSheet:
	.incbin "games/tbs/src/graphics/battle_anims/dagger.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/dagger.bitmap.lz"
	.byte 0x0a, 0x77, 0x19	@ PACKER: left in the buffer by BattleAnim_ShieldSheet+0xb45
