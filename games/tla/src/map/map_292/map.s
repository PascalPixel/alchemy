	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map292
Field_Map292:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map292, .LField_Map292_METATILES
	map_block Field_Map292, .LField_Map292_SHAPES
	map_block Field_Map292, .LField_Map292_GRID
	map_block Field_Map292, .LField_Map292_SHAPE_GRID
	map_block Field_Map292
	map_block Field_Map292
	map_block Field_Map292, .LField_Map292_SCENE
.LField_Map292_METATILES:
	.incbin "games/tla/src/map/map_000/metatiles.delta0.lz"
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
