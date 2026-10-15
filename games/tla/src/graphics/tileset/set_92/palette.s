	.section .rodata
	.global Tileset_Set92Palette
Tileset_Set92Palette:
	.incbin "games/tla/src/graphics/tileset/set_92/palette.gbapal.lz"
	.byte 0xa8, 0xad, 0x01	@ PACKER: left in the buffer by Field_Map218+0x141
