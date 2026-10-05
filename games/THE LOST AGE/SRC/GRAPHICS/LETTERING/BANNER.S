	.section .rodata
	.global Lettering_Banner
Lettering_Banner:
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/LETTERING/BANNER_DE.parts.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/LETTERING/BANNER_ES.parts.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/LETTERING/BANNER_FR.parts.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/LETTERING/BANNER_IT.parts.lz"
	.else
	.incbin "GRAPHICS/LETTERING/BANNER.parts.lz"
	.endif
	.endif
	.endif
	.endif
