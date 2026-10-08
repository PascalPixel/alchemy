	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_MimicSprites
Monster_MimicSprites:
	.incbin "GRAPHICS/MONSTER/MIMIC.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_CrawlerSprites at +0x199f */
	.byte 0xb9
	.endif
