	.section .rodata
	.global BattleFx_RuneCircleSheet
BattleFx_RuneCircleSheet:
	.incbin "games/tla/src/graphics/battle_fx/rune_circle.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/rune_circle.bitmap.lz"
	.byte 0x6b, 0xa1	@ PACKER: left in the buffer by BattleFx_PetalSheet+0x1142
