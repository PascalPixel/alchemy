	.section .rodata
	.global Tileset_Set59Palette
Tileset_Set59Palette:
	.incbin "games/tla/src/graphics/tileset/set_59/palette.gbapal.lz"
	.byte 0xd5, 0x1b	@ PACKER: left in the buffer by Field_Map118+0x1a2
