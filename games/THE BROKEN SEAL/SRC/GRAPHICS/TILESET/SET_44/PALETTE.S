	.section .rodata
	.global Tileset_Set44Palette
Tileset_Set44Palette:
	.incbin "GRAPHICS/TILESET/SET_44/PALETTE.gbapal.lz"
	.byte 0x32, 0xc8	@ PACKER: left in the buffer by Field_Map060+0x1be
