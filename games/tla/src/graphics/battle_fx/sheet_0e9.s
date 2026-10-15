	.section .rodata
@ 色なしの絵: BattleFx_OrangePaletteB の色で描いてある (同じ効果が読む)
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0e9.bitmap.lz"
	.byte 0x1f, 0x50	@ PACKER: left in the buffer by BattleFx_ClawSlashSheet+0x2ca
