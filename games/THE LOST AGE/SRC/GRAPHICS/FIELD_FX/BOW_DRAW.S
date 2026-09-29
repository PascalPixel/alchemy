@ FieldFx_BowDraw: a golden bow drawn and loosed, 96 tiles of 8-bit colour in 32x32
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_BowDraw
FieldFx_BowDraw:
	.incbin "GRAPHICS/FIELD_FX/BOW_DRAW.8bpp.lz"
