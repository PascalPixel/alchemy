@ FieldFx_WhitePuff: a white puff of smoke fading out, 32 tiles of 8-bit colour in 16x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_WhitePuff
FieldFx_WhitePuff:
	.incbin "GRAPHICS/FIELD_FX/WHITE_PUFF.8bpp.lz"
