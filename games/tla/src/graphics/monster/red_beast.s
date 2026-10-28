	.section .rodata
	.global Monster_RedBeastSprites
Monster_RedBeastSprites:
	.incbin "games/tla/src/graphics/monster/red_beast.pictures.lz"
	.byte 0x19, 0xbb, 0x95	@ PACKER: left in the buffer by Ui_Icons+0x5e69
