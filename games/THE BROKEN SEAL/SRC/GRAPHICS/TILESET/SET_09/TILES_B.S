	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Tileset_Set09TilesB
Tileset_Set09TilesB:
	.incbin "GRAPHICS/TILESET/SET_09/TILES_B.4bpp.mtf"
	/* PACKER: left in the packer's buffer by earlier streams, which differ by edition */
	.ifdef TBS_EDITION_JA
	.byte 0xee
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xee
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xee
	.else
	.byte 0x86
	.endif
	.endif
	.endif
