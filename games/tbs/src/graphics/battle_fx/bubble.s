	.section .rodata
	.global BattleAnim_BubbleSheet
BattleAnim_BubbleSheet:
	.incbin "games/tbs/src/graphics/battle_fx/bubble.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/bubble.bitmap.lz"
	.byte 0x14, 0xb1, 0x50	@ PACKER: left in the buffer by BattleAnim_YellowOrbSheet+0x381
