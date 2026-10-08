	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_MenardiSprites
Monster_MenardiSprites:
	.incbin "GRAPHICS/MONSTER/MENARDI.frames.lz"
	.ifeq GAME_TLA
	.byte 0x52	@ PACKER: left in the buffer by Monster_SaturosSprites+0x3da7
	.endif
