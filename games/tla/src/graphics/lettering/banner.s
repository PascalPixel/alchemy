	.section .rodata
	.global Lettering_Banner
Lettering_Banner:
	.ifdef TLA_EDITION_DE
	.incbin "graphics/lettering/banner_de.parts.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "graphics/lettering/banner_es.parts.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "graphics/lettering/banner_fr.parts.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "graphics/lettering/banner_it.parts.lz"
	.else
	.incbin "graphics/lettering/banner.parts.lz"
	.endif
	.endif
	.endif
	.endif
