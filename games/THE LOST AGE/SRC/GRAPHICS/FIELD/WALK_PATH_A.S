@ Field_WalkPathA: a walk across a town map, recorded for one
@ minute: x and y for each of 3600 frames, zero once it ends, packed by
@ the resource packer.
	.section .rodata
	.global Field_WalkPathA
Field_WalkPathA:
	.incbin "GRAPHICS/FIELD/WALK_PATH_A.table.lz"
