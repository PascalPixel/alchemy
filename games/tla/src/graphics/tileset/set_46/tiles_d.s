	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Tileset_Set46TilesD
Tileset_Set46TilesD:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_d_ja.4bpp.mtf"
	.else
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_d.4bpp.mtf"
	.endif
	.ifndef TLA_EDITION_EN
	.ifndef TLA_EDITION_DE
	.ifndef TLA_EDITION_ES
	.ifndef TLA_EDITION_FR
	.ifndef TLA_EDITION_IT
	.byte 0x01	@ PACKER: left in the buffer by Tileset_Set46TilesC+0x13f
	.endif
	.endif
	.endif
	.endif
	.endif
