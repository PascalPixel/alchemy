	.section .rodata
	.global Tileset_Set35Palette
Tileset_Set35Palette:
	.incbin "GRAPHICS/TILESET/SET_35/PALETTE.gbapal.lz"
	.byte 0x05	@ PACKER: left in the buffer by Field_Map045+0x17b
