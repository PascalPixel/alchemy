	.section .rodata
@ 色なしの絵: BattleFx_BlueFlameSheet の色で描いてある (同じ効果が読む)
	.global Resource_Data0FC
Resource_Data0FC:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0fc.bitmap.lz"
	.byte 0x6b, 0x06	@ PACKER: left in the buffer by Resource_Data0FB+0x34a
