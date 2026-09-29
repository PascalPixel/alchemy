@ Field_WalkPathC: a walk across a town map, recorded for one
@ minute: x and y for each of 3600 frames, zero once it ends, packed by
@ the resource packer.
	.section .rodata
	.global Field_WalkPathC
Field_WalkPathC:
	.incbin "GRAPHICS/FIELD/WALK_PATH_C.table.lz"
