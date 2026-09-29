@ FieldFx_BlueFlameBurst: a pale blue flame bursting upwards, 64 tiles of 8-bit colour in 32x32
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_BlueFlameBurst
FieldFx_BlueFlameBurst:
	.incbin "GRAPHICS/FIELD_FX/BLUE_FLAME_BURST.8bpp.lz"
