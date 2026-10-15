	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_SerpentSprites
Monster_SerpentSprites:
	.incbin "games/common/src/graphics/monster/serpent.frames.lz"
	.ifeq GAME_TLA
	.byte 0x0c, 0xb5	@ PACKER: left in the buffer by Monster_SirenSprites+0x2712
	.else
	.ifdef TLA_EDITION_JA
	.byte 0x0c, 0xb5	@ PACKER: left in the buffer by Monster_SirenSprites+0x2712
	.endif
	.endif
