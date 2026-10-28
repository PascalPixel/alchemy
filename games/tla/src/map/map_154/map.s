	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map154
Field_Map154:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map154, .LField_Map154_CELL_PICTURES
	map_block Field_Map154, .LField_Map154_SHAPES
	map_block Field_Map154, .LField_Map154_GRID
	map_block Field_Map154, .LField_Map154_SHAPE_GRID
	map_block Field_Map154, .LField_Map154_ANIMATION
	map_block Field_Map154, .LField_Map154_BLEND
	map_block Field_Map154, .LField_Map154_SCENE
.LField_Map154_CELL_PICTURES:
	.incbin "games/tla/src/map/map_154/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map154_SHAPES:
	.incbin "games/tla/src/map/map_154/shapes.table.lz"
	.balign 4, 0
.LField_Map154_GRID:
	.incbin "games/tla/src/map/map_154/grid.parts.lz"
	.balign 4, 0
.LField_Map154_SHAPE_GRID:
	.incbin "games/tla/src/map/map_154/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map154_ANIMATION:
	.incbin "games/tla/src/map/map_150/animation.script.lz"
	.balign 4, 0
.LField_Map154_BLEND:
	.incbin "games/tla/src/map/map_150/blend.script.lz"
	.balign 4, 0
.LField_Map154_SCENE:
	.incbin "games/tla/src/map/map_154/scene.parts.lz"
