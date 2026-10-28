	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global Monster_SerpentSprites
Monster_SerpentSprites:
	.incbin "games/tla/src/graphics/monster/serpent.pictures.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x0c, 0xb5	@ PACKER: left in the buffer by Monster_SirenSprites+0x2712
	.endif
