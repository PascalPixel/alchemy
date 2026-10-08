	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
@ 「つづく」の文字
	.global Title_IntroTilesB
Title_IntroTilesB:
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/TITLE/TSUZUKU_DE.gbapal"
	.incbin "GRAPHICS/TITLE/TSUZUKU_DE.8bpp.lz"
	.byte 0xab, 0x02	@ PACKER: left in the buffer by Field_WorldMapPicture+0x876
	.else
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/TITLE/TSUZUKU_ES.gbapal"
	.incbin "GRAPHICS/TITLE/TSUZUKU_ES.8bpp.lz"
	.byte 0xe8, 0x30, 0x22	@ PACKER: left in the buffer by Field_WorldMapPicture+0x719
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/TITLE/TSUZUKU_FR.gbapal"
	.incbin "GRAPHICS/TITLE/TSUZUKU_FR.8bpp.lz"
	.byte 0x85, 0x01, 0x5b	@ PACKER: left in the buffer by Field_WorldMapPicture+0x6a5
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/TITLE/TSUZUKU_IT.gbapal"
	.incbin "GRAPHICS/TITLE/TSUZUKU_IT.8bpp.lz"
	.byte 0x8a	@ PACKER: left in the buffer by Field_WorldMapPicture+0x687
	.else
	.incbin "GRAPHICS/TITLE/TSUZUKU.parts.lz"
	.endif
	.endif
	.endif
	.endif
