	.section .rodata
	.global BattleFx_BlueArcSheetA
BattleFx_BlueArcSheetA:
	.incbin "games/tla/src/graphics/battle_fx/blue_arc_a.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/blue_arc_a.bitmap.lz"
	.byte 0x15, 0xc0, 0x04	@ PACKER: left in the buffer by BattleFx_EmberStreakSheet+0x7d5
