	.section .rodata
	.global Tileset_Set36TilesA
Tileset_Set36TilesA:
	.incbin "games/tla/src/graphics/tileset/set_36/tiles_a.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.byte 0x05, 0x08	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.endif
