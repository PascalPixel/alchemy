	.section .rodata
	.global FieldEffect_Pebbles
FieldEffect_Pebbles:
	.incbin "games/tla/src/graphics/field_effects/pebbles.8bpp.lz"
	.byte 0x49	@ PACKER: left in the buffer by FieldEffect_Runes+0x9b
