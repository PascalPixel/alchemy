@ FieldFx_Pebbles: grey pebbles scattering, 16 tiles of 8-bit colour in 16x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_Pebbles
FieldFx_Pebbles:
	.incbin "GRAPHICS/FIELD_FX/PEBBLES.8bpp.lz"
