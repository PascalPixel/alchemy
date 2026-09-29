@ FieldFx_GoldStar: a golden star, 4 tiles of 8-bit colour in 16x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_GoldStar
FieldFx_GoldStar:
	.incbin "GRAPHICS/FIELD_FX/GOLD_STAR.8bpp.lz"
