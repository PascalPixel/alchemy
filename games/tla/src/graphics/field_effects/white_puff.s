	.section .rodata
	.global FieldEffect_WhitePuff
FieldEffect_WhitePuff:
	.incbin "games/tla/src/graphics/field_effects/white_puff.8bpp.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x00, 0x0b	@ PACKER: left in the buffer by FieldEffect_Explosion+0x1d2
	.endif
