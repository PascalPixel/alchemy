	.section .rodata
	.global Tileset_Set66TilesA
Tileset_Set66TilesA:
	.incbin "GRAPHICS/TILESET/SET_66/TILES_A.4bpp.mtf"
	.byte 0xfd, 0x17, 0x05	@ PACKER: left in the buffer by Field_Map169+0x299
