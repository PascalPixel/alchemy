	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_BlueWispSprites
Monster_BlueWispSprites:
	.incbin "games/common/src/graphics/monster/blue_wisp.frames.lz"
	.ifeq GAME_TLA
	.byte 0xe9, 0x10	@ PACKER: left in the buffer by Monster_SirenSprites+0x105a
	.else
	.byte 0xe9, 0x10	@ PACKER: left in the buffer by Monster_SirenSprites+0x105a
	.endif
