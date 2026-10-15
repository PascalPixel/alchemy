	.section .rodata
	.global FieldFx_BowDraw
FieldFx_BowDraw:
	.incbin "games/tla/src/graphics/field_fx/bow_draw.8bpp.lz"
	.byte 0x68, 0xea	@ PACKER: left in the buffer by FieldFx_DustPuffB+0x196
