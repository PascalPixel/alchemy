	.section .rodata
	.global BattleFx_RainSheet
BattleFx_RainSheet:
	.incbin "graphics/battle_fx/rain.gbapal"
	.incbin "graphics/battle_fx/rain.bitmap.lz"
	.byte 0x20	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x89f
