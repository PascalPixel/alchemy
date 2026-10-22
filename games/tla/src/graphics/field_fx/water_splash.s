	.section .rodata
	.global FieldEffect_WaterSplash
FieldEffect_WaterSplash:
	.incbin "games/tla/src/graphics/field_fx/water_splash.8bpp.lz"
	.byte 0x1b, 0x3e, 0x01	@ PACKER: left in the buffer by FieldEffect_FallingRock+0xcd
