	.section .rodata
	.global Tileset_Set101TilesC
Tileset_Set101TilesC:
	.incbin "games/tla/src/graphics/tileset/set_101/tiles_c.4bpp.mtf"
	.byte 0x10, 0x00, 0x00	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
