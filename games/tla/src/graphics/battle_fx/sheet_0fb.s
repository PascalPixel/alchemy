	.section .rodata
@ 色なしの絵: BattleFx_BlueFlameSheet の色で描いてある (同じ効果が読む)
	.global Resource_Data0FB
Resource_Data0FB:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0fb.bitmap.lz"
	.byte 0x3e, 0xe1	@ PACKER: left in the buffer by Resource_Data0F8+0x54a
