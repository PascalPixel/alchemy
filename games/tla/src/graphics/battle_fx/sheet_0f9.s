	.section .rodata
@ 色なしの絵: BattleFx_LightningBoltSheet の色で描いてある (同じ効果が読む)
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0f9.bitmap.lz"
	.byte 0xa3, 0x08, 0x1a	@ PACKER: left in the buffer by Resource_Data0F8+0x3c1
