@ FieldFx_StarSparkleFade: a four-pointed sparkle fading out, 4 tiles of 8-bit colour in 8x8
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_StarSparkleFade
FieldFx_StarSparkleFade:
	.incbin "GRAPHICS/FIELD_FX/STAR_SPARKLE_FADE.8bpp.lz"
