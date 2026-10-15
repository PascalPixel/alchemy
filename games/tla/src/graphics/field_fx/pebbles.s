	.section .rodata
	.global FieldFx_Pebbles
FieldFx_Pebbles:
	.incbin "games/tla/src/graphics/field_fx/pebbles.8bpp.lz"
	.byte 0x49	@ PACKER: left in the buffer by FieldFx_Runes+0x9b
