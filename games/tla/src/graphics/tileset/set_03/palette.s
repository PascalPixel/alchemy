	.section .rodata
	.global Tileset_Set03Palette
Tileset_Set03Palette:
	.incbin "games/common/src/graphics/tileset/palettes/pal_41.gbapal.lz"
	.byte 0x71, 0xc0, 0x05	@ PACKER: left in the buffer by Field_Map004+0x171
