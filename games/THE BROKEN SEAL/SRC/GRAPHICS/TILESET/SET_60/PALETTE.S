	.section .rodata
	.global Tileset_Set60Palette
Tileset_Set60Palette:
	.incbin "GRAPHICS/TILESET/SET_60/PALETTE.gbapal.lz"
	.byte 0x31, 0x9d, 0x0f	@ PACKER: left in the buffer by Field_Map127+0x14d
