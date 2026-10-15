	.section .rodata
@ 色なしの絵: BattleFx_OrangePaletteB の色で描いてある (同じ効果が読む)
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0ba.bitmap.lz"
	.byte 0x20	@ PACKER: left in the buffer by BattleFx_WhirlBeamSheet+0x24b
