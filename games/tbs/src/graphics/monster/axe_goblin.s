	.section .rodata
	.global Monster_AxeGoblinSprites
Monster_AxeGoblinSprites:
	.incbin "games/tbs/src/graphics/monster/axe_goblin.frames.lz"
	.byte 0xda, 0x01	@ PACKER: left in the buffer by Ui_CommandIcons+0x21b2
