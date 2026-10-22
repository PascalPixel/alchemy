	.section .rodata
	.global FieldEffect_WhiteSparkle
FieldEffect_WhiteSparkle:
	.incbin "games/tla/src/graphics/field_fx/white_sparkle.8bpp.lz"
	.byte 0xd2, 0xe2	@ PACKER: left in the buffer by FieldEffect_IceShard+0x2e
