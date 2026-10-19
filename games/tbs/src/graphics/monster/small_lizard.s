	.section .rodata
	.global Monster_SmallLizardSprites
Monster_SmallLizardSprites:
	.incbin "games/tbs/src/graphics/monster/small_lizard.frames.lz"
	.byte 0x70	@ PACKER: left in the buffer by Monster_FairySprites+0xc6f
