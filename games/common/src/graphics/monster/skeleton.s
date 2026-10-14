	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_SkeletonSprites
Monster_SkeletonSprites:
	.incbin "games/common/src/graphics/monster/skeleton.frames.lz"
	.ifeq GAME_TLA
	.byte 0xe6, 0x60, 0x43	@ PACKER: left in the buffer by Monster_WingedDragonSprites+0x2a39
	.endif
