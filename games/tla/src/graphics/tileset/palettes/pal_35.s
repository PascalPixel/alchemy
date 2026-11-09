	.section .rodata
	.global Tileset_Palette35
Tileset_Palette35:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_35.gbapal.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x05, 0x08	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
	.endif
