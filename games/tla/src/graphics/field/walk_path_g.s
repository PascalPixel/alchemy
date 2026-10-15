	.section .rodata
	.global Field_WalkPathG
Field_WalkPathG:
	.incbin "games/tla/src/graphics/field/walk_path_g.table.lz"
	.byte 0xa2, 0x38	@ PACKER: left in the buffer by Field_WalkPathF+0x882
