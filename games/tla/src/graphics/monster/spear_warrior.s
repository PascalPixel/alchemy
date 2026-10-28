	.section .rodata
	.global Monster_SpearWarriorSprites
Monster_SpearWarriorSprites:
	.incbin "games/tla/src/graphics/monster/spear_warrior.pictures.lz"
	.byte 0x20, 0x2a	@ PACKER: left in the buffer by Monster_GreenMageSprites+0x30f6
