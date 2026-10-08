	.section .rodata
	.global Tileset_Set58Palette
Tileset_Set58Palette:
	.incbin "graphics/tileset/set_58/palette.gbapal.lz"
	.byte 0xbe, 0xee	@ PACKER: left in the buffer by Field_Map108+0x186
