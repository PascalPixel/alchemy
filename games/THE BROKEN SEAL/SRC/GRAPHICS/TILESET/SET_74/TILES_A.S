	.section .rodata
	.global Tileset_Set74TilesA
Tileset_Set74TilesA:
	.incbin "GRAPHICS/TILESET/SET_68/TILES_A.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map178+0x14e
