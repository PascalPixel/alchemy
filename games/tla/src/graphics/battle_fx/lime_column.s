	.section .rodata
	.global BattleFx_LimeColumnSheet
BattleFx_LimeColumnSheet:
	.incbin "games/tla/src/graphics/battle_fx/lime_column.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/lime_column.bitmap.lz"
	.byte 0x02	@ PACKER: left in the buffer by BattleFx_RevealMaskF+0x3cf
