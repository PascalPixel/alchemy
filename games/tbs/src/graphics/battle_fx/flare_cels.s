	.section .rodata
	.global BattleFx_FlareImage
BattleFx_FlareImage:
	.incbin "graphics/battle_fx/flare_cels.bitmap.lz"
	.byte 0x05	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x277
