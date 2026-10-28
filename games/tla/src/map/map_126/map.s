	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map126
Field_Map126:
	map_header 0, 0, 50, 128, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, -1, -3, 0x3f, 0x3f
	map_block Field_Map126, .LField_Map126_CELL_PICTURES
	map_block Field_Map126, .LField_Map126_SHAPES
	map_block Field_Map126, .LField_Map126_GRID
	map_block Field_Map126, .LField_Map126_SHAPE_GRID
	map_block Field_Map126, .LField_Map126_ANIMATION
	map_block Field_Map126, .LField_Map126_BLEND
	map_block Field_Map126, .LField_Map126_SCENE
.LField_Map126_CELL_PICTURES:
	.incbin "games/tla/src/map/map_126/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map126_SHAPES:
	.incbin "games/tla/src/map/map_126/shapes.table.lz"
	.balign 4, 0
.LField_Map126_GRID:
	.incbin "games/tla/src/map/map_126/grid.parts.lz"
	.balign 4, 0
.LField_Map126_SHAPE_GRID:
	.incbin "games/tla/src/map/map_126/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map126_ANIMATION:
	.incbin "games/tla/src/map/map_126/animation.script.lz"
	.balign 4, 0
.LField_Map126_BLEND:
	.incbin "games/tla/src/map/map_126/blend.script.lz"
	.balign 4, 0
.LField_Map126_SCENE:
	.incbin "games/tla/src/map/map_126/scene.parts.lz"
