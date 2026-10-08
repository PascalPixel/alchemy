	.section .rodata
	.global Tileset_Set01Palette
Tileset_Set01Palette:
	.incbin "GRAPHICS/TILESET/SET_01/PALETTE.gbapal.lz"
	.byte 0xb3	@ PACKER: left in the buffer by Field_Map002+0x193
