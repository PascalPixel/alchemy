	.section .rodata
	.global Tileset_Set04Palette
Tileset_Set04Palette:
	.incbin "games/common/src/graphics/tileset/palettes/pal_42.gbapal.lz"
	.byte 0x1f, 0x44	@ PACKER: left in the buffer by Field_Map006+0x1b6
