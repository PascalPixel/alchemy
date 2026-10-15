	.section .rodata
	.global Field_WalkPathD
Field_WalkPathD:
	.incbin "games/tla/src/graphics/field/walk_path_d.table.lz"
	.byte 0xe6, 0xf4, 0x80	@ PACKER: left in the buffer by WorldMap_TilesD3+0xf3d
