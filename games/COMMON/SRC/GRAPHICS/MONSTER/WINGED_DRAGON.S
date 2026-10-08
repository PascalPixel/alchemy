	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_WingedDragonSprites
Monster_WingedDragonSprites:
	.incbin "GRAPHICS/MONSTER/WINGED_DRAGON.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by earlier streams, which differ by edition */
	.ifdef TBS_EDITION_JA
	.byte 0xee
	.else
	.byte 0x8c
	.endif
	.endif
