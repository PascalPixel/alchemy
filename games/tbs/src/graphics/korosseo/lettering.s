	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Korosseo_Lettering
Korosseo_Lettering:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/korosseo/lettering_ja.parts.lz"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "graphics/korosseo/lettering_de.parts.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "graphics/korosseo/lettering_es.parts.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "graphics/korosseo/lettering_fr.parts.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "graphics/korosseo/lettering_it.parts.lz"
	.else
	.incbin "graphics/korosseo/lettering.parts.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x30, 0x58	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_FR
	.byte 0x73	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_JA
	.byte 0xbd, 0x8e	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xc5	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.endif
	.endif
	.endif
	.endif
