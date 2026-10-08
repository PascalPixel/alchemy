	.section .rodata
	.global BattleFx_FlareImage
BattleFx_FlareImage:
	.incbin "GRAPHICS/BATTLE_FX/FLARE_CELS.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_FlareSheet at +0x277 */
	.byte 0x05
