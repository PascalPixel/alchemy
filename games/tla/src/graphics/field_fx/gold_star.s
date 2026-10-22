	.section .rodata
	.global FieldEffect_GoldStar
FieldEffect_GoldStar:
	.incbin "games/tla/src/graphics/field_fx/gold_star.8bpp.lz"
	.byte 0x3e	@ PACKER: left in the buffer by Resource_Data1E3+0x5b
