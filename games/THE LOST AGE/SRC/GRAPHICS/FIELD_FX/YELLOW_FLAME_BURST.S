@ FieldFx_YellowFlameBurst: a yellow flame bursting upwards, 64 tiles of 8-bit colour in 32x32
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_YellowFlameBurst
FieldFx_YellowFlameBurst:
	.incbin "GRAPHICS/FIELD_FX/YELLOW_FLAME_BURST.8bpp.lz"
