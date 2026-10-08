	.section .rodata
	.global BattleFx_StarBurstSheet
BattleFx_StarBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/STAR_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/STAR_BURST.bitmap.lz"
	.byte 0xa1	@ PACKER: left in the buffer by BattleFx_CyanSparkSheet+0x487
