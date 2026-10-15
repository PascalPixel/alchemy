	.section .rodata
	.global Field_WalkPathE
Field_WalkPathE:
	.incbin "games/tla/src/graphics/field/walk_path_e.table.lz"
	.byte 0x1c, 0x38, 0x13	@ PACKER: left in the buffer by Field_WalkPathD+0x839
