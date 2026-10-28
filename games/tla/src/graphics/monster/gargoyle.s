	.section .rodata
	.global Monster_GargoyleSprites
Monster_GargoyleSprites:
	.incbin "games/tla/src/graphics/monster/gargoyle.pictures.lz"
	.byte 0xb1, 0x5b	@ PACKER: left in the buffer by Monster_OrcLordSprites+0x3092
