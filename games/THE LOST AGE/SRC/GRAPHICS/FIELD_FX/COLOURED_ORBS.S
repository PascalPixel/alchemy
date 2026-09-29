@ FieldFx_ColouredOrbs: glowing orbs in five colours, a large and a small one per line, 16 tiles of 8-bit colour in 8x8
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_ColouredOrbs
FieldFx_ColouredOrbs:
	.incbin "GRAPHICS/FIELD_FX/COLOURED_ORBS.8bpp.lz"
