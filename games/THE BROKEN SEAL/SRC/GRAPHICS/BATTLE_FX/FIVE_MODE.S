	.section .rodata
	.global BattleFx_FiveModeImage
BattleFx_FiveModeImage:
	.incbin "GRAPHICS/BATTLE_FX/FIVE_MODE.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_IceTileSheet at +0xfc7 */
	.byte 0x96
