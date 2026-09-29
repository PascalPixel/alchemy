@ FieldFx_FlamingArrow: a flaming arrow, 4 tiles of 8-bit colour in 16x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_FlamingArrow
FieldFx_FlamingArrow:
	.incbin "GRAPHICS/FIELD_FX/FLAMING_ARROW.8bpp.lz"
