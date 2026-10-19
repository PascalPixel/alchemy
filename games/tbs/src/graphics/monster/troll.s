	.section .rodata
	.global Monster_TrollSprites
Monster_TrollSprites:
	.incbin "games/tbs/src/graphics/monster/troll.frames.lz"
	.byte 0x2b, 0x00	@ PACKER: left in the buffer by Ui_CommandIcons+0x3222
