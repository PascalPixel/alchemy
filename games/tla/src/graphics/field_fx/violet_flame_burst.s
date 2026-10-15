	.section .rodata
	.global FieldFx_VioletFlameBurst
FieldFx_VioletFlameBurst:
	.incbin "games/tla/src/graphics/field_fx/violet_flame_burst.8bpp.lz"
	.byte 0x05	@ PACKER: left in the buffer by FieldFx_RedFlameBurst+0x22f
