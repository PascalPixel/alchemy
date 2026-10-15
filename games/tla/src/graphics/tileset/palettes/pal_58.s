	.section .rodata
	.global Tileset_Palette58
Tileset_Palette58:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_58.gbapal.lz"
	.byte 0xbb, 0x1e	@ PACKER: left in the buffer by Field_Map255+0x162
