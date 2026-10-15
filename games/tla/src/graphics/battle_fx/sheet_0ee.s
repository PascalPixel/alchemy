	.section .rodata
@ 色なしの絵: BattleFx_EffectPalette02 の色で描いてある (同じ効果が読む)
	.global Resource_Data0EE
Resource_Data0EE:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0ee.bitmap.lz"
	.byte 0x01, 0x2e	@ PACKER: left in the buffer by BattleFx_CornerRuneBSheet+0x65a
