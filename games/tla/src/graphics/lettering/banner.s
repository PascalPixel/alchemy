	.section .rodata
	.global Lettering_Banner
Lettering_Banner:
	.ifdef TLA_EDITION_DE
	.incbin "games/tla/src/graphics/lettering/banner_de.parts.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/graphics/lettering/banner_es.parts.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "games/tla/src/graphics/lettering/banner_fr.parts.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "games/tla/src/graphics/lettering/banner_it.parts.lz"
	.else
	.incbin "games/tla/src/graphics/lettering/banner.parts.lz"
	.endif
	.endif
	.endif
	.endif
	.ifdef TLA_EDITION_DE
	.byte 0x29, 0x3b	@ PACKER: left in the buffer by Field_WalkPathD+0xdfa
	.else
	.ifdef TLA_EDITION_ES
	.else
	.ifdef TLA_EDITION_FR
	.byte 0x2d	@ PACKER: left in the buffer by Field_WalkPathD+0xdb3
	.else
	.ifdef TLA_EDITION_IT
	.else
	.byte 0x7c, 0xfe	@ PACKER: left in the buffer by Field_WalkPathD+0xf02
	.endif
	.endif
	.endif
	.endif
