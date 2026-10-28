	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map071
Field_Map071:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map071, .LField_Map071_CELL_PICTURES
	map_block Field_Map071, .LField_Map071_SHAPES
	map_block Field_Map071, .LField_Map071_GRID
	map_block Field_Map071, .LField_Map071_SHAPE_GRID
	map_block Field_Map071, .LField_Map071_ANIMATION
	map_block Field_Map071, .LField_Map071_BLEND
	map_block Field_Map071, .LField_Map071_SCENE
.LField_Map071_CELL_PICTURES:
	.incbin "games/tla/src/map/map_071/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map071_SHAPES:
	.incbin "games/tla/src/map/map_071/shapes.table.lz"
	.balign 4, 0
.LField_Map071_GRID:
	.incbin "games/tla/src/map/map_071/grid.parts.lz"
	.balign 4, 0
.LField_Map071_SHAPE_GRID:
	.incbin "games/tla/src/map/map_071/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map071_ANIMATION:
	.incbin "games/tla/src/map/map_071/animation.script.lz"
	.balign 4, 0
.LField_Map071_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map071_SCENE:
	.incbin "games/tla/src/map/map_071/scene.parts.lz"
