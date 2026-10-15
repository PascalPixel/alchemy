	.section .rodata
@ 色なしの絵: BattleFx_OrangePaletteB の色で描いてある (同じ効果が読む)
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0d8.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleFx_LightBurstSheet+0x57b
