	.section .rodata
	.global Tileset_Set00TilesC
Tileset_Set00TilesC:
	.incbin "games/tla/src/graphics/tileset/set_00/tiles_c.4bpp.mtf"
	.byte 0x4c, 0x9e, 0x10	@ PACKER: left in the buffer by Monster_RedWarriorSprites+0x22d5
