@ FieldFx_WaterSplash: a ring of water splashing, 16 tiles of 8-bit colour in 16x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_WaterSplash
FieldFx_WaterSplash:
	.incbin "GRAPHICS/FIELD_FX/WATER_SPLASH.8bpp.lz"
