	.section .rodata
	.global BattleFx_RainSheet
BattleFx_RainSheet:
	.incbin "GRAPHICS/BATTLE_FX/RAIN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RAIN.bitmap.lz"
	.byte 0x20	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x89f
