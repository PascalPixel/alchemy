	.section .rodata
	.global BattleAnim_BlueArcSheetB
BattleAnim_BlueArcSheetB:
	.incbin "games/tbs/src/graphics/battle_fx/blue_arc_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/blue_arc_b.bitmap.lz"
	.byte 0xaa, 0x30	@ PACKER: left in the buffer by BattleAnim_EmberStreakSheet+0xace
