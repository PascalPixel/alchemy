	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map236
Field_Map236:
	map_header 0, 0, 128, 64, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map236, .LField_Map236_CELL_PICTURES
	map_block Field_Map236, .LField_Map236_SHAPES
	map_block Field_Map236, .LField_Map236_GRID
	map_block Field_Map236, .LField_Map236_SHAPE_GRID
	map_block Field_Map236, .LField_Map236_ANIMATION
	map_block Field_Map236, .LField_Map236_BLEND
	map_block Field_Map236, .LField_Map236_SCENE
.LField_Map236_CELL_PICTURES:
	.incbin "games/tla/src/map/map_236/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map236_SHAPES:
	.incbin "games/tla/src/map/map_236/shapes.table.lz"
	.balign 4, 0
.LField_Map236_GRID:
	.incbin "games/tla/src/map/map_236/grid.parts.lz"
	.balign 4, 0
.LField_Map236_SHAPE_GRID:
	.incbin "games/tla/src/map/map_236/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map236_ANIMATION:
	.incbin "games/tla/src/map/map_235/animation.script.lz"
	.balign 4, 0
.LField_Map236_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map236_SCENE:
	.incbin "games/tla/src/map/map_236/scene.parts.lz"
	.balign 4, 0
