@ FieldFx_RedFlameBurst: a red and orange flame bursting upwards, 64 tiles of 8-bit colour in 32x32
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_RedFlameBurst
FieldFx_RedFlameBurst:
	.incbin "GRAPHICS/FIELD_FX/RED_FLAME_BURST.8bpp.lz"
