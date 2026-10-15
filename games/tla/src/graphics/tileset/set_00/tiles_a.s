	.section .rodata
	.global Tileset_Set00TilesA
Tileset_Set00TilesA:
	.incbin "games/tla/src/graphics/tileset/set_00/tiles_a.4bpp.mtf"
	.byte 0x92	@ PACKER: left in the buffer by Monster_RedWarriorSprites+0x209f
