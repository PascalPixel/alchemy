	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map102
Field_Map102:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map102, .LField_Map102_CELL_PICTURES
	map_block Field_Map102, .LField_Map102_SHAPES
	map_block Field_Map102, .LField_Map102_GRID
	map_block Field_Map102, .LField_Map102_SHAPE_GRID
	map_block Field_Map102, .LField_Map102_ANIMATION
	map_block Field_Map102, .LField_Map102_BLEND
	map_block Field_Map102, .LField_Map102_SCENE
.LField_Map102_CELL_PICTURES:
	.incbin "games/tla/src/map/map_102/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map102_SHAPES:
	.incbin "games/tla/src/map/map_102/shapes.table.lz"
	.balign 4, 0
.LField_Map102_GRID:
	.incbin "games/tla/src/map/map_102/grid.parts.lz"
	.balign 4, 0
.LField_Map102_SHAPE_GRID:
	.incbin "games/tla/src/map/map_102/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map102_ANIMATION:
	.incbin "games/tla/src/map/map_102/animation.script.lz"
	.balign 4, 0
.LField_Map102_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map102_SCENE:
	.incbin "games/tla/src/map/map_102/scene.parts.lz"
	.balign 4, 0
