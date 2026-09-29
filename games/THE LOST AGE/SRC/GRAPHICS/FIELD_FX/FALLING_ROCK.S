@ FieldFx_FallingRock: a brown rock crumbling away, 20 tiles of 8-bit colour in 16x16
@ sprite frames, one frame wide, drawn with the system palette and packed
@ by the resource packer.
	.section .rodata
	.global FieldFx_FallingRock
FieldFx_FallingRock:
	.incbin "GRAPHICS/FIELD_FX/FALLING_ROCK.8bpp.lz"
