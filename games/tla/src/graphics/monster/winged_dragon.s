	.section .rodata
	.global Monster_WingedDragonSprites
Monster_WingedDragonSprites:
	.incbin "games/tla/src/graphics/monster/winged_dragon.frames.lz"
	.byte 0xc6	@ PACKER: left in the buffer by Monster_SeaSerpentSprites+0x4f53
