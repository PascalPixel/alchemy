	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_SerpentSprites
Monster_SerpentSprites:
	.incbin "games/common/src/graphics/monster/serpent.frames.lz"
	.ifeq GAME_TLA
	.byte 0x0c, 0xb5	@ PACKER: left in the buffer by Monster_SirenSprites+0x2712
	.else
	.ifndef TLA_EDITION_EN
	.ifndef TLA_EDITION_DE
	.ifndef TLA_EDITION_ES
	.ifndef TLA_EDITION_FR
	.ifndef TLA_EDITION_IT
	.byte 0x0c, 0xb5	@ PACKER: left in the buffer by Monster_SirenSprites+0x2712
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
