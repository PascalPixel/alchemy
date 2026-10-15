	.section .rodata
	.global FieldFx_GoldCrystal
FieldFx_GoldCrystal:
	.incbin "games/tla/src/graphics/field_fx/gold_crystal.8bpp.lz"
	.byte 0xc7, 0xb9	@ PACKER: left in the buffer by FieldFx_GoldStar+0x32
