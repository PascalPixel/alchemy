	.section .rodata
@ 色なしの絵: BattleFx_MarsDjinnSheet の色で描いてある (同じ効果が読む)
	.global Resource_Data0B4
Resource_Data0B4:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0b4.bitmap.lz"
	.byte 0x7b, 0x3d, 0x80	@ PACKER: left in the buffer by Resource_Data0B0+0x829
