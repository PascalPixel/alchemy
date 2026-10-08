	.section .rodata
	.global BattleFx_FlareImage
BattleFx_FlareImage:
	.incbin "GRAPHICS/BATTLE_FX/FLARE_CELS.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x277
