	.section .rodata
	.global Tileset_Set116Palette
Tileset_Set116Palette:
	.incbin "games/tla/src/graphics/tileset/set_116/palette.gbapal.lz"
	.byte 0x47, 0x90	@ PACKER: left in the buffer by Field_Map283+0x166
