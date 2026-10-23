	.section .rodata
	.global BattleAnim_RedCrescentSheetB
BattleAnim_RedCrescentSheetB:
	.incbin "games/tbs/src/graphics/battle_anims/red_crescent_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/red_crescent_b.bitmap.lz"
	.byte 0xf0, 0x79	@ PACKER: left in the buffer by BattleAnim_BlueBeastSheet+0x3ea
