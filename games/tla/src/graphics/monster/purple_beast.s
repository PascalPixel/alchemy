	.section .rodata
	.global Monster_PurpleBeastSprites
Monster_PurpleBeastSprites:
	.incbin "games/tla/src/graphics/monster/purple_beast.pictures.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
