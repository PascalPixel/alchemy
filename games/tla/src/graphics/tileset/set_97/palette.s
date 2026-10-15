	.section .rodata
	.global Tileset_Set97Palette
Tileset_Set97Palette:
	.incbin "games/tla/src/graphics/tileset/set_97/palette.gbapal.lz"
	.byte 0xd1, 0x0c	@ PACKER: left in the buffer by Field_Map224+0x172
