	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map293
Field_Map293:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map293, .LField_Map293_CELL_PICTURES
	map_block Field_Map293, .LField_Map293_SHAPES
	map_block Field_Map293, .LField_Map293_GRID
	map_block Field_Map293, .LField_Map293_SHAPE_GRID
	map_block Field_Map293
	map_block Field_Map293
	map_block Field_Map293, .LField_Map293_SCENE
.LField_Map293_CELL_PICTURES:
	.incbin "games/tla/src/map/map_000/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map293_SHAPES:
	.incbin "games/tla/src/map/map_041/shapes.table.lz"
	.balign 4, 0
.LField_Map293_GRID:
	.incbin "games/tla/src/map/map_293/grid.parts.lz"
	.balign 4, 0
.LField_Map293_SHAPE_GRID:
	.incbin "games/tla/src/map/map_293/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map293_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
