	.include "games/common/include/game/ed_asm.h"
	.section .rodata
@ 「つづく」の文字
	.global Title_IntroTilesB
Title_IntroTilesB:
	.ifdef TBS_EDITION_DE
	.incbin "graphics/title/tsuzuku_de.gbapal"
	.incbin "graphics/title/tsuzuku_de.8bpp.lz"
	.byte 0xab, 0x02	@ PACKER: left in the buffer by Field_WorldMapPicture+0x876
	.else
	.ifdef TBS_EDITION_ES
	.incbin "graphics/title/tsuzuku_es.gbapal"
	.incbin "graphics/title/tsuzuku_es.8bpp.lz"
	.byte 0xe8, 0x30, 0x22	@ PACKER: left in the buffer by Field_WorldMapPicture+0x719
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/title/tsuzuku_fr.gbapal"
	.incbin "graphics/title/tsuzuku_fr.8bpp.lz"
	.byte 0x85, 0x01, 0x5b	@ PACKER: left in the buffer by Field_WorldMapPicture+0x6a5
	.else
	.ifdef TBS_EDITION_IT
	.incbin "graphics/title/tsuzuku_it.gbapal"
	.incbin "graphics/title/tsuzuku_it.8bpp.lz"
	.byte 0x8a	@ PACKER: left in the buffer by Field_WorldMapPicture+0x687
	.else
	.incbin "graphics/title/tsuzuku.parts.lz"
	.endif
	.endif
	.endif
	.endif
