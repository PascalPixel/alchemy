	.section .rodata
	.global Tileset_Set15TilesB
Tileset_Set15TilesB:
	.incbin "GRAPHICS/TILESET/SET_15/TILES_B.4bpp.mtf"
	.byte 0x3b	@ PACKER: left in the buffer by Field_Map018+0x2547
