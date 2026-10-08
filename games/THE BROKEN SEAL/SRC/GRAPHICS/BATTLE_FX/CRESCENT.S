	.section .rodata
	.global BattleFx_CrescentSheet
BattleFx_CrescentSheet:
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CRESCENT.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_CyanSparkSheet at +0x5ea */
	.byte 0x09, 0x01
