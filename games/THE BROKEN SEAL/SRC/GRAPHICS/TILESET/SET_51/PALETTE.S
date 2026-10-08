	.section .rodata
	.global Tileset_Set51Palette
Tileset_Set51Palette:
	.incbin "GRAPHICS/TILESET/SET_51/PALETTE.gbapal.lz"
	.byte 0x94	@ PACKER: left in the buffer by Field_Map093+0x17b
