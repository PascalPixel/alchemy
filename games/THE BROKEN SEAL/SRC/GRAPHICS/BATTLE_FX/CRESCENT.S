	.section .rodata
	.global BattleFx_CrescentSheet
BattleFx_CrescentSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT.bitmap.lz"
	.byte 0x09, 0x01	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x5ea
