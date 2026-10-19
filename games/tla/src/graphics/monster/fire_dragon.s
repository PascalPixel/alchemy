	.section .rodata
	.global Monster_FireDragonSprites
Monster_FireDragonSprites:
	.incbin "games/tla/src/graphics/monster/fire_dragon.frames.lz"
	.byte 0x0f	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x4bb3
