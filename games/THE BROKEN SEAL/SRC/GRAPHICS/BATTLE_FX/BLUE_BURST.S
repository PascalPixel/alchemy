	.section .rodata
	.global BattleFx_BlueBurstSheet
BattleFx_BlueBurstSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BURST.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_BURST.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_LightFanSheet at +0x331 */
	.byte 0xc4, 0x99, 0xd3
