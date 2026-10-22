	.section .rodata
	.global BattleEffect_GreenVineSheet
BattleEffect_GreenVineSheet:
	.incbin "games/tbs/src/graphics/battle_fx/green_vine.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/green_vine.bitmap.lz"
	.byte 0x1c, 0xaf	@ PACKER: left in the buffer by BattleEffect_WispSheet+0x516
