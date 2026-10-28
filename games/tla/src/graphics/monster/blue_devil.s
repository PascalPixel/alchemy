	.section .rodata
	.global Monster_BlueDevilSprites
Monster_BlueDevilSprites:
	.incbin "games/tla/src/graphics/monster/blue_devil.pictures.lz"
	.byte 0x2f	@ PACKER: left in the buffer by Ui_Icons+0x5237
