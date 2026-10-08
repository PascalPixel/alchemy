	.section .rodata
	.global Tileset_Set04TilesB
Tileset_Set04TilesB:
	.incbin "GRAPHICS/TILESET/SET_04/TILES_B.4bpp.mtf"
	.byte 0x02, 0xf6, 0x07	@ PACKER: left in the buffer by Field_Map005+0x253d
