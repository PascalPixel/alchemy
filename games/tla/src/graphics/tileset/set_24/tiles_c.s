	.section .rodata
	.global Tileset_Set24TilesC
Tileset_Set24TilesC:
	.incbin "games/tla/src/graphics/tileset/set_24/tiles_c.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0x1e	@ PACKER: left in the buffer by Field_Map041+0x1976
	.else
	.byte 0x01, 0x00	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.endif
