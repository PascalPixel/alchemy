	.section .rodata
@ 色なしの絵: BattleFx_BlueFlameSheet の色で描いてある (同じ効果が読む)
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0ce.bitmap.lz"
	.byte 0xf7, 0x1c	@ PACKER: left in the buffer by BattleFx_CloudSwirlSheet+0x526
