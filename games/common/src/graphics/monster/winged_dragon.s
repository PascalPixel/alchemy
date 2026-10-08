	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_WingedDragonSprites
Monster_WingedDragonSprites:
	.incbin "graphics/monster/winged_dragon.frames.lz"
	.ifeq GAME_TLA
	.ifdef TBS_EDITION_JA
	.byte 0xee	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.byte 0x8c	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.endif
	.endif
