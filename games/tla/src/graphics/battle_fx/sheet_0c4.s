	.section .rodata
@ 色なしの絵: BattleFx_EffectPalette02 の色で描いてある (同じ効果が読む)
	.global Resource_Data0C4
Resource_Data0C4:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0c4.bitmap.lz"
	.byte 0xaa, 0x32	@ PACKER: left in the buffer by Resource_Data0C3+0x152
