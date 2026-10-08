	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_ManedLionSprites
Monster_ManedLionSprites:
	.incbin "GRAPHICS/MONSTER/MANED_LION.frames.lz"
	.ifeq GAME_TLA
	.byte 0x35	@ PACKER: left in the buffer by Monster_SirenSprites+0x26b3
	.endif
