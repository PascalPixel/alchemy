	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Tileset_Set09TilesB
Tileset_Set09TilesB:
	.incbin "GRAPHICS/TILESET/SET_09/TILES_B.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xee	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xee	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xee	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.byte 0x86	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.endif
	.endif
	.endif
