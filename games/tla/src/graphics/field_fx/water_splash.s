	.section .rodata
	.global FieldFx_WaterSplash
FieldFx_WaterSplash:
	.incbin "games/tla/src/graphics/field_fx/water_splash.8bpp.lz"
	.byte 0x1b, 0x3e, 0x01	@ PACKER: left in the buffer by FieldFx_FallingRock+0xcd
