@ FieldFx_WhiteSparkle: a white sparkle, large and small, 2 tiles of 8-bit colour in 8x8
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_WhiteSparkle
FieldFx_WhiteSparkle:
	.incbin "GRAPHICS/FIELD_FX/WHITE_SPARKLE.8bpp.lz"
