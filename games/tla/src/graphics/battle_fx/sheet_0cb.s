	.section .rodata
@ 色なしの絵: BattleFx_OrangePaletteB の色で描いてある (同じ効果が読む)
	.global Resource_Data0CB
Resource_Data0CB:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0cb.bitmap.lz"
	.byte 0x40, 0xe7, 0x09	@ PACKER: left in the buffer by Resource_Data0C8+0x7bd
