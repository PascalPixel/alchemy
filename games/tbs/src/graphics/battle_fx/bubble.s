	.section .rodata
	.global BattleEffect_BubbleSheet
BattleEffect_BubbleSheet:
	.incbin "games/tbs/src/graphics/battle_fx/bubble.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/bubble.bitmap.lz"
	.byte 0x14, 0xb1, 0x50	@ PACKER: left in the buffer by BattleEffect_YellowOrbSheet+0x381
