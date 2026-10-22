	.section .rodata
	.global FieldEffect_FallingRock
FieldEffect_FallingRock:
	.incbin "games/tla/src/graphics/field_fx/falling_rock.8bpp.lz"
	.byte 0xde, 0x47, 0x2f	@ PACKER: left in the buffer by FieldEffect_ColouredOrbs+0xe1
