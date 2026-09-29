@ FieldFx_DustPuffB: a sandy dust puff settling into grains, a second copy, 32 tiles of 8-bit colour in 16x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_DustPuffB
FieldFx_DustPuffB:
	.incbin "GRAPHICS/FIELD_FX/DUST_PUFF.8bpp.lz"
