	.section .rodata
	.global Tileset_Set53Palette
Tileset_Set53Palette:
	.incbin "GRAPHICS/TILESET/SET_53/PALETTE.gbapal.lz"
	.byte 0x14, 0x41, 0xb9	@ PACKER: left in the buffer by Field_Map098+0x189
