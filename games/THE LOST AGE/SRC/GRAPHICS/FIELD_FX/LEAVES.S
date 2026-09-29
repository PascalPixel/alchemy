@ FieldFx_Leaves: green leaves blowing, 6 tiles of 8-bit colour in 8x8
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_Leaves
FieldFx_Leaves:
	.incbin "GRAPHICS/FIELD_FX/LEAVES.8bpp.lz"
