	.section .rodata
	.global Monster_GargoyleSprites
Monster_GargoyleSprites:
	.incbin "games/tbs/src/graphics/monster/gargoyle.frames.lz"
	.byte 0xb1, 0x5b	@ PACKER: left in the buffer by Monster_OrcLordSprites+0x3092
