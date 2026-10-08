	.section .rodata
	.global BattleFx_RainSheet
BattleFx_RainSheet:
	.incbin "GRAPHICS/BATTLE_FX/RAIN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RAIN.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_CyanSparkSheet at +0x89f */
	.byte 0x20
