	.section .rodata
	.global BattleFx_GreyCrescentSheet
BattleFx_GreyCrescentSheet:
	.incbin "games/tla/src/graphics/battle_fx/grey_crescent.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/grey_crescent.bitmap.lz"
	.byte 0x06	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0xd33
