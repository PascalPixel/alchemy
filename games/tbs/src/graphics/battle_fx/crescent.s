	.section .rodata
	.global BattleFx_CrescentSheet
BattleFx_CrescentSheet:
	.incbin "graphics/battle_fx/crescent.gbapal"
	.incbin "graphics/battle_fx/crescent.bitmap.lz"
	.byte 0x09, 0x01	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x5ea
