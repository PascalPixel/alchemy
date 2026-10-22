	.section .rodata
	.global FieldEffect_GoldCrystal
FieldEffect_GoldCrystal:
	.incbin "games/tla/src/graphics/field_fx/gold_crystal.8bpp.lz"
	.byte 0xc7, 0xb9	@ PACKER: left in the buffer by FieldEffect_GoldStar+0x32
