	.section .rodata
	.global BattleFx_StarBurstSheet
BattleFx_StarBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/STAR_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/STAR_BURST.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_CyanSparkSheet at +0x487 */
	.byte 0xa1
