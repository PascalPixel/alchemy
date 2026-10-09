	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Tileset_Set09TilesB
Tileset_Set09TilesB:
	.incbin "graphics/tileset/set_09/tiles_b.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xee	@ PACKER: left in the buffer by Tileset_Set09TilesA+0xd7b
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xee	@ PACKER: left in the buffer by Tileset_Set09TilesA+0xd7b
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xee	@ PACKER: left in the buffer by Tileset_Set09TilesA+0xd7b
	.else
	.byte 0x86	@ PACKER: left in the buffer by Tileset_Set09TilesA+0xd7b
	.endif
	.endif
	.endif
