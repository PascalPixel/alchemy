	.section .rodata
	.global Tileset_Set121TilesA
Tileset_Set121TilesA:
	.incbin "games/tla/src/graphics/tileset/set_121/tiles_a.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.byte 0x26, 0x00, 0x00	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.endif
