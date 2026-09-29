@ FieldFx_GoldenTrident: a golden trident, 8 tiles of 8-bit colour in 64x8
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_GoldenTrident
FieldFx_GoldenTrident:
	.incbin "GRAPHICS/FIELD_FX/GOLDEN_TRIDENT.8bpp.lz"
