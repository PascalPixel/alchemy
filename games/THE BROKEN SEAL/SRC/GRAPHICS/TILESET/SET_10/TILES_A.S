	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Tileset_Set10TilesA
Tileset_Set10TilesA:
	.incbin "GRAPHICS/TILESET/SET_10/TILES_A.4bpp.mtf"
	/* PACKER: left in the packer's buffer by earlier streams, which differ by edition */
	.ifdef TBS_EDITION_JA
	.byte 0xf0, 0x4b
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xf0, 0x4b
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xf0, 0x4b
	.else
	.byte 0xc9, 0x36
	.endif
	.endif
	.endif
