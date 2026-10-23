	.section .rodata
	.global BattleAnim_GreenVineSheet
BattleAnim_GreenVineSheet:
	.incbin "games/tbs/src/graphics/battle_anims/green_vine.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/green_vine.bitmap.lz"
	.byte 0x1c, 0xaf	@ PACKER: left in the buffer by BattleAnim_WispSheet+0x516
