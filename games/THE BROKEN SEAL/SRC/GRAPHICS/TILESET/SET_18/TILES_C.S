	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Tileset_Set18TilesC
Tileset_Set18TilesC:
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/TILESET/SET_18/TILES_C_ES.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/TILESET/SET_18/TILES_C_ES.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/TILESET/SET_18/TILES_C_ES.4bpp.mtf"
	.else
	.incbin "GRAPHICS/TILESET/SET_18/TILES_C.4bpp.mtf"
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x60, 0x82, 0xc0	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_FR
	.byte 0x60, 0x82, 0xc0	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_IT
	.byte 0x60, 0x82, 0xc0	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.byte 0x00, 0x90	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.endif
	.endif
	.endif
