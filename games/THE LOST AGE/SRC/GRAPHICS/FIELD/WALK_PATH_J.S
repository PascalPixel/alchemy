@ Field_WalkPathJ: a walk across a town map, recorded for one
@ minute: x and y for each of 3600 frames, zero once it ends, packed by
@ the resource packer.
	.section .rodata
	.global Field_WalkPathJ
Field_WalkPathJ:
	.incbin "GRAPHICS/FIELD/WALK_PATH_J.table.lz"
