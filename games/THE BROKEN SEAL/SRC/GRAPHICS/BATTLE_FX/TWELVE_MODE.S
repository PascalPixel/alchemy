	.section .rodata
	.global BattleFx_TwelveModeImage
BattleFx_TwelveModeImage:
	.incbin "GRAPHICS/BATTLE_FX/TWELVE_MODE.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_LightningBoltSheet at +0x42f */
	.byte 0xa5
