	.section .rodata
	.global FieldFx_StarSparkleFade
FieldFx_StarSparkleFade:
	.incbin "games/tla/src/graphics/field_fx/star_sparkle_fade.8bpp.lz"
	.ifndef TLA_EDITION_EN
	.ifndef TLA_EDITION_DE
	.ifndef TLA_EDITION_ES
	.ifndef TLA_EDITION_FR
	.ifndef TLA_EDITION_IT
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.endif
	.endif
	.endif
	.endif
