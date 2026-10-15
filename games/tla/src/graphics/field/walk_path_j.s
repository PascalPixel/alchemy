	.section .rodata
	.global Field_WalkPathJ
Field_WalkPathJ:
	.incbin "games/tla/src/graphics/field/walk_path_j.table.lz"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
