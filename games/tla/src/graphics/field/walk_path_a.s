	.section .rodata
	.global Field_WalkPathA
Field_WalkPathA:
	.incbin "games/tla/src/graphics/field/walk_path_a.table.lz"
	.byte 0x41, 0xad, 0x6c	@ PACKER: left in the buffer by WorldMap_TilesD3+0xbb1
