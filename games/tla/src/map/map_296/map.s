	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map296
Field_Map296:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map296, .LField_Map296_METATILES
	map_block Field_Map296, .LField_Map296_SHAPES
	map_block Field_Map296, .LField_Map296_GRID
	map_block Field_Map296, .LField_Map296_SHAPE_GRID
	map_block Field_Map296
	map_block Field_Map296
	map_block Field_Map296, .LField_Map296_SCENE
.LField_Map296_METATILES:
	.incbin "games/common/src/map/map_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map296_SHAPES:
	.incbin "games/tla/src/map/map_099/shapes.table.lz"
	.balign 4, 0
.LField_Map296_GRID:
	.incbin "games/tla/src/map/map_296/grid.parts.lz"
	.balign 4, 0
.LField_Map296_SHAPE_GRID:
	.incbin "games/tla/src/map/map_099/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map296_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
