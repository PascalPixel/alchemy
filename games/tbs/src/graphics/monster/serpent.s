	.section .rodata
	.global Monster_SerpentSprites
Monster_SerpentSprites:
	.incbin "games/tbs/src/graphics/monster/serpent.frames.lz"
	.byte 0x0c, 0xb5	@ PACKER: left in the buffer by Monster_SirenSprites+0x2712
