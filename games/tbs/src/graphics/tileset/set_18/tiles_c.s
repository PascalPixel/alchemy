	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Tileset_Set18TilesC
Tileset_Set18TilesC:
	.ifdef TBS_EDITION_ES
	.incbin "graphics/tileset/set_18/tiles_c_es.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/tileset/set_18/tiles_c_es.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "graphics/tileset/set_18/tiles_c_es.4bpp.mtf"
	.else
	.incbin "graphics/tileset/set_18/tiles_c.4bpp.mtf"
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x60, 0x82, 0xc0	@ PACKER: left in the buffer by Tileset_Set18TilesB+0xec1
	.else
	.ifdef TBS_EDITION_FR
	.byte 0x60, 0x82, 0xc0	@ PACKER: left in the buffer by Tileset_Set18TilesB+0xec1
	.else
	.ifdef TBS_EDITION_IT
	.byte 0x60, 0x82, 0xc0	@ PACKER: left in the buffer by Tileset_Set18TilesB+0xec1
	.else
	.byte 0x00, 0x90	@ PACKER: left in the buffer by Tileset_Set18TilesB+0xfaa
	.endif
	.endif
	.endif
