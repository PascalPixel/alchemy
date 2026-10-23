	.section .rodata
	.global BattleAnim_FlameColumnSheet
BattleAnim_FlameColumnSheet:
	.incbin "games/tbs/src/graphics/battle_fx/flame_column.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/flame_column.bitmap.lz"
	.byte 0x41	@ PACKER: left in the buffer by BattleAnim_RuneSheet+0x3ab
