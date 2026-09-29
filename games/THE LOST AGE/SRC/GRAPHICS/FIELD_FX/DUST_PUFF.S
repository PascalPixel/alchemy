@ FieldFx_DustPuffA: a sandy dust puff settling into grains, 32 tiles of 8-bit colour in 16x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_DustPuffA
FieldFx_DustPuffA:
	.incbin "GRAPHICS/FIELD_FX/DUST_PUFF.8bpp.lz"
