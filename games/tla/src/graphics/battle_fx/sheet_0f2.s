	.section .rodata
@ 色なしの絵: BattleFx_MercuryDjinnSheet の色で描いてある (同じ効果が読む)
	.global Resource_Data0F2
Resource_Data0F2:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0f2.bitmap.lz"
	.byte 0x2e, 0x15, 0x23	@ PACKER: left in the buffer by Resource_Data0F1+0x6e1
