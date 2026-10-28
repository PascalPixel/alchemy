	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map045
Field_Map045:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map045, .LField_Map045_CELL_PICTURES
	map_block Field_Map045, .LField_Map045_SHAPES
	map_block Field_Map045, .LField_Map045_GRID
	map_block Field_Map045, .LField_Map045_SHAPE_GRID
	map_block Field_Map045, .LField_Map045_ANIMATION
	map_block Field_Map045, .LField_Map045_BLEND
	map_block Field_Map045, .LField_Map045_SCENE
.LField_Map045_CELL_PICTURES:
	.incbin "games/tla/src/map/map_045/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map045_SHAPES:
	.incbin "games/tla/src/map/map_045/shapes.table.lz"
	.balign 4, 0
.LField_Map045_GRID:
	.incbin "games/tla/src/map/map_045/grid.parts.lz"
	.balign 4, 0
.LField_Map045_SHAPE_GRID:
	.incbin "games/tla/src/map/map_045/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map045_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map045_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map045_SCENE:
	.incbin "games/tla/src/map/map_045/scene.parts.lz"
