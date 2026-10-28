	.section .rodata
	.global FieldEffect_YellowFlameBurst
FieldEffect_YellowFlameBurst:
	.incbin "games/tla/src/graphics/field_effects/yellow_flame_burst.8bpp.lz"
	.byte 0x50	@ PACKER: left in the buffer by FieldEffect_VioletFlameBurst+0x20b
