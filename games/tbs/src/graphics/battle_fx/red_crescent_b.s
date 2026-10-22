	.section .rodata
	.global BattleEffect_RedCrescentSheetB
BattleEffect_RedCrescentSheetB:
	.incbin "games/tbs/src/graphics/battle_fx/red_crescent_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/red_crescent_b.bitmap.lz"
	.byte 0xf0, 0x79	@ PACKER: left in the buffer by BattleEffect_BlueBeastSheet+0x3ea
