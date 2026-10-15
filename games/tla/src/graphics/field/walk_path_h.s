	.section .rodata
	.global Field_WalkPathH
Field_WalkPathH:
	.incbin "games/tla/src/graphics/field/walk_path_h.table.lz"
	.byte 0x00, 0x5b	@ PACKER: left in the buffer by Field_WalkPathF+0xab6
