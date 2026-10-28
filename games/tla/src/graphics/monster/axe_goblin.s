	.section .rodata
	.global Monster_AxeGoblinSprites
Monster_AxeGoblinSprites:
	.incbin "games/tla/src/graphics/monster/axe_goblin.pictures.lz"
	.byte 0x83, 0x12	@ PACKER: left in the buffer by Monster_SpearWarriorSprites+0x21b2
