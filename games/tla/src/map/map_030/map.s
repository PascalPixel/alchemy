	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map030
Field_Map030:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map030, .LField_Map030_CELL_PICTURES
	map_block Field_Map030, .LField_Map030_SHAPES
	map_block Field_Map030, .LField_Map030_GRID
	map_block Field_Map030, .LField_Map030_SHAPE_GRID
	map_block Field_Map030, .LField_Map030_ANIMATION
	map_block Field_Map030, .LField_Map030_BLEND
	map_block Field_Map030, .LField_Map030_SCENE
.LField_Map030_CELL_PICTURES:
	.incbin "games/tla/src/map/map_030/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map030_SHAPES:
	.incbin "games/tla/src/map/map_030/shapes.table.lz"
	.balign 4, 0
.LField_Map030_GRID:
	.incbin "games/tla/src/map/map_030/grid.parts.lz"
	.balign 4, 0
.LField_Map030_SHAPE_GRID:
	.incbin "games/tla/src/map/map_030/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map030_ANIMATION:
	.incbin "games/tla/src/map/map_030/animation.script.lz"
	.balign 4, 0
.LField_Map030_BLEND:
	.incbin "games/tla/src/map/map_030/blend.script.lz"
	.balign 4, 0
.LField_Map030_SCENE:
	.incbin "games/tla/src/map/map_030/scene.parts.lz"
	.balign 4, 0
