	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map287
Field_Map287:
	map_header 0, 0, 128, 128, 3, 2, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 100, 100, 0, 0, 0xff, 0xff
	map_block Field_Map287, .LField_Map287_METATILES
	map_block Field_Map287, .LField_Map287_SHAPES
	map_block Field_Map287, .LField_Map287_GRID
	map_block Field_Map287, .LField_Map287_SHAPE_GRID
	map_block Field_Map287, .LField_Map287_ANIMATION
	map_block Field_Map287, .LField_Map287_BLEND
	map_block Field_Map287, .LField_Map287_SCENE
.LField_Map287_METATILES:
	.incbin "games/tla/src/map/map_287/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map287_SHAPES:
	.incbin "games/tla/src/map/map_287/shapes.table.lz"
	.balign 4, 0
.LField_Map287_GRID:
	.incbin "games/tla/src/map/map_287/grid.parts.lz"
	.balign 4, 0
.LField_Map287_SHAPE_GRID:
	.incbin "games/tla/src/map/map_287/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map287_ANIMATION:
	.incbin "games/tla/src/map/map_062/animation.script.lz"
	.balign 4, 0
.LField_Map287_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map287_SCENE:
	.incbin "games/tla/src/map/map_287/scene.parts.lz"
