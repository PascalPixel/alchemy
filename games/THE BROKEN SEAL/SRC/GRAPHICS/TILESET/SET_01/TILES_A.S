	.section .rodata
	.global Tileset_Set01TilesA
Tileset_Set01TilesA:
	.incbin "GRAPHICS/TILESET/SET_01/TILES_A.4bpp.mtf"
	.byte 0x97	@ PACKER: left in the buffer by Field_Map002+0x2c53
