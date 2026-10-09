	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Tileset_Set10TilesA
Tileset_Set10TilesA:
	.incbin "graphics/tileset/set_10/tiles_a.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xf0, 0x4b	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x1f7e
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xf0, 0x4b	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x1f7e
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xf0, 0x4b	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x1f7e
	.else
	.byte 0xc9, 0x36	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x1f7e
	.endif
	.endif
	.endif
