	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Korosseo_Lettering
Korosseo_Lettering:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/KOROSSEO/LETTERING_JA.parts.lz"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/KOROSSEO/LETTERING_DE.parts.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/KOROSSEO/LETTERING_ES.parts.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/KOROSSEO/LETTERING_FR.parts.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/KOROSSEO/LETTERING_IT.parts.lz"
	.else
	.incbin "GRAPHICS/KOROSSEO/LETTERING.parts.lz"
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
