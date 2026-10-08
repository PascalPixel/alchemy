	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SalamanderSprites
Monster_SalamanderSprites:
	.incbin "GRAPHICS/MONSTER/SALAMANDER.frames.lz"
	.ifeq GAME_TLA
	.byte 0x62, 0x4f	@ PACKER: left in the buffer by Monster_WispGhostSprites+0xe16
	.endif
