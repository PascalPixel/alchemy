	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map046
Field_Map046:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map046, .LField_Map046_CELL_PICTURES
	map_block Field_Map046, .LField_Map046_SHAPES
	map_block Field_Map046, .LField_Map046_GRID
	map_block Field_Map046, .LField_Map046_SHAPE_GRID
	map_block Field_Map046, .LField_Map046_ANIMATION
	map_block Field_Map046, .LField_Map046_BLEND
	map_block Field_Map046, .LField_Map046_SCENE
.LField_Map046_CELL_PICTURES:
	.incbin "games/tla/src/map/map_046/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map046_SHAPES:
	.incbin "games/tla/src/map/map_046/shapes.table.lz"
	.balign 4, 0
.LField_Map046_GRID:
	.incbin "games/tla/src/map/map_046/grid.parts.lz"
	.balign 4, 0
.LField_Map046_SHAPE_GRID:
	.incbin "games/tla/src/map/map_046/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map046_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map046_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map046_SCENE:
	.incbin "games/tla/src/map/map_046/scene.parts.lz"
	.balign 4, 0
