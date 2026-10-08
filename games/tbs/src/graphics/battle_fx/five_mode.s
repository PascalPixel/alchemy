	.section .rodata
	.global BattleFx_FiveModeImage
BattleFx_FiveModeImage:
	.incbin "graphics/battle_fx/five_mode.bitmap.lz"
	.byte 0x96	@ PACKER: left in the buffer by BattleFx_IceTileSheet+0xfc7
