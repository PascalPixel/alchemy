	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_PurpleBeastSprites
Monster_PurpleBeastSprites:
	.incbin "GRAPHICS/MONSTER/PURPLE_BEAST.frames.lz"
	.ifeq GAME_TLA
	.balign 4, 0
	.endif
