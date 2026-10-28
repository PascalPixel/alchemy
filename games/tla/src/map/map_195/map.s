	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map195
Field_Map195:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map195, .LField_Map195_CELL_PICTURES
	map_block Field_Map195, .LField_Map195_SHAPES
	map_block Field_Map195, .LField_Map195_GRID
	map_block Field_Map195, .LField_Map195_SHAPE_GRID
	map_block Field_Map195, .LField_Map195_ANIMATION
	map_block Field_Map195, .LField_Map195_BLEND
	map_block Field_Map195, .LField_Map195_SCENE
.LField_Map195_CELL_PICTURES:
	.incbin "games/tla/src/map/map_195/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map195_SHAPES:
	.incbin "games/tla/src/map/map_195/shapes.table.lz"
	.balign 4, 0
.LField_Map195_GRID:
	.incbin "games/tla/src/map/map_195/grid.parts.lz"
	.balign 4, 0
.LField_Map195_SHAPE_GRID:
	.incbin "games/tla/src/map/map_195/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map195_ANIMATION:
	.incbin "games/tla/src/map/map_195/animation.script.lz"
	.balign 4, 0
.LField_Map195_BLEND:
	.incbin "games/tla/src/map/map_194/blend.script.lz"
	.balign 4, 0
.LField_Map195_SCENE:
	.incbin "games/tla/src/map/map_195/scene.parts.lz"
	.balign 4, 0
