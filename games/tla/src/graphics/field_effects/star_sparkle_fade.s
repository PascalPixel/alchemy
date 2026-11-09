	.section .rodata
	.global FieldEffect_StarSparkleFade
FieldEffect_StarSparkleFade:
	.incbin "games/tla/src/graphics/field_effects/star_sparkle_fade.8bpp.lz"
	.ifdef TLA_EDITION_JA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.else
	.byte 0x01, 0x00, 0x00	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
	.endif
