	.section .rodata
	.global Tileset_Set15Palette
Tileset_Set15Palette:
	.incbin "games/tla/src/graphics/tileset/set_15/palette.gbapal.lz"
	.byte 0xc2, 0x8d	@ PACKER: left in the buffer by Field_Map032+0x14e
