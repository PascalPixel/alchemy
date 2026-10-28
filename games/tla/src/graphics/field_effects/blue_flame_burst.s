	.section .rodata
	.global FieldEffect_BlueFlameBurst
FieldEffect_BlueFlameBurst:
	.incbin "games/tla/src/graphics/field_effects/blue_flame_burst.8bpp.lz"
	.byte 0x27	@ PACKER: left in the buffer by FieldEffect_YellowFlameBurst+0x203
