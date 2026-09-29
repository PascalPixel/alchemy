@ FieldFx_VioletFlameBurst: a violet flame bursting upwards, 64 tiles of 8-bit colour in 32x32
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_VioletFlameBurst
FieldFx_VioletFlameBurst:
	.incbin "GRAPHICS/FIELD_FX/VIOLET_FLAME_BURST.8bpp.lz"
