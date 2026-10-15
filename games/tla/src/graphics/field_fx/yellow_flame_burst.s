	.section .rodata
	.global FieldFx_YellowFlameBurst
FieldFx_YellowFlameBurst:
	.incbin "games/tla/src/graphics/field_fx/yellow_flame_burst.8bpp.lz"
	.byte 0x50	@ PACKER: left in the buffer by FieldFx_VioletFlameBurst+0x20b
