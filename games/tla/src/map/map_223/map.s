	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map223
Field_Map223:
	map_header 0, 0, 64, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map223, .LField_Map223_CELL_PICTURES
	map_block Field_Map223, .LField_Map223_SHAPES
	map_block Field_Map223, .LField_Map223_GRID
	map_block Field_Map223, .LField_Map223_SHAPE_GRID
	map_block Field_Map223, .LField_Map223_ANIMATION
	map_block Field_Map223, .LField_Map223_BLEND
	map_block Field_Map223, .LField_Map223_SCENE
.LField_Map223_CELL_PICTURES:
	.incbin "games/tla/src/map/map_223/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map223_SHAPES:
	.incbin "games/tla/src/map/map_223/shapes.table.lz"
	.balign 4, 0
.LField_Map223_GRID:
	.incbin "games/tla/src/map/map_223/grid.parts.lz"
	.balign 4, 0
.LField_Map223_SHAPE_GRID:
	.incbin "games/tla/src/map/map_223/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map223_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map223_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map223_SCENE:
	.incbin "games/tla/src/map/map_223/scene.parts.lz"
