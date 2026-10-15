	.section .rodata
	.global FieldFx_BlueFlameBurst
FieldFx_BlueFlameBurst:
	.incbin "games/tla/src/graphics/field_fx/blue_flame_burst.8bpp.lz"
	.byte 0x27	@ PACKER: left in the buffer by FieldFx_YellowFlameBurst+0x203
