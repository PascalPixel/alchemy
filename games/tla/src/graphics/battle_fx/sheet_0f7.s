	.section .rodata
@ 色なしの絵: BattleFx_OrangePaletteB の色で描いてある (同じ効果が読む)
	.global Resource_Data0F7
Resource_Data0F7:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0f7.bitmap.lz"
	.byte 0x00, 0x38	@ PACKER: left in the buffer by Resource_Data0F5+0x646
