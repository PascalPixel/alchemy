@ Field_WalkPathD: a walk across a town map, recorded for one
@ minute: x and y for each of 3600 frames, zero once it ends, packed by
@ the resource packer.
	.section .rodata
	.global Field_WalkPathD
Field_WalkPathD:
	.incbin "GRAPHICS/FIELD/WALK_PATH_D.table.lz"
