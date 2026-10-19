	.section .rodata
	.global BattleFx_RedCrescentSheetB
BattleFx_RedCrescentSheetB:
	.incbin "games/tla/src/graphics/battle_fx/red_crescent_b.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/red_crescent_b.bitmap.lz"
	.byte 0xf0, 0x79	@ PACKER: left in the buffer by BattleFx_BlueBeastSheet+0x3ea
