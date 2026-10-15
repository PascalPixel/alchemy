	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_PurpleBeastSprites
Monster_PurpleBeastSprites:
	.incbin "games/common/src/graphics/monster/purple_beast.frames.lz"
	.ifeq GAME_TLA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
