@ A 32x64 sprite of 256 colours: a column shaded in colours 224 to 255.
@ Overlay 685's Func_02002d34 unpacks it, turns each colour 240 to 255 into
@ 96 to 111 and makes one 32x64 sprite of 256 colours. Its callback
@ Func_02002de4 writes 32 colours it computes from the sine table to OBJ
@ colours 96 to 111 and 224 to 239 each frame, from a counter that starts at
@ 0. The picture carries the shared OBJ palette and, for its colours 224 to
@ 255, the colours the sprite shows for them on the first frame.
	.section .rodata
	.global FieldEffect_FireColumn
FieldEffect_FireColumn:
	.incbin "games/tla/src/graphics/field_fx/fire_column.8bpp.lz"
	.byte 0xee, 0xd3	@ PACKER: left in the buffer by FieldEffect_GlowDisc+0x3fe
