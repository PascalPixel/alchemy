	.section .rodata
	.global Field_WalkPathC
Field_WalkPathC:
	.incbin "games/tla/src/graphics/field/walk_path_c.table.lz"
	.byte 0xa0, 0x81	@ PACKER: left in the buffer by Field_WalkPathB+0xb02
