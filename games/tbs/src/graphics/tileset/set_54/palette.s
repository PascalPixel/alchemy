	.section .rodata
	.global Tileset_Set54Palette
Tileset_Set54Palette:
	.incbin "graphics/tileset/set_54/palette.gbapal.lz"
	.byte 0x2a, 0x91, 0x1b	@ PACKER: left in the buffer by Field_Map101+0x1a5
