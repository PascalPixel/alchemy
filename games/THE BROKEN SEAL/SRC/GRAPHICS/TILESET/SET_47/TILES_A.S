	.section .rodata
	.global Tileset_Set47TilesA
Tileset_Set47TilesA:
	.incbin "GRAPHICS/TILESET/SET_47/TILES_A.4bpp.mtf"
	.byte 0x01, 0x6d	@ PACKER: left in the buffer by Field_Map073+0x150e
