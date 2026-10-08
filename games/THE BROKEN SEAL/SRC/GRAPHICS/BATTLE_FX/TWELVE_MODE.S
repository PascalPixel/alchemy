	.section .rodata
	.global BattleFx_TwelveModeImage
BattleFx_TwelveModeImage:
	.incbin "GRAPHICS/BATTLE_FX/TWELVE_MODE.bitmap.lz"
	.byte 0xa5	@ PACKER: left in the buffer by BattleFx_LightningBoltSheet+0x42f
