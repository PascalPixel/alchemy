	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map292
Field_Map292:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map292, .LField_Map292_CELL_PICTURES
	map_block Field_Map292, .LField_Map292_SHAPES
	map_block Field_Map292, .LField_Map292_GRID
	map_block Field_Map292, .LField_Map292_SHAPE_GRID
	map_block Field_Map292
	map_block Field_Map292
	map_block Field_Map292, .LField_Map292_SCENE
.LField_Map292_CELL_PICTURES:
	.incbin "games/tla/src/map/map_000/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map292_SHAPES:
	.incbin "games/tla/src/map/map_292/shapes.table.lz"
	.balign 4, 0
.LField_Map292_GRID:
	.incbin "games/tla/src/map/map_292/grid.parts.lz"
	.balign 4, 0
.LField_Map292_SHAPE_GRID:
	.incbin "games/tla/src/map/map_292/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map292_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
	.global Resource_Data61A
Resource_Data61A:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map292+0xb
	.global Resource_Data61B
Resource_Data61B:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x01, 0xff	@ PACKER: left in the buffer by Field_Map292+0x14e
	.global Resource_Data61C
Resource_Data61C:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data61B+0x13f
	.global Resource_Data61D
Resource_Data61D:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data61C+0x13f
	.global Resource_Data61E
Resource_Data61E:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data61D+0x13f
