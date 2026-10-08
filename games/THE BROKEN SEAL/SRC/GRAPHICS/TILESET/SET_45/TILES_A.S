	.section .rodata
	.global Tileset_Set45TilesA
Tileset_Set45TilesA:
	.incbin "GRAPHICS/TILESET/SET_45/TILES_A.4bpp.mtf"
	.byte 0x13, 0x00	@ PACKER: left in the buffer by Field_Map060+0x24ee
