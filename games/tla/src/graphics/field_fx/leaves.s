	.section .rodata
	.global FieldEffect_Leaves
FieldEffect_Leaves:
	.incbin "games/tla/src/graphics/field_fx/leaves.8bpp.lz"
	.byte 0x08, 0xd7	@ PACKER: left in the buffer by FieldEffect_BlueFlameBurst+0x8e
