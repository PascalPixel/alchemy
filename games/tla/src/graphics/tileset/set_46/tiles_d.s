	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Tileset_Set46TilesD
Tileset_Set46TilesD:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_d_ja.4bpp.mtf"
	.else
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_d.4bpp.mtf"
	.endif
