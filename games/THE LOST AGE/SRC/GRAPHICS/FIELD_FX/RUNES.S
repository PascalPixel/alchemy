@ FieldFx_Runes: sixteen runes, in white and then in blue, 32 tiles of 8-bit colour in 8x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_Runes
FieldFx_Runes:
	.incbin "GRAPHICS/FIELD_FX/RUNES.8bpp.lz"
