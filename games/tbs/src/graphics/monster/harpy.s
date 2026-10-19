	.section .rodata
	.global Monster_HarpySprites
Monster_HarpySprites:
	.incbin "games/tbs/src/graphics/monster/harpy.frames.lz"
	.byte 0xbb, 0x01, 0x75	@ PACKER: left in the buffer by Ui_CommandIcons+0x2871
