	.section .rodata
	.global BattleFx_BubbleSheet
BattleFx_BubbleSheet:
	.incbin "games/tla/src/graphics/battle_fx/bubble.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/bubble.bitmap.lz"
	.byte 0x14, 0xb1, 0x50	@ PACKER: left in the buffer by BattleFx_YellowOrbSheet+0x381
