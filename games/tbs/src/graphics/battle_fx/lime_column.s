	.section .rodata
	.global BattleEffect_LimeColumnSheet
BattleEffect_LimeColumnSheet:
	.incbin "games/tbs/src/graphics/battle_fx/lime_column.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/lime_column.bitmap.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleEffect_RevealMaskF+0x3cf
