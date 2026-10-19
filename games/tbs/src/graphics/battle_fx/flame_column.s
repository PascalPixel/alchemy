	.section .rodata
	.global BattleFx_FlameColumnSheet
BattleFx_FlameColumnSheet:
	.incbin "games/tbs/src/graphics/battle_fx/flame_column.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/flame_column.bitmap.lz"
	.byte 0x41	@ PACKER: left in the buffer by BattleFx_RuneSheet+0x3ab
