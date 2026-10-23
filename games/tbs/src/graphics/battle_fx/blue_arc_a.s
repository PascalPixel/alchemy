	.section .rodata
	.global BattleAnim_BlueArcSheetA
BattleAnim_BlueArcSheetA:
	.incbin "games/tbs/src/graphics/battle_fx/blue_arc_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/blue_arc_a.bitmap.lz"
	.byte 0x15, 0xc0, 0x04	@ PACKER: left in the buffer by BattleAnim_EmberStreakSheet+0x7d5
