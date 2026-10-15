	.section .rodata
	.global Field_WalkPathB
Field_WalkPathB:
	.incbin "games/tla/src/graphics/field/walk_path_b.table.lz"
	.byte 0xce, 0x03	@ PACKER: left in the buffer by WorldMap_TilesD3+0xcaa
