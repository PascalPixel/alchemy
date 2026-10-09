	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Tileset_Set09TilesA
Tileset_Set09TilesA:
	.ifdef TBS_EDITION_ES
	.incbin "graphics/tileset/set_09/tiles_a_es.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/tileset/set_09/tiles_a_es.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "graphics/tileset/set_09/tiles_a_es.4bpp.mtf"
	.else
	.incbin "graphics/tileset/set_09/tiles_a.4bpp.mtf"
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x89, 0x99	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x2482
	.else
	.ifdef TBS_EDITION_FR
	.byte 0x89, 0x99	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x2482
	.else
	.ifdef TBS_EDITION_IT
	.byte 0x89, 0x99	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x2482
	.else
	.byte 0x06	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x247f
	.endif
	.endif
	.endif
