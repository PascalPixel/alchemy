	.section .rodata
	.global Tileset_Set51TilesA
Tileset_Set51TilesA:
	.incbin "games/tla/src/graphics/tileset/set_51/tiles_a.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0xd4, 0x0e, 0x40	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.else
	.byte 0xef, 0x12, 0x40	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.endif
