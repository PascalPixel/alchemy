	.section .rodata
	.global FieldFx_WhitePuff
FieldFx_WhitePuff:
	.incbin "games/tla/src/graphics/field_fx/white_puff.8bpp.lz"
	.ifndef TLA_EDITION_EN
	.ifndef TLA_EDITION_DE
	.ifndef TLA_EDITION_ES
	.ifndef TLA_EDITION_FR
	.ifndef TLA_EDITION_IT
	.byte 0x00, 0x0b	@ PACKER: left in the buffer by Resource_Data1E8+0x1d2
	.endif
	.endif
	.endif
	.endif
	.endif
