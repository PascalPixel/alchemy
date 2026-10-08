	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_BlueWispSprites
Monster_BlueWispSprites:
	.incbin "GRAPHICS/MONSTER/BLUE_WISP.frames.lz"
	.ifeq GAME_TLA
	.byte 0xe9, 0x10	@ PACKER: left in the buffer by Monster_SirenSprites+0x105a
	.endif
