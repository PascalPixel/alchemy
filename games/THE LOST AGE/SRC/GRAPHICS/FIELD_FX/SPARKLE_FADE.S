@ FieldFx_SparkleFade: a round sparkle fading out, 4 tiles of 8-bit colour in 8x8
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_SparkleFade
FieldFx_SparkleFade:
	.incbin "GRAPHICS/FIELD_FX/SPARKLE_FADE.8bpp.lz"
