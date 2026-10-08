	.section .rodata
	.global Tileset_Set29Palette
Tileset_Set29Palette:
	.incbin "GRAPHICS/TILESET/SET_29/PALETTE.gbapal.lz"
	.byte 0xc0, 0x23	@ PACKER: left in the buffer by Field_Map037+0x182
