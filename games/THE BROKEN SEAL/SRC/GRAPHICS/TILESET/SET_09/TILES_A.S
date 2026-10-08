	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Tileset_Set09TilesA
Tileset_Set09TilesA:
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/TILESET/SET_09/TILES_A_ES.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/TILESET/SET_09/TILES_A_ES.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/TILESET/SET_09/TILES_A_ES.4bpp.mtf"
	.else
	.incbin "GRAPHICS/TILESET/SET_09/TILES_A.4bpp.mtf"
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x89, 0x99	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_FR
	.byte 0x89, 0x99	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_IT
	.byte 0x89, 0x99	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.byte 0x06	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.endif
	.endif
	.endif
