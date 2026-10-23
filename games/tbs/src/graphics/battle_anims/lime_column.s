	.section .rodata
	.global BattleAnim_LimeColumnSheet
BattleAnim_LimeColumnSheet:
	.incbin "games/tbs/src/graphics/battle_anims/lime_column.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/lime_column.bitmap.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleAnim_RevealMaskF+0x3cf
