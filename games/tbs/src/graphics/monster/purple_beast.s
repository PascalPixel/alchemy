	.section .rodata
	.global Monster_PurpleBeastSprites
Monster_PurpleBeastSprites:
	.incbin "games/tbs/src/graphics/monster/purple_beast.frames.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
