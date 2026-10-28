	.section .rodata
	.global FieldEffect_BowDraw
FieldEffect_BowDraw:
	.incbin "games/tla/src/graphics/field_effects/bow_draw.8bpp.lz"
	.byte 0x68, 0xea	@ PACKER: left in the buffer by FieldEffect_DustPuffB+0x196
