@ Eight 16x16 pictures of 16 colours, one under another: a bordered box whose
@ diagonal fill grows over four pictures, then four empty pictures. No loader of
@ the resource was found; r1e3.png carries the first row of the shared OBJ
@ palette, as r1ed.png does.
	.section .rodata
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "games/tla/src/graphics/field_fx/r1e3.4bpp16x16.lz"
	.byte 0x88	@ PACKER: left in the buffer by FieldEffect_DustPuffB+0x6f
