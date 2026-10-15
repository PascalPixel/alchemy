	.section .rodata
	.global Tileset_Set76Palette
Tileset_Set76Palette:
	.incbin "games/tla/src/graphics/tileset/set_76/palette.gbapal.lz"
	.byte 0xfd, 0xa6, 0x32	@ PACKER: left in the buffer by Field_Map163+0x14d
