	.section .rodata
	.global Tileset_Set63Palette
Tileset_Set63Palette:
	.incbin "GRAPHICS/TILESET/SET_63/PALETTE.gbapal.lz"
	.byte 0x4a, 0xf0, 0x09	@ PACKER: left in the buffer by Field_Map145+0x161
