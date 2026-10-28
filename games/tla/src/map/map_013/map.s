	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map013
Field_Map013:
	map_header 0, 0, 110, 110, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map013, .LField_Map013_CELL_PICTURES
	map_block Field_Map013, .LField_Map013_SHAPES
	map_block Field_Map013, .LField_Map013_GRID
	map_block Field_Map013, .LField_Map013_SHAPE_GRID
	map_block Field_Map013, .LField_Map013_ANIMATION
	map_block Field_Map013, .LField_Map013_BLEND
	map_block Field_Map013, .LField_Map013_SCENE
.LField_Map013_CELL_PICTURES:
	.incbin "games/tla/src/map/map_013/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map013_SHAPES:
	.incbin "games/tla/src/map/map_013/shapes.table.lz"
	.balign 4, 0
.LField_Map013_GRID:
	.incbin "games/tla/src/map/map_013/grid.parts.lz"
	.balign 4, 0
.LField_Map013_SHAPE_GRID:
	.incbin "games/tla/src/map/map_013/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map013_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map013_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map013_SCENE:
	.incbin "games/tla/src/map/map_013/scene.parts.lz"
	.balign 4, 0
