	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map168
Field_Map168:
	map_header 0, 0, 130, 112, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map168, .LField_Map168_CELL_PICTURES
	map_block Field_Map168, .LField_Map168_SHAPES
	map_block Field_Map168, .LField_Map168_GRID
	map_block Field_Map168, .LField_Map168_SHAPE_GRID
	map_block Field_Map168, .LField_Map168_ANIMATION
	map_block Field_Map168, .LField_Map168_BLEND
	map_block Field_Map168, .LField_Map168_SCENE
.LField_Map168_CELL_PICTURES:
	.incbin "games/tla/src/map/map_168/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map168_SHAPES:
	.incbin "games/tla/src/map/map_168/shapes.table.lz"
	.balign 4, 0
.LField_Map168_GRID:
	.incbin "games/tla/src/map/map_168/grid.parts.lz"
	.balign 4, 0
.LField_Map168_SHAPE_GRID:
	.incbin "games/tla/src/map/map_168/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map168_ANIMATION:
	.incbin "games/tla/src/map/map_168/animation.script.lz"
	.balign 4, 0
.LField_Map168_BLEND:
	.incbin "games/tla/src/map/map_168/blend.script.lz"
	.balign 4, 0
.LField_Map168_SCENE:
	.incbin "games/tla/src/map/map_168/scene.parts.lz"
