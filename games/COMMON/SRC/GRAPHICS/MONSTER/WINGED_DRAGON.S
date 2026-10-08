	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_WingedDragonSprites
Monster_WingedDragonSprites:
	.incbin "GRAPHICS/MONSTER/WINGED_DRAGON.frames.lz"
	.ifeq GAME_TLA
	.ifdef TBS_EDITION_JA
	.byte 0xee	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.byte 0x8c	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.endif
	.endif
