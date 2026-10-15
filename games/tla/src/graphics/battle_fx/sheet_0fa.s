	.section .rodata
@ 色なしの絵: BattleFx_OrangePaletteB の色で描いてある (同じ効果が読む)
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0fa.bitmap.lz"
	.byte 0xb2	@ PACKER: left in the buffer by Resource_Data0F9+0x1c7
