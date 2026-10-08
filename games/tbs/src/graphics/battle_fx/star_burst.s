	.section .rodata
	.global BattleFx_StarBurstSheet
BattleFx_StarBurstSheet:
	.incbin "graphics/battle_fx/star_burst.gbapal"
	.incbin "graphics/battle_fx/star_burst.bitmap.lz"
	.byte 0xa1	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x487
