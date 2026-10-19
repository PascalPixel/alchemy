	.section .rodata
	.global Monster_OrcLordSprites
Monster_OrcLordSprites:
	.incbin "games/tbs/src/graphics/monster/orc_lord.frames.lz"
	.byte 0x0b, 0x00	@ PACKER: left in the buffer by Monster_ManticoreSprites+0x364a
