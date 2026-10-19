	.section .rodata
	.global Tileset_Set01Palette
Tileset_Set01Palette:
	.incbin "games/tla/src/graphics/tileset/set_01/pal_49.gbapal.lz"
	.byte 0x24, 0x04	@ PACKER: left in the buffer by Field_Map002+0x106
