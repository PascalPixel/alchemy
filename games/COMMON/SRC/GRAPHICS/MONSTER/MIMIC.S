	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_MimicSprites
Monster_MimicSprites:
	.incbin "GRAPHICS/MONSTER/MIMIC.frames.lz"
	.ifeq GAME_TLA
	.byte 0xb9	@ PACKER: left in the buffer by Monster_CrawlerSprites+0x199f
	.endif
